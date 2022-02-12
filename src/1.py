import serial
import time

ser = serial.Serial("/dev/cu.usbserial-1440", 57600)

l1 = [1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0]  # odsviraj
l3 = [0.5, 0.75, 0.25, 0.25, 1, 0.25, 0.25,
      0.25, 0.25, 0.25, 0.5, 0.5]  # čekaj

while True:
    for x, t in zip(l1, l3):
        if x:
            ser.write(b"1")
        time.sleep(0.07)

        ser.write(b"3")
        time.sleep(0.05)

        time.sleep(t*.5)

    ser.write(b"1")
    time.sleep(0.05)
    ser.write(b"1")
    time.sleep(0.05)
    ser.write(b"1")
    time.sleep(0.05)

    time.sleep(0.5)
