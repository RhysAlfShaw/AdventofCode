# import torch
import torch
import torch.nn as nn
import torch.nn.functional as F


class EncoderDecoder(nn.Module):
    def __init__(self):
        super(EncoderDecoder, self).__init__()

        self.enc1 = nn.Conv2d(1, 64, kernel_size=3, padding=1)
        self.enc2 = nn.Conv2d(64, 128, kernel_size=3, padding=1)
        self.enc3 = nn.Conv2d(128, 256, kernel_size=3, padding=1)

        self.dec3 = nn.ConvTranspose2d(256, 128, kernel_size=3, padding=1)
        self.dec2 = nn.ConvTranspose2d(128, 64, kernel_size=3, padding=1)
        self.dec1 = nn.ConvTranspose2d(64, 1, kernel_size=3, padding=1)

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

    def predict_binary_mask(self, x, threshold=0.5):
        output = self.forward(x)

        binary_mask = (output > threshold).float()

        return binary_mask


model_path = "/data/typhon2/Rhys/adeventofcodemodel.pth"

# set device as cpu
device = torch.device("cpu")


print("Loading in model")
model = EncoderDecoder()
model.load_state_dict(torch.load(model_path, weights_only=True))


# let do inference on a single image
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt


print("Loading in data")
input = "input.txt"  # the advent of code input data.

# read the input data
with open(input, "r") as file:
    data = file.read().strip()

# convert X,M,A,S to 1,2,3,4
data = data.replace("X", "1").replace("M", "2").replace("A", "3").replace("S", "4")
# remove any newlines

# convert to array where '\n' is the delimiter for a new row and each character is a column

data = np.array([list(row) for row in data.split("\n")], dtype=np.float32)
print(data)
print(data.shape)

# reshape the data to (1, 1, 140, 140)
data = data.reshape(1, 1, 140, 140)

print("Doing inference")

# min max scaling
print(data.shape)

# min max scaling

# data_minmax = (data - np.min(data)) / (np.max(data) - np.min(data))

# convert to tensor
data_minmax = torch.tensor(data, dtype=torch.float32)

# send to device
data_minmax = data_minmax.to(device)

# do inference
model.eval()
with torch.no_grad():
    output = model.predict_binary_mask(data_minmax)

# convert to numpy
output = output.cpu().numpy()[0][0]

# undo min max scaling
output = output * (np.max(data) - np.min(data)) + np.min(data)
# plot the output
plt.imshow(output, cmap="viridis")
plt.colorbar()
plt.savefig("output.png")
print("Done")

# threshold the output
# output = np.where(output > , 1, 0)

plt.figure()
plt.hist(output.flatten())
plt.savefig("output_hist.png")
