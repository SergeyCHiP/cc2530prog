#ifndef __CC2530PROG_GPIO_H
#define __CC2530PROG_GPIO_H

#ifndef GPIO_BACKEND
#define GPIO_BACKEND gpio-libgpiod
#endif

#include <stdbool.h>

/* Modern GPIO configuration for Raspberry Pi 5 */
#define RST_GPIO        17    // GPIO 17 (physical pin 11)
#define RST_GPIO_POL    !     // Active low polarity
#define CCLK_GPIO       27    // GPIO 27 (physical pin 13)
#define DATA_GPIO       22    // GPIO 22 (physical pin 15)

/*
 * GPIO direction enumeration
 */
enum gpio_direction {
	GPIO_DIRECTION_IN,
	GPIO_DIRECTION_OUT,
	GPIO_DIRECTION_HIGH,
};

/*
 * GPIO interface functions
 */
int gpio_export(int n);
int gpio_unexport(int n);
int gpio_set_direction(int n, enum gpio_direction direction);
int gpio_get_value(int n, bool *value);
int gpio_set_value(int n, bool value);

/*
 * Cleanup function for libgpiod backend
 */
void gpio_cleanup(void);

#endif /* __CC2530PROG_GPIO_H */
