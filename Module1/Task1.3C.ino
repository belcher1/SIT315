// Set the PINS for the button and LED
const uint8_t SENSOR_PIN = 7;
const uint8_t LED_PIN = 13;
const uint8_t LED2_PIN = 12;

// Set PIN for PID sensor
const uint8_t PID_PIN = 2;

// Variable for reading the PID state
volatile int PIDState = 0;

// Interrupt logic
ISR(TIMER1_OVF_vect)
{
  // Reset timer value so interrupt triggers every 100ms
  TCNT1 = 40535;
  
  long duration, cm;
  
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(SENSOR_PIN, OUTPUT);
  digitalWrite(SENSOR_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(SENSOR_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(SENSOR_PIN, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(SENSOR_PIN, INPUT);
  duration = pulseIn(SENSOR_PIN, HIGH);

  cm = microsecondsToCentimeters(duration);

  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  if(cm < 100)
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }
}

long microsecondsToCentimeters(long microseconds) {
  	// The speed of sound is 340 m/s or 29 microseconds per centimeter.
    // The ping travels out and back, so to find the distance of the object we
    // take half of the distance travelled.
 	return microseconds / 29 / 2; 
}

void setup()
{
  // Start Serial monitor
  Serial.begin(9600);
  
  // Initialise Timer1A
  TCCR1A = 0;
  // Initialise Timer1B
  TCCR1B = 0;
  // Set Timer Prescaler to 64
  TCCR1B |= B00000011;
  // Timer Preloading so that first interrupt is at 100ms
  TCNT1 = 40535;
  // Enable Timer Overflow Interrupt
  TIMSK1 |= B00000001;
  // Set LED Pins to output
  pinMode(LED_PIN, OUTPUT);
  
  pinMode(LED2_PIN, OUTPUT);
  
  // Set PID pin to input
  pinMode(PID_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PID_PIN), handlePID, CHANGE);
}

void handlePID() 
{
  Serial.println("PID On / Off");
  
  // Read the state of the PID sensor
  PIDState = digitalRead(PID_PIN);
  // Change the state of the LED
  digitalWrite(LED2_PIN, PIDState);
}
 
void loop()
{
  /*
  Serial.println(PIDState);
  delay(1000);
  */
}  
