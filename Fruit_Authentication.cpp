#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Change the I2C address (0x27) if necessary

float calibration_value = 23.34;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;

float ph_act;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long previousMillis = 0;
const long interval = 500L; // Interval in milliseconds

void setup() {
  Wire.begin();
  lcd.init();                      // Initialize the LCD
  lcd.backlight();                 // Turn on the backlight
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);

  // Initialize previousMillis to the current time
  previousMillis = millis();

  
 
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Save the last time the display was updated

    // Call your display function here
    display_pHValue();
  }

  // The rest of your loop code here
  for (int i = 0; i < 10; i++) {
    buffer_arr[i] = analogRead(A0);
    delay(30);
  }

  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buffer_arr[i] > buffer_arr[j]) {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }

  avgval = 0;
  for (int i = 2; i < 8; i++)
    avgval += buffer_arr[i];

  float volt = (float)avgval * 5.0 / 1024 / 6;
  
  ph_act = -5.70 * volt + calibration_value;

  Serial.println();
  Serial.println();
  Serial.print("Voltage: ");
  Serial.println(volt);
  Serial.print("pH Value: ");
  Serial.println(ph_act);
  Serial.print("Fruit Authenticity: "); 
  lcd.setCursor(0, 0);          
  lcd.print("Fruit Authenticity:");      
  lcd.setCursor(0, 1);     
  if (ph_act > 7)
  {
    lcd.print("Non Edible");
    Serial.println("NOT EDIBLE");
  }    
  else
  {
    lcd.print("Edible");
    Serial.println("EDIBLE");
  }  
  delay(2500);
}

void display_pHValue() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("pH: ");

  display.setTextSize(2);
  display.setCursor(55, 0);
  display.print(ph_act);
  display.display();
}