#include <SoftwareSerial.h>
#include<string.h>
SoftwareSerial gsm_module(9, 10);   //initialise serial ports for communication with GSM module

#include <LiquidCrystal.h>
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);   //initialise LCD display

int pump=13;   //pin for contrlling water pump
int LED=12;    //pin for controlling LEDs
int buz=11;    //pin for controlling buzzer
int d=0;

int ledState = LOW;    //initial LED state
int pumpState = LOW;   //initial water pump state
unsigned long previousMillis = 0; 
const long interval = 200;  
unsigned long previousMillis2 = 0; 
const long interval2 = 30000;  

void setup() {
    gsm_module.begin(9600);    //start communication with GSM
    lcd.begin(16, 2);          //start communication with LCD
    pinMode(LED, OUTPUT);      //initialise LED pin as OUTPUT
    pinMode(buz, OUTPUT);      //initialise buzzer pin as OUTPUT
    pinMode(pump, OUTPUT);     //initialise pump pin as OUTPUT
    lcd.print("HEllO");
    Serial.begin(9600);        //start serial communication with PC
}

void loop() {
    d=Serial.read();        //read output from PC
   
    if(d == 'p') {         //if fire detected
        lcd.clear();
        lcd.print(" Fire Detected ");    //diaplay message on LCD
        SendMessage();     //send message to owners number
        Blink();           //activate water pump, LEDs and buzzer
    }
  
    if(d == 's' ) {        //if no fire detected
        lcd.clear();
        lcd.print("Capturing Video...");    
        digitalWrite(LED, LOW); 
        digitalWrite(buz, LOW); 
        unsigned long currentMillis2 = millis();
        if((unsigned long)(currentMillis2 - previousMillis2) >= interval2) {   
            digitalWrite(pump, LOW);
            previousMillis2 = currentMillis2;
        }
        delay(5);
    }
}

void SendMessage() {        //Functon for sending message
    gsm_module.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    gsm_module.println("AT+CMGS=\"+919988872138\"\r"); //Phone number on which alert is sent
    delay(1000);
    gsm_module.println("Fire detected at LAT: 30.742308, LONG: 76.738843");    //SMS alert
    delay(100);
    gsm_module.println((char)26);    //ASCII code of CTRL+Z
    delay(1000);
}

void Blink() {      //Function for controlling LEDs, pump, buzzer
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledState = HIGH;
        digitalWrite(LED, HIGH);
        digitalWrite(buz, HIGH);
        digitalWrite(pump, HIGH);
        delay(5000);
        
    }
}
