#!/bin/bash

echo "=== GPIO Debug Script for Raspberry Pi 5 ==="
echo "This script helps debug GPIO issues"
echo "Testing GPIO: 17, 27, 22"
echo ""

# Check if running as root
if [ "$EUID" -ne 0 ]; then
    echo "❌ This script should be run as root (use sudo)"
    exit 1
fi

echo "✅ Running as root"

# Check GPIO sysfs interface
echo ""
echo "=== Checking GPIO Sysfs Interface ==="
if [ -d "/sys/class/gpio" ]; then
    echo "✅ /sys/class/gpio exists"
    ls -la /sys/class/gpio/
else
    echo "❌ /sys/class/gpio does not exist"
fi

# Check gpiochip
echo ""
echo "=== Checking GPIO Chips ==="
if [ -d "/sys/class/gpio/gpiochip512" ]; then
    echo "✅ gpiochip512 exists"
    echo "Base: $(cat /sys/class/gpio/gpiochip512/base 2>/dev/null || echo 'unknown')"
    echo "NGPIO: $(cat /sys/class/gpio/gpiochip512/ngpio 2>/dev/null || echo 'unknown')"
    echo "Label: $(cat /sys/class/gpio/gpiochip512/label 2>/dev/null || echo 'unknown')"
else
    echo "❌ gpiochip512 does not exist"
fi

# Check libgpiod
echo ""
echo "=== Checking libgpiod ==="
if command -v gpioinfo &> /dev/null; then
    echo "✅ gpioinfo command available"
    echo "GPIO Info:"
    gpioinfo | head -20
else
    echo "❌ gpioinfo command not available"
    echo "Install with: sudo apt install gpiod"
fi

# Test GPIO export manually
echo ""
echo "=== Testing Manual GPIO Export ==="
for gpio in 17 27 22; do
    echo "Testing GPIO $gpio..."
    if echo $gpio > /sys/class/gpio/export 2>&1; then
        echo "✅ GPIO $gpio exported successfully"
        echo $gpio > /sys/class/gpio/unexport 2>&1
    else
        echo "❌ Failed to export GPIO $gpio"
    fi
done

# Check for libgpiod development files
echo ""
echo "=== Checking libgpiod Development Files ==="
if [ -f "/usr/include/gpiod.h" ]; then
    echo "✅ libgpiod headers found"
else
    echo "❌ libgpiod headers not found"
    echo "Install with: sudo apt install libgpiod-dev"
fi

# Check pkg-config
echo ""
echo "=== Checking pkg-config ==="
if pkg-config --exists libgpiod; then
    echo "✅ libgpiod found via pkg-config"
    echo "CFLAGS: $(pkg-config --cflags libgpiod)"
    echo "LIBS: $(pkg-config --libs libgpiod)"
else
    echo "❌ libgpiod not found via pkg-config"
fi

echo ""
echo "=== Debug Script Complete ==="
echo "If you see any ❌ errors, fix them before building the project" 