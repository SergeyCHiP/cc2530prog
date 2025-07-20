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

# Check if gpiochip0 exists
if [ ! -e "/dev/gpiochip0" ]; then
    echo "❌ Error: /dev/gpiochip0 not found"
    echo "   Make sure you're running on Raspberry Pi with GPIO support"
    exit 1
fi

echo "✅ /dev/gpiochip0 found"
echo ""

# Check if libgpiod tools are available
if command -v gpioset >/dev/null 2>&1; then
    echo "✅ libgpiod tools available"
else
    echo "❌ libgpiod tools not found"
    echo "   Install with: sudo apt install gpiod"
    exit 1
fi

echo ""

# Test GPIO control using libgpiod
echo "Testing GPIO control with libgpiod..."

# Export and test RST
echo "  Testing RST (GPIO $RST_GPIO)..."
if gpioset gpiochip0 $RST_GPIO=0 2>/dev/null; then
    echo "    ✅ RST set to LOW"
    gpioset gpiochip0 $RST_GPIO=1 2>/dev/null
    echo "    ✅ RST set to HIGH"
else
    echo "    ❌ Failed to control RST"
fi

# Export and test CLK
echo "  Testing CLK (GPIO $CLK_GPIO)..."
if gpioset gpiochip0 $CLK_GPIO=0 2>/dev/null; then
    echo "    ✅ CLK set to LOW"
    gpioset gpiochip0 $CLK_GPIO=1 2>/dev/null
    echo "    ✅ CLK set to HIGH"
else
    echo "    ❌ Failed to control CLK"
fi

# Export and test DATA
echo "  Testing DATA (GPIO $DATA_GPIO)..."
if gpioset gpiochip0 $DATA_GPIO=0 2>/dev/null; then
    echo "    ✅ DATA set to LOW"
    gpioset gpiochip0 $DATA_GPIO=1 2>/dev/null
    echo "    ✅ DATA set to HIGH"
else
    echo "    ❌ Failed to control DATA"
fi

echo ""

# Show GPIO info
echo "GPIO Chip Information:"
gpioinfo gpiochip0 | head -20
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
echo ""
echo "4. For debugging, use:"
echo "   gpioinfo gpiochip0"
echo "   gpioget gpiochip0 $RST_GPIO $CLK_GPIO $DATA_GPIO" 