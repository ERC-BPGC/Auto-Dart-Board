import serial
import time
import random

# Configure Serial Port (Replace 'COMx' with your actual port)
ser = serial.Serial('/dev/tty.usbmodem28064601', 115200, timeout=1)

# Define coordinate limits
X_MIN, X_MAX = 0, 10  # Change these as per actual limits
Y_MIN, Y_MAX = 0, 12

# Track current position (starts at 0, 0)
current_x, current_y = 0, 0

# Send motor commands with smooth speed adjustments
def send_smooth_motion(Vdirection, Hdirection, max_speed, max_accel):
    global current_x, current_y
    
    # Ensure the new position stays within limits
    if X_MIN <= Hdirection <= X_MAX and Y_MIN <= Vdirection <= Y_MAX:
        command = f"{Vdirection} {Hdirection} {max_speed} {max_accel}\n"
        ser.write(command.encode())
        print(f"Moving to: ({Hdirection}, {Vdirection})")
        wait()  # Wait for the move to complete
        current_x, current_y = Hdirection, Vdirection  # Update current position
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
    
    # Move to home position first (assuming home is at 100, 100)
    send_smooth_motion(2, 2, max_speed=15000, max_accel=2500)
    current_x, current_y = 2, 2  # Reset tracked position

    while True:
        # Ask the user for input
        print("\nChoose an option:")
        print("1. Generate random coordinates")
        print("2. Manually input coordinates")
        print("3. Exit")
        choice = input("Enter your choice (1, 2, or 3): ")

        if choice == "1":
            # Generate a random coordinate within limits
            new_x = random.randint(X_MIN, X_MAX)
            new_y = random.randint(Y_MIN, Y_MAX)

            # Move to the new coordinate
            send_smooth_motion(new_y, new_x, max_speed=15000, max_accel=2500)

            # Return to home position
           # send_smooth_motion(5, 8, max_speed=15000, max_accel=2500)

        elif choice == "2":
            # Manually input coordinates
            try:
                new_x = int(input(f"Enter X coordinate ({X_MIN} to {X_MAX}): "))
                new_y = int(input(f"Enter Y coordinate ({Y_MIN} to {Y_MAX}): "))

                # Move to the new coordinate
                send_smooth_motion(new_y, new_x, max_speed=15000, max_accel=2500)

                # Return to home position
               # send_smooth_motion(5, 8, max_speed=15000, max_accel=2500)
            except ValueError:
                print("Invalid input. Please enter numeric values.")

        elif choice == "3":
            print("Exiting program.")
            break

        else:
            print("Invalid choice. Please enter 1, 2, or 3.")

if __name__ == "__main__":
    main()