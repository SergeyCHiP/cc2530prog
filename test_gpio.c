/*
 * Test program for gpio-libgpiod backend
 *
 * Copyright (C) 2024, Updated for Raspberry Pi 5
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "gpio.h"
#include "debug.h"

extern int current_log_level;

int test_gpio_export(void)
{
    printf("=== Testing GPIO Export ===\n");
    
    // Test exporting each GPIO
    for (int i = 0; i < 3; i++) {
        int gpio_num = (i == 0) ? 17 : (i == 1) ? 27 : 22;
        printf("Testing export of GPIO %d...\n", gpio_num);
        
        int ret = gpio_export(gpio_num);
        if (ret == 0) {
            printf("✅ GPIO %d exported successfully\n", gpio_num);
        } else {
            printf("❌ Failed to export GPIO %d\n", gpio_num);
            return -1;
        }
    }
    
    return 0;
}

int test_gpio_direction(void)
{
    printf("=== Testing GPIO Direction ===\n");
    
    // Test setting direction for each GPIO
    for (int i = 0; i < 3; i++) {
        int gpio_num = (i == 0) ? 17 : (i == 1) ? 27 : 22;
        printf("Testing direction setting for GPIO %d...\n", gpio_num);
        
        int ret = gpio_set_direction(gpio_num, GPIO_DIRECTION_OUT);
        if (ret == 0) {
            printf("✅ GPIO %d direction set to OUTPUT successfully\n", gpio_num);
        } else {
            printf("❌ Failed to set direction for GPIO %d\n", gpio_num);
            return -1;
        }
    }
    
    return 0;
}

int test_gpio_value(void)
{
    printf("=== Testing GPIO Value Setting ===\n");
    
    // Test setting and getting values
    for (int i = 0; i < 3; i++) {
        int gpio_num = (i == 0) ? 17 : (i == 1) ? 27 : 22;
        printf("Testing value setting for GPIO %d...\n", gpio_num);
        
        // Set to HIGH
        int ret = gpio_set_value(gpio_num, true);
        if (ret == 0) {
            printf("✅ GPIO %d set to HIGH successfully\n", gpio_num);
        } else {
            printf("❌ Failed to set GPIO %d to HIGH\n", gpio_num);
            return -1;
        }
        
        // Get value
        bool value;
        ret = gpio_get_value(gpio_num, &value);
        if (ret == 0) {
            printf("✅ GPIO %d value read: %s\n", gpio_num, value ? "HIGH" : "LOW");
        } else {
            printf("❌ Failed to read GPIO %d value\n", gpio_num);
            return -1;
        }
        
        // Set to LOW
        ret = gpio_set_value(gpio_num, false);
        if (ret == 0) {
            printf("✅ GPIO %d set to LOW successfully\n", gpio_num);
        } else {
            printf("❌ Failed to set GPIO %d to LOW\n", gpio_num);
            return -1;
        }
        
        // Small delay
        usleep(100000); // 100ms
    }
    
    return 0;
}

int test_gpio_unexport(void)
{
    printf("=== Testing GPIO Unexport ===\n");
    
    // Test unexporting each GPIO
    for (int i = 0; i < 3; i++) {
        int gpio_num = (i == 0) ? 17 : (i == 1) ? 27 : 22;
        printf("Testing unexport of GPIO %d...\n", gpio_num);
        
        int ret = gpio_unexport(gpio_num);
        if (ret == 0) {
            printf("✅ GPIO %d unexported successfully\n", gpio_num);
        } else {
            printf("❌ Failed to unexport GPIO %d\n", gpio_num);
            return -1;
        }
    }
    
    return 0;
}

int main(void)
{
    printf("=== GPIO Libgpiod Test Program ===\n");
    printf("This program tests the new libgpiod backend\n");
    printf("Testing GPIO: 17, 27, 22\n\n");
    
    // Set debug level
    current_log_level = LOG_LEVEL_DEBUG;
    
    // Run tests
    int ret = 0;
    
    ret = test_gpio_export();
    if (ret != 0) {
        printf("❌ GPIO export test failed\n");
        return -1;
    }
    
    ret = test_gpio_direction();
    if (ret != 0) {
        printf("❌ GPIO direction test failed\n");
        return -1;
    }
    
    ret = test_gpio_value();
    if (ret != 0) {
        printf("❌ GPIO value test failed\n");
        return -1;
    }
    
    ret = test_gpio_unexport();
    if (ret != 0) {
        printf("❌ GPIO unexport test failed\n");
        return -1;
    }
    
    printf("\n=== All Tests Passed! ===\n");
    printf("The libgpiod backend is working correctly.\n");
    
    return 0;
} 