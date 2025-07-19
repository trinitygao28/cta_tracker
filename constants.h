#define MAX_LEN 32
#define NUM_TRAIN_TARGETS 2
#define NUM_BUS_TARGETS 3

#ifndef CONSTANTS_H
#define CONSTANTS_H
typedef struct {
    char route[MAX_LEN];
    char destination[MAX_LEN];
    char name[MAX_LEN];
    int stpid;
    char stop_name[MAX_LEN];
} Line;



extern Line lines[NUM_TRAIN_TARGETS];
extern Line bus_lines[NUM_BUS_TARGETS];


extern const char* TRAIN_KEY;
extern const char* TRAIN_URL;
extern const int MAP_ID;
extern const char* apiURL;

extern const char* BUS_KEY;
extern const char* BUS_URL;
extern const char* BUS_STOP_NAME;

extern const char* ssid;
extern const char* password;

extern const unsigned long startAttemptTime;
extern const unsigned long wifiTimeout;  // 10 seconds

extern const char* ntpServer;
extern const long  gmtOffset_sec;    // Example: Chicago is UTC-5 (Central Daylight Time)
extern const int   daylightOffset_sec;    // 1 hour daylight saving

#endif