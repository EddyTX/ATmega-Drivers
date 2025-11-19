import serial
import time
import socket

# --- CONFIGURARE ---
SERIAL_PORT = 'COM4'       # <--- Verifică portul!
BAUD_RATE = 9600
AUTHORIZED_HOST = "Siemens-Console" # Parola de securitate
# -------------------

def print_help():
    print("\n=== CLI COMMAND TEMPLATES ===")
    print(" 1. PWM Control:      POST /pwm/set/<channel>/<duty>")
    print("                      (channel: 0=D6, 1=D5 | duty: 0-255)")
    print(" 2. GPIO Toggle:      POST /gpio/toggle/<port>/<pin>")
    print(" 3. GPIO Set Level:   POST /gpio/set/<port>/<pin>/<level>")
    print("                      (level: high/low)")
    print(" 4. ADC Read:         GET /adc/read/<channel>")
    print(" 5. System Info:      GET /timer/elapsed")
    print(" 6. Security Pair:    pair  (sau POST /admin/pair)")
    print(" 7. Local Commands:   help, exit")
    print("=============================\n")

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
except Exception as e:
    print(f"Error opening serial port: {e}")
    exit()

time.sleep(2) # Așteptăm resetarea Arduino

my_pc_name = socket.gethostname()
print(f"\n[CONNECTED] Client: '{my_pc_name}' -> Authorized as: '{AUTHORIZED_HOST}'")
print_help()

while True:
    cmd = input("Cmd > ").strip()
    
    if cmd.lower() == "exit":
        break
    
    if cmd.lower() == "help":
        print_help()
        continue
    
    # Shortcut pentru pairing (ca să nu scrii tot URL-ul lung)
    if cmd.lower() == "pair":
        final_cmd = "POST /admin/pair"
    else:
        final_cmd = cmd

    # Construim request-ul HTTP/1.1 Industrial
    # Adăugăm automat Header-ul de Securitate Host
    request = f"{final_cmd} HTTP/1.1\r\nHost: {AUTHORIZED_HOST}\r\n\r\n"

    try:
        # Trimitem
        # print(f"[TX] {final_cmd}...") # Uncomment pentru debug
        ser.write(request.encode())

        # Citim răspunsul
        response = ""
        while True:
            line = ser.readline().decode('utf-8', errors='ignore')
            if not line: break
            response += line
        
        # Afișăm doar body-ul răspunsului sau tot, depinde cum preferi.
        # Aici afișăm tot răspunsul brut de la server.
        print(f"\n{response.strip()}\n")
        
    except Exception as e:
        print(f"Communication Error: {e}")
        break

ser.close()