#include <avr/io.h>
#include <util/delay.h>

typedef enum { KEY_DOWN, KEY_UP } key_state_t;
uint8_t cnt_mod = 0, cnt = 0;

int segment7[10] = {
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b11111111,
    0b01101111
};

void scan_key(void) {
    static uint8_t shreg;
    shreg <<= 1;

    if ((PINC & (1 << PC0)) == 0) {
        shreg |= 1;
    }

    // push key1
    if ((shreg & 0x07) == 0x04) {
        key_handler(KEY_DOWN);
    }
}

void key_handler(key_state_t ks) {
    int number = 0;
    if (ks == KEY_DOWN) {
        cnt_mod = 1 - cnt_mod; // Toggle between "run" and "stop" modes
    }
}

int main() {
    DDRD = 0b11111111;
    DDRC = 0b00000000;

    PORTD = segment7[0];

    while (1) {
        scan_key();

        if (cnt_mod == 1) {
            for (int i = 0; i < 10; i++) {
                PORTD = segment7[i];
                _delay_ms(1000);
            }
        }
    }

    return 0;
}
