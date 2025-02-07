import serial
import time
from pynput.keyboard import Key, Listener
import random
# 15 x 18 space
# 62 x 73 cm space
# Center: (8.5, 15)
# Configure Serial Port (Replace 'COMx' with your actual port)
ser = serial.Serial('COM7', 115200, timeout=1)

# Define coordinate limits
X_MIN, X_MAX = 0, 15  # Change these as per actual limits
Y_MIN, Y_MAX = 0, 18

# Track current position (starts at 0, 0)
current_x, current_y = 0, 0

# Send motor commands with smooth speed adjustments
def send_smooth_motion(Hdirection, Vdirection, max_speed, max_accel):
    global current_x, current_y
    
    # Ensure the new position stays within limits
    if X_MIN <= Hdirection <= X_MAX and Y_MIN <= Vdirection <= Y_MAX:
        if(abs(Hdirection - current_x) <=5):
            max_accel = 5000
        command = f"{Hdirection} {Vdirection} {max_speed} {max_accel}\n"
        ser.write(command.encode())
        print(f"Moving to: ({Hdirection}, {Vdirection})")
        current_x, current_y = Hdirection, Vdirection  # Update current position
    elif Hdirection == 100 and Vdirection == 100:
        command = f"{Hdirection} {Vdirection} {max_speed} {max_accel}\n"
        ser.write(command.encode())
    elif Hdirection == 200 and Vdirection == 200:
        command = f"{Hdirection} {Vdirection} {max_speed} {max_accel}\n"
        ser.write(command.encode())
    else:
        print(f"Skipped invalid move to ({Hdirection}, {Vdirection}) - Out of bounds")

def wait():
    timeout = time.time() + 2 # 2-second timeout
    while True:
        response = ser.readline().decode().strip()
        if response.lower() == "done":
            break  # Move to next step
        if time.time() > timeout:   
            print("Warning: No response from Teensy, moving to next step.")
            break  # Prevent infinite loop


def main():
    global current_x, current_y
    tries = 1
    #Move to home position first (assuming home is at 100, 100)
    send_smooth_motion(200, 200, max_speed=15000, max_accel=12500)
    wait()
    current_x, current_y = 8.5, 15  # Reset tracked position

    while (tries <= 5):
            # Generate a random coordinate within limits
            new_x = random.randint(X_MIN, X_MAX)
            new_y = random.randint(Y_MIN, Y_MAX)

            # Move to the new coordinate
            send_smooth_motion(new_x, new_y, max_speed=15000, max_accel=12500)
            wait()
            current_x = new_x
            current_y = new_y
            time.sleep(1.5)

            # Return to home position
            send_smooth_motion(100, 100, max_speed=15000, max_accel=12500)
            wait()
            time.sleep(2)
            tries = tries + 1

if __name__ == "__main__":
    main()