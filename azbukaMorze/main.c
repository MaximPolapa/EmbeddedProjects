#include <avr/io.h>
#include <util/delay.h>
#include <string.h> 

#define LED_PIN PB5 


const char *morseCodes[] = {
    ".-",   // A
    "-...", // B
    "-.-.", // C
    "-..",  // D
    ".",    // E
    "..-.", // F
    "--.",  // G
    "....", // H
    "..",   // I
    ".---", // J
    "-.-",  // K
    ".-..", // L
    "--",   // M
    "-.",   // N
    "---",  // O
    ".--.", // P
    "--.-", // Q
    ".-.",  // R
    "...",  // S
    "-",    // T
    "..-",  // U
    "...-", // V
    ".--",  // W
    "-..-", // X
    "-.--", // Y
    "--..", // Z
    "-----", // 0
    ".----", // 1
    "..---", // 2
    "...--", // 3
    "....-", // 4
    ".....", // 5
    "-....", // 6
    "--...", // 7
    "---..", // 8
    "----.", // 9
};

void outputMorseChar(char c) 
{
    if ((c >= 'A' && c <= 'Z')  (c >= '0' && c <= '9')  c == '.'  c == ','  c == '?'  c == '\'' 
        c == '!'  c == '/'  c == '('  c == ')'  c == '&'  c == ':'  c == '='  c == '+' 
        c == '-'  c == '_'  c == '"'  c == '$'  c == '@') {
        int index = 0;
        if (c >= 'A' && c <= 'Z') {
            index = c - 'A'; 
        } else if (c >= '0' && c <= '9') {
            index = c - '0' + 26;
        } 
        const char *code = morseCodes[index]; 
        for (size_t i = 0; i < strlen(code); i++) { 
            char symbol = code[i];  
            if (symbol == '.') {
                PORTB |= 1 << LED_PIN;    
                _delay_ms(100);           
                PORTB &= ~(1 << LED_PIN); 
                _delay_ms(300);
            } else if (symbol == '-') {
                PORTB |= 1 << LED_PIN;    
                _delay_ms(300);           
                PORTB &= ~(1 << LED_PIN); 
                _delay_ms(300);
            }
        }
        _delay_ms(500); 
    } 
}


void blink_msg(const char *message) 
{
    for (size_t i = 0; i < strlen(message); i++) {
        char c = message[i];
        if (c == ' ') {
            _delay_ms(300);
        } else {
            outputMorseChar(c);
        }
    }
}

int main(void) 
{
    const char *message = "HELLO"; // Message
    DDRB |= 1 << LED_PIN; 

    while (1) 
    {
        blink_msg(message);
        _delay_ms(3000); 
    }
    return 0;
}