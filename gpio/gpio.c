


#include <stdio.h>
#include <gpiod.h>

#define GPIO_PIN    4

struct gpiod_chip *chip;
struct gpiod_line *line;
int rv, value;

int main()
{
    chip= gpiod_chip_open("/dev/gpiochip0");

    if (!chip)
     return -1;

    line = gpiod_chip_get_line(chip, GPIO_PIN);

    if (!line) 
    {
     gpiod_chip_close(chip);
     return -1; 
    }

    rv = gpiod_line_request_input(line, "foobar");

    if (rv) 
    {
     gpiod_chip_close(chip);
     return -1;
    }

    value = gpiod_line_get_value(line);
    printf("GPIO%d value is %d", GPIO_PIN, value);
    gpiod_chip_close(chip);

    return 0;
}
