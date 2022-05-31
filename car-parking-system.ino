#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

#include <Servo.h>

Servo myservo; // create servo object to control a servo
// twelve servo objects can be created on most boards

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
  attachInterrupt(digitalPinToInterrupt(IRSensorOutPin), car_out, RISING);

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

void car_in()
{
  delay(100);
  if (digitalRead(IRSensorInPin) == 1)
  {
    car_in_gate = true;
  }
  else
  {
    car_at_gate = true;
  }
}

void car_out()
{
  delay(100);
  if (digitalRead(IRSensorOutPin) == 1)
  {
    car_out_gate = true;
  }
}

void openGate()
{
  myservo.write(90);
}

void closeGate()
{
  myservo.write(0);
}

void display_lcd(String text1, String text2 = "")
{

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(text1);
  lcd.setCursor(0, 1);
  lcd.print(text2);
}

void display_count()
{

  display_lcd(" Net Space: " + String(max_car_count - count), "Cars: " + String(count));
}

void loop()
{
  delay(50);

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
    if (count < max_car_count)
    {
      delay(2000);
      closeGate();
      count++;
      display_count();
    }
    car_in_gate = false;
  }


  if (car_out_gate)
  {
    if (count)
    {
      count--;
      display_count();
    }

    car_out_gate = false;
  }
}
