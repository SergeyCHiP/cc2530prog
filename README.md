# cc2530prog - Texas Instruments CC2530 Microcontroller Programming Utility

## 1. General Information

This utility uses the CC2530 Debug Port to program the microcontroller. The
specific details of this interface are described in the following documents:
- [swru191b](http://www.ti.com/lit/swru191)
- [swra124](http://www.ti.com/lit/ug/swra124/swra124.pdf)

The hardware is programmed using 3 GPIO pins:
- **Reset (RST)** - Active low polarity (configurable)
- **Data (DATA)** - Bidirectional data line
- **Clock (CLK)** - Clock signal for data transfer

### Programming Principle:
1. Pulse the reset line to enter debug mode
2. Configure hardware with DMA descriptors for transferring data from DEBUG port directly to Flash
3. Clock out data to the debug port using GPIOs

## 2. Hardware Setup for Raspberry Pi 5

### Required Connections:
```
CC2530 Pin    Raspberry Pi 5 Pin    Function
-----------    ------------------    --------
Reset (P2.0)   GPIO 17 (Pin 11)     Reset (active low)
Debug Data     GPIO 22 (Pin 15)     Data (bidirectional)
Debug Clock    GPIO 27 (Pin 13)     Clock
GND           GND                   Ground
VCC           3.3V                  Power (if needed)
```

### Physical Pin Layout:
- **GPIO 17** (Pin 11) - Reset line
- **GPIO 27** (Pin 13) - Clock line  
- **GPIO 22** (Pin 15) - Data line

## 3. Software Requirements

### Dependencies:
```bash
# Install libgpiod for modern GPIO support
sudo apt update
sudo apt install libgpiod-dev

# For development
sudo apt install build-essential
```

### GPIO Backend Support:
The project now supports two GPIO backends:

1. **libgpiod** (Recommended for Raspberry Pi 5)
   - Modern GPIO character device interface
   - Full support for Raspberry Pi 5
   - Better error handling and debugging

2. **sysfs** (Legacy)
   - Linux GPIO sysfs interface
   - May not work on newer kernels
   - Limited support on Raspberry Pi 5

## 4. Building and Installation

### Quick Build:
```bash
# Clone and build
git clone <repository>
cd cc2530prog

# Build with libgpiod backend (recommended)
make

# Build with debug symbols
make debug

# Build test program
make test-debug
```

### Build Options:
```bash
# Select GPIO backend
make GPIO_BACKEND=gpio-libgpiod    # Modern backend (default)
make GPIO_BACKEND=gpio-sysfs       # Legacy backend

# Enable debug output
make DEBUG=1

# Install system-wide
sudo make install
```

### Testing GPIO:
```bash
# Test GPIO functionality
sudo ./test_gpio

# Test with debug output
make test-debug
sudo ./test_gpio
```

## 5. Usage

### Basic Commands:
```bash
# Identify device
sudo ./cc2530prog -i

# List available commands
sudo ./cc2530prog -l

# Program firmware
sudo ./cc2530prog -f firmware.bin

# Read back firmware
sudo ./cc2530prog -r -f readback.bin

# Send single command
sudo ./cc2530prog -c "command"

# Verbose output
sudo ./cc2530prog -v -f firmware.bin
```

### Debug Mode:
```bash
# Build with debug symbols
make debug

# Run with verbose output
sudo ./cc2530prog -v -i
```

## 6. Troubleshooting

### Common Issues:

**GPIO Export Errors:**
```bash
# Check GPIO availability
ls /dev/gpiochip*

# Check libgpiod installation
gpioinfo

# Test GPIO manually
sudo ./test_gpio
```

**Permission Errors:**
```bash
# Run with sudo (required for GPIO access)
sudo ./cc2530prog -i
```

**Device Not Found:**
- Check physical connections
- Verify GPIO pin assignments
- Ensure CC2530 is powered correctly

### Debug Information:
The project includes comprehensive debug logging:
- GPIO operations are logged with timestamps
- Error conditions are clearly reported
- Use `make debug` for detailed output

## 7. Software Integration

### GPIO Interface Functions:
If your system doesn't support the provided backends, implement these functions:

```c
int gpio_export(int n);                    // Export GPIO pin
int gpio_unexport(int n);                  // Unexport GPIO pin
int gpio_set_direction(int n, enum gpio_direction direction);  // Set direction
int gpio_get_value(int n, bool *value);    // Get GPIO value
int gpio_set_value(int n, bool value);     // Set GPIO value
void gpio_cleanup(void);                   // Cleanup resources
```

### Adding New Backends:
1. Create `gpio-yourbackend.c`
2. Implement the GPIO interface functions
3. Update `gpio.h` to support your backend
4. Set `GPIO_BACKEND=gpio-yourbackend` in Makefile

## 8. Performance Considerations

The CC2530 firmware size matches available hardware flash sizes (64KB to 256KB).
Since programming via debug port is slow, consider:

1. **Bootstrap Method**: Use debug port for initial programming
2. **UART/SPI Bootloader**: Use TI's bootloader for larger transfers
3. **Optimized Transfers**: Use DMA descriptors for efficient programming

## 9. Future Developments

- Interactive debugging using debug interface
- Support for additional CC2530 variants
- Enhanced error recovery mechanisms
- Integration with TI's debug dongle functionality

## 10. License

This project is distributed under a 2-clause BSD license. See LICENSE for details.

---

**Original Author:** Florian Fainelli  
**Updated for Raspberry Pi 5:** 2024
