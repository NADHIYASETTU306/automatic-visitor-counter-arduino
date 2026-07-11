#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Ultrasonic Sensor Pins
#define trigPin 13
#define echoPin 12

// Initialize LCD (I2C Address: 0x27, 16 Columns, 2 Rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables
int totalVisitors = 0;
int insideCount = 0;
int outsideCount = 0;

int currentState1 = 0;
int previousState1 = 0;

int currentState2 = 0;
int previousState2 = 0;

void setup()
{
  lcd.init();
  lcd.backlight();

  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("Visitor Counter");
  lcd.setCursor(0, 1);
  lcd.print("Initializing");
  delay(2000);
  lcd.clear();
}

void loop()
{
  long duration;
  float distance;

  // Trigger Ultrasonic Sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // Read Echo
  duration = pulseIn(echoPin, HIGH, 30000);

  // If no object detected
  if (duration == 0)
  {
    return;
  }

  // Calculate Distance
  distance = duration * 0.0343 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // ----------------------------
  // ENTRY DETECTION
  // ----------------------------
  if (distance <= 20)
    currentState1 = 1;
  else
    currentState1 = 0;

  if (currentState1 != previousState1 && currentState1 == 1)
  {
    totalVisitors++;
    insideCount++;

    Serial.println("Person Entered");
  }

  // ----------------------------
  // EXIT DETECTION
  // ----------------------------
  if (distance > 20 && distance <= 40)
    currentState2 = 1;
  else
    currentState2 = 0;

  if (currentState2 != previousState2 && currentState2 == 1)
  {
    if (totalVisitors > 0)
    {
      totalVisitors--;
    }

    outsideCount++;

    Serial.println("Person Exited");
  }

  // ----------------------------
  // LCD DISPLAY
  // ----------------------------
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("IN:");
  lcd.print(insideCount);

  lcd.setCursor(8, 0);
  lcd.print("OUT:");
  lcd.print(outsideCount);

  lcd.setCursor(0, 1);
  lcd.print("TOTAL:");
  lcd.print(totalVisitors);

  // Save Current States
  previousState1 = currentState1;
  previousState2 = currentState2;

  delay(200);
}