#!/bin/bash

echo "ESP32 Mode Monitor - Starting..."
echo "Timestamp: $(date)"
echo "=================================="

# Function to check device status
check_status() {
    local timestamp=$(date '+%H:%M:%S.%3N')
    
    # Check for ttyACM devices
    if ls /dev/ttyACM* >/dev/null 2>&1; then
        echo "[$timestamp] UART MODE: $(ls /dev/ttyACM*)"
    else
        echo "[$timestamp] UART MODE: No ttyACM devices"
    fi
    
    # Check for ttyUSB devices (JTAG mode)
    if ls /dev/ttyUSB* >/dev/null 2>&1; then
        echo "[$timestamp] JTAG MODE: $(ls /dev/ttyUSB*)"
    else
        echo "[$timestamp] JTAG MODE: No ttyUSB devices"
    fi
    
    # Check USB devices
    local usb_devices=$(lsusb | grep -i esp)
    if [ -n "$usb_devices" ]; then
        echo "[$timestamp] USB: $usb_devices"
    else
        echo "[$timestamp] USB: No ESP devices detected"
    fi
    
    echo "---"
}

# Initial check
check_status

# Monitor loop
while true; do
    sleep 0.5
    check_status
done
