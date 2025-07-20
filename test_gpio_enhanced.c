/*
 * Enhanced test program for gpio-libgpiod backend
 *
 * Copyright (C) 2024, Updated for Raspberry Pi 5
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>

#include "gpio.h"
#include "debug.h"

extern int current_log_level;

// Test configuration
#define TEST_GPIO_PINS {17, 27, 22}
#define TEST_ITERATIONS 1000
#define TEST_DELAY_US 1000
#define MAX_CONCURRENT_THREADS 3

// Test results structure
typedef struct {
    int total_tests;
    int passed_tests;
    int failed_tests;
    double total_time;
} test_results_t;

static test_results_t test_results = {0, 0, 0, 0.0};

// Helper function to measure time
static double get_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1000000.0;
}

// Helper function to print test result
static void print_test_result(const char *test_name, int result, double time_ms) {
    if (result == 0) {
        printf("✅ %s passed (%.2f ms)\n", test_name, time_ms);
        test_results.passed_tests++;
    } else {
        printf("❌ %s failed (%.2f ms)\n", test_name, time_ms);
        test_results.failed_tests++;
    }
    test_results.total_tests++;
    test_results.total_time += time_ms;
}

// 1. Enhanced basic functionality tests
int test_gpio_export_enhanced(void)
{
    printf("\n=== Enhanced GPIO Export Tests ===\n");
    double start_time = get_time_ms();
    
    int gpio_pins[] = TEST_GPIO_PINS;
    int num_pins = sizeof(gpio_pins) / sizeof(gpio_pins[0]);
    
    // Test 1: Normal export
    for (int i = 0; i < num_pins; i++) {
        printf("Testing export of GPIO %d...\n", gpio_pins[i]);
        
        int ret = gpio_export(gpio_pins[i]);
        if (ret != 0) {
            printf("❌ Failed to export GPIO %d\n", gpio_pins[i]);
            return -1;
        }
        printf("✅ GPIO %d exported successfully\n", gpio_pins[i]);
    }
    
    // Test 2: Double export (should handle gracefully)
    printf("Testing double export of GPIO 17...\n");
    int ret = gpio_export(17);
    if (ret == 0) {
        printf("✅ Double export handled correctly\n");
    } else {
        printf("❌ Double export failed unexpectedly\n");
        return -1;
    }
    
    // Test 3: Invalid GPIO number
    printf("Testing export of invalid GPIO 999...\n");
    ret = gpio_export(999);
    if (ret == -1) {
        printf("✅ Invalid GPIO correctly rejected\n");
    } else {
        printf("❌ Invalid GPIO should have been rejected\n");
        return -1;
    }
    
    double end_time = get_time_ms();
    print_test_result("Enhanced GPIO Export", 0, end_time - start_time);
    return 0;
}

// 2. Direction tests
int test_gpio_direction_enhanced(void)
{
    printf("\n=== Enhanced GPIO Direction Tests ===\n");
    double start_time = get_time_ms();
    
    int gpio_pins[] = TEST_GPIO_PINS;
    int num_pins = sizeof(gpio_pins) / sizeof(gpio_pins[0]);
    
    // Test all direction types
    enum gpio_direction directions[] = {
        GPIO_DIRECTION_IN,
        GPIO_DIRECTION_OUT,
        GPIO_DIRECTION_HIGH
    };
    
    for (int i = 0; i < num_pins; i++) {
        for (int d = 0; d < 3; d++) {
            printf("Testing GPIO %d direction %d...\n", gpio_pins[i], directions[d]);
            
            int ret = gpio_set_direction(gpio_pins[i], directions[d]);
            if (ret != 0) {
                printf("❌ Failed to set direction %d for GPIO %d\n", directions[d], gpio_pins[i]);
                return -1;
            }
            printf("✅ GPIO %d direction %d set successfully\n", gpio_pins[i], directions[d]);
        }
    }
    
    // Test invalid direction
    printf("Testing invalid direction...\n");
    int ret = gpio_set_direction(17, 99); // Invalid direction
    if (ret == -1) {
        printf("✅ Invalid direction correctly rejected\n");
    } else {
        printf("❌ Invalid direction should have been rejected\n");
        return -1;
    }
    
    double end_time = get_time_ms();
    print_test_result("Enhanced GPIO Direction", 0, end_time - start_time);
    return 0;
}

// 3. Value tests with patterns
int test_gpio_value_enhanced(void)
{
    printf("\n=== Enhanced GPIO Value Tests ===\n");
    double start_time = get_time_ms();
    
    int gpio_pins[] = TEST_GPIO_PINS;
    int num_pins = sizeof(gpio_pins) / sizeof(gpio_pins[0]);
    
    // Test patterns
    bool patterns[] = {true, false, true, false, true};
    int pattern_size = sizeof(patterns) / sizeof(patterns[0]);
    
    for (int i = 0; i < num_pins; i++) {
        printf("Testing value patterns for GPIO %d...\n", gpio_pins[i]);
        
        // Set to output
        int ret = gpio_set_direction(gpio_pins[i], GPIO_DIRECTION_OUT);
        if (ret != 0) {
            printf("❌ Failed to set GPIO %d to output\n", gpio_pins[i]);
            return -1;
        }
        
        // Test pattern
        for (int p = 0; p < pattern_size; p++) {
            ret = gpio_set_value(gpio_pins[i], patterns[p]);
            if (ret != 0) {
                printf("❌ Failed to set GPIO %d to %s\n", gpio_pins[i], patterns[p] ? "HIGH" : "LOW");
                return -1;
            }
            
            // Read back value
            bool read_value;
            ret = gpio_get_value(gpio_pins[i], &read_value);
            if (ret != 0) {
                printf("❌ Failed to read GPIO %d value\n", gpio_pins[i]);
                return -1;
            }
            
            if (read_value != patterns[p]) {
                printf("❌ GPIO %d value mismatch: expected %s, got %s\n", 
                       gpio_pins[i], patterns[p] ? "HIGH" : "LOW", read_value ? "HIGH" : "LOW");
                return -1;
            }
            
            usleep(TEST_DELAY_US);
        }
        
        printf("✅ GPIO %d value pattern test passed\n", gpio_pins[i]);
    }
    
    double end_time = get_time_ms();
    print_test_result("Enhanced GPIO Value", 0, end_time - start_time);
    return 0;
}

// 4. Performance tests
int test_gpio_performance(void)
{
    printf("\n=== GPIO Performance Tests ===\n");
    double start_time = get_time_ms();
    
    int gpio_pins[] = TEST_GPIO_PINS;
    int num_pins = sizeof(gpio_pins) / sizeof(gpio_pins[0]);
    
    // Performance test: rapid value changes
    for (int i = 0; i < num_pins; i++) {
        printf("Testing performance for GPIO %d...\n", gpio_pins[i]);
        
        double pin_start = get_time_ms();
        
        for (int j = 0; j < TEST_ITERATIONS; j++) {
            int ret = gpio_set_value(gpio_pins[i], (j % 2) == 0);
            if (ret != 0) {
                printf("❌ Performance test failed for GPIO %d\n", gpio_pins[i]);
                return -1;
            }
        }
        
        double pin_end = get_time_ms();
        double ops_per_sec = (TEST_ITERATIONS * 1000.0) / (pin_end - pin_start);
        
        printf("✅ GPIO %d: %.0f operations/sec\n", gpio_pins[i], ops_per_sec);
    }
    
    double end_time = get_time_ms();
    print_test_result("GPIO Performance", 0, end_time - start_time);
    return 0;
}

// 5. Error handling tests
int test_gpio_error_handling(void)
{
    printf("\n=== GPIO Error Handling Tests ===\n");
    double start_time = get_time_ms();
    
    // Test 1: Operations on unexported GPIO
    printf("Testing operations on unexported GPIO...\n");
    
    // Try to set direction on unexported GPIO
    int ret = gpio_set_direction(17, GPIO_DIRECTION_OUT);
    if (ret == -1) {
        printf("✅ Correctly rejected operation on unexported GPIO\n");
    } else {
        printf("❌ Should have rejected operation on unexported GPIO\n");
        return -1;
    }
    
    // Test 2: Get value on unexported GPIO
    bool value;
    ret = gpio_get_value(17, &value);
    if (ret == -1) {
        printf("✅ Correctly rejected get value on unexported GPIO\n");
    } else {
        printf("❌ Should have rejected get value on unexported GPIO\n");
        return -1;
    }
    
    // Test 3: Set value on unexported GPIO
    ret = gpio_set_value(17, true);
    if (ret == -1) {
        printf("✅ Correctly rejected set value on unexported GPIO\n");
    } else {
        printf("❌ Should have rejected set value on unexported GPIO\n");
        return -1;
    }
    
    // Test 4: Operations on invalid GPIO
    printf("Testing operations on invalid GPIO...\n");
    ret = gpio_export(999);
    if (ret == -1) {
        printf("✅ Correctly rejected export of invalid GPIO\n");
    } else {
        printf("❌ Should have rejected export of invalid GPIO\n");
        return -1;
    }
    
    double end_time = get_time_ms();
    print_test_result("GPIO Error Handling", 0, end_time - start_time);
    return 0;
}

// 6. Resource cleanup tests
int test_gpio_cleanup(void)
{
    printf("\n=== GPIO Cleanup Tests ===\n");
    double start_time = get_time_ms();
    
    int gpio_pins[] = TEST_GPIO_PINS;
    int num_pins = sizeof(gpio_pins) / sizeof(gpio_pins[0]);
    
    // Export all GPIOs
    for (int i = 0; i < num_pins; i++) {
        int ret = gpio_export(gpio_pins[i]);
        if (ret != 0) {
            printf("❌ Failed to export GPIO %d for cleanup test\n", gpio_pins[i]);
            return -1;
        }
    }
    
    // Test individual unexport
    for (int i = 0; i < num_pins; i++) {
        printf("Testing unexport of GPIO %d...\n", gpio_pins[i]);
        
        int ret = gpio_unexport(gpio_pins[i]);
        if (ret != 0) {
            printf("❌ Failed to unexport GPIO %d\n", gpio_pins[i]);
            return -1;
        }
        printf("✅ GPIO %d unexported successfully\n", gpio_pins[i]);
    }
    
    // Test double unexport (should handle gracefully)
    printf("Testing double unexport...\n");
    int ret = gpio_unexport(17);
    if (ret == 0) {
        printf("✅ Double unexport handled correctly\n");
    } else {
        printf("❌ Double unexport failed unexpectedly\n");
        return -1;
    }
    
    double end_time = get_time_ms();
    print_test_result("GPIO Cleanup", 0, end_time - start_time);
    return 0;
}

// 7. Stress test
int test_gpio_stress(void)
{
    printf("\n=== GPIO Stress Tests ===\n");
    double start_time = get_time_ms();
    
    int gpio_pins[] = TEST_GPIO_PINS;
    int num_pins = sizeof(gpio_pins) / sizeof(gpio_pins[0]);
    
    // Stress test: rapid export/unexport cycles
    for (int cycle = 0; cycle < 10; cycle++) {
        printf("Stress test cycle %d/10...\n", cycle + 1);
        
        // Export all GPIOs
        for (int i = 0; i < num_pins; i++) {
            int ret = gpio_export(gpio_pins[i]);
            if (ret != 0) {
                printf("❌ Stress test: failed to export GPIO %d\n", gpio_pins[i]);
                return -1;
            }
        }
        
        // Set values rapidly
        for (int j = 0; j < 100; j++) {
            for (int i = 0; i < num_pins; i++) {
                int ret = gpio_set_value(gpio_pins[i], (j % 2) == 0);
                if (ret != 0) {
                    printf("❌ Stress test: failed to set GPIO %d value\n", gpio_pins[i]);
                    return -1;
                }
            }
        }
        
        // Unexport all GPIOs
        for (int i = 0; i < num_pins; i++) {
            int ret = gpio_unexport(gpio_pins[i]);
            if (ret != 0) {
                printf("❌ Stress test: failed to unexport GPIO %d\n", gpio_pins[i]);
                return -1;
            }
        }
    }
    
    printf("✅ Stress test completed successfully\n");
    
    double end_time = get_time_ms();
    print_test_result("GPIO Stress", 0, end_time - start_time);
    return 0;
}

// 8. Print test summary
void print_test_summary(void)
{
    printf("\n=== Test Summary ===\n");
    printf("Total tests: %d\n", test_results.total_tests);
    printf("Passed: %d\n", test_results.passed_tests);
    printf("Failed: %d\n", test_results.failed_tests);
    printf("Success rate: %.1f%%\n", 
           (double)test_results.passed_tests / test_results.total_tests * 100.0);
    printf("Total time: %.2f ms\n", test_results.total_time);
    printf("Average time per test: %.2f ms\n", 
           test_results.total_time / test_results.total_tests);
    
    if (test_results.failed_tests == 0) {
        printf("\n🎉 All tests passed! GPIO backend is working correctly.\n");
    } else {
        printf("\n⚠️  Some tests failed. Please check the implementation.\n");
    }
}

int main(void)
{
    printf("=== Enhanced GPIO Libgpiod Test Program ===\n");
    printf("This program performs comprehensive testing of the libgpiod backend\n");
    printf("Testing GPIO: 17, 27, 22\n\n");
    
    // Set debug level
    current_log_level = LOG_LEVEL_DEBUG;
    
    // Run enhanced tests
    int ret = 0;
    
    ret = test_gpio_export_enhanced();
    if (ret != 0) return -1;
    
    ret = test_gpio_direction_enhanced();
    if (ret != 0) return -1;
    
    ret = test_gpio_value_enhanced();
    if (ret != 0) return -1;
    
    ret = test_gpio_performance();
    if (ret != 0) return -1;
    
    ret = test_gpio_error_handling();
    if (ret != 0) return -1;
    
    ret = test_gpio_cleanup();
    if (ret != 0) return -1;
    
    ret = test_gpio_stress();
    if (ret != 0) return -1;
    
    print_test_summary();
    
    return 0;
} 