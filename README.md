# CC2530 Programming Tool

Modern CC2530 programming tool for Raspberry Pi 5 using libgpiod.

## 🎯 Features

- **Modern GPIO backend** - Uses libgpiod for Raspberry Pi 5 compatibility
- **Full CC2530 support** - Identify, program, and verify CC2530 chips
- **Comprehensive testing** - Built-in GPIO and connection testing tools
- **Debug support** - Detailed logging and diagnostics
- **Easy setup** - Simple build and installation process

## 📋 Requirements

- Raspberry Pi 5 (or compatible)
- libgpiod: `sudo apt install gpiod libgpiod-dev`
- CC2530 chip and wiring tools

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

## 🔧 Hardware Setup

Connect CC2530 to Raspberry Pi 5:
```
CC2530 Pin    →  Raspberry Pi 5 Pin
─────────────────────────────────────
RST (Reset)   →  GPIO 17 (Pin 11)
CLK (Clock)   →  GPIO 27 (Pin 13)  
DATA (Data)   →  GPIO 22 (Pin 15)
GND           →  GND
VCC           →  3.3V (IMPORTANT: not 5V!)
```

## 📖 Usage

### Identify CC2530
```bash
./cc2530prog -i -v
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

## 🔍 Troubleshooting

### Test GPIO Functionality
```bash
./test_connection.sh
```

### Detailed CC2530 Diagnostics
```bash
./debug_cc2530.sh
```

### Loopback Test (without CC2530)
```bash
./test_loopback.sh
```

## 📁 Project Structure

```
cc2530prog/
├── cc2530prog.c          # Main application
├── gpio-libgpiod.c       # GPIO backend (libgpiod)
├── gpio.h                # GPIO interface
├── debug.c/h             # Debug system
├── test_gpio.c           # GPIO test application
├── Makefile              # Build system
├── CC2530ZNP-*.hex      # Firmware files
├── test_*.sh            # Test scripts
├── CC2530_SETUP.md      # Detailed setup guide
└── README.md            # This file
```

## 🛠️ Build Options

### Raspberry Pi (with libgpiod)
```bash
# Standard build
make

# Debug build
make debug

# Test with debug
make test-debug

# Clean build artifacts
make clean

# Install to system
sudo make install

# Show help
make help
```

### macOS (with mock GPIO)
```bash
# Standard build
make -f Makefile.macos

# Debug build
make -f Makefile.macos debug

# Test with debug
make -f Makefile.macos test-debug

# Clean build artifacts
make -f Makefile.macos clean

# Show help
make -f Makefile.macos help
```

## 🔧 Development

### Debug Mode
```bash
make DEBUG=1
```

### Custom Compiler
```bash
make CC=clang
```

### Custom Flags
```bash
make CFLAGS="-Wall -Wextra -O0 -g"
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
```

## 📝 Notes

- CC2530 requires 3.3V power supply
- Debug interface is sensitive to timing
- Physical connections must be solid
- Some CC2530 chips may be locked or damaged

## 📚 Documentation

- [CC2530_SETUP.md](CC2530_SETUP.md) - Detailed setup and troubleshooting guide
- [CC2530 Datasheet](http://www.ti.com/lit/ds/symlink/cc2530.pdf)
- [libgpiod Documentation](https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/about/)

## 📄 License

This project is distributed under a 2-clause BSD license. See [LICENSE](LICENSE) for details.

## 👥 Authors

Original by Florian Fainelli <f.fainelli@gmail.com>
Updated for Raspberry Pi 5 with libgpiod support
