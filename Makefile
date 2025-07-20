#
# Makefile for cc2530prog
# Updated for Raspberry Pi 5 with libgpiod support
#

# Disable all built-in rules
MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

CC?=gcc
CFLAGS?=-Wall -Wextra
DEBUG?=0
APP=cc2530prog
TEST_APP=test_gpio

# Debug flags
ifeq ($(DEBUG),1)
    CFLAGS += -DDEBUG=1 -g -O0
else
    CFLAGS += -O2
endif

# GPIO backend - only libgpiod supported
GPIO_BACKEND=gpio-libgpiod
LIBS += -lgpiod
GPIO_SRC = gpio-libgpiod.c

# Main application
all: $(APP) $(TEST_APP)

# Object files
OBJS=$(APP).o $(GPIO_SRC:.c=.o) debug.o
TEST_OBJS=test_gpio.o $(GPIO_SRC:.c=.o) debug.o

# Main application
$(APP): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

# Test application
$(TEST_APP): $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_OBJS) $(LIBS) -o $@

# Explicit compilation rules
cc2530prog.o: cc2530prog.c
	$(CC) $(CFLAGS) -DGPIO_BACKEND=$(GPIO_BACKEND) -c $< -o $@

gpio-libgpiod.o: gpio-libgpiod.c
	$(CC) $(CFLAGS) -DGPIO_BACKEND=$(GPIO_BACKEND) -c $< -o $@

debug.o: debug.c
	$(CC) $(CFLAGS) -c $< -o $@

test_gpio.o: test_gpio.c
	$(CC) $(CFLAGS) -DGPIO_BACKEND=$(GPIO_BACKEND) -c $< -o $@

# Debug version
debug: CFLAGS += -DDEBUG=1 -g -O0
debug: $(APP)

# Test with debug
test-debug: CFLAGS += -DDEBUG=1 -g -O0
test-debug: $(TEST_APP)

# Clean
clean:
	rm -f *.o $(APP) $(TEST_APP)

# Install (requires sudo)
install: $(APP)
	sudo cp $(APP) /usr/local/bin/

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/$(APP)

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build main app and test (default)"
	@echo "  debug        - Build with debug symbols"
	@echo "  test-debug   - Build test with debug symbols"
	@echo "  clean        - Remove build artifacts"
	@echo "  install      - Install to /usr/local/bin"
	@echo "  uninstall    - Remove from /usr/local/bin"
	@echo "  help         - Show this help"
	@echo ""
	@echo "Variables:"
	@echo "  DEBUG        - Enable debug (0/1)"
	@echo "  CC           - C compiler"
	@echo "  CFLAGS       - C compiler flags"
	@echo ""
	@echo "Note: Only libgpiod backend is supported"

.PHONY: all debug test-debug clean install uninstall help
