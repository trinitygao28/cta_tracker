#include "constants.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

const char *TRAIN_KEY = "4af37ca4b8e648b29f683631a3a11224";
const char *TRAIN_URL = "https://lapi.transitchicago.com/api/1.0/ttarrivals.aspx";
const int MAP_ID = 40540; // this is wilson

const char *BUS_KEY = "Q8tGa2iAiykjYAuWAsiuFwVH4";
const char *BUS_URL = "https://www.ctabustracker.com/bustime/api/v3";
const char *BUS_STOP_NAME = "Clark & Leland";

const char* ssid = "Big Red’s Home";
const char* password = "helloworld";

const unsigned long startAttemptTime = millis();
const unsigned long wifiTimeout = 10000;  // 10 seconds

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -5 * 3600;
const int   daylightOffset_sec = 3600;

Line lines[NUM_TRAIN_TARGETS] = {
    {"Red", "95th/Dan Ryan", "95th", MAP_ID, "None"},
    {"P", "Loop", "Loop", MAP_ID}
};

Line bus_lines[NUM_BUS_TARGETS] = {
  {"22", "Northbound", "22N", -1, "Clark & Leland"},
  {"22", "Southbound", "22S", -1, "Clark & Leland"},
  {"81", "Westbound", "81W", -1, "Wilson & Magnolia"}
};