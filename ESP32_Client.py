import serial
from PIL import Image
import io
import mss
import time

counter = 0
fps_counter = 0
start_time = time.time()

ser = serial.Serial("/dev/ttyUSB0", 3000000, timeout=1)

def send_image_to_esp32():
    global counter, fps_counter, start_time
    with mss.mss() as sct:
        monitor = sct.monitors[1]
        img = sct.grab(monitor)
        img = Image.frombytes('RGB', img.size, img.rgb)
    img.thumbnail((320,170))

    # Convert image to JPEG format with specified quality
    with io.BytesIO() as buffer:
        img.save(buffer, format='JPEG', optimize=True, quality=30)
        
        jpeg_buf = buffer.getvalue()
        data_size = len(jpeg_buf)

    FIXED_SIZE = 6144
    padded_buf = bytearray(FIXED_SIZE)
    padded_buf[:data_size] = jpeg_buf
    
    ser.write(padded_buf)
    counter += 1
    fps_counter += 1
    

    current_time = time.time()
    if current_time - start_time >= 1.0:
        fps = fps_counter / (current_time - start_time)
        print(f"FPS: {fps:.2f}")
        print(f"Last image sent: frame {counter}, {data_size} bytes")
        fps_counter = 0
        start_time = current_time

while True:
    send_image_to_esp32()
