#include <Arduino.h>
#include <U8x8lib.h>
#define MOSFET 2 // The MOSFET driver for the water pump on digital I/O 2
#define REDLED 4 // Big red LED on digital I/O 4
#define BUTTON 6 // Push button on digital I/O 6
#define MOISTURE A1
#define WET_THRESH 600 // (update) below 600 is considered wet.


auto display = U8X8_SSD1306_128X64_NONAME_HW_I2C(U8X8_PIN_NONE);

unsigned long elapsedTime = 0;       // "global" variable as long integer, positive values only.
unsigned long startTime = 0;         // "global" variable as long integer, positive values only.
int moistureValue = 0;
int variable=0; 
int counter=0;

void setup() {
 Serial.begin(9600);
 display.begin();
 display.setFlipMode(1);
 display.clearDisplay();
 pinMode(MOSFET, OUTPUT); // Sets the D2 pin (MOSFET + Pump) to output
 pinMode(REDLED, OUTPUT); // Sets the D4 pin (LED) to output
 pinMode(BUTTON, INPUT); // Sets the D6 pin (Button) to input
 digitalWrite(MOSFET, LOW);    // pump off

display.begin(); // start up the OLED display
    display.setFlipMode(1); // set to 1 or 0, depending on orientation of board
    display.clearDisplay();                     // blank display
    display.setFont(u8x8_font_profont29_2x3_r); // set font

 
}
void sendMoistureData() {
 const auto value = analogRead(A1);
 const byte data[] = {0, 0, highByte(value), lowByte(value)};
 Serial.write(data, 4); // send 0,0, "high byte", "low byte"
 Serial.println(); // send "newline"
}

void loop() {
display.setFont(u8x8_font_profont29_2x3_r);
 display.setCursor(0, 0);
auto data = -50;
   
if (!Serial.available()) {
 const auto receivedData = Serial.read();
 data=receivedData;
}
else {
   

const auto receivedData = Serial.read();
data=receivedData;

char buf[16];
 
if(receivedData<100){
 sprintf(buf, "%03d", receivedData);
 display.print(buf);
 
}
}

if (data==255){
  variable=1;
}

if (data==0){
  variable=0;
}

if (variable==1){
  digitalWrite(MOSFET, HIGH);
   
}

else {
  

//For Moisture Sensor 

    // Read soil moisture sensor & assign to a variable
    moistureValue = analogRead(MOISTURE);
    
    

  if(moistureValue>=650) // Dry soil 
  {
      //digitalWrite(MOSFET, HIGH);    // pump on
      digitalWrite(REDLED, HIGH);    // LED on

      //display.print("water!  ");     // msg on OLED

   } 
   else   // wet soil
   {
      digitalWrite(MOSFET, LOW);    // pump off
      digitalWrite(REDLED, LOW);    // LED off
      
      //display.print("no water");

     

   }
   
   counter=counter+1; 

if (counter==20000){
sendMoistureData();
  counter=0;

}
}
}
