#include <SevenSeg.h>
#define THERMISTORPIN A0
#define THERMISTORNOMINAL 10000
#define TEMPERATURENOMINAL 25
#define NUMSAMPLES 1
#define BCOEFFICIENT 3950
#define SERIESRESISTOR 10000
#define SERIESRESISTOR 10000
int samples[NUMSAMPLES];

// Стандартная яркость дисплея (в диапазоне от 0 до 100)
int dc = 70;
int sw = 0;
int menu = 0;A
boolean Menu = true;
boolean Slider = true;
boolean regFreq = false;
boolean regBrit = false;
boolean regTime = false;
boolean Exit = true;
// boolean menu = true;

uint8_t i;
int buttons[3] = {22, 23, 24};
unsigned long curMill1 = 0;
unsigned long curMill2 = 0;
long freq = 100;
long prevMill1 = 0;
long prevMill2 = 0;
long interval1 = 100;

// Указываем библиотеке SevenSeg на каких пинах уст. дисплей
SevenSeg disp(7, 5, 4, 9, 10, 6, 3);

const int numOfDigits = 4;
int digitPins[numOfDigits] = {11, 12, 13, 2};

char buffer[20];
String output;

void setup() {
  // Отправляем библиотеке данные
  disp.setDigitPins(numOfDigits, digitPins);
  disp.setDPPin(8);

  // Включаем возможность менять аналоговое напряжение с 5 на 3.3 В
  analogReference(EXTERNAL);

  // Некий дебаунс для кнопок на пинах 22, 23, 24
  for (i = 0; i < 3; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }
}

void loop() {
  curMill2 = millis();
  if (curMill2 - prevMill2 > interval1 && Menu == true) {
    prevMill2 = curMill2;
    if (digitalRead(24) == LOW) {
      menu = menu + 1;
      if (menu > 1) {
        menu = 0;
      }
    }
  }

  switch (menu) {
    case 0:
      if  (digitalRead(22) == LOW && digitalRead(23) == LOW && digitalRead(24) == HIGH && Exit == true) {
        sw = 0;
        menu = 0;
        Menu = true;
        Exit = true;
        Slider = true;
        regFreq = false;
        regBrit = false;
        regTime = false;
      }

      float average;
      for (i = 0; i < NUMSAMPLES; i++) {
        samples[i] = analogRead(THERMISTORPIN);
        delay(1);
      }
      average = 0;
      for (i = 0; i < NUMSAMPLES; i++) {
        average += samples[i];
      }
      average /= NUMSAMPLES;
      average = 1023 / average - 1;
      average = SERIESRESISTOR / average;
      float steinhart;
      steinhart = average / THERMISTORNOMINAL;
      steinhart = log(steinhart);
      steinhart /= BCOEFFICIENT;
      steinhart += 1.0 / (TEMPERATURENOMINAL + 273.1);
      steinhart = 1.0 / steinhart;
      steinhart -= 273.1;

      // Избавляемся от второго знака после ","
      output = dtostrf(steinhart, 5, 1, buffer);

      disp.write(output);
      break;
    case 1:
      if  (digitalRead(22) == LOW && digitalRead(23) == LOW && digitalRead(24) == HIGH && Exit == true) {
        menu = 0;
        sw = 0;
        Menu = true;
        Exit = true;
        Slider = true;
        regFreq = false;
        regBrit = false;
        regTime = false;
      }

      curMill1 = millis();
      if (curMill1 - prevMill1 > interval1 && Slider == true && digitalRead(24) == HIGH) {
        prevMill1 = curMill1;
        if (digitalRead(22) == LOW) {
          sw = sw - 1;
          if (sw < 0) {
            sw = 2;
          }
        }
        if (digitalRead(23) == LOW) {
          sw = sw + 1;
          if (sw > 2) {
            sw = 0;
          }
        }
      }

      // Меню настроек (в разработке)
      switch (sw) {
        case 0:
          if  (digitalRead(22) == LOW && digitalRead(23) == LOW && digitalRead(24) == HIGH && Exit == true) {
            menu = 0;
            sw = 0;
            Menu = true;
            Exit = true;
            Slider = true;
            regFreq = false;
            regBrit = false;
            regTime = false;
          }

          // Slider = false;
          Menu = false;
          Exit = true;
          regFreq = true;
          regBrit = false;
          regTime = false;

          if (digitalRead(24) == HIGH) {
            disp.write("freq");
          }
          if (digitalRead(24) == LOW && Menu == false) {
            disp.write(freq);

            // Регулировка частоты мерцания
            if (digitalRead(22) == LOW && regFreq == true) {
              freq = freq - 1;
              if (freq < 1) {
                freq = 1;
              }
            }
            if (digitalRead(23) == LOW && regFreq == true) {
              freq = freq + 1;
              if (freq > 300) {
                freq = 300;
              }
            }
            disp.setRefreshRate(freq);
          }
          break;
        case 1:
          if  (digitalRead(22) == LOW && digitalRead(23) == LOW && digitalRead(24) == HIGH && Exit == true) {
            menu = 0;
            sw = 0;
            Menu = true;
            Exit = true;
            Slider = true;
            regFreq = false;
            regBrit = false;
            regTime = false;
          }

          // Slider = false;
          Menu = false;
          regFreq = false;
          regBrit = true;
          regTime = false;

          if (digitalRead(24) == HIGH) {
            disp.write("Brit");
          }
          if (digitalRead(24) == LOW && Menu == false) {
            disp.write(dc);

            // Регулировка яркости
            if (digitalRead(22) == LOW && regBrit == true && digitalRead(24) == LOW) {
              dc = dc - 2;
              if (dc < 10) {
                dc = 10;
              }
            }
            if (digitalRead(23) == LOW && regBrit == true && digitalRead(24) == LOW) {
              dc = dc + 2;
              if (dc > 100) {
                dc = 100;
              }
            }
            disp.setDutyCycle(dc);
          }
          break;
        case 2:
          if  (digitalRead(22) == LOW && digitalRead(23) == LOW && digitalRead(24) == HIGH && Exit == true) {
            menu = 0;
            sw = 0;
            Menu = true;
            Exit = true;
            Slider = true;
            regFreq = false;
            regBrit = false;
            regTime = false;
          }

          Menu = false;
          // Slider = false;
          regFreq = false;
          regBrit = false;
          regTime = true;

          if (regTime == true) {
            long sec = millis();
            sec /= 1000;
            sec = round(sec);
            disp.writeClock(sec);
          }
          break;
      }
      break;
  }
}
