# CC2530 Programming Setup Guide

## 🎯 Overview

This guide helps you set up and use the CC2530 programming tool on Raspberry Pi 5 with libgpiod.

## 📋 Prerequisites

- Raspberry Pi 5 (or compatible)
- CC2530 chip
- libgpiod installed: `sudo apt install gpiod libgpiod-dev`
- Wiring tools (breadboard, jumper wires)

## 🔧 Hardware Setup

### GPIO Pin Configuration
```
CC2530 Pin    →  Raspberry Pi 5 Pin
─────────────────────────────────────
RST (Reset)   →  GPIO 17 (Pin 11)
CLK (Clock)   →  GPIO 27 (Pin 13)  
DATA (Data)   →  GPIO 22 (Pin 15)
GND           →  GND
VCC           →  3.3V (IMPORTANT: not 5V!)
```

### Physical Pin Layout
```
Raspberry Pi 5 Header:
┌─────────────────────────────────┐
│ 1  3V3    2  5V    3  GPIO2  │
│ 4  5V     5  GND   6  GPIO3  │
│ 7  GPIO4  8  GPIO14 9  GND   │
│ 10 GPIO15 11 GPIO17 12 GPIO18 │ ← RST
│ 13 GPIO27 14 GND   15 GPIO22 │ ← CLK, DATA
│ 16 GPIO23 17 3V3   18 GPIO24 │
│ 19 GPIO10 20 GND   21 GPIO9  │
│ 22 GPIO25 23 GPIO11 24 GPIO8 │
│ 25 GND    26 GPIO7  27 GPIO0 │
│ 28 GPIO1  29 GPIO5  30 GND   │
│ 31 GPIO6  32 GPIO12 33 GPIO13│
│ 34 GND    35 GPIO19 36 GPIO16│
│ 37 GPIO26 38 GPIO20 39 GND   │
│ 40 GPIO21                     │
└─────────────────────────────────┘
```

## 🚀 Quick Start

### 1. Build the Project
```bash
make clean
make
```

### 2. Test GPIO Functionality
```bash
./test_connection.sh
```

### 3. Test with CC2530
```bash
./cc2530prog -i -v
```

## 🔍 Troubleshooting

### If CC2530 Not Responding (Chip ID = 0xFF)

#### Step 1: Check Physical Connections
```bash
./test_connection.sh
```
Verify all GPIO pins are working.

#### Step 2: Detailed Diagnostics
```bash
./debug_cc2530.sh
```
This runs the exact debug sequence and checks DATA line response.

#### Step 3: Loopback Test (without CC2530)
```bash
./test_loopback.sh
```
Connect GPIO pins directly to verify hardware functionality.

### Common Issues

#### 1. Power Supply Issues
- **Problem**: CC2530 needs exactly 3.3V, not 5V
- **Solution**: Use 3.3V pin, measure voltage at CC2530 VCC pin
- **Test**: `gpioget gpiochip0 17 27 22`

#### 2. Connection Issues
- **Problem**: Loose or incorrect wiring
- **Solution**: Double-check all connections
- **Test**: Use multimeter to verify continuity

#### 3. CC2530 State Issues
- **Problem**: CC2530 locked or damaged
- **Solution**: Try different CC2530 chip
- **Test**: Check if chip responds to manual debug sequence

#### 4. GPIO Resource Conflicts
- **Problem**: "Device or resource busy"
- **Solution**: Reboot Raspberry Pi
- **Test**: `gpioinfo gpiochip0`

## 📖 Usage Examples

### Identify CC2530
```bash
./cc2530prog -i -v
```
Expected output:
```
Texas Instruments CC2530 (ID: 0xA5, rev 0xXX)
Flash size: XXX KB
```

### Program Firmware
```bash
./cc2530prog -f CC2530ZNP-Prod.hex -v
```

### Program with Verification
```bash
./cc2530prog -f CC2530ZNP-Prod.hex -r -v
```

### Single Commands
```bash
./cc2530prog -c "get_chip_id" -v
./cc2530prog -c "read_status" -v
```

### List Available Commands
```bash
./cc2530prog -l
```

## 🛠️ Advanced Debugging

### Manual GPIO Control
```bash
# Set GPIO values
gpioset gpiochip0 17=1 27=0 22=0

# Read GPIO values
gpioget gpiochip0 17 27 22

# Get GPIO info
gpioinfo gpiochip0
```

### Debug Sequence Manual Test
```bash
# 1. Set RST LOW
gpioset gpiochip0 17=0

# 2. Send 2 clock pulses
gpioset gpiochip0 27=0; sleep 0.01; gpioset gpiochip0 27=1; sleep 0.01; gpioset gpiochip0 27=0
gpioset gpiochip0 27=0; sleep 0.01; gpioset gpiochip0 27=1; sleep 0.01; gpioset gpiochip0 27=0

# 3. Keep CLK LOW
gpioset gpiochip0 27=0

# 4. Set RST HIGH
gpioset gpiochip0 17=1

# 5. Read DATA
gpioget gpiochip0 22
```

## 📊 Expected Results

### Successful CC2530 Identification
```
[DEBUG] Chip ID response: 0xa5 0xXX
Texas Instruments CC2530 (ID: 0xA5, rev 0xXX)
Flash size: XXX KB
```

### Failed CC2530 Identification
```
[DEBUG] Chip ID response: 0xff 0xff
unknown Chip ID: ff
someone is holding the CLK/DATA lines against us
```

## 🔧 Technical Details

### Debug Sequence
The CC2530 enters debug mode with this sequence:
1. **RST → LOW** (active low reset)
2. **CLK → 2 pulses** (0→1→0→1)
3. **CLK → LOW** (hold clock low)
4. **RST → HIGH** (release reset)

### GPIO Configuration
- **RST**: Active low, output
- **CLK**: Output, used for timing
- **DATA**: Bidirectional, changes direction during communication

### Communication Protocol
- **Commands**: Sent MSB first on rising clock edge
- **Responses**: Read MSB first on falling clock edge
- **Timing**: Critical for reliable communication

## 📝 Notes

- CC2530 requires 3.3V power supply
- Debug interface is sensitive to timing
- Physical connections must be solid
- Some CC2530 chips may be locked or damaged
- Always verify power supply before testing

## 🆘 Getting Help

If you're still having issues:

1. **Check hardware**: Use multimeter to verify connections
2. **Test GPIO**: Run loopback test without CC2530
3. **Verify power**: Measure voltage at CC2530 VCC pin
4. **Try different chip**: Some CC2530 chips may be defective
5. **Check timing**: Debug sequence timing is critical

## 📚 References

- [CC2530 Datasheet](http://www.ti.com/lit/ds/symlink/cc2530.pdf)
- [libgpiod Documentation](https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/about/)
- [Raspberry Pi GPIO Documentation](https://www.raspberrypi.org/documentation/usage/gpio/) 