#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9

int Red = 4;
int Green = 5;
int Bell = 7;
int buttonState = 0;
int lastButtonState = 0;

unsigned long interval = 20000; // the time we need to wait
unsigned long previousMillis; // millis() returns an unsigned long.
unsigned long currentMillis;


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo servo;
int pos = 0;
const int buttonPin = 8;
int sensorVal;
int counter = 0;
int stat;


void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  servo.attach(6);
  servo.write(0);
  pinMode(buttonPin, INPUT);
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Bell, OUTPUT);
}

void loop()
{

  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    if (buttonState == LOW)
    {
      counter++;
      delay(100);
    }
  }
  lastButtonState = buttonState;

  if (counter == 0) {
    servo.write (25);
    digitalWrite(Green, HIGH);
    digitalWrite(Red, LOW);
    currentMillis = millis();
    if ((unsigned long)(currentMillis - previousMillis) >= interval) {
      counter = 1;
    }
  }

  else if (counter == 1) {
    servo.write(0);
    digitalWrite(Green, LOW);
    digitalWrite(Red, HIGH);
    previousMillis = millis();
  }

  else
    counter = 0;

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "C0 00 04 15" || content.substring(1) == "47 99 B2 DD" || content.substring(1) == "97 9F 65 DD" || content.substring(1) == "77 7D 67 DD" || content.substring(1) == "B7 45 F4 DD" || content.substring(1) == "A7 B1 62 DD" || content.substring(1) == "47 19 67 DD" || content.substring(1) == "37 AA 61 DD" || content.substring(1) == "27 2A 64 DD" || content.substring(1) == "F7 59 6C DD" || content.substring(1) == "E7 BD 60 DD" || content.substring(1) == "01 02 03 04" || content.substring(1) == "5D 5F EE C5") //change here the UID of the card/cards that you want to give access
  {
    if (counter == 1) {
      Serial.println("Opened");
      servo.write (25);
      digitalWrite(Green, HIGH);
      digitalWrite(Red, LOW);
      counter = 0;
      digitalWrite(Bell, HIGH);
      delay(200);
      digitalWrite(Bell, LOW);
      delay(500);

      currentMillis = millis();
      if ((unsigned long)(currentMillis - previousMillis) >= interval) {
        counter = 1;
      }
    }

    else {
      Serial.println("Closed");
      servo.write(0);
      digitalWrite(Green, LOW);
      digitalWrite(Red, HIGH);
      counter = 1;
      digitalWrite(Bell, HIGH);
      delay(200);
      digitalWrite(Bell, LOW);
      delay(500);
      previousMillis = millis();
    }
  }

  else   {

    if (counter == 0) {
      Serial.println("Access denied");
      digitalWrite(Bell, HIGH);
      digitalWrite(Green, LOW);
      delay(100);
      digitalWrite(Bell, LOW);
      digitalWrite(Green, HIGH);
      delay(100);
      digitalWrite(Bell, HIGH);
      digitalWrite(Green, LOW);
      delay(100);
      digitalWrite(Bell, LOW);
      digitalWrite(Green, HIGH);
      delay(100);
      digitalWrite(Green, LOW);
      delay(100);
      digitalWrite(Green, HIGH);
      delay(500);
    }

    else {
      Serial.println("Access denied");
      digitalWrite(Bell, HIGH);
      digitalWrite(Red, LOW);
      delay(100);
      digitalWrite(Bell, LOW);
      digitalWrite(Red, HIGH);
      delay(100);
      digitalWrite(Bell, HIGH);
      digitalWrite(Red, LOW);
      delay(100);
      digitalWrite(Bell, LOW);
      digitalWrite(Red, HIGH);
      delay(100);
      digitalWrite(Red, LOW);
      delay(100);
      digitalWrite(Red, HIGH);
      delay(500);
    }
  }


}
