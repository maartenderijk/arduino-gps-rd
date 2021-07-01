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
    printRD(gps.location.lat(), gps.location.lng(), gps.location.isValid());
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
    float x;
    float y;
};

static void printRD(float lat, float lng, bool valid)
{
    float dF = 0.36 * (lat - 52.15517440);
    float dL = dL = 0.36 * (lng - 5.38720621);

    float SomX = (190094.945 * dL) + (-11832.228 * dF * dL) + (-144.221 * pow(dF, 2) * dL) + (-32.391 * pow(dL, 3)) + (-0.705 * dF) + (-2.340 * pow(dF, 3) * dL) + (-0.608 * dF * pow(dL, 3)) + (-0.008 * pow(dL, 2)) + (0.148 * pow(dF, 2) * pow(dL, 3));
    float SomY = (309056.544 * dF) + (3638.893 * pow(dL, 2)) + (73.077 * pow(dF, 2)) + (-157.984 * dF * pow(dL, 2)) + (59.788 * pow(dF, 3)) + (0.433 * dL) + (-6.439 * pow(dF, 2) * pow(dL, 2)) + (-0.032 * dF * dL) + (0.092 * pow(dL, 4)) + (-0.054 * dF * pow(dL, 4));

    float X = 155000 + SomX;
    float Y = 463000 + SomY;

    rdcoordinates result = {X, Y};
    Serial.print(result.x);
    Serial.print(' ');
    Serial.print(result.y);
}
