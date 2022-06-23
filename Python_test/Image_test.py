from socket import timeout
import serial
import time
import numpy as np
import matplotlib.pyplot as plt

ser = serial.Serial(
    port = 'COM12', 
    baudrate = 115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout = 1
)
ser.setDTR(False)
ser.setRTS(False)
time.sleep(2)

print("connected to: " + ser.portstr)
while(1):
    # Reading all bytes available bytes till EOL
    line = ser.readline() 
    if line:
        # Converting Byte Strings into unicode strings
        string = line.decode()
        print(string)  
        if string == "Sending raw data: \r\n":
            line=ser.readline()
            str_img_test_0 = line.decode()
            print(str_img_test_0)
        elif string == "Sending low_filter data\r\n":
            line=ser.readline()
            str_img_test_1 = line.decode()
            print(str_img_test_1)
        elif string == "Sending gaussian_filter data\r\n":
            line=ser.readline()
            str_img_test_2 = line.decode()
            print(str_img_test_2)
        elif string == "Sending laplacian_filter data\r\n":
            line=ser.readline()
            str_img_test_3 = line.decode()
            print(str_img_test_3)
        elif (string == "End process!!!\r\n"):
            ser.close()
            break

rows = 2
columns = 2

#Read raw pixel and filtered pixel from txt file
# with open("image_raw_320x240.txt") as f:
#     str_img_test_0 = f.read()
# with open("image_data_(low_pass)_1.txt") as f:
#     str_img_test_1 = f.read()
# with open("image_data_(laplacian)_1.txt") as f:
#     str_img_test_2 = f.read()

#Convert strings to  numpy array with "comma separate"
img_raw = np.array(str_img_test_0.split(",")[:-1], dtype=np.uint8)
img_raw = img_raw.reshape(240, 320)

img_low = np.array(str_img_test_1.split(",")[:-1], dtype=np.uint8)
img_low = img_low.reshape(239, 318)

img_gau = np.array(str_img_test_2.split(",")[:-1], dtype=np.uint8)
img_gau = img_gau.reshape(239, 318)

img_high = np.array(str_img_test_3.split(",")[:-1], dtype=np.uint8)
img_high = img_high.reshape(239, 318)

#Stack the raw picture to a 3D array (For gray-scale in this case!)
gray_img_raw = np.stack((img_raw,)*3, axis=-1)
gray_img_low = np.stack((img_low,)*3, axis=-1)
gray_img_gau = np.stack((img_gau,)*3, axis=-1)
gray_img_high = np.stack((img_high,)*3, axis=-1)

# print(img_low,'\n')
# print(gray_img_low)

#Plot the figure
fig = plt.figure(figsize=(10,7))

fig.add_subplot(rows, columns, 1)
plt.imshow(gray_img_raw)
plt.title("Raw image")

fig.add_subplot(rows, columns, 2)
plt.imshow(gray_img_low)
plt.title("Low pass filter")

fig.add_subplot(rows, columns, 3)
plt.imshow(gray_img_gau)
plt.title("Gaussian filter")

fig.add_subplot(rows, columns, 4)
plt.imshow(gray_img_high)
plt.title("High pass filter")

plt.show()