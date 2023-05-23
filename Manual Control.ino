
// Include LiquidCrystal library for managing with LCD screen
#include <LiquidCrystal.h>

// Define the pins for the LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


int sensor = A0;    // Set the pin for the Sensor
int pM = A5;        // Set the pin for the potentiometer which will be the SP
int actuator = 10;  // Select the pin for the LED
int PV;             // The process variable that also represents LDR value
int inputVal;       // Potentiometer value
int CO;             // Control output value

void setup() {
  pinMode(actuator, OUTPUT);  // Declare the actuator pin as an OUTPUT
  Serial.begin(9600);         // Initalize Serial Monitor with baud rate of 9600
  lcd.begin(16, 2);           // Set up LCD's number of colums and rows
}

void loop() {
  // Read the sensor's value, which will range from 0 to 1024
  PV = analogRead(sensor);    

  // Read the input value ranges from 0 to 1024.
  inputVal = analogRead(pM);

  // Map the input value to the control output range of 0 to 255
  CO = map (inputVal, 0, 1023, 0, 255);

  // Writing the control output (CO) to the actuator (LED)
  analogWrite(actuator, CO);   // Writing the CO to the actuator

  // Print the Input value, PV and CO on the Serial Monitor
  Serial.print("input:");  
  Serial.print(inputVal);
  Serial.print("\t  PV:");
  Serial.print(PV);
  Serial.print("\t  CO:");
  Serial.println(CO);

  // Display input value on the LCD Screen
  lcd.setCursor(0,0);
  lcd.print("input=");
  lcd.print(inputVal);
  lcd.print("  ");

  // Display CO and PV values on the LCD Screen
  lcd.setCursor(0,1);
  lcd.print("CO=");
  lcd.print(CO);
  lcd.print(" PV=");
  lcd.print(PV);
  lcd.print("  ");
 
}
