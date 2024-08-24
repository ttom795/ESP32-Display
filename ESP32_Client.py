import serial
import mss
import cv2
import numpy as np

ser = serial.Serial("/dev/ttyUSB0", 3000000, timeout=1)
sct = mss.mss()

def resize_image():
    monitor = sct.monitors[1]
    img = sct.grab(monitor)
    img_np = np.array(img)

    img_rgb = cv2.cvtColor(img_np, cv2.COLOR_BGRA2BGR)
    img = cv2.resize(img_rgb, (320, 170),interpolation=cv2.INTER_AREA)
    encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 40]
    buf = cv2.imencode('.jpeg', img, encode_param)[1].tobytes()
    
    data_size = len(buf)
    print(data_size)
    FIXED_SIZE = 10000
    if data_size > FIXED_SIZE:
        padded_buf = buf[:FIXED_SIZE]
    else:
        padded_buf = bytearray(FIXED_SIZE)
        padded_buf[:data_size] = buf
    ser.write(padded_buf)
    
while True:
    resize_image()


