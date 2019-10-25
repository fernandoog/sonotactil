// www.Electrosmash.com/pedalshield-mega
/* pedalshield_mega_daftpunk_distortion_octaver.ino creates a lo-fi distortion effect similar to the used 
 * in Television Rules the Nation by Daft Punk. It also octaves the signal up/down presing the pushbuttons
 */
//defining the output PWM parameters
#define PWM_FREQ 0x00FF // pwm frequency - 31.3KHz
#define PWM_MODE 0 // Fast (1) or Phase Correct (0)
#define PWM_QTY 2 // 2 PWMs in parallel

//other variables

int input;
int dist_variable=10;
byte ADC_low, ADC_high;


void setup() {
//setup IO
  pinMode(6, OUTPUT); //PWM0 as output
  pinMode(7, OUTPUT); //PWM1 as output
  
  // setup ADC
  ADMUX = 0x60; // left adjust, adc0, internal vcc
  ADCSRA = 0xe5; // turn on adc, ck/32, auto trigger
  ADCSRB = 0x00; // ADC free running mode
  DIDR0 = 0x01; // turn off digital inputs for adc0

  // setup PWM
  TCCR4A = (((PWM_QTY - 1) << 5) | 0x80 | (PWM_MODE << 1)); //
  TCCR4B = ((PWM_MODE << 3) | 0x11); // ck/1
  TIMSK4 = 0x20; // interrupt on capture interrupt
  ICR4H = (PWM_FREQ >> 8);
  ICR4L = (PWM_FREQ & 0xff);
  DDRB |= ((PWM_QTY << 1) | 0x02); // turn on outputs
  sei(); // turn on interrupts - not really necessary with arduino
}

void loop() 
{
 
}

ISR(TIMER4_CAPT_vect) {

  // get ADC data
  ADC_low = ADCL; // you need to fetch the low byte first
  ADC_high = ADCH;
  //construct the input sumple summing the ADC low and high byte.
  input = ((ADC_high << 8) | ADC_low) + 0x8000; // make a signed 16b value
  OCR4AL = ((input + 0x8000) >> 8); // convert to unsigned, send out high byte
  OCR4BL = input; // send out low byte

}
