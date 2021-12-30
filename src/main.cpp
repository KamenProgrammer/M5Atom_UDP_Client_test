#include <vector>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "M5Atom.h"
#include "LEDWalker/LEDWalker.h"

static void WiFi_setup(void);

const char ssid[] = "ESP32_wifi"; // SSID
const char pass[] = "esp32pass";  // password

static WiFiUDP wifiUdp;
static const char *DestinationIpadr = "192.168.4.1";
static const int DestinationUdpPort = 10000;

int led_posX = 0;
int led_posY = 0;
float accX = 0, accY = 0, accZ = 0;
bool IMU6886Flag = false;

std::vector<LEDWalker *> walkers;

void setup()
{
    M5.begin(true, false, true);

    if (M5.IMU.Init() != 0)
        IMU6886Flag = false;
    else
    {
        M5.IMU.SetGyroFsr(M5.IMU.GFS_250DPS); // 250,,500,1000,2000
        M5.IMU.SetAccelFsr(M5.IMU.AFS_2G);    // 2,4,8,16
        IMU6886Flag = true;
    }

    walkers.push_back(new LEDWalker(0, 0));
    walkers.push_back(new LEDWalker(0, 3));
    walkers.push_back(new LEDWalker(3, 0));
    walkers.push_back(new LEDWalker(3, 3));

    WiFi_setup();
}

void loop()
{
    int direction = 0;

    if (IMU6886Flag == true)
    {
        M5.IMU.getAccelData(&accX, &accY, &accZ);
        Serial.printf("%.2f,%.2f,%.2f G\r\n", accX, accY, accZ);

        // convert accel data to direction
        if (accX > 0.3)
        {
            direction = DIR_R;
        }
        else if (accX < -0.3)
        {
            direction = DIR_L;
        }

        if ((accY < -0.3) && ((-1 * accY) > abs(accX)))
        {
            direction = DIR_FRONT;
        }
        else if ((accY > 0.3) && (accY > abs(accX)))
        {
            direction = DIR_BACK;
        }
    }

    // update led position
    for (LEDWalker *w : walkers)
    {
        w->updatePos(direction);
        w->printPos();
    }

    // led off on
    M5.dis.clear();
    for (LEDWalker *w : walkers)
    {
        w->draw();
    }

    // UDP send
    wifiUdp.beginPacket(DestinationIpadr, DestinationUdpPort);
    wifiUdp.write(direction);
    wifiUdp.endPacket();

    delay(250);
    M5.update();
}

static void WiFi_setup(void)
{
    static const int SourcePort = 5000;

    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
    wifiUdp.begin(SourcePort);
}
