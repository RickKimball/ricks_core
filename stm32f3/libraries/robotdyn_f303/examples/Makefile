.phony: clean all

NULL=

TARGETS = \
Blink \
BlinkWithoutDelay \
Blink_ws281x_leds_wDMA \
Timer_PWM_TIM2 \
$(NULL)


all:
	make -C Blink all
	make -C BlinkWithoutDelay all
	make -C Blink_ws281x_leds_wDMA all
	make -C Timer_PWM_TIM2 all

clean:
	make -C Blink clean
	make -C BlinkWithoutDelay clean
	make -C Blink_ws281x_leds_wDMA clean
	make -C Timer_PWM_TIM2 clean

$(V).SILENT:

# vim: set ts=4 sw=4 noexpandtab :
