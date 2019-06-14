# vim: set ts=4 sw=4 noexpandtab :
.phony: clean all

NULL=

TARGETS = \
Blink \
BlinkWithoutDelay \
Blink_ws281x_leds_wDMA \
$(NULL)


all:
	make -C Blink all
	make -C BlinkWithoutDelay all
	make -C Blink_ws281x_leds_wDMA all

clean:
	make -C Blink clean
	make -C BlinkWithoutDelay clean
	make -C Blink_ws281x_leds_wDMA clean

$(V).SILENT:

