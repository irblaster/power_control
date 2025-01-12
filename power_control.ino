
#include <ESP8266WiFi.h>


char command[16];
char incoming_array[512];


int gpio13Led = 13;
int gpio12Relay = 12;
int gpio0Button = 0;

bool currentState = true; 


void recvWithEndMarker() {
    uint16_t ndx = 0;
    //char endMarker = '\n';
    char rc;
    bool done = false;

    while (done == false )
    {
        if (Serial.available() > 0 ) {
          rc = Serial.read();
          if (rc != '\n' && rc != '\r') {
             incoming_array[ndx] = rc;
             ndx++;
//             if (ndx > 512 ) { 
//                Serial.println ("buffer overflow");
//                Serial.println (ndx);
//             }
          }  else {
            incoming_array[ndx] = '\0'; // terminate the string
            done = true;
//            Serial.println ("indx =");
//            Serial.println (ndx);
         }
      }
    }
    
}

void setup(void){

  WiFi.mode(WIFI_OFF);    //we don't need wifi, just using processor


  pinMode(gpio13Led, OUTPUT);
  digitalWrite(gpio13Led, HIGH);
  
  pinMode(gpio12Relay, OUTPUT);
  digitalWrite(gpio12Relay, HIGH);

  pinMode(gpio0Button, INPUT);
  
 
  Serial.begin(115200); 
  Serial.println("");
  Serial.println("");
  Serial.println("RS232 POWER CONTROL");
  Serial.println("https://www.irblaster.info V0.2");
  Serial.println("Supported Commands: ON, OFF, ?");
  Serial.println("");
  }
 
  void loop(void){
    while (Serial.available()==0) { 
      if (digitalRead(gpio0Button) == 0) {
        if (currentState == false) { 
          digitalWrite(gpio12Relay, HIGH);
          currentState = true;
        } else {
         digitalWrite(gpio12Relay, LOW);
          currentState = false;       
        }
        delay (400);
      }
    }
    
      recvWithEndMarker();
      sscanf (incoming_array, "%[A-Z0-9?]", command);

      if(strcmp(command, "ON") == 0) {
        digitalWrite(gpio12Relay, HIGH);
        currentState = true;
      } else if(strcmp(command, "OFF") == 0) {
        digitalWrite(gpio12Relay, LOW);
        currentState = false;
      } else if (strcmp(command, "?") == 0) {
          if (currentState) {
            Serial.println("ON");
          } else {
            Serial.println("OFF");
          }
      } else {
        Serial.println ("invalid command");      
      }
    }
   
 
  
