/*
 * Mock GPIO backend for macOS development
 *
 * Copyright (C) 2024, Updated for macOS development
 *
 * This file is part of "cc2530prog", this file is distributed under
 * a 2-clause BSD license, see LICENSE for details.
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "gpio.h"
#include "debug.h"

// Mock GPIO state
static bool gpio_states[3] = {false, false, false}; // RST, CLK, DATA
static bool gpio_directions[3] = {false, false, false}; // false = input, true = output
static bool gpio_exported[3] = {false, false, false};

// GPIO line numbers for Raspberry Pi 5 (mocked)
static const unsigned int line_offsets[] = {17, 27, 22};

// Helper function to check if GPIO is properly initialized
static int check_gpio_initialization(void)
{
    return 0; // Always succeed in mock
}

int gpio_export(int n)
{
    GPIO_DEBUG("Exporting GPIO %d (MOCK)", n);
    
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
    
    // Check if line is already exported
    if (gpio_exported[line_index]) {
        GPIO_WARN("GPIO %d already exported", n);
        return 0;
    }
    
    gpio_exported[line_index] = true;
    GPIO_INFO("Successfully exported GPIO %d (MOCK)", n);
    return 0;
}

int gpio_unexport(int n)
{
    GPIO_DEBUG("Unexporting GPIO %d (MOCK)", n);
    
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
    
    if (gpio_exported[line_index]) {
        gpio_exported[line_index] = false;
        gpio_directions[line_index] = false;
        GPIO_INFO("Successfully unexported GPIO %d (MOCK)", n);
    }
    
    return 0;
}

int gpio_set_direction(int n, enum gpio_direction direction)
{
    GPIO_DEBUG("Setting direction for GPIO %d to %d (MOCK)", n, direction);
    
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
    
    if (!gpio_exported[line_index]) {
        GPIO_ERROR("GPIO %d not exported", n);
        return -1;
    }
    
    gpio_directions[line_index] = true; // Set as output
    
    switch (direction) {
        case GPIO_DIRECTION_IN:
            GPIO_DEBUG("Setting GPIO %d to INPUT (MOCK)", n);
            break;
        case GPIO_DIRECTION_OUT:
            GPIO_DEBUG("Setting GPIO %d to OUTPUT (MOCK)", n);
            break;
        case GPIO_DIRECTION_HIGH:
            GPIO_DEBUG("Setting GPIO %d to OUTPUT HIGH (MOCK)", n);
            gpio_states[line_index] = true;
            break;
        default:
            GPIO_ERROR("Invalid direction: %d", direction);
            return -1;
    }
    
    GPIO_INFO("Successfully set direction for GPIO %d to %d (MOCK)", n, direction);
    return 0;
}

int gpio_get_value(int n, bool *value)
{
    GPIO_DEBUG("Getting value for GPIO %d (MOCK)", n);
    
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
    
    if (!gpio_exported[line_index]) {
        GPIO_ERROR("GPIO %d not exported", n);
        return -1;
    }
    
    *value = gpio_states[line_index];
    GPIO_DEBUG("GPIO %d value: %d (MOCK)", n, *value);
    return 0;
}

int gpio_set_value(int n, bool value)
{
    GPIO_DEBUG("Setting value for GPIO %d to %d (MOCK)", n, value);
    
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
    
    if (!gpio_exported[line_index]) {
        GPIO_ERROR("GPIO %d not exported", n);
        return -1;
    }
    
    gpio_states[line_index] = value;
    GPIO_DEBUG("Successfully set GPIO %d to %d (MOCK)", n, value);
    return 0;
}

// Cleanup function
void gpio_cleanup(void)
{
    GPIO_INFO("Cleaning up GPIO resources (MOCK)");
    
    // Reset all GPIOs
    for (int i = 0; i < 3; i++) {
        gpio_exported[i] = false;
        gpio_directions[i] = false;
        gpio_states[i] = false;
    }
    
    GPIO_INFO("GPIO cleanup completed (MOCK)");
} 