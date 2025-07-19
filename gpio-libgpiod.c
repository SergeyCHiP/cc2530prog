/*
 * Modern GPIO backend using libgpiod
 *
 * Copyright (C) 2024, Updated for Raspberry Pi 5
 *
 * This file is part of "cc2530prog", this file is distributed under
 * a 2-clause BSD license, see LICENSE for details.
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <gpiod.h>

#include "gpio.h"
#include "debug.h"

// Global variables for libgpiod
static struct gpiod_chip *chip = NULL;
static struct gpiod_line *lines[3] = {NULL, NULL, NULL}; // RST, CLK, DATA

// GPIO line numbers for Raspberry Pi 5
static const unsigned int line_offsets[] = {17, 27, 22};

int gpio_export(int n)
{
    GPIO_DEBUG("Exporting GPIO %d", n);
    
    // Find the line index
    int line_index = -1;
    for (int i = 0; i < 3; i++) {
        if (line_offsets[i] == (unsigned int)n) {
            line_index = i;
            break;
        }
    }
    
    if (line_index == -1) {
        GPIO_ERROR("Invalid GPIO number: %d", n);
        return -1;
    }
    
    // Open chip if not already open
    if (chip == NULL) {
        chip = gpiod_chip_open("/dev/gpiochip0");
        if (chip == NULL) {
            GPIO_ERROR("Failed to open gpiochip0");
            return -1;
        }
        GPIO_INFO("Opened gpiochip0 successfully");
    }
    
    // Get the line
    lines[line_index] = gpiod_chip_get_line(chip, line_offsets[line_index]);
    if (lines[line_index] == NULL) {
        GPIO_ERROR("Failed to get line %d", line_offsets[line_index]);
        return -1;
    }
    
    GPIO_INFO("Successfully exported GPIO %d (line %d)", n, line_offsets[line_index]);
    return 0;
}

int gpio_unexport(int n)
{
    GPIO_DEBUG("Unexporting GPIO %d", n);
    
    // Find the line index
    int line_index = -1;
    for (int i = 0; i < 3; i++) {
        if (line_offsets[i] == (unsigned int)n) {
            line_index = i;
            break;
        }
    }
    
    if (line_index == -1) {
        GPIO_ERROR("Invalid GPIO number: %d", n);
        return -1;
    }
    
    // Release the line
    if (lines[line_index] != NULL) {
        gpiod_line_release(lines[line_index]);
        lines[line_index] = NULL;
        GPIO_INFO("Successfully unexported GPIO %d", n);
    }
    
    return 0;
}

int gpio_set_direction(int n, enum gpio_direction direction)
{
    GPIO_DEBUG("Setting direction for GPIO %d to %d", n, direction);
    
    // Find the line index
    int line_index = -1;
    for (int i = 0; i < 3; i++) {
        if (line_offsets[i] == (unsigned int)n) {
            line_index = i;
            break;
        }
    }
    
    if (line_index == -1) {
        GPIO_ERROR("Invalid GPIO number: %d", n);
        return -1;
    }
    
    if (lines[line_index] == NULL) {
        GPIO_ERROR("GPIO %d not exported", n);
        return -1;
    }
    
    int ret;
    switch (direction) {
        case GPIO_DIRECTION_IN:
            ret = gpiod_line_request_input(lines[line_index], "cc2530prog");
            break;
        case GPIO_DIRECTION_OUT:
            ret = gpiod_line_request_output(lines[line_index], "cc2530prog", 0);
            break;
        case GPIO_DIRECTION_HIGH:
            ret = gpiod_line_request_output(lines[line_index], "cc2530prog", 1);
            break;
        default:
            GPIO_ERROR("Invalid direction: %d", direction);
            return -1;
    }
    
    if (ret < 0) {
        GPIO_ERROR("Failed to set direction for GPIO %d", n);
        return -1;
    }
    
    GPIO_INFO("Successfully set direction for GPIO %d to %d", n, direction);
    return 0;
}

int gpio_get_value(int n, bool *value)
{
    GPIO_DEBUG("Getting value for GPIO %d", n);
    
    // Find the line index
    int line_index = -1;
    for (int i = 0; i < 3; i++) {
        if (line_offsets[i] == (unsigned int)n) {
            line_index = i;
            break;
        }
    }
    
    if (line_index == -1) {
        GPIO_ERROR("Invalid GPIO number: %d", n);
        return -1;
    }
    
    if (lines[line_index] == NULL) {
        GPIO_ERROR("GPIO %d not exported", n);
        return -1;
    }
    
    int val = gpiod_line_get_value(lines[line_index]);
    if (val < 0) {
        GPIO_ERROR("Failed to get value for GPIO %d", n);
        return -1;
    }
    
    *value = (val != 0);
    GPIO_DEBUG("GPIO %d value: %d", n, *value);
    return 0;
}

int gpio_set_value(int n, bool value)
{
    GPIO_DEBUG("Setting value for GPIO %d to %d", n, value);
    
    // Find the line index
    int line_index = -1;
    for (int i = 0; i < 3; i++) {
        if (line_offsets[i] == (unsigned int)n) {
            line_index = i;
            break;
        }
    }
    
    if (line_index == -1) {
        GPIO_ERROR("Invalid GPIO number: %d", n);
        return -1;
    }
    
    if (lines[line_index] == NULL) {
        GPIO_ERROR("GPIO %d not exported", n);
        return -1;
    }
    
    int ret = gpiod_line_set_value(lines[line_index], value ? 1 : 0);
    if (ret < 0) {
        GPIO_ERROR("Failed to set value for GPIO %d", n);
        return -1;
    }
    
    GPIO_DEBUG("Successfully set GPIO %d to %d", n, value);
    return 0;
}

// Cleanup function
void gpio_cleanup(void)
{
    GPIO_INFO("Cleaning up GPIO resources");
    
    // Release all lines
    for (int i = 0; i < 3; i++) {
        if (lines[i] != NULL) {
            gpiod_line_release(lines[i]);
            lines[i] = NULL;
        }
    }
    
    // Close chip
    if (chip != NULL) {
        gpiod_chip_close(chip);
        chip = NULL;
    }
    
    GPIO_INFO("GPIO cleanup completed");
} 