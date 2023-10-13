#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN_A PB4
#define LED_PIN_B PB3
#define LED_PIN_C PB2
#define LED_PIN_D PB1
#define LED_PIN_E PD0  
#define LED_PIN_F PD1  
#define LED_PIN_G PD2
#define BUTTON_PIN  PC4
#define BUTTON_PIN2 PC3
#define BUTTON_PIN3 PC2



uint8_t counter = 0;
uint8_t counter_state = 0;
int k = 0;

void button_handler(uint8_t ks);

void checkButton(void)
{
    static uint8_t but_st1;
    static uint8_t but_st2;
    static uint8_t but_st3;
    

    but_st1 <<= 1;
    but_st2 <<= 1;
    but_st3 <<= 1;
    
    //Button1
    if ((PINC & (1 << BUTTON_PIN)) == 0b0) {
      but_st1 |= 0b1;
    }
    if ((but_st1 & 0b00000111) == 0b00000100) {
       button_handler(1);
    }

    //Button2
    if ((PINC & (1 << BUTTON_PIN2)) == 0b0) {
      but_st2 |= 0b1;
    }
    if ((but_st2 & 0b00000111) == 0b00000100) {
       button_handler(2);
    }

    //Button 3
   if ((PINC & (1 << BUTTON_PIN3)) == 0b0) {
      but_st3 |= 0b1;
    }
    if ((but_st3 & 0b00000111) == 0b00000100) {
       button_handler(3);
    }

}
void displayDigit(uint8_t digit) 
{
    PORTB &= ~((1 << LED_PIN_A) | (1 << LED_PIN_B) | (1 << LED_PIN_C) | (1 << LED_PIN_D));
    PORTD &= ~((1 << LED_PIN_E) | (1 << LED_PIN_F) | (1 << LED_PIN_G));

    switch (digit) {
        case 0:
            PORTB |= (1 << LED_PIN_A) | (1 << LED_PIN_B) | (1 << LED_PIN_C) | (1 << LED_PIN_D);
            PORTD |= (1 << LED_PIN_E) | (1 << LED_PIN_F);
            break;
        case 1:
            PORTB |= (1 << LED_PIN_B) | (1 << LED_PIN_C);
            break;
        case 2:
            PORTB |= (1 << LED_PIN_B) | (1 << LED_PIN_A) | (1 << LED_PIN_D);
            PORTD |= (1 << LED_PIN_E) | (1 << LED_PIN_G);
            break;
        case 3:
            PORTB |= (1 << LED_PIN_A) | (1 << LED_PIN_B) | (1 << LED_PIN_C) | (1 << LED_PIN_D);
            PORTD |= (1 << LED_PIN_G);
            break;
        case 4:
            PORTB |= (1 << LED_PIN_B) | (1 << LED_PIN_C);
            PORTD |= (1 << LED_PIN_F) | (1 << LED_PIN_G);
            break;
        case 5:
            PORTB |= (1 << LED_PIN_A) |  (1 << LED_PIN_C) | (1 << LED_PIN_D);
            PORTD |= (1 << LED_PIN_F) | (1 << LED_PIN_G);
            break;
        case 6:
            PORTB |= (1 << LED_PIN_A) | (1 << LED_PIN_C) | (1 << LED_PIN_D);
            PORTD |= (1 << LED_PIN_E) | (1 << LED_PIN_F) | (1 << LED_PIN_G);
            break;
        case 7:
            PORTB |= (1 << LED_PIN_A) | (1 << LED_PIN_B) | (1 << LED_PIN_C);
            break;
        case 8:
            PORTB |= (1 << LED_PIN_A) | (1 << LED_PIN_B) | (1 << LED_PIN_C) | (1 << LED_PIN_D);
            PORTD |= (1 << LED_PIN_E) | (1 << LED_PIN_F) | (1 << LED_PIN_G);
            break;
        case 9:
            PORTB |= (1 << LED_PIN_A) | (1 << LED_PIN_B) | (1 << LED_PIN_C) | (1 << LED_PIN_D);
            PORTD |= (1 << LED_PIN_F) | (1 << LED_PIN_G);
            break;
    }
}

void button_handler(uint8_t ks)
{
    if (ks == 1) {
        if (counter >= 9) {
            counter = 0;
        } else {
            counter++;
        }
        displayDigit(counter);
    }
    if (ks == 2) {
        if (counter <= 0) {
            counter = 9;
        } else {
            counter--;
        }
        displayDigit(counter);
    }
    if (ks == 3) {
        counter = 0;
        displayDigit(counter);
    }
}

int main(void)
{
    uint8_t dl = 0;

    DDRB |= 1 << LED_PIN_A; 
    DDRB |= 1 << LED_PIN_B; 
    DDRB |= 1 << LED_PIN_C; 
    DDRB |= 1 << LED_PIN_D;
    DDRD |= 1 << LED_PIN_E;
    DDRD |= 1 << LED_PIN_F;
    DDRD |= 1 << LED_PIN_G; 
    DDRC = 0b00000000;

    // Встановлення підтягуючих резисторів для кнопок
    PORTC |= (1 << BUTTON_PIN) | (1 << BUTTON_PIN2) | (1 << BUTTON_PIN3);

    displayDigit(0);

    while (1) {
        if (++dl > 40) {
            dl = 0;
        }
        checkButton();
        _delay_ms(10);
    }
    
    return 0;
}