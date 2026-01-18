// Arduino UNO: ~1.5 kHz PWM on D9 (Timer1)
// Pot on A0 controls duty (0..80%)

#define PWM_PIN   9
#define POT_PIN   A0

#define PWM_TOP   1333          // ~1.5 kHz with prescaler 8
#define DUTY_MAX  1067          // 80% of 1334 counts

void setup() {
  pinMode(PWM_PIN, OUTPUT);

  // ---- Timer1: Fast PWM, TOP=ICR1, non-inverting on OC1A (D9) ----
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  ICR1  = PWM_TOP;              // sets frequency
  OCR1A = 0;                    // start at 0% duty

  TCCR1A |= (1 << COM1A1);      // OC1A non-inverting
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13); // Fast PWM, mode 14

  TCCR1B |= (1 << CS11);        // prescaler = 8
}

void loop() {
  int pot = analogRead(POT_PIN);   // 0..1023

  // 0..80% duty
  uint16_t duty = (uint32_t)pot * DUTY_MAX / 1023;

  // Basit yumuşatma
  static uint16_t duty_filt = 0;
  duty_filt = (duty_filt * 7 + duty) / 8;

  OCR1A = duty_filt;

  delay(5);
}
