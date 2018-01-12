/* Inspired by and adapted from: 

	  https://gist.github.com/michaelschrijver/8566294

   That version is great, but I needed a version that used CP437 half blocks 
   blocks in order to make the code fit more easily on an 80x24 terminal 

	 Compile with: 

 		gcc -o qr2 qr2.c -lqrencode

   You will need to pipe the output through iconv for now to get UTF-8 
	 compatible output, eg.

	 ./qr2 "foobarbaz.com" | iconv -f CP437 -t UTF-8

   Since the output of this program is in obsolete CP437 format. 

*/

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <qrencode.h>


static const int border = 4;

static void put_block (bool upper, bool lower) {

    if (upper && lower) {
        putchar(0xdb);
        return;
    }

    if (upper && !lower) {
        putchar(0xdf);
        return;
    }

    if (lower && !upper) {
        putchar(0xdc);
        return;
    }

    if (!lower && !upper) {
        putchar(0x20);
        return;
    }

    assert(NULL);
}
static void horizontal_border(void) {
    int x;
    for (x = 0 ; x < border ; x++)
        put_block(true, true);
}
static void vertical_border(int qr_width) {
    int x, y;

    for (y = 0 ; y < border ; y++) {
        for (x = 0 ; x < (qr_width + (2* border)) ; x++)
            put_block(true, true);
        putchar('\n');
    }
}
static void put_qrcode (const char *str) {
    bool upper = false;
    bool lower = false;
    QRcode *qr = QRcode_encodeString(str, 0, QR_ECLEVEL_H, QR_MODE_8, 1);
    int x, y;

    vertical_border (qr->width);

    for (y = 0 ; y < qr->width ; y+=2) {
        horizontal_border ();
        for (x = 0 ; x < qr->width ; x++) {
            upper = !(qr->data[y * qr->width + x] & 1);
            lower = !(qr->data[(y+1) * qr->width + x] & 1);
            if (y + 1 >= qr->width) {
                lower = true;
            }
            put_block(upper, lower);
        }
        horizontal_border ();
        putchar('\n');
    }

    vertical_border (qr->width);

    QRcode_free (qr);
}
int main (int argc, char *argv[]) {
    int i;
    for (i = 1 ; i < argc ; i++) {
        put_qrcode (argv[i]);
    }
}
