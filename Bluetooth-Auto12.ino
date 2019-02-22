#include "DualMC33926MotorShield.h"
#include "Servo.h"

DualMC33926MotorShield md;
Servo servo1;

const int motorPin1 = 7;    // Virta
const int motorPin2 = 9;    // Suunta
const int motor2Pin1 = 8;   // Virta
const int motor2Pin2 = 10;  // Suunta
int angle1 = 20;            // Kääntymisasteet
int angle2 = 56;




int left = 0;             // muuttuja, kääntymisen rajoitus
int right = 0;



const int enablePin = 4;  // moottorien virta/suunta enable pinni
int state;                // nykyinen komento
int flag=0;               // muuttuja, komennot print näytölle vain kerran
const int speakerPin = 6; // buzzer
int length = 70;          // Imperial marchin nuottien määrä
  // nuotit
String notes[] = {"G4","G4", "G4", "D#4/Eb4", "A#4/Bb4", "G4", "D#4/Eb4","A#4/Bb4", "G4", "D5", "D5", "D5", "D#5/Eb5", "A#4/Bb4", "F#4/Gb4", "D#4/Eb4","A#4/Bb4", "G4", "G5","G4","G4","G5","F#5/Gb5", "F5","E5","D#5/Eb5","E5", "rest", "G4", "rest","C#5/Db5","C5","B4","A#4/Bb4","A4","A#4/Bb4", "rest", "D#4/Eb4", "rest", "F#4/Gb4", "D#4/Eb4","A#4/Bb4", "G4" ,"D#4/Eb4","A#4/Bb4", "G4"}; 
  // rytmi
int beats[] = { 8, 8, 8, 6, 2, 8, 6 , 2 ,16 , 8, 8, 8, 6, 2, 8, 6, 2, 16,8,6,2,8,6,2,2, 2, 2,6,2,2,8,6,2,2,2,2,6,2,2,9,6,2,8,6,2,16  };
int tempo = 50;

  //buzzerin toiminta, HIGHlla ääntä, LOW hiljaa
void playTone(int tone, int duration) {
  for (long u = 0; u < duration * 1000L; u += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);      
  }
}

  // nuottien toimintaa, määritellään nuotit taajuuksien mukaan
void playNote(String note, int duration) {
  String noteNames[] = { "D#4/Eb4", "E4", "F4", "F#4/Gb4", "G4", "G#4/Ab4", "A4", "A#4/Bb4", "B4", "C5", "C#5/Db5", "D5", "D#5/Eb5", "E5", "F5", "F#5/Gb5", "G5", "G#5/Ab5", "A5", "A#5/Bb5", "B5", "C6", "C#6/Db6", "D6", "D#6/Eb6", "E6", "F6", "F#6/Gb6", "G6" };
  int tones[] = { 1607, 1516, 1431, 1351, 1275, 1203, 1136, 1072, 1012, 955, 901, 851, 803, 758, 715, 675, 637, 601, 568, 536, 506, 477, 450, 425, 401, 379, 357, 337, 318 };
  for (int u = 0; u < 29; u++) {
    if (noteNames[u] == note) {
      playTone(tones[u], duration);
    }
  }
}
  
void setup() {
      // lähetetään vain, ei oteta vastaan
    pinMode(speakerPin, OUTPUT);
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(enablePin, OUTPUT);
    
    servo1.attach(11);
    digitalWrite(enablePin, HIGH);    // moottorien enable pinni, HIGH: ON, LOW: OFF
    Serial.begin(9600);               // kommunikointi 9600 bittiä sekunnissa
    servo1.write(35);                 // servon kulma alussa (suoraan) 

    Serial.println("Engine online");  // viesti terminaaliin
}
  


void loop() {  
      
      
      // vastaanotettu komento tallennetaan state, flag nollataan
    if(Serial.available() > 0){
      state = Serial.read();   
      flag=0;
    }   
    
      // moottorit pois päältä
    if (state == '0') {
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW);
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, LOW);

        if(flag == 0){
          Serial.println("Motor: off");
          flag=1;
        }
    }
    
      // takamoottorit täysillä taaksepäin
    else if (state == '1') {
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2, HIGH);
        digitalWrite(motor2Pin1, HIGH);
        digitalWrite(motor2Pin2, HIGH);

        if(flag == 0){
          Serial.println("Motor: Forward");
          flag=1;
        }
    }
    
      // takamoottorit täysillä eteenpäin
    else if (state == '2') {
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, HIGH);
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, HIGH);

        if(flag == 0){
          Serial.println("Motor: Backward");
          flag=1;
          }
    }
      // etupyörien kääntyminen vasemmalle
    else if (state == '3' and left <1) {
      servo1.write(angle1);
      left = left + 1;
      Serial.println("Motor: Left");
      right = 0;
    }
      // etupyörien kääntyminen oikealle
    else if (state == '5' and right <1) {
      servo1.write(angle2);
      right = right + 1;
      Serial.println("Motor: Right");
      left = 0;
    }
      // etupyörien kääntäminen suoraan 
    else if (state == '4') {
      if (left >=1){
        servo1.write(35);
        left = 0;
        Serial.println("Motor: Backward");
      } else if (right >=1){
        servo1.write(35);
        right = 0;
        Serial.println("Motor: Backward");
        }
      
    }   



      // tööt tööt
    else if (state == '7'){
      
      state = Serial.read();
      tone (6, 330);
      delay (300);
      noTone (6);
      delay (100);
      tone (6, 330);
      delay (300);
      noTone (6);
        // muita komentoja voi laittaa samaan aikaan
      if (state == '3' and left <1) {
        servo1.write(angle1);
        left = left + 1;
        Serial.println("Motor: Left");
        right = 0;
      }
      if (state == '5' and right <1) {
        servo1.write(angle2);
        right = right + 1;
        Serial.println("Motor: right");
        right = 0;
      }
      if (state == '4') {
        if (left >=1){
          servo1.write(35);
          left = 0;
          Serial.println("Motor: Backward");
        } else if (right >=1){
          servo1.write(35);
          right = 0;
          Serial.println("Motor: Backward");
        }
      }
      if (state == '0') {
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW);
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, LOW);
        Serial.println("Motor: Off");
      }
      if (state == '1') {
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2, HIGH);
        digitalWrite(motor2Pin1, HIGH);
        digitalWrite(motor2Pin2, HIGH);
        Serial.println("Motor: Forward");
      }
      if (state == '2') {
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, HIGH);
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, HIGH);
        Serial.println("Motor: Backward");
      }
      
      }




      // sireeni
    else if (state == '8') {
    
      int x = 1;
      for (int i = 261; i > 260; i = i + x){  // äänen taajuus nousee, laskee, nousee, laskee...
        state = Serial.read();
        tone (6, i);
        delay (10);
        if (i == 392) x = -1;
        delay(2);
        if (i == 261) x = 1;
        delay(2);
        if (state == '9') {
          noTone(6);
          break;
          i = 261;
        }
          // komentoja voi syöttää sireenin ollessa päällä
        else if (state == '3' and left <1) {
          servo1.write(angle1);
          left = left + 1;
          Serial.println("Motor: Left");
          right = 0;
        }
        else if (state == '5' and right <1) {
          servo1.write(angle2);
          right = right + 1;
          Serial.println("Motor: right");
          left = 0;
        }
        else if (state == '4') {
          if (left >=1){
            servo1.write(35);
            left = 0;
            Serial.println("Motor: Backward");
          } else if (right >=1){
            servo1.write(35);
            right = 0;
            Serial.println("Motor: Backward");
          }
        }
        else if (state == '0') {
          digitalWrite(motorPin1, LOW);
          digitalWrite(motorPin2, LOW);
          digitalWrite(motor2Pin1, LOW);
          digitalWrite(motor2Pin2, LOW);
          Serial.println("Motor: Off");
        }
        else if (state == '1') {
          digitalWrite(motorPin1, HIGH);
          digitalWrite(motorPin2, HIGH);
          digitalWrite(motor2Pin1, HIGH);
          digitalWrite(motor2Pin2, HIGH);
          Serial.println("Motor: Forward");
        }
        else if (state == '2') {
          digitalWrite(motorPin1, LOW);
          digitalWrite(motorPin2, HIGH);
          digitalWrite(motor2Pin1, LOW);
          digitalWrite(motor2Pin2, HIGH);
          Serial.println("Motor: Backward");
        }
      }
    }



      // musiikki/sireeni pois päältä
    else if (state == '9') {
        noTone(6);
        length = 70;
        }
      
      
      
    
      // musiikki: The Imperial March
    else if (state == 'e'){
      for (int u = 0; u < length; u++) {
        state = Serial.read();
        if (notes[u] == "rest") {
          delay(beats[u] * tempo); 
        } else {
          playNote(notes[u], beats[u] * tempo);      
        }
          // komentoja musiikin aikana
        if (state == '9') {
          noTone(6);
          break;
          
        }
        else if (state == '3' and left <1) {
          servo1.write(angle1);
          left = left + 1;
          Serial.println("Motor: Left");
          right = 0;
        }
        else if (state == '5' and right <1) {
          servo1.write(angle2);
          right = right + 1;
          Serial.println("Motor: right");
          left = 0;
        }
        else if (state == '4') {
          if (left >=1){
            servo1.write(35);
            left = 0;
            Serial.println("Motor: Backward");
          } else if (right >=1){
            servo1.write(35);
            right = 0;
            Serial.println("Motor: Backward");
          }
        }
        else if (state == '0') {
          digitalWrite(motorPin1, LOW);
          digitalWrite(motorPin2, LOW);
          digitalWrite(motor2Pin1, LOW);
          digitalWrite(motor2Pin2, LOW);
          Serial.println("Motor: Off");
        }
        else if (state == '1') {
          digitalWrite(motorPin1, HIGH);
          digitalWrite(motorPin2, HIGH);
          digitalWrite(motor2Pin1, HIGH);
          digitalWrite(motor2Pin2, HIGH);
          Serial.println("Motor: Forward");
        }
        else if (state == '2') {
          digitalWrite(motorPin1, LOW);
          digitalWrite(motorPin2, HIGH);
          digitalWrite(motor2Pin1, LOW);
          digitalWrite(motor2Pin2, HIGH);
          Serial.println("Motor: Backward");
        }
      
          
        }
        delay(tempo / 2);
        
      }

}
     
    
