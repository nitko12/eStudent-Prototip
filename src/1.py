import serial
import time

ser = serial.Serial("/dev/cu.usbserial-1430", 57600)

if __name__ == "__main__":
    ser.write(b"1")
    time.sleep(0.07)

    ser.write(b"0")
    time.sleep(0.4)
    ser.write(b"0")
    time.sleep(0.6)

    ser.write(b"0")
    time.sleep(0.2)
    ser.write(b"2")
    time.sleep(0.07)
    ser.write(b"0")
    time.sleep(0.2)
    ser.write(b"3")
    time.sleep(0.07)
    ser.write(b"0")
    time.sleep(0.8)

    ser.write(b"0")
    time.sleep(0.2)
    ser.write(b"0")
    time.sleep(0.2)

    ser.write(b"2")
    time.sleep(0.07)
    ser.write(b"0")
    time.sleep(0.2)
    ser.write(b"0")
    time.sleep(0.2)

    ser.write(b"1")
    time.sleep(0.07)
    ser.write(b"0")
    time.sleep(0.3)

    ser.write(b"0")
    time.sleep(0.4)
    ser.write(b"0")
    time.sleep(0.4)
