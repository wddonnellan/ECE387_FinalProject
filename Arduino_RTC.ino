#include <DFRobot_RGBMatrix.h> // Hardware-specific library
#include "RTClib.h"

#define OE    9
#define LAT   10
#define CLK   11
#define A     A0
#define B     A1
#define C     A2
#define D     A3
#define E     A4
#define WIDTH 64 // # of LEDs vertically
#define HEIGHT  64 // # of LEDs horizontally
#define b1    50 // Hour and Day Set
#define b2    51 // Minute and Month Set
#define b3    52 // AM/PM and Year Set
#define enter 53 // Button pressed once desired date and time are set

RTC_PCF8523 rtc;

DFRobot_RGBMatrix matrix(A, B, C, D, E, CLK, LAT, OE, false, WIDTH, HEIGHT);
int day_int = 1;
String day = String(day_int);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int month_int = 0;
char *Months[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
String month = Months[month_int];

int year_int = 2000;
String year = String(year_int);

String Date = month + " " + day;

int hour_int = 1;
String hour = String(hour_int);

int minute_int = 0; 
String minute = String(minute_int);

int second_int = 0;
String second = String(second_int);

String Time = hour + ":" + minute;

bool AM = true;

int white = matrix.Color333(7, 7, 7);
int black = matrix.Color333(0, 0, 0);


void setup() {
  // put your setup code here, to run once:
  matrix.begin();//This line must come before modifying the matrix screen
  matrix.setTextSize(1);
  matrix.setCursor(1,1);
  
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);
  pinMode(enter, INPUT);

   Serial.begin(57600);

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (! rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtc.start();
  
  float drift = 43; // seconds plus or minus over oservation period - set to 0 to cancel previous calibration.
  float period_sec = (7 * 86400);  // total obsevation period in seconds (86400 = seconds in 1 day:  7 days = (7 * 86400) seconds )
  float deviation_ppm = (drift / period_sec * 1000000); //  deviation in parts per million (μs)
  float drift_unit = 4.34; // use with offset mode PCF8523_TwoHours
  // float drift_unit = 4.069; //For corrections every min the drift_unit is 4.069 ppm (use with offset mode PCF8523_OneMinute)
  int offset = round(deviation_ppm / drift_unit);
  // rtc.calibrate(PCF8523_TwoHours, offset); // Un-comment to perform calibration once drift (seconds) and observation period (seconds) are correct
  // rtc.calibrate(PCF8523_TwoHours, 0); // Un-comment to cancel previous calibration

  Serial.print("Offset is "); Serial.println(offset); // Print to control offset
  print_Time();
}

void loop() {
  // put your main code here, to run repeatedly:
  DateTime i = rtc.now();
  if(i.second() == 0) {
   print_Time();
   delay(1000); 
  }
  if (digitalRead(enter) == HIGH) {
    delay(100);
    setTime_Date();
    print_Time();
  }
}



void print_Time() {
  matrix.fillScreen(black);
  matrix.setCursor(1,1);
  DateTime now = rtc.now();
  matrix.print(now.hour());
  matrix.print(":");
  if(now.minute() < 10) {
    matrix.print(0);
    matrix.print(now.minute());
  } else {
    matrix.print(now.minute());
  }
  matrix.setCursor(1,17);
  matrix.print(Months[now.month()-1]);
  matrix.print(" ");
  matrix.print(now.day());
  matrix.setCursor(1,32);
  matrix.print(now.year());
}

void setTime_Date() {
  Set_Time_toMatrix();
  while (digitalRead(enter) == LOW) {
    
    if (digitalRead(b1) == HIGH) {
      hour_int++;
      if (hour_int > 24) {
        hour_int = 1;
      }
      Set_Time_toMatrix();
      delay(100);
    }
    
    if (digitalRead(b2) == HIGH) {
      minute_int++;
      if (minute_int > 59) {
        minute_int = 0;
      }
      Set_Time_toMatrix();
      delay(100);
    }
  }
  
  delay(100);
  Set_Date_toMatrix();
  
  while(digitalRead(enter) == LOW) {
     if (digitalRead(b1) == HIGH) {
        month_int++;
        if (month_int > 11) {
          month_int = 0;
        }
        Set_Date_toMatrix();
        delay(100);
     }
    
     if (digitalRead(b2) == HIGH) {
        day_int++;
        if (day_int > 31) {
          day_int = 1;
        }
        Set_Date_toMatrix();
        delay(100);
     } 
     if (digitalRead(b3) == HIGH) {
        year_int++;
        Set_Date_toMatrix();
        delay(100);
     }
  }
  rtc.adjust(DateTime(year_int,(month_int+1),day_int,hour_int,minute_int,0));
}

void Set_Time_toMatrix() {
  matrix.fillScreen(black);
  matrix.setCursor(1,1);
  matrix.print("Set Time:");
  hour = String(hour_int);
  if(minute_int > 9) {
    minute = String(minute_int);
  } else {
    minute= "0" + String(minute_int);
  }
  second = String(second_int);

  if(AM) {
    Time = hour + ":" + minute;
  }
  else {
    Time = hour + ":" + minute;
  }
  matrix.setCursor(1,17);
  matrix.print(Time);
}

void Set_Date_toMatrix() {
  matrix.fillScreen(black);
  matrix.setCursor(1,1);
  matrix.print("Set Date:");
  month = Months[month_int];
  day = String(day_int);
  year = String(year_int);

  Date = month + " " + day;
  matrix.setCursor(1,17);
  matrix.print(Date);
  matrix.setCursor(1,32);
  matrix.print(year);
}

void Time_Date_toMatrix() {
  matrix.fillScreen(black);
  matrix.setCursor(1,1);
  hour = String(hour_int);
  if(minute_int > 9) {
    minute = String(minute_int);
  } else {
    minute= "0" + String(minute_int);
  }
  second = String(second_int);

  if(AM) {
    Time = hour + ":" + minute + " AM";
  }
  else {
    Time = hour + ":" + minute + " PM";
  }

  month = Months[month_int];
  day = String(day_int);
  year = String(year_int);

  Date = month + " " + day;
  
  matrix.print(Time);
  matrix.setCursor(1,17);
  matrix.print(Date);
  matrix.setCursor(1,32);
  matrix.print(year);
}
