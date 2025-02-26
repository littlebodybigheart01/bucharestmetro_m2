#include <SoftwareSerial.h>
#include <CytronEZMP3.h>

SoftwareSerial mySerial(10, 11); // RX, TX

CytronEZMP3 mp3;

#define JOYSTICK_VRX_PIN A0
#define JOYSTICK_SW_PIN 8

int currentStation = 5; // Pornim de la stația 5
bool playingStation = false;
bool playingDoor = false;

unsigned long stationStartTime = 0;
const unsigned long stationDuration = 5000; // 5 secunde pentru redare stație

unsigned long doorStartTime = 0;
const unsigned long doorDuration = 6000; // 6 secunde pentru semnal uși

void setup() {
  pinMode(JOYSTICK_VRX_PIN, INPUT);
  pinMode(JOYSTICK_SW_PIN, INPUT_PULLUP);

  Serial.begin(9600);       // Pentru monitorul serial
  mySerial.begin(9600);     // Pentru comunicarea cu Mega
  mp3.begin();
  mp3.setVolume(20);

  Serial.println("UNO Ready! Starting at station 5.");
  Serial.println("Joystick stanga/dreapta schimba statie, apasare pentru semnal usi.");
}

void loop() {
  int joystickValue = analogRead(JOYSTICK_VRX_PIN);
  bool joystickPressed = (digitalRead(JOYSTICK_SW_PIN) == LOW);

  // Doar dacă nu redăm nimic
  if (!playingStation && !playingDoor) {
    // Mișcare dreapta
    if (joystickValue > 600) {
      if (currentStation < 15) {
        currentStation++;
        Serial.print("Moved right to station ");
        Serial.println(currentStation);
        playStation();
      } else {
        Serial.println("Already at last station.");
      }
    }

    // Mișcare stânga
    if (joystickValue < 400) {
      if (currentStation > 1) {
        currentStation--;
        Serial.print("Moved left to station ");
        Serial.println(currentStation);
        playStation();
      } else {
        Serial.println("Already at first station.");
      }
    }

    // Apăsare joystick pentru semnal uși
    if (joystickPressed) {
      Serial.println("Joystick pressed - playing door signal.");
      playDoorSignal();
      delay(200); // Debounce scurt
    }
  }

  // Gestionarea redării stației
  if (playingStation) {
    if (millis() - stationStartTime >= stationDuration) {
      playingStation = false;
      Serial.println("Station playback done.");
    }
  }

  // Gestionarea semnalului de uși
  if (playingDoor) {
    if (millis() - doorStartTime >= doorDuration) {
      playingDoor = false;
      Serial.println("Door signal done.");
    }
  }

  // Mic delay pentru stabilitate
  delay(100);
}

void playStation() {
  Serial.print("Playing station: 00");
  if (currentStation < 10) Serial.print("0");
  Serial.print(currentStation);
  Serial.println(".mp3");

  mp3.playTrack(currentStation);

  // Trimite indexul stației la Mega
  mySerial.write((byte)currentStation); // Cast explicit pentru a evita eroarea
  Serial.println("Data (station) sent to Mega.");

  stationStartTime = millis();
  playingStation = true;
}

void playDoorSignal() {
  Serial.println("Playing 0016.mp3 - door closing signal.");
  mp3.playTrack(16);

  // Trimite 0 la Mega pentru semnalul ușilor
  mySerial.write((byte)0); // Cast explicit pentru a evita eroarea
  Serial.println("Data (door) sent to Mega.");

  doorStartTime = millis();
  playingDoor = true;
}
