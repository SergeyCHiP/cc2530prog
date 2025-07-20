#!/bin/bash

echo "=== CC2530 Debug Test ==="
echo "Detailed diagnostics for CC2530 connection"
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

# Check current GPIO states
echo "=== Current GPIO States ==="
echo "Reading current GPIO values..."
gpioget gpiochip0 $RST_GPIO $CLK_GPIO $DATA_GPIO
echo ""

# Test manual debug sequence
echo "=== Manual Debug Sequence Test ==="
echo "This simulates the exact sequence used by cc2530prog"
echo ""

echo "Step 1: Setting RST to LOW (active low)"
gpioset gpiochip0 $RST_GPIO=0
sleep 0.1

echo "Step 2: Sending 2 clock pulses"
for i in {1..2}; do
    echo "  Clock pulse $i: LOW → HIGH → LOW"
    gpioset gpiochip0 $CLK_GPIO=0
    sleep 0.01
    gpioset gpiochip0 $CLK_GPIO=1
    sleep 0.01
    gpioset gpiochip0 $CLK_GPIO=0
    sleep 0.01
done

echo "Step 3: Keeping CLK LOW"
gpioset gpiochip0 $CLK_GPIO=0
sleep 0.1

echo "Step 4: Setting RST to HIGH"
gpioset gpiochip0 $RST_GPIO=1
sleep 0.1

echo ""
echo "=== GPIO States After Debug Sequence ==="
gpioget gpiochip0 $RST_GPIO $CLK_GPIO $DATA_GPIO
echo ""

# Test DATA line as input
echo "=== Testing DATA Line as Input ==="
echo "Setting DATA to input mode and reading value..."

# Set DATA as input
gpioset gpiochip0 $DATA_GPIO=0 2>/dev/null
sleep 0.1

# Read DATA value
DATA_VALUE=$(gpioget gpiochip0 $DATA_GPIO 2>/dev/null | cut -d' ' -f1)
echo "DATA line value: $DATA_VALUE"

if [ "$DATA_VALUE" = "0" ]; then
    echo "✅ DATA line is LOW - CC2530 might be responding"
elif [ "$DATA_VALUE" = "1" ]; then
    echo "❌ DATA line is HIGH - CC2530 not responding"
else
    echo "⚠️  DATA line value unclear: $DATA_VALUE"
fi

echo ""

# Reset all GPIOs to safe state
echo "=== Resetting GPIOs to Safe State ==="
gpioset gpiochip0 $RST_GPIO=1 $CLK_GPIO=0 $DATA_GPIO=0
echo "All GPIOs reset"
echo ""

echo "=== Troubleshooting Guide ==="
echo "If CC2530 is not responding (DATA = HIGH):"
echo ""
echo "1. Check Physical Connections:"
echo "   - RST  → GPIO $RST_GPIO (Pin 11)"
echo "   - CLK  → GPIO $CLK_GPIO (Pin 13)"
echo "   - DATA → GPIO $DATA_GPIO (Pin 15)"
echo "   - GND  → GND"
echo "   - VCC  → 3.3V"
echo ""
echo "2. Check Power Supply:"
echo "   - CC2530 needs 3.3V (not 5V!)"
echo "   - Measure voltage at CC2530 VCC pin"
echo "   - Ensure stable power supply"
echo ""
echo "3. Check CC2530 State:"
echo "   - CC2530 should be powered but not running"
echo "   - No other debugger should be connected"
echo "   - CC2530 should not be in sleep mode"
echo ""
echo "4. Test with Multimeter:"
echo "   - Measure voltage at CC2530 pins during debug sequence"
echo "   - RST should go LOW then HIGH"
echo "   - CLK should pulse twice"
echo "   - DATA should respond to commands"
echo ""
echo "5. Try Different CC2530:"
echo "   - Test with another CC2530 chip"
echo "   - Some chips may be damaged or locked"
echo ""
echo "6. Run Full Test:"
echo "   ./cc2530prog -i -v" 