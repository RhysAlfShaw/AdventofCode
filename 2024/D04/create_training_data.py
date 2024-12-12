import numpy as np
from tqdm import tqdm


def inject_sequences(images, n_sequences=4):
    # Spread out sequence

    sequence1 = [1, 2, 3, 4]
    sequence2 = [4, 3, 2, 1]

    # Diagonal patterns (already spread out)
    diagonals = [
        np.array([[1, 0, 0, 0], [0, 2, 0, 0], [0, 0, 3, 0], [0, 0, 0, 4]]),
        np.array([[0, 0, 0, 4], [0, 0, 3, 0], [0, 2, 0, 0], [1, 0, 0, 0]]),
        np.array([[0, 0, 0, 1], [0, 0, 2, 0], [0, 3, 0, 0], [4, 0, 0, 0]]),
        np.array([[4, 0, 0, 0], [0, 3, 0, 0], [0, 0, 2, 0], [0, 0, 0, 1]]),
    ]

    def inject_diagonal(image, mask, diagonal, x, y):
        for i in range(4):
            if diagonal[i, i] != 0:
                image[x + i, y + i] = diagonal[i, i]
                mask[x + i, y + i] = 1

    # Masks for tracking sequence injections
    masks = np.zeros_like(images)

    numberof_inkections_perimage_per_sequence = n_sequences

    for i in tqdm(
        range(images.shape[0]), desc="Injecting sequences", total=images.shape[0]
    ):

        # for each sequence we want to inject into the image
        # sequence 1

        for _ in range(numberof_inkections_perimage_per_sequence):
            x = np.random.randint(0, images.shape[1] - 4)
            y = np.random.randint(0, images.shape[2] - 4)
            images[i, x, y : y + 4] = sequence1
            masks[i, x, y : y + 4] = 1

        # sequence 2
        for _ in range(numberof_inkections_perimage_per_sequence):
            x = np.random.randint(0, images.shape[1] - 4)
            y = np.random.randint(0, images.shape[2] - 4)
            images[i, x : x + 4, y] = sequence2
            masks[i, x : x + 4, y] = 1

        # Diagonals
        for _ in range(numberof_inkections_perimage_per_sequence):
            x = np.random.randint(0, images.shape[1] - 4)
            y = np.random.randint(0, images.shape[2] - 4)
            inject_diagonal(
                images[i], masks[i], diagonals[np.random.randint(0, 4)], x, y
            )

    return images, masks


# Set dimensions and number of images
dims = 140
num_images = 100_000

# create images with 0s
print("Creating Empty images")
images = np.zeros((num_images, dims, dims))

print("Creating Empty masks")
# Inject sequences
images, masks = inject_sequences(images, n_sequences=100)

# add noise which is a random number for each pixel between 1 and 4.

noise = np.random.randint(1, 5, size=images.shape)
# apply mask so that noise is only applied to pixels that are a 0
images = np.where(images == 0, noise, images)

print(images.shape)
print(images[0])

import matplotlib.pyplot as plt

plt.imshow(images[0], cmap="viridis")
plt.colorbar()
plt.savefig("images.png")

plt.imshow(masks[0], cmap="viridis")
plt.colorbar()
plt.savefig("masks.png")
print(masks.shape)
print(masks[0])

# save images and masks
np.save("/data/typhon2/Rhys/d4_images.npy", images)
np.save("/data/typhon2/Rhys/d4_masks.npy", masks)
