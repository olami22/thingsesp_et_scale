/*
 *  15th may 2023
 *  Adeola Akinola
 */


#include <HX711.h>
#include <WiFi.h>    
#include <ThingESP.h>

#include <Pushbutton.h>
 // Replace with your usrname, projectname and projectusername from your thingsesp account
ThingESP32 thing("username", "ProjectName","ProjectUsername");

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;

HX711 scale;
int reading;
float read1;
int lastReading;

//REPLACE WITH YOUR CALIBRATION FACTOR
#define CALIBRATION_FACTOR 101.5

//Button
#define BUTTON_PIN 21
Pushbutton button(BUTTON_PIN);

void displayWeight(float weight){

  Serial.println("Weight:");
  Serial.print(weight);
  Serial.print(" ");
  Serial.print("kg");

}

void setup() {
  Serial.begin(115200);

 thing.SetWiFi("SSID", "PASSWORD");
  thing.initDevice();

  delay(2000);
  
  Serial.println("Initializing the scale");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale(CALIBRATION_FACTOR);   // this value is obtained by calibrating the scale with known weights; 
  scale.tare(); 

}

String HandleResponse(String query)
{
  
  if (button.getSingleDebouncedPress()){
    Serial.print("tare...");
    scale.tare();
  }
  
  if (scale.wait_ready_timeout(200)) {
    reading = round(scale.get_units());
     read1 = reading /1000.0;
    Serial.print("Weight: ");
    Serial.println(read1);
    delay(2000);
 
    if (read1 != lastReading){
      displayWeight(read1); 
    }
    lastReading = read1;
  }
  else {
    Serial.println("HX711 not found.");
  }
  delay(500);

String readin = "Weight : " + String(read1) + "kg \n";  

  if (query =="Weight") {
    
    return readin;
  }
  
}  

void loop() {
  
thing.Handle();
 
}
