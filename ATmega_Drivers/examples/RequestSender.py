import serial
import time

# Configurare port serial
ser = serial.Serial(
    port='COM4',     
    baudrate=9600,
    timeout=1
)

time.sleep(2)

print("=== UART Request Console ===")
print("Write a request (ex: GET /msg) or 'exit' to quit.\n")

while True:
    request = input("Request > ").strip()
    if request.lower() == "exit":
        break

 
    ser.write((request + "\r\n").encode())

 
    response = ""
    while True:
        line = ser.readline().decode('utf-8', errors='ignore')
        if not line:
            break
        response += line

    print("\n--- Response ---")
    if response:
        print(response)
    else:
        print("(no response)")
    print("----------------------------\n")

ser.close()
print("Connection closed.")
