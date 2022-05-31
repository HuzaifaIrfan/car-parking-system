#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <Servo.h>

Servo myservo;

int servoPin = 6;
int IRSensorInPin = 2;
int IRSensorOutPin = 3;

int pos = 0;

int max_car_count = 5;
int count = 0;

void setup()
{
  Serial.begin(9600);
  myservo.attach(servoPin); // attaches the servo on pin 6 to the servo object
  pinMode(IRSensorInPin, INPUT_PULLUP);
  pinMode(IRSensorOutPin, INPUT_PULLUP);
  closeGate();
  Serial.println(count);
  attachInterrupt(digitalPinToInterrupt(IRSensorInPin), car_in, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IRSensorOutPin), car_out, CHANGE);

  lcd.init();
  lcd.clear();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello, world!");
  lcd.setCursor(0, 1);
  lcd.print("IM Project");
}

bool car_at_gate = false;
bool car_in_gate = false;
bool car_out_gate = false;
bool car_at_out_gate = false;

void car_in()
{

  delay(100);
  if (digitalRead(IRSensorInPin) == 1)
  {
    car_in_gate = true;
    Serial.println("car in");
  }
  else
  {
    car_at_gate = true;
    Serial.println("car gate");
  }
}

void car_out()
{
  delay(100);
  if (digitalRead(IRSensorOutPin) == 1)
  {
    if (car_at_out_gate)
    {
      car_out_gate = true;
      car_at_out_gate = false;
      Serial.println("car out");
    }
  }
  else
  {
    car_at_out_gate = true;
  }
}

void display_lcd(String text1, String text2 = "")
{

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(text1);
  lcd.setCursor(0, 1);
  lcd.print(text2);
  Serial.println(text1 + " " + text2);
}

void display_count(String message = "")
{
  display_lcd("  Net Space: " + String(max_car_count - count), "Cars:" + String(count) + "  " + message);
}

void openGate()
{
  myservo.write(90);
  display_count(" Opening");
  Serial.println("gate open");
}

void closeGate()
{
  myservo.write(0);
  Serial.println("gate close");
}

void loop()
{
  delay(300);

  if (car_out_gate)
  {
    if (count)
    {

      if (digitalRead(IRSensorOutPin) == 1)
      {
        count--;
        display_count();

        if (digitalRead(IRSensorInPin) == 0)
        {
          car_at_gate = true;
        }
      }

      car_out_gate = false;
    }
  }

  if (car_at_gate)
  {

    if (count < max_car_count)
    {
      openGate();
    }
    else
    {
      display_lcd("Not enough space");
    }
    car_at_gate = false;
  }

  if (car_in_gate)
  {
    delay(100);
    if (digitalRead(IRSensorInPin) == 1)
    {
      if (count < max_car_count)
      {
        display_count(" Closing");
        delay(2000);
        closeGate();
        count++;
        display_count();
      }
      car_in_gate = false;
      car_at_gate = false;
    }
  }
  else
  {
    car_in_gate = false;
  }
}
