/* Conectarea pinilor la placa Arduino UNO:
 * LCD RS -> 11
 * LCD E (Enable) -> 12
 * LCD DB4 -> 4
 * LCD DB5 -> 5
 * LCD DB6 -> 6
 * LCD DB7 -> 7
 * Exemplu de aplicatie 55
 * LCD RW -> GND
 * LCD VSS -> GND
 * LCD V0 -> Rezistor 70Ohm -> GND
 * LCD VCC -> 5V
 */

// include libraria pentru LCD
#include <LiquidCrystal.h>

// asociere pini Arduino <-> LCD
const byte RS = 11, EN = 12;
const byte DB4 = 4, DB5 = 5, DB6 = 6, DB7 = 7;
LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);

int contor = 0;
int secunde = 0;
int minute = 0;
int ore = 0;
int start = 1;

void setup() {
  // definire numar linii si coloane pentru LCD
  lcd.begin(16, 2);
  // afisare mesaj pe prima linie a LCD-ului
  lcd.print("Ora exacta: ");

  // setare pini butoane ca pini de intrare
  DDRD &= ~((1 << PD2) | (1 << PD3));

  // configurare intreruperi externe
  EICRA |= (1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00);
  EIMSK |= (1 << INT1) | (1 << INT0);
  EIFR |= (0 << INTF1) | (0 << INTF0);
  PCICR |= (0 << PCIE2) | (0 << PCIE1) | (0 << PCIE0);

  // configurare timer sa execute o intrerupere la fiecare 100ms
  // setare mod de functionare CTC

  TCCR1A = 0b00000000;
  TCCR1B = 0b00001101;
  OCR1A = 0x619;
  TIMSK1 |= (1 << OCIE1A);

  PCICR = 0b00000001;
  PCMSK0 = 0b00000011;
  // activare intreruperi globale
  SREG |= (1 << SREG_I);
};

void loop() {
  // afisare ora
  if(start){
    lcd.setCursor(0, 1);
    if (ore < 10) {
      lcd.print(0);
      lcd.setCursor(1, 1);
    }
    lcd.print(ore);

    // afisare minute
    lcd.setCursor(2, 1);
    lcd.print(": ");
    lcd.setCursor(3, 1);
    if (minute < 10) {
      lcd.print(0);
      lcd.setCursor(4, 1);
    }
    lcd.print(minute);

    // afisare secunde
    lcd.setCursor(5, 1);
    lcd.print(": ");
    lcd.setCursor(6, 1);
    if (secunde < 10) {
      lcd.print(0);
      lcd.setCursor(7, 1);
    }
    lcd.print(secunde);
  }
};

// functie pentru incrementarea orelor
// numarul maxim de ore pe zi este 24 (0-23)
inline void incrementOre() {
  ++ore;
  if (ore >= 24) {
    ore %= 24;
  }
};

// functie pentru incrementarea minutelor
// numarul maxim de minute intr-o ora este 60 (0-59)
inline void incrementMinute() {
  ++minute;
  if (minute >= 60) {
    incrementOre();
    minute %= 60;
  }
};

// rutina de intrerupere pentru contorizarea secundelor
ISR(TIMER1_COMPA_vect) {
  // dezactivare intreruperi globale
  SREG &= ~(1 << SREG_I);

  // au mai trecut 100ms, deci incrementam contorul
  if(start)
  {
    ++contor;

    // daca contorul a ajuns la 10, adica 10x100ms=1000ms -> 1s
    if (contor >= 10) {
      // incrementam secundele si resetam contorul
      ++secunde;
      contor = 0;

      if (secunde >= 60) {
        // daca avem 60 de secunde, incrementam minutele si resetam secundele
        incrementMinute();
        secunde %= 60;
      }
    };
  }

  // activare intreruperi globale
  SREG |= (1 << SREG_I);
};

// rutina de intrerupere pentru INT0 (pinul 2)
ISR(INT0_vect) {
  // dezactivare intreruperi globale
  SREG &= ~(1 << SREG_I);
  if(start)
  {
    // incrementam minutele
    incrementMinute();
  };
  // activare intreruperi globale
  SREG |= (1 << SREG_I);
};

// rutina de intrerupere pentru INT1 (pinul 3)
ISR(INT1_vect) {
  // dezactivare intreruperi globale
  SREG &= ~(1 << SREG_I);
  if(start)
  {
    // incrementam orele
    incrementOre();
  };
  // activare intreruperi globale
  SREG |= (1 << SREG_I);
};

ISR(PCINT0_vect) {
  // dezactivare intreruperi globale
  SREG &= ~(1 << SREG_I);
  // Toggle start / stop
  if(PINB & (1 << PB0))
  {
    minute = 0;
    ore = 0;
    secunde = 0;
    start = true;
    // activare intreruperi globale
    SREG |= (1 << SREG_I);
  }
  else if(PINB & (1 << PB1))
  {
    start = !start;
  };
};