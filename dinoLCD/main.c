#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>


uint8_t dl = 0, state = 1;
uint8_t data = 0;
uint8_t mbuf[64];

const uint8_t font[] = {
    0xC6,  /*  %%...%%.  */
    0xEE,  /*  %%%.%%%.  */
    0xFE,  /*  %%%%%%%.  */
    0xD6,  /*  %%.%.%%.  */
    0xC6,  /*  %%...%%.  */
    0xC6,  /*  %%...%%.  */
    0xC6,  /*  %%...%%.  */
    0x00,  /*  ........  */

    0x7C,  /*  .%%%%%..  */
    0xC6,  /*  %%...%%.  */
    0xC6,  /*  %%...%%.  */
    0xFE,  /*  %%%%%%%.  */
    0xC6,  /*  %%...%%.  */
    0xC6,  /*  %%...%%.  */
    0xC6,  /*  %%...%%.  */
    0x00,   /*  ........  */

    0xC6,  /*  %%...%%.  */
    0x6C,  /*  .%%.%%..  */
    0x38,  /*  ..%%%...  */
    0x38,  /*  ..%%%...  */
    0x38,  /*  ..%%%...  */
    0x6C,  /*  .%%.%%..  */
    0xC6,  /*  %%...%%.  */
    0x00,  /*  ........  */

0x7E,
0x60,
0x30,
0x18,
0x0C,
0x06,
0x06,
0x00,
0x3E,
0x63,
0x60,
0x30,
0x0C,
0x06,
0x7F,
0x00,
0x18,
0x1C,
0x1E,
0x18,
0x18,
0x18,
0x3C,
0x00,
0x3E,
0x63,
0x63,
0x7E,
0x60,
0x63,
0x3E,
0x00,

    // 0xFE,  /*  %%%%%%%.  */
    // 0x06,  /*  .....%%.  */
    // 0x0C,  /*  ....%%..  */
    // 0x18,  /*  ...%%...  */
    // 0x30,  /*  ..%%....  */
    // 0x60,  /*  .%%.....  */
    // 0x60,  /*  .%%.....  */
    // 0x00,  /*  ........  */

    // 0x7C,  /*  .%%%%%..  */
    // 0xC6,  /*  %%...%%.  */
    // 0x06,  /*  .....%%.  */
    // 0x0C,  /*  ....%%..  */
    // 0x30,  /*  ..%%....  */
    // 0x60,  /*  .%%.....  */
    // 0xFE,  /*  %%%%%%%.  */
    // 0x00,  /*  ........  */

    // 0x18,  /*  ...%%...  */
    // 0x38,  /*  ..%%%...  */
    // 0x78,  /*  .%%%%...  */
    // 0x18,  /*  ...%%...  */
    // 0x18,  /*  ...%%...  */
    // 0x18,  /*  ...%%...  */
    // 0x3C,  /*  ..%%%%..  */
    // 0x00,  /*  ........  */

    // 0x7C,  /*  .%%%%%..  */
    // 0xC6,  /*  %%...%%.  */
    // 0xC6,  /*  %%...%%.  */
    // 0x7E,  /*  .%%%%%%.  */
    // 0x06,  /*  .....%%.  */
    // 0xC6,  /*  %%...%%.  */
    // 0x7C,  /*  .%%%%%..  */
    // 0x00,  /*  ........  */

    0x00,  /*  ........  */
    0x00,  /*  ........  */
    0x00,  /*  ........  */
    0x00,  /*  ........  */
    0x00,  /*  ........  */
    0x00,  /*  ........  */
    0x00,  /*  ........  */
    0x00   /*  ........  */
};

const uint8_t disp_cnt = 2;

uint8_t SPI_Transfer(uint8_t cData);

//-----------------------------------------------------------------------------
void SPI_MasterInit(void)
{
  // Налаштувати піни MOSI (PB3) і SCK (PB5) на вивід
  DDRB |= (1<<PB3) | (1<<PB5) | (1 << PB2);
  // Включити SPI, задати режим Master, 
  // встановити швидкість fck/16
  SPCR = (1<<SPE) | (1<<MSTR) | (0<<SPR0);
}

//-----------------------------------------------------------------------------
uint8_t SPI_Transfer(uint8_t cData)
{
  // Початок передачі
  SPDR = cData;
  // Чекати завершення передачі
  while(!(SPSR & (1<<SPIF)));
  return SPDR;
}

//-----------------------------------------------------------------------------
void max7219_sendData(uint8_t addr, uint8_t data) 
{
  // CS = 0 
  //PORTB &= ~(1 << PB2);
  // Почати передачу адресу
  SPDR = addr;
  // Чекати завершення передачі
  while(!(SPSR & (1<<SPIF)));
  // Почати передачу даних 
  SPDR = data;
  // Чекати завершення передачі
  while(!(SPSR & (1<<SPIF)));
  // CS = 1
  //PORTB |= (1 << PB2);
}

//-----------------------------------------------------------------------------
void max7219_Init(void)
{
  const uint16_t init_seq[] = {
      0x0F00,  // normal mode
      0x0900,  // no decode
      0x0B07,  
      0x0A0A,  // intensity
      0x0C01   // power on
  };

  SPI_MasterInit();
  PORTB |= (1 << PB2);

for (uint8_t i = 0; i < 5; i++) {
    PORTB &= ~(1 << PB2);  // CS = 0 
    for (uint8_t j = 0; j < disp_cnt; j++) {
      max7219_sendData(init_seq[i] >> 8, init_seq[i] & 0xFF);
    }
    PORTB |= (1 << PB2);  // CS = 1
  }
   
  // очистка дисплея
  for (uint8_t i = 0; i < 8; i++) { 
    PORTB &= ~(1 << PB2);  // CS = 0 
    for (uint8_t j = 0; j < disp_cnt; j++) {
      max7219_sendData(i + 1, 0);
    }
    PORTB |= (1 << PB2);  // CS = 1
  }
}

//-----------------------------------------------------------------------------
void max7219_setPixel(uint8_t x, uint8_t y, uint8_t p)
{
  x &= 15;
  y &= 7;
  uint8_t xx = x >> 3; // x / 8

  if (p == 0) {
    mbuf[y + xx * 8] &= ~(1 << (x & 7)); // clear
  } else {
    mbuf[y + xx * 8] |= 1 << (x & 7); //set
  }

  for (uint8_t i = 0; i < 8; i++) {  
    PORTB &= ~(1 << PB2);  // CS = 0 
    //for (uint8_t j = 1; j >= 0; --j) {
    //  max7219_sendData(i + 1, mbuf[j * 8 + i]);
    // }
    max7219_sendData(1 + i, mbuf[8 + i]);
    max7219_sendData(1 + i, mbuf[i]);

    PORTB |= (1 << PB2);  // CS = 1
  }
}

//-----------------------------------------------------------------------------
void max7219_update(void)
{
  for(uint8_t i = 0; i < 8; i++) {
    PORTB &= ~(1 << PB2);  // CS = 0 
    max7219_sendData(1 + i, mbuf[8 + i]);
    max7219_sendData(1 + i, mbuf[i]);
    PORTB |= (1 << PB2);  // CS = 1
  }
}

void max7219_shift(void)
{
  uint8_t i, j, a;
  
  for (i = 0; i < 8; i++) {
    a = mbuf[i];
    for (j = 0; j < 7; j++) {
      mbuf[i + j * 8] >>= 1;
      if (mbuf[i + (j+1) * 8] & 1) {
        mbuf[i + j * 8] |= 128;
      }
    }
    mbuf[i + (j) * 8] >>= 1;
    if (a & 1) {
      mbuf[i + (j) * 8] |= 128; 
    } 
  }
}

//-----------------------------------------------------------------------------
void key_handler(uint8_t st)
{
  switch(st) {
    case 1: //short press
   
    break;
    case 2: //long press
     ;
  }
}

//-----------------------------------------------------------------------------
/*void scan_key(void)
{
  static uint8_t shreg = 0xFF;
  static uint8_t lcnt, dbc_cnt;

  shreg <<= 1;
  if((PINB & (1<<PB1)) != 0) {
    shreg |= 1;
  }
  if((shreg & 0x07) == 0x04) {  //11111 100 00000
    key_handler(0); //short press down "100"
   // LOG_DEBUG("MAIN", "Short press down");
    lcnt=0;
  }
  if((shreg & 0x0F) == 0x03) {
    if(dbc_cnt <= 32 && dbc_cnt > 5) {
      //db click
      //LOG_DEBUG("MAIN", "Double-click: dbc_cnt= %d\n", dbc_cnt);
    } else {
      if(lcnt != 101 && dbc_cnt >= 34) {
        //LOG_DEBUG("MAIN", "Short press up");
        key_handler(1); //short press up //0011
      }
    }
    if(dbc_cnt >= 34) {
      dbc_cnt = 0;
    } 
  }
  if(dbc_cnt < 35) {
     dbc_cnt++; //200ms
  } 
  // long press
  if((shreg & 15) == 0) {
    if(lcnt < 100) {
      lcnt++; //200ms
    } else if (lcnt == 100) {
      lcnt++; 
      //LOG_DEBUG("MAIN", "Long press");
      key_handler(2);
    }
  // printf("[D] dbc_cnt= %d\n", dbc_cnt);
  }
}
*/
//-----------------------------------------------------------------------------
uint16_t char_triangle[8] = {
    0b0000001000000000, // 1-й рядок 
    0b0000011000000000, // 2-й рядок
    0b0000101000000000, // 3-й рядок
    0b0001001000000000, // 4-й рядок
    0b0010001000000000, // 5-й рядок
    0b0100001000000000, // 6-й рядок
    0b1111111000000000, // 7-й рядок
    0b0000000000000000  // 8-й рядок 
};


void shift_picture(uint16_t *buf)
{
   uint16_t i, t;

   for (i = 0; i < 8; i++) {
     t = buf[i];
     buf[i] >>= 1;
     if (t & 1) {
       buf[i] |= 0x8000;
     }
   }
}

void displayChar(const uint16_t* charData, uint8_t x, uint8_t y)
{
    for (uint8_t row = 0; row < 8; row++) {

        uint16_t rowData = charData[row];
        for (uint8_t col = 0; col < 16; col++) {
            
            if (rowData & (1 << (15 - col)) && (x + col) < 16) {
                max7219_setPixel(x + col, y + row, 1);
            } else {
                max7219_setPixel(x + col, y + row, 0);
            }
      }
    }
}

int main(void) {
    max7219_Init(); // Ініціалізуємо LED-дисплей
    uint8_t x = 0; // Початкова позиція трикутника



    while (1) {
        // Очищаємо дисплей
        for (uint8_t i = 0; i < 16; i++) {
            for (uint8_t j = 0; j < 8; j++) {
                max7219_setPixel(i, j, 0);
            }
        }

        // Відображаємо трикутник
        displayChar(char_triangle, 0, 0);
        shift_picture(char_triangle);
      
        // Оновлюємо дисплей
        max7219_update();

        // Затримка для переміщення трикутника
        _delay_ms(200);

        // Збільшуємо x для переміщення трикутника вправо
        x++;
		if(x > 15){
			x=0;
		}

        
    }

    return 0;
}