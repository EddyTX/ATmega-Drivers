import serial
import time

# Configurare port serial
ser = serial.Serial(
    port='COM3',
    baudrate=9600,
    timeout=1
)

time.sleep(2)


request = "GET /msg\r\n"
ser.write(request.encode())


response = ""
while True:
    line = ser.readline().decode('utf-8', errors='ignore')
    if not line:
        break
    response += line

print("Response from device:")
print(response)

ser.close()
