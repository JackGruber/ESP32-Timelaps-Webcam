#include "Arduino.h"
#include "esp_camera.h"
#include <stdio.h>
#include "SD.h"

unsigned int fileIndex = 0;
unsigned int lapseIndex = 0;
unsigned long frameInterval = 1000;
bool mjpeg = true;
bool lapseRunning = false;
unsigned long lastFrameDelta = 0;

void TimeLapsSetInterval(unsigned long delta)
{
    frameInterval = delta;
}

bool TimeLapsStart()
{
    if(lapseRunning) return true;
    fileIndex = 0;
    char path[32];
    for(; lapseIndex < 10000; lapseIndex++)
    {
        sprintf(path, "/lapse%03u", lapseIndex);
        if (!SDFileExists(path))
        {
            SDCreateDir(path);
            lastFrameDelta = 0;
            lapseRunning = true;
            return true;
        }
    }
	return false;
}

bool TimeLapsStop()
{
    lapseRunning = false;
    return true;
}

bool TimeLapsProcess(unsigned long dt)
{
    if(!lapseRunning) return false;

    lastFrameDelta += dt;
    if(lastFrameDelta >= frameInterval)
    {
        lastFrameDelta -= frameInterval;
        camera_fb_t *fb = NULL;
        fb = esp_camera_fb_get();
        if (!fb)
        {
	        Serial.println("Camera capture failed");
	        return false;
        }

        char path[32];
        sprintf(path, "/lapse%03u/pic%05u.jpg", lapseIndex, fileIndex);
        Serial.println(path);
        if(!SDWriteFile(path, (const unsigned char *)fb->buf, fb->len))
        {
            lapseRunning = false;
            return false;
        }
        fileIndex++;
        esp_camera_fb_return(fb);
    }
    return true;
}
