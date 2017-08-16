#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <gpio.h>
#include <misc/util.h>
#include <misc/printk.h>

/* define LED and Button pin */
#define LED_PORT LED0_GPIO_PORT
#define LED_PIN LED0_GPIO_PIN

#define LED_FLASH_TIME 100//ms

#define BTN_PORT SW0_GPIO_NAME
#define BTN_PIN SW0_GPIO_PIN


/* define global device */
struct device *gDevLed;
struct device *gDevBtn;

static struct gpio_callback gpio_cb;

/* define thread parameter */
#define STACKSIZE 1024
#define PRIORITY_EVENT 7
#define PRIORITY_INTR 6
#define LEDON	1
#define LEDOFF 0

void ledOnOff(u32_t on)
{
	gpio_pin_write(gDevLed, LED_PIN, on);
}
void ledFlash(u32_t times)
{
	while(times) {
		ledOnOff(1);
		k_sleep(LED_FLASH_TIME);
		ledOnOff(0);

		if(--times){
			k_sleep(LED_FLASH_TIME);
		}
	}
}
u32_t isKeyDown(void)
{
	u32_t val;
	gpio_pin_read(gDevBtn, BTN_PIN, &val);
	return !val;
}


void btnIntrCb(struct device *gpio, struct gpio_callback *cb, u32_t pin)
{
	u32_t OnOff = isKeyDown();
	ledOnOff(OnOff);
	k_sleep(LED_FLASH_TIME);
	ledOnOff(OnOff);
}


void main (void)
{


	gDevLed = device_get_binding(LED_PORT);

	gpio_pin_configure(gDevLed, LED_PIN, GPIO_DIR_OUT);

	ledOnOff(1);

	gDevBtn = device_get_binding(BTN_PORT);

	gpio_pin_configure(gDevBtn, BTN_PIN,
			GPIO_DIR_IN | GPIO_INT | GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW | GPIO_PUD_PULL_UP);

	gpio_init_callback(&gpio_cb, btnIntrCb, BIT(BTN_PIN));
	gpio_add_callback(gDevBtn, &gpio_cb);
	gpio_pin_enable_callback(gDevBtn, BTN_PIN);
}
