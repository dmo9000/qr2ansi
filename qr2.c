#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <qrencode.h>

#define DEFAULT_WIDTH		79	
#define DEFAULT_HEIGHT	23

static uint8_t tty_width;
static uint8_t tty_height;

static const uint8_t border = 4;

static void put_block (bool upper, bool lower) {

    if (upper && lower) {
        (void) putchar(0xdb);
        return;
    }

    if (upper && !lower) {
        (void) putchar(0xdf);
        return;
    }

    if (lower && !upper) {
        (void) putchar(0xdc);
        return;
    }

    if (!lower && !upper) {
        (void) putchar(0x20);
        return;
    }

    assert(NULL);
}

static void horizontal_border(uint8_t my_padding) {
   	uint8_t x;
    for (x = 0 ; x < my_padding ; x++) {
        put_block(true, true);
				}
}

static void vertical_border(uint8_t my_width) {
    uint8_t x, y;

    for (y = 0 ; y < border ; y++) {
        for (x = 0 ; x < my_width ; x++) {
            put_block(true, true);
					}
        (void) putchar('\n');
    }
}

static void put_qrcode (const char *str) {
    bool upper = false;
    bool lower = false;
    QRcode *qr = QRcode_encodeString(str, 0, QR_ECLEVEL_H, QR_MODE_8, 1);
		uint8_t x, y; 
		uint8_t padding = 0;

		tty_width = DEFAULT_WIDTH;
		tty_height = DEFAULT_HEIGHT;

		padding = (tty_width - (uint8_t) qr->width) / 2; 
		vertical_border(tty_width);
    for (y = 0 ; y < (uint8_t) qr->width ; y+=2) {
        horizontal_border (padding);
        for (x = 0 ; x < (uint8_t) qr->width ; x++) {
            upper = !((bool) (qr->data[y * (uint8_t) qr->width + x] & 1));
            lower = !((bool) (qr->data[(y+1) * (uint8_t) qr->width + x] & 1));
            if (y + 1 >= (uint8_t) qr->width) {
                lower = true;
            }
            put_block(upper, lower);
        }
        horizontal_border (padding);
        (void) putchar('\n');
    }
		vertical_border(tty_width);

    QRcode_free (qr);
}

int main (int argc, char *argv[]) {
    uint8_t i;
    for (i = 1 ; i < (uint8_t) argc ; i++) {
        put_qrcode (argv[i]);
    }

    exit(0);
}
