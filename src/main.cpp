#include <Arduino.h>
#include "camera.h"
#include "WiFi_func.h"
#include "SD.h"
#include "TimeLaps.h"

void setup() 
{
  Serial.begin(115200);
	Serial.println();

  SDInitFileSystem();
  CameraInit();

  WiFiInit();  
}

void loop() 
{
	unsigned long t = millis();
	static unsigned long ot = 0;
	unsigned long dt = t - ot;
	ot = t;
	TimeLapsProcess(dt);
}
