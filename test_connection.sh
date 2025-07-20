#!/bin/bash

echo "=== CC2530 Connection Test ==="
echo "Checking GPIO pins for CC2530 connection..."
echo ""

# GPIO pins for CC2530
RST_GPIO=17
CLK_GPIO=27
DATA_GPIO=22

echo "GPIO Configuration:"
echo "  RST  (Reset): GPIO $RST_GPIO (Pin 11)"
echo "  CLK  (Clock): GPIO $CLK_GPIO (Pin 13)"
echo "  DATA (Data):  GPIO $DATA_GPIO (Pin 15)"
echo ""

# Check if GPIOs are available
echo "Checking GPIO availability..."
for gpio in $RST_GPIO $CLK_GPIO $DATA_GPIO; do
    if [ -d "/sys/class/gpio/gpio$gpio" ]; then
        echo "  GPIO $gpio: Already exported"
    else
        echo "  GPIO $gpio: Available"
    fi
done
echo ""

# Test GPIO control
echo "Testing GPIO control..."
echo "  Setting RST to LOW..."
echo 0 > /sys/class/gpio/gpio$RST_GPIO/value 2>/dev/null && echo "    ✓ RST set to LOW" || echo "    ✗ Failed to set RST"

echo "  Setting CLK to LOW..."
echo 0 > /sys/class/gpio/gpio$CLK_GPIO/value 2>/dev/null && echo "    ✓ CLK set to LOW" || echo "    ✗ Failed to set CLK"

echo "  Setting DATA to LOW..."
echo 0 > /sys/class/gpio/gpio$DATA_GPIO/value 2>/dev/null && echo "    ✓ DATA set to LOW" || echo "    ✗ Failed to set DATA"
echo ""

echo "=== Connection Instructions ==="
echo "1. Connect CC2530 pins to Raspberry Pi:"
echo "   - RST  → GPIO $RST_GPIO (Pin 11)"
echo "   - CLK  → GPIO $CLK_GPIO (Pin 13)"
echo "   - DATA → GPIO $DATA_GPIO (Pin 15)"
echo "   - GND  → GND"
echo "   - VCC  → 3.3V"
echo ""
echo "2. Run the program:"
echo "   ./cc2530prog -i -v"
echo ""
echo "3. If chip ID is 0xFF, check:"
echo "   - Physical connections"
echo "   - Power supply (3.3V)"
echo "   - CC2530 is not damaged"
echo "   - No other debugger connected" 