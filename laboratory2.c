#include <avr/io.h>
#include <avr/interrupt.h>
byte contor = 0;
const byte starePortNumere[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

ISR (INT0_vect) {
  if (contor > 0) { 
    contor--;
  }
}

ISR (INT1_vect) {
  if (contor < 9) { 
    contor++;
  }
}

void setup() {
  DDRD &= ~((1 << PD2) | (1 << PD3) |(1 << PD0));

  DDRC = 0xFF;
  PORTC = 0x00;
  DDRB = 0xFF;
  PORTB = 0x00;

  SREG |= (1 << SREG_I);

  EICRA = (1 << ISC11) | (0 << ISC10) | (1 << ISC01) | (0 << ISC00);
  EIMSK = (1 << INT1) | (1 << INT0);
  EIFR = (0 << INTF1) | (0 << INTF0);
}

void loop() {
  PORTC = ~starePortNumere[contor];
  if(contor == 7 | contor == 0 | contor == 1)
  {
	PORTB = 0xFF;
  }
  else
  {
	PORTB = 0x00;
  };
  if(PIND & (1<<PD0))
  {
    contor = 0;
  }
}