#include <avr/io.h>
#include <util/delay.h>

typedef enum { KEY_DOWN, KEY_UP} key_state_t;
#define button2 PB2

uint8_t cnt_mod = 0, cnt = 0;


void key_handler(key_state_t ks);

void scan_key(void)
{
  static uint8_t shreg;
  shreg <<= 1;

  if  ((PINB & (1 << PB1)) != 0) {
    shreg |= 1;
  }
 
  // push key1
  if ((shreg & 0x07) == 0x04) {
    key_handler(KEY_DOWN);
  }

}

//-----------------------------------------------------------
void key_handler(key_state_t ks)
{
  if (ks == KEY_DOWN) {
    cnt_mod = 1 - cnt_mod; // Переключаємо між режимами "run" і "stop"
  }
    
}

//-----------------------------------------------------------
uint8_t dec_to_bcd(uint8_t val) 
{
  uint8_t a, b;

  a = val / 10;
  b = val % 10;
  return (a << 4) | b;
}

void checkButton(void)
{
    static uint8_t shreg;
    shreg <<= 1;

    if ((PINB & (1 << button2)) == 0b0) {
      shreg |= 0b1;
    }

    // push key1
    if ((shreg & 0b00000111) == 0b00000100) {
      PORTD = 0b00000000; 
    }
}

//-----------------------------------------------------------
int main(void)
{
  uint8_t dl = 0;

  DDRD = 0xFF; // Всі лінії порта D виходи
  DDRB = 0;    // Всі лінії порта B входи

  for (;;) {
    // ~1 s thread -----
    if (++dl > 40) {
      dl = 0;

      if(cnt_mod == 1) {
        cnt++;
        if (cnt > 99) {
          cnt = 0;
        }
        PORTD = dec_to_bcd(cnt);
      }
    }
    // 10 ms thread ---------
    scan_key();
    checkButton();
    _delay_ms(10);
  }

  return 0;
}