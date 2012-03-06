#include <LiquidCrystal.h>
#include "notes.h"

const int lightSource = 8;
const int button = A5;
const int sensor = A4;
const int buzzer = 9;
// lcd @ BUS 2
LiquidCrystal lcd(10, 11, 12, 13, 14, 15, 16);

const int numMessages = 8;
char *messages[numMessages] = {
    // 1234567890123456
      "  Omnomomomom!",
      "  Danke sch\xefn!",
      "   Gib zwei!",
      "    Merci!",
      "    Yam, yam!",
      "   Mehr! Mehr!",
      "KtT sagt danke!",
      "Error: try again",
};
// \xef ö
// \xf5 ü
// \xe1 ä

// tune me
const int sensorThreshold = 3;

int sensorCalib;
enum gameModes {
    WAITING,
    COUNTING,
};
enum gameModes mode = WAITING;
unsigned long startCounter;

int lastMessage = 0;

void displayText(char * text, int repeat=5) {
    int row = random(0, 2);
    for (int i = 0; i < repeat; i++) {
        lcd.clear();
        lcd.setCursor(0, row);
        lcd.print(text);
        delay(300);
        lcd.clear();
        switch (i) {
            case 0:
                tone(buzzer, NOTE_C5,  50);
                delay(100);
                break;
            case 1:
                tone(buzzer, NOTE_C2,  50);
                delay(100);
                break;
            case 2:
                tone(buzzer, NOTE_C7,  50);
                delay(100);
                break;
        }
    }
}

bool inline sensorTriggered() {
    int sensorValue = analogRead(sensor);
    return abs(sensorValue - sensorCalib) > sensorThreshold;
}

void displayWaitScreen() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("KtT Spendenspiel");
}


void setup() {
    lcd.begin(2, 16);
    lcd.clear();
    Serial.begin(57600);

    pinMode(buzzer, OUTPUT);
    pinMode(lightSource, OUTPUT);
    digitalWrite(lightSource, HIGH);

    pinMode(sensor, INPUT);
    sensorCalib = analogRead(sensor);
    displayWaitScreen();
}


void loop() {
    if (digitalRead(button) == HIGH) {
        sensorCalib = analogRead(sensor);
    }

    if (mode == WAITING && (millis() - startCounter) > 2000) {
        mode = COUNTING;
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sch\xe1tze 3 Sek.");
        lcd.setCursor(0, 1);
        lcd.print("Ab 3 ");
        delay(1000);
        lcd.print("2 ");
        delay(1000);
        lcd.print("1 ");
        delay(1000);
        lcd.print("JETZT!");
        startCounter = millis();
    }
    
    if (sensorTriggered()) {
        if (mode == COUNTING) {
            mode = WAITING;
            word duration = millis() - startCounter;
            tone(buzzer, NOTE_C7,  50);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print((float)duration/1000, 3);
            lcd.print(" Sekunden.");
            
            lcd.setCursor(0, 1);
            int delta = duration - 3000;
            delta = abs(delta);
            
            if (delta < 50) {
                lcd.print("  Perfekt!");
            } else if (delta < 200) {
                lcd.print("  Sehr gut!");
            } else if (delta < 500) {
                lcd.print("   Knapp!");
            } else if (delta < 800) {
                lcd.print("   Noch OK");
            } else if (delta < 1500) {
                lcd.print(" Hallo timing?");
            } else {
                lcd.print("Total daneben...");
            }
            
            delay(4000);
        } else {
            lastMessage += 1;
            if (lastMessage >= numMessages) {
                lastMessage = 0;
            }
            displayText(messages[lastMessage]);
        }
        displayWaitScreen();
        startCounter = millis();
    }
    if (mode == COUNTING && (millis() - startCounter) > 5000) {
        mode = WAITING;
        displayWaitScreen();
        startCounter = millis();
    }
}

