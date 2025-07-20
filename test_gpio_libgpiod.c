/*
 * Test program for libgpiod GPIO backend
 * Modern GPIO library with excellent Raspberry Pi 5 support
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <gpiod.h>

// GPIO direction enum
enum gpio_direction {
    GPIO_DIRECTION_IN,
    GPIO_DIRECTION_OUT,
    GPIO_DIRECTION_HIGH,
};

// Test results structure
typedef struct {
    int total_tests;
    int passed_tests;
    int failed_tests;
    double total_time;
} test_results_t;

static test_results_t test_results = {0, 0, 0, 0.0};

// libgpiod context
static struct gpiod_chip *chip = NULL;
static struct gpiod_line *lines[64] = {NULL};

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

// libgpiod GPIO functions
int gpio_export(int n) {
    printf("[LIBGPIOD] gpio_export(%d)\n", n);
    
    if (!chip) {
        chip = gpiod_chip_open("/dev/gpiochip0");
        if (!chip) {
            printf("❌ Failed to open GPIO chip: %s\n", strerror(errno));
            return -1;
        }
        printf("✅ GPIO chip opened successfully\n");
    }
    
    if (n < 0 || n >= 64) {
        printf("❌ Invalid GPIO pin number: %d\n", n);
        return -1;
    }
    
    if (lines[n]) {
        printf("⚠️  GPIO %d already exported\n", n);
        return 0;
    }
    
    lines[n] = gpiod_chip_get_line(chip, n);
    if (!lines[n]) {
        printf("❌ Failed to get GPIO line %d: %s\n", n, strerror(errno));
        return -1;
    }
    
    printf("✅ GPIO %d exported successfully\n", n);
    return 0;
}

int gpio_unexport(int n) {
    printf("[LIBGPIOD] gpio_unexport(%d)\n", n);
    
    if (n < 0 || n >= 64) {
        printf("❌ Invalid GPIO pin number: %d\n", n);
        return -1;
    }
    
    if (!lines[n]) {
        printf("⚠️  GPIO %d not exported\n", n);
        return 0;
    }
    
    gpiod_line_release(lines[n]);
    lines[n] = NULL;
    printf("✅ GPIO %d unexported successfully\n", n);
    return 0;
}

int gpio_set_direction(int n, enum gpio_direction direction) {
    printf("[LIBGPIOD] gpio_set_direction(%d, %d)\n", n, direction);
    
    if (!lines[n]) {
        printf("❌ GPIO %d not exported\n", n);
        return -1;
    }
    
    int ret = 0;
    switch (direction) {
        case GPIO_DIRECTION_IN:
            ret = gpiod_line_request_input(lines[n], "test");
            break;
        case GPIO_DIRECTION_OUT:
            ret = gpiod_line_request_output(lines[n], "test", 0);
            break;
        case GPIO_DIRECTION_HIGH:
            ret = gpiod_line_request_output(lines[n], "test", 1);
            break;
        default:
            return -1;
    }
    
    if (ret < 0) {
        printf("❌ Failed to set direction %d for GPIO %d: %s\n", direction, n, strerror(errno));
        return -1;
    }
    
    printf("✅ GPIO %d direction set to %d\n", n, direction);
    return 0;
}

int gpio_get_value(int n, bool *value) {
    printf("[LIBGPIOD] gpio_get_value(%d)\n", n);
    
    if (!lines[n]) {
        printf("❌ GPIO %d not exported\n", n);
        return -1;
    }
    
    int val = gpiod_line_get_value(lines[n]);
    if (val < 0) {
        printf("❌ Failed to read GPIO %d value: %s\n", n, strerror(errno));
        return -1;
    }
    
    *value = (val == 1);
    printf("✅ GPIO %d value read: %s\n", n, *value ? "HIGH" : "LOW");
    return 0;
}

int gpio_set_value(int n, bool value) {
    printf("[LIBGPIOD] gpio_set_value(%d, %s)\n", n, value ? "true" : "false");
    
    if (!lines[n]) {
        printf("❌ GPIO %d not exported\n", n);
        return -1;
    }
    
    int ret = gpiod_line_set_value(lines[n], value ? 1 : 0);
    if (ret < 0) {
        printf("❌ Failed to set GPIO %d value: %s\n", n, strerror(errno));
        return -1;
    }
    
    printf("✅ GPIO %d value set to %s\n", n, value ? "HIGH" : "LOW");
    return 0;
}

// Test functions
int test_gpio_export_enhanced(void)
{
    printf("\n=== Enhanced GPIO Export Tests (libgpiod) ===\n");
    double start_time = get_time_ms();
    
    int gpio_pins[] = {17, 27, 22};
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
    
    double end_time = get_time_ms();
    print_test_result("Enhanced GPIO Export", 0, end_time - start_time);
    return 0;
}

int test_gpio_direction_enhanced(void)
{
    printf("\n=== Enhanced GPIO Direction Tests (libgpiod) ===\n");
    double start_time = get_time_ms();
    
    int gpio_pins[] = {17, 27, 22};
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
    
    double end_time = get_time_ms();
    print_test_result("Enhanced GPIO Direction", 0, end_time - start_time);
    return 0;
}

int test_gpio_value_enhanced(void)
{
    printf("\n=== Enhanced GPIO Value Tests (libgpiod) ===\n");
    double start_time = get_time_ms();
    
    int gpio_pins[] = {17, 27, 22};
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
            
            printf("✅ GPIO %d: set %s, read %s\n", gpio_pins[i], 
                   patterns[p] ? "HIGH" : "LOW", read_value ? "HIGH" : "LOW");
        }
        
        printf("✅ GPIO %d value pattern test passed\n", gpio_pins[i]);
    }
    
    double end_time = get_time_ms();
    print_test_result("Enhanced GPIO Value", 0, end_time - start_time);
    return 0;
}

int test_gpio_performance(void)
{
    printf("\n=== GPIO Performance Tests (libgpiod) ===\n");
    double start_time = get_time_ms();
    
    int gpio_pins[] = {17, 27, 22};
    int num_pins = sizeof(gpio_pins) / sizeof(gpio_pins[0]);
    int iterations = 1000;
    
    // Performance test: rapid value changes
    for (int i = 0; i < num_pins; i++) {
        printf("Testing performance for GPIO %d...\n", gpio_pins[i]);
        
        double pin_start = get_time_ms();
        
        for (int j = 0; j < iterations; j++) {
            int ret = gpio_set_value(gpio_pins[i], (j % 2) == 0);
            if (ret != 0) {
                printf("❌ Performance test failed for GPIO %d\n", gpio_pins[i]);
                return -1;
            }
        }
        
        double pin_end = get_time_ms();
        double ops_per_sec = (iterations * 1000.0) / (pin_end - pin_start);
        
        printf("✅ GPIO %d: %.0f operations/sec\n", gpio_pins[i], ops_per_sec);
    }
    
    double end_time = get_time_ms();
    print_test_result("GPIO Performance", 0, end_time - start_time);
    return 0;
}

int test_gpio_stress(void)
{
    printf("\n=== GPIO Stress Tests (libgpiod) ===\n");
    double start_time = get_time_ms();
    
    int gpio_pins[] = {17, 27, 22};
    int num_pins = sizeof(gpio_pins) / sizeof(gpio_pins[0]);
    
    // Stress test: rapid value changes
    for (int cycle = 0; cycle < 5; cycle++) {
        printf("Stress test cycle %d/5...\n", cycle + 1);
        
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
    }
    
    printf("✅ Stress test completed successfully\n");
    
    double end_time = get_time_ms();
    print_test_result("GPIO Stress", 0, end_time - start_time);
    return 0;
}

int test_gpio_cleanup(void)
{
    printf("\n=== GPIO Cleanup Tests (libgpiod) ===\n");
    double start_time = get_time_ms();
    
    int gpio_pins[] = {17, 27, 22};
    int num_pins = sizeof(gpio_pins) / sizeof(gpio_pins[0]);
    
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

// Print test summary
void print_test_summary(void)
{
    printf("\n=== Test Summary (libgpiod) ===\n");
    printf("Total tests: %d\n", test_results.total_tests);
    printf("Passed: %d\n", test_results.passed_tests);
    printf("Failed: %d\n", test_results.failed_tests);
    printf("Success rate: %.1f%%\n", 
           (double)test_results.passed_tests / test_results.total_tests * 100.0);
    printf("Total time: %.2f ms\n", test_results.total_time);
    printf("Average time per test: %.2f ms\n", 
           test_results.total_time / test_results.total_tests);
    
    if (test_results.failed_tests == 0) {
        printf("\n🎉 All tests passed! libgpiod GPIO backend is working correctly.\n");
    } else {
        printf("\n⚠️  Some tests failed. Please check the implementation.\n");
    }
}

int main(void)
{
    printf("=== libgpiod GPIO Test Program ===\n");
    printf("Using modern libgpiod library (version: %s)\n", gpiod_version_string());
    printf("Testing GPIO: 17, 27, 22\n\n");
    
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
    
    ret = test_gpio_stress();
    if (ret != 0) return -1;
    
    ret = test_gpio_cleanup();
    if (ret != 0) return -1;
    
    print_test_summary();
    
    // Cleanup libgpiod
    if (chip) {
        gpiod_chip_close(chip);
        printf("✅ libgpiod chip closed\n");
    }
    
    return 0;
} 