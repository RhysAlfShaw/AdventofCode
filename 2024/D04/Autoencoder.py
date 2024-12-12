import torch
import torch.nn as nn
import torch.nn.functional as F
from tqdm import tqdm
import matplotlib.pyplot as plt


class EncoderDecoder(nn.Module):
    def __init__(self):
        super(EncoderDecoder, self).__init__()

        # encoder
        self.enc1 = nn.Conv2d(1, 64, kernel_size=4, padding=1)
        self.enc2 = nn.Conv2d(64, 128, kernel_size=4, padding=1)
        self.enc3 = nn.Conv2d(128, 256, kernel_size=4, padding=1)

        # decoder
        self.dec3 = nn.ConvTranspose2d(256, 128, kernel_size=4, padding=1)
        self.dec2 = nn.ConvTranspose2d(128, 64, kernel_size=4, padding=1)
        self.dec1 = nn.ConvTranspose2d(64, 1, kernel_size=4, padding=1)

        # max pooling
        self.pool = nn.MaxPool2d(kernel_size=2, stride=2)

    def forward(self, x):
        x1 = F.relu(self.enc1(x))
        x2 = self.pool(F.relu(self.enc2(x1)))
        x3 = self.pool(F.relu(self.enc3(x2)))

        x = F.interpolate(x3, scale_factor=2, mode="nearest")
        x = F.relu(self.dec3(x))
        x = F.interpolate(x, scale_factor=2, mode="nearest")
        x = F.relu(self.dec2(x))
        x = torch.sigmoid(self.dec1(x))

        return x


import numpy as np


Data = np.load("/data/typhon2/Rhys/d4_images.npy")
Masks = np.load("/data/typhon2/Rhys/d4_masks.npy")


# reshape data from (N, 140, 140) to (N, 1, 140, 140)
Data = Data[:, np.newaxis, :, :]
Masks = Masks[:, np.newaxis, :, :]

# reduce data size to 10_000
Data = Data[:1_000]
Masks = Masks[:1_000]


Data = (Data - Data.min()) / (Data.max() - Data.min())


from sklearn.model_selection import train_test_split

X_train, X_val = train_test_split(Data, test_size=0.01, random_state=42)
Y_train, Y_val = train_test_split(Masks, test_size=0.01, random_state=42)
# send data to gpu
X_train_tensor = torch.tensor(X_train, dtype=torch.float32)
y_train_tensor = torch.tensor(Y_train, dtype=torch.float32)

X_val_tensor = torch.tensor(X_val, dtype=torch.float32)
y_val_tensor = torch.tensor(Y_val, dtype=torch.float32)

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# model initialization
model = EncoderDecoder().to(device)
criterion = nn.BCELoss()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

# Prepare tensors
X_train_tensor = X_train_tensor
y_train_tensor = y_train_tensor

X_val_tensor = X_val_tensor
y_val_tensor = y_val_tensor
batch_size = 124
batches = len(X_train_tensor) // batch_size
num_epochs = 20
val_losses = []
train_loss = []
print("Training model....")
for epoch in range(num_epochs):
    model.train()
    epoch_train_loss = 0.0

    for i in tqdm(range(batches), desc=f"Epoch {epoch+1}/{num_epochs}", total=batches):
        batch_data = X_train_tensor[i * batch_size : (i + 1) * batch_size].to(
            device
        )  # load each batch to gpu
        batch_target = y_train_tensor[i * batch_size : (i + 1) * batch_size].to(
            device
        )  # load each batch to gpu
        output = model(batch_data)

        loss = criterion(output, batch_target)

        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        epoch_train_loss += loss.item()

    avg_train_loss = epoch_train_loss / batches
    model.eval()
    train_loss.append(avg_train_loss)
    # due to limited GPU memory, we need to move the model to CPU for validation
    with torch.no_grad():
        model.to("cpu")  # move to cpu
        val_output = model(X_val_tensor)
        val_loss = criterion(val_output, y_val_tensor)
        val_losses.append(val_loss.item())
        model.to(device)  # back to gpu for more training!

    print(
        f"Epoch [{epoch+1}/{num_epochs}], Train Loss: {avg_train_loss:.5f}, Val Loss: {val_loss:.5f}"
    )

    torch.save(model.state_dict(), "/data/typhon2/Rhys/adeventofcodemodel.pth")

    plt.figure()
    plt.plot(train_loss, label="Train Loss")
    plt.plot(val_losses, label="Val Loss")
    plt.legend()
    plt.savefig("loss.png")


# save the model
torch.save(model.state_dict(), "/data/typhon2/Rhys/adeventofcodemodel.pth")
