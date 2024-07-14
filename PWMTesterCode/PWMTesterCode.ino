/*
 * 
 * This program was written for the PWM Tester Board. 
 * This code was designed to be flashed onto a ATtiny85
 * 
 * The circuit board is designed to have 3 buttons which pull the input pins to ground
 * There is 1 PWM output for the motor controller
 * 
 */

// Helpers
#define BIT(n) (1<<(n))
#define BITSET(x, n)        ((x) |= bit(n))
#define BITGET(x, n)        (((x) & bit(n)) > 0)



void setup() {
  cli(); // Disable interrupts
  CLKPR = (1<<CLKPCE); // Prescaler enable
  CLKPR = 0x00; // Clock division factor
  sei(); // Enable interrupts

  /* PLL Setup and config
  */
  PLLCSR = (1<<PLLE);
  // Check for PLL Lock insead of just delaying
  delay(10);
  PLLCSR |= (1<<PCKE);

  /* Config Timer 
  *  CTC1 causes the counter to reset at OCR1C counts.
  *  The CS bits select the prescale value. 13, 11, and 10 set the value to 1024
  */
  TCCR1 = (1<<CTC1) | (1<<CS13) | (1<<CS11) | (1<<CS10);

  /* Config Timer (cont.)
  *  PWM1B enables PWM on ouput B
  *  COM1B0 and COM1B1 set output mode
  */
  GTCCR = (1<<PWM1B) | (1<<COM1B0);

/*
 * Setup the main frequency
 * 
 * 250 ticks = 4ms 
 * (64 000 000 / 1024 = 62 500 ticks/sec)
 * (62 500 * 0.004 = 250) 
 * Subtract 1 because hardware reasons
 */
  OCR1C = 250 - 1;

  /*
   * For this motor controller, the input range is 1-2ms. 
   * 1ms is reverse, 1.5ms is neutral, and 2ms is forward. 
   */
  // Set pulse width (94 is ~1.5ms)
  OCR1B = 94;

  // Setup input pins and PWM output
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  
  pinMode(4, OUTPUT);
}

/*
 * Constants
 */
const int debounceDelay = 80;

const byte FORWARDBTN = PINB0; 
const byte NEUTRALBTN = PINB1;
const byte REVERSEBTN = PINB2;
const byte NEUTRAL_VALUE = 94;
const byte INC_VALUE = 3;
const byte MAX_RANGE_VALUE = 5;

/* 
 *  Variables
 */

char current_inc = 0;
byte pins[3] = {FORWARDBTN, NEUTRALBTN, REVERSEBTN};
bool lastButtonState[3] = {true};
bool buttonState[3] = {true};
int lastDebounceTime[3] = {0};

// Main loop. Loops over each button one at a time
void loop() {


  /*
   * Read the state of the current button, debounce, check to see if a button is pressed, and then act accordingly
   */
  for(int i = 0; i < 3; i++) {
    bool reading = BITGET(PINB, pins[i]);

    // If the switch changed due to noise or pressing, then reset the debouncing timer
    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      // Whatever the reading is at, it's been there for longer than the debounce
      // It is safe to take action

      // Check if the button state has changed
      if (reading != buttonState[i]) {
        buttonState[i] = reading;

        // Only change the PWM output if the physical button is pressed (Pin is pulled low)
        if (buttonState[i] == 0) {
          switch(pins[i]) {
            case FORWARDBTN:
              if(current_inc < MAX_RANGE_VALUE) { // Bound check
                current_inc++;
              }
              break;
            case NEUTRALBTN:
              current_inc = 0;
              break;
            case REVERSEBTN:
              if(current_inc > -MAX_RANGE_VALUE) { // Bound Check
                current_inc--;
              }
              break;
          }
          OCR1B = NEUTRAL_VALUE + current_inc * INC_VALUE; // Recalculate the PWM value
        }
      }// If the button is pressed
    } // If debounce passes

    // Setup for the next loop
    lastButtonState[i] = reading;
  } // For(...) each button
} // loop()
