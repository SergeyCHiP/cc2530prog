#!/bin/bash

echo "=== GPIO Loopback Test ==="
echo "Testing GPIO connections without CC2530"
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

echo "=== Loopback Test Instructions ==="
echo "1. Disconnect CC2530 from GPIO pins"
echo "2. Connect GPIO pins directly:"
echo "   - GPIO $RST_GPIO → GPIO $DATA_GPIO (RST to DATA)"
echo "   - GPIO $CLK_GPIO → GPIO $DATA_GPIO (CLK to DATA)"
echo "3. Run this test to verify GPIO functionality"
echo ""

read -p "Press Enter when ready to test..."

echo ""
echo "=== Testing RST → DATA Loopback ==="

# Test RST to DATA loopback
echo "Setting RST HIGH, reading DATA..."
gpioset gpiochip0 $RST_GPIO=1
sleep 0.1
DATA_VALUE=$(gpioget gpiochip0 $DATA_GPIO 2>/dev/null | cut -d' ' -f1)
echo "DATA value when RST=HIGH: $DATA_VALUE"

echo "Setting RST LOW, reading DATA..."
gpioset gpiochip0 $RST_GPIO=0
sleep 0.1
DATA_VALUE=$(gpioget gpiochip0 $DATA_GPIO 2>/dev/null | cut -d' ' -f1)
echo "DATA value when RST=LOW: $DATA_VALUE"

echo ""
echo "=== Testing CLK → DATA Loopback ==="

# Test CLK to DATA loopback
echo "Setting CLK HIGH, reading DATA..."
gpioset gpiochip0 $CLK_GPIO=1
sleep 0.1
DATA_VALUE=$(gpioget gpiochip0 $DATA_GPIO 2>/dev/null | cut -d' ' -f1)
echo "DATA value when CLK=HIGH: $DATA_VALUE"

echo "Setting CLK LOW, reading DATA..."
gpioset gpiochip0 $CLK_GPIO=0
sleep 0.1
DATA_VALUE=$(gpioget gpiochip0 $DATA_GPIO 2>/dev/null | cut -d' ' -f1)
echo "DATA value when CLK=LOW: $DATA_VALUE"

echo ""
echo "=== Reset GPIOs ==="
gpioset gpiochip0 $RST_GPIO=0 $CLK_GPIO=0 $DATA_GPIO=0
echo "All GPIOs reset to LOW"

echo ""
echo "=== Test Complete ==="
echo "If DATA values change when RST/CLK change, GPIO is working"
echo "If DATA values don't change, check connections or GPIO setup" 