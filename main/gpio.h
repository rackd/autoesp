#ifndef GPIO_H_
#define GPIO_H_

void gpio_init();

void gpio_set_low();

void gpio_set_high();

int gpio_get_state();

void gpio_toggle();

#endif // GPIO_H_