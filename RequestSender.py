import serial
import time

# Configurare port serial
ser = serial.Serial(
    port='COM3',       # schimbă cu portul tău
    baudrate=9600,
    timeout=1
)

time.sleep(2)  # așteptare să se inițializeze Arduino/ATmega

# Trimite cererea HTTP-like
request = "GET /msg\r\n"
ser.write(request.encode())

# Citește răspunsul
response = ""
while True:
    line = ser.readline().decode('utf-8', errors='ignore')
    if not line:
        break
    response += line

print("Response from device:")
print(response)

ser.close()
