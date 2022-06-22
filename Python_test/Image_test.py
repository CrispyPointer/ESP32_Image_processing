import numpy as np
import matplotlib.pyplot as plt

rows = 2
columns = 2

#Read raw pixel from txt file
with open("image_data_(low_pass)_1.txt") as f:
    str_img_test = f.read()
with open("image_data_(laplacian)_1.txt") as f:
    str_img_test_2 = f.read()

#Convert strings to  numpy array with "comma separate"
img_low = np.array(str_img_test.split(",")[:-1], dtype=np.uint8)
img_low = img_low.reshape(239, 318)

img_high = np.array(str_img_test_2.split(",")[:-1], dtype=np.uint8)
img_high = img_high.reshape(239, 318)

#Stack the raw picture to a 3D array (For gray-scale in this case!)
gray_img_low = np.stack((img_low,)*3, axis=-1)
gray_img_high = np.stack((img_high,)*3, axis=-1)

print(img_low,'\n')
print(gray_img_low)

#Plot the figure
fig = plt.figure(figsize=(7,5))

fig.add_subplot(rows, columns, 1)
plt.imshow(gray_img_low)
plt.title("Low pass filter")

fig.add_subplot(rows, columns, 2)
plt.imshow(gray_img_high)
plt.title("High pass filter")

plt.show()