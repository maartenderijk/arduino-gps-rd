#include <TinyGPS++.h>
#include <SoftwareSerial.h>

/*
   Based on sample code from TinyGPS++ (TinyGPSPlus).
*/
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 57600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
    Serial.begin(115200);
    ss.begin(GPSBaud);

    Serial.println(F("Sats Latitude   Longitude   Alt   "));
    Serial.println(F("     (deg)      (deg)       (m)   "));
    Serial.println(F("----------------------------------"));
}

void loop()
{
    printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
    printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
    printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
    printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
    printRD(gps.location.lat(), gps.location.lng(),gps.location.isValid());
    Serial.println();

    smartDelay(1000);

    if (millis() > 5000 && gps.charsProcessed() < 10)
        Serial.println(F("No GPS data received: check wiring"));
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do
    {
        while (ss.available())
            gps.encode(ss.read());
    } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
    if (!valid)
    {
        while (len-- > 1)
            Serial.print('*');
        Serial.print(' ');
    }
    else
    {
        Serial.print(val, prec);
        int vi = abs((int)val);
        int flen = prec + (val < 0.0 ? 2 : 1); // . and -
        flen += vi >= 1000 ? 4 : vi >= 100 ? 3
                             : vi >= 10    ? 2
                                           : 1;
        for (int i = flen; i < len; ++i)
            Serial.print(' ');
    }
    smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
    char sz[32] = "*****************";
    if (valid)
        sprintf(sz, "%ld", val);
    sz[len] = 0;
    for (int i = strlen(sz); i < len; ++i)
        sz[i] = ' ';
    if (len > 0)
        sz[len - 1] = ' ';
    Serial.print(sz);
    smartDelay(0);
}


static void printStr(const char *str, int len)
{
    int slen = strlen(str);
    for (int i = 0; i < len; ++i)
        Serial.print(i < slen ? str[i] : ' ');
    smartDelay(0);
}

struct rdcoordinates
{
    double x;
    double y;
};

static void printRD(float lat, float lng, bool valid) {
    // Dummy function, should return RD X,Y Coodinates
    rdcoordinates result = {1.0, 2.0};
    Serial.print(result.x);
    Serial.print(' ');
    Serial.print(result.y);
}