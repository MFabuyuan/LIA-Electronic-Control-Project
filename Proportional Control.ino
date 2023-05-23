
// Include LiquidCrystal library for managing with LCD screen
#include <LiquidCrystal.h>

// Define the pins for the LCD screen 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


int sensor = A1;     // Set the pin for the LDR sensor
int actuator = 10;   // Select the pin for the LED
int setPoint = 0;    // Setpoint variable value
int PV = 0;          // Process variable which is also the LDR value
int CO = 0;          // Control output value
int Error;           // Declare a variable to store the Error
float pGain;         // Declare the Proportional Gain variable

// Set the pins for the Pushbuttons
const int button1Pin = 6;   // left pushbutton
const int button2Pin = 7;   // right pushbutton
const int button3Pin = 8;   // up pushbutton
const int button4Pin = 9;   // down pushbutton

 // LCD screen variable
int Screen = 0;

// Store the current state of the pushbuttons
bool PB1state = false;
bool PB2state = false;
bool PB3state = false;
bool PB4state = false;

// Store the previous state of the pushbuttons
bool lastButtonState1 = false;
bool lastButtonState2 = false;
bool lastButtonState3 = false;
bool lastButtonState4 = false;

// Store the timestamp of the previous press for the pushbuttons
unsigned long lastButtonPress1 = 0;
unsigned long lastButtonPress2 = 0;
unsigned long lastButtonPress3 = 0;
unsigned long lastButtonPress4 = 0;

const unsigned long debounceDelay = 10;   // Delay time for debouncing buttons


void setup() {
 pinMode(button1Pin, INPUT_PULLUP);  // Declare PB1 (left) as an INPUT PULLUP
 pinMode(button2Pin, INPUT_PULLUP);  // Declare PB2 (right) as an INPUT PULLUP
 pinMode(button3Pin, INPUT_PULLUP);  // Declare PB3 (up) as an INPUT PULLUP
 pinMode(button4Pin, INPUT_PULLUP);  // Declare PB4 (down) as an INPUT PULLUP
 pinMode(actuator, OUTPUT);          // Declare actuator pin as an OUTPUT
 Serial.begin(9600);       // Initialize Serial Monitor with baud rate of 9600
 lcd.begin(16, 2);         // Set up LCD's number of colums and  rows          
}


void loop() {
 // Read the sensor's value, which will range from 0 to 1024
 PV = analogRead(sensor);   

 PB1state = digitalRead(button1Pin);   // Read state of PB1 (left)
 PB2state = digitalRead(button2Pin);   // Read state of PB2 (right)
 PB3state = digitalRead(button3Pin);   // Read state of PB3 (up)
 PB4state = digitalRead(button4Pin);   // Read state of PB4 (down)

 // Calculate the Error
 Error = setPoint - PV;     // Error is SP - PV

 // Calculate control output (CO) by multiplying the Erro by the p Gain
 // Divide it by 10 for more accurate results when working with small pGain values
 CO = (pGain * Error)/10;   

 
    if (CO > 255) {
     CO = 255;         // Putting cap on the max value to not exceed 255
   } 
    if (CO < 0) {
      CO = 0;          // Putting cap on the min value not to be less than 0
   }

   if (Error < 0) {
     Error = 0;        // Putting cap on the Error value not to go below 0
   }

   // Writing the control output (CO) to the actuator (LED)
   analogWrite(actuator, CO);


 if (!PB1state && lastButtonState1 && (millis() - lastButtonPress1) > debounceDelay) {
   Screen --;  // Decrement screen value
   if (Screen < 0) Screen= 2;
   lastButtonPress1 = millis();   // Update the last PB1 press time
 }
 lastButtonState1 = PB1state;     // Store current PB1 state for next iteration


 if (!PB2state && lastButtonState2 && (millis() - lastButtonPress2) > debounceDelay) {
   Screen ++;  // Increment screen value
   if (Screen > 2) Screen = 0;
   lastButtonPress2 = millis();   // Update the last PB2 press time
 }
 lastButtonState2 = PB2state;     // Store current PB2 state for next iteration


   if (!PB3state && lastButtonState3 && (millis() - lastButtonPress3) > debounceDelay) {
   if (Screen == 0) {  
     setPoint += 50;  // Increase SP by 50 if on the first(0) screen
   } else if (Screen == 1) {
     pGain += 0.01;   // Increase pGain by 0.01 if on the second(1) screen
   }
   lastButtonPress3 = millis();   // Update the last PB3 press time
 }
 lastButtonState3 = PB3state;     // Store current PB3 state for next iteration


 if (!PB4state && lastButtonState4 && (millis() - lastButtonPress4) > debounceDelay) {
   if (Screen == 0) {
     setPoint -= 50;   // Decrease SP by 50 if on the first(0) screen
   } else if (Screen == 1) {
     pGain -= 0.01;   // Decrease pGai by 0.01 if on the second(1) screen
   }
   lastButtonPress4 = millis();   // Update the last PB4 press time
 }
 lastButtonState4 = PB4state;     // Store current PB4 state for next iteration



 // Print SP, PV, Error, and pGain on the Serial Monitor
 // SP, PV, and Error will be also be displayed on the Serial Plotter
 Serial.print("SP:");
 Serial.print(setPoint);
 Serial.print("\t PV:");
 Serial.print(PV);
 Serial.print("\t  Error:");
 Serial.print(Error);
 Serial.print("\t  CO = ");
 Serial.print(CO);
 Serial.print("\t  pGain = ");
 Serial.println(pGain);
 //Serial.print("\t  Screen = ");
 //Serial.println(Screen);


 lcd.clear();
 // Swicth statement to handle the different cases of button press
 switch (Screen) {
   case 0:  // Display the values of PV, CO, and variable SP on LCD
     lcd.setCursor(0, 0);
     lcd.print("PV=");
     lcd.print(PV);

     lcd.setCursor(8, 0);
     lcd.print("CO=");
     lcd.print(CO);

     lcd.setCursor(0, 1);
     lcd.print("SP = ");
     lcd.print(setPoint);
     break;

    case 1:  // Display the values of PV, CO, and variable pGain on LCD
     lcd.setCursor(0, 0);
     lcd.print("PV=");
     lcd.print(PV);

     lcd.setCursor(8, 0);
     lcd.print("CO=");
     lcd.print(CO);
     
     lcd.setCursor(0, 1);
     lcd.print("pGain = ");
     lcd.print(pGain);
     break;

     case 2:  // Display the values of PV, CO, and Error on LCD
     lcd.setCursor(0, 0);
     lcd.print("PV=");
     lcd.print(PV);

     lcd.setCursor(8, 0);
     lcd.print("CO=");
     lcd.print(CO);

     lcd.setCursor(0, 1);
     lcd.print("Error= ");
     lcd.print(Error);
     break;
 }

}
