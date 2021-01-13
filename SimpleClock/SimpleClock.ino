//==========================================
// Title:  Simple Clock for PocketScreen
// Author: Willem van dam
// Website: willemvandam.nl
// Github: https://github.com/Wimmie/PocketScreen-SimpleClock
//==========================================

#include <PocketScreen.h>
PocketScreen pocketscreen;
const uint16_t textColor = ORANGE_16b;
const int xScreenCenter = pocketscreen.xMax / 2;
const int yScreenCenter = pocketscreen.yMax / 2;
const int minuteIndicatorLenght = pocketscreen.yMax / 2 * 0.6;
const int hourIndicatorLenght = minuteIndicatorLenght / 2;

// Only use 5, 6, or 7 for the font size!
const int fontSize = 5;

// Margin between the minute indicator and the center of the hour numbers
const int indicatorMargin = 8;
const int indicatorPlusMargin = minuteIndicatorLenght + indicatorMargin;
int xClockHours[12], yClockHours[12];

// Duration of a minute in ms
const int minuteDuration = 200;
int hour, minute, lastMillis;

void setup() {
  pocketscreen = PocketScreen();
  pocketscreen.begin();
  pocketscreen.setBitDepth(BitDepth16);
  if (fontSize == 5) {
    pocketscreen.setFont(pocketScreen5pt);
  } else if (fontSize == 6) {
    pocketscreen.setFont(pocketScreen6pt);
  } else {
    pocketscreen.setFont(pocketScreen7pt);
  }
  pocketscreen.setFontColor(textColor, BLACK_16b);

  for (int i = 0; i < 12; i++) {
    String printText;
    if (i == 0) {
      printText = "12";
    } else {
      printText = String(i);
    }
    char charArray[printText.length() + 1];
    printText.toCharArray(charArray, printText.length() + 1);
    int printWidth = pocketscreen.getPrintWidth(charArray);

    xClockHours[i] = findHandPositionX(findHourAngle(i, 0), indicatorPlusMargin) - printWidth / 2;
    yClockHours[i] = findHandPositionY(findHourAngle(i, 0), indicatorPlusMargin) - fontSize / 2;
  }
}

void loop() {
  if (millis() - lastMillis > minuteDuration) {
    lastMillis = millis();
    minute++;
    if (minute > 59) {
      minute = 0;
      hour++;
      if (hour > 11) {
        hour = 0;
      }
    }
    pocketscreen.clearScreen();
    showClock(hour, minute);
  }
}

void showClock(int hour, int minute) {
  pocketscreen.clearScreen();
  for (int i = 0; i < 12; i++) {
    pocketscreen.setCursor(xClockHours[i], yClockHours[i]);
    if (i == 0) {
      pocketscreen.print(12);
    } else {
      pocketscreen.print(i);
    }
  }
  drawHourHand(hour, minute);
  drawMinuteHand(minute);
}

/*
  Returns the hour angle in degrees
*/
int findHourAngle(float hour, float min)
{
  // the degrees start at 3:00 (0 / 360 degrees)
  hour -= 3;
  return (hour + min / 60) * 360 / 12;
}

/*
  Returns the minute angle in degrees
*/
int findMinuteAngle(float min) {
  // the degrees start at 0:15 (0 / 360 degrees)
  min -= 15;
  return (min * 360) / (60);
}

int findHandPositionX(float angle, int handLength) {
  return xScreenCenter + cos(radians(angle)) * handLength;
}

int findHandPositionY(float angle, int handLength) {
  return yScreenCenter + sin(radians(angle)) * handLength;
}

void drawHourHand(int hour, int minute) {
  float hourAngle = findHourAngle(hour, minute);
  int x = findHandPositionX(hourAngle, hourIndicatorLenght);
  int y = findHandPositionY(hourAngle, hourIndicatorLenght);
  pocketscreen.drawLine(xScreenCenter, yScreenCenter, x, y, textColor);
}

void drawMinuteHand(int minute) {
  float minuteAngle = findMinuteAngle(minute);
  int x = findHandPositionX(minuteAngle, minuteIndicatorLenght);
  int y = findHandPositionY(minuteAngle, minuteIndicatorLenght);
  pocketscreen.drawLine(xScreenCenter, yScreenCenter, x, y, textColor);
}
