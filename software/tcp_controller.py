import socket
from pynput import keyboard
import threading
import time

# Connect to the ESP32's access point IP (default AP IP for ESP32)
ESP32_IP = "192.168.4.1"  # Default IP when ESP32 is in AP mode
ESP32_PORT = 8888

print(f"Connecting to bird-control hotspot at {ESP32_IP}:{ESP32_PORT}")
print("Make sure you've connected your computer to the 'bird-control' WiFi network")
print("WiFi Password: 12345678")

# Create socket connection
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
    sock.connect((ESP32_IP, ESP32_PORT))
    print("Connected successfully!")
except Exception as e:
    print(f"Connection failed: {e}")
    print("Please check if you're connected to the ESP32's WiFi hotspot")
    exit(1)

step = 100
wartosc = 10
laser_power = 0

def get_delta(val):
    if val < 10:
        return 1
    elif val < 100:
        return 10
    elif val < 1000:
        return 100
    else:
        return 1000

def adjust(val, inc):
    delta = get_delta(val + 1 if inc > 0 else val - 1)
    return clamp(val + inc * delta)

def clamp(val, min_val=0, max_val=99999):
    return max(min_val, min(val, max_val))

def read_socket():
    while True:
        try:
            data = sock.recv(1024).decode().strip()
            if data:
                print(f"Received: {data}")
        except Exception as e:
            print(f"Error reading from socket: {e}")
            break
        time.sleep(0.1)

def on_press(key):
    global step, wartosc, laser_power

    try:
        if key.char == 'a':
            step = adjust(step, 1)
            print(f"New step: {step}")
        elif key.char == 'z':
            step = adjust(step, -1)
            print(f"New step: {step}")
        elif key.char == 's':
            wartosc = adjust(wartosc, 1)
            print(f"New end value: {wartosc}")
        elif key.char == 'x':
            wartosc = adjust(wartosc, -1)
            print(f"New end value: {wartosc}")
        elif key.char == 'd':
            laser_power = clamp(laser_power + 1, 0, 255)
            print(f"New laser power: {laser_power}")
        elif key.char == 'c':
            laser_power = clamp(laser_power - 1, 0, 255)
            print(f"New laser power: {laser_power}")
    except AttributeError:
        a, b = 0, 0
        if key == keyboard.Key.right:
            a = step
        elif key == keyboard.Key.left:
            a = -step
        elif key == keyboard.Key.up:
            b = step
        elif key == keyboard.Key.down:
            b = -step
        elif key == keyboard.Key.space:
            msg = f"2,{laser_power}\n"
            sock.sendall(msg.encode())
            print(f"Sent: {msg.strip()}")
            return
        elif key == keyboard.Key.esc:
            # Exit program
            sock.close()
            exit(0)
        else:
            return

        msg = f"0,{a},{b},{wartosc}\n"
        sock.sendall(msg.encode())
        print(f"Sent: {msg.strip()}")

print("TCP Controller started. Press ESC to exit.")
print("Controls:")
print("  Arrow keys: Move stepper motors")
print("  Space: Toggle laser")
print("  A/Z: Increase/decrease step size")
print("  S/X: Increase/decrease target value")
print("  D/C: Increase/decrease laser power")

threading.Thread(target=read_socket, daemon=True).start()
keyboard.Listener(on_press=on_press).run()