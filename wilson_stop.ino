#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "constants.h"

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#define PANEL_RES_X 64
#define PANEL_RES_Y 64
#define PANEL_CHAIN 1
 
//MatrixPanel_I2S_DMA dma_display;
MatrixPanel_I2S_DMA *dma_display = nullptr;

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 150, 150);//(255, 0, 0);
uint16_t myBLUE = dma_display->color565(100, 180, 230);//(0, 0, 255);
uint16_t myPURPLE = dma_display -> color565(160, 120, 210);//(128, 0, 128);


void drawSimpleText(char *text, int x, int y, uint16_t color) {
  dma_display->setTextSize(1);
  dma_display->setTextColor(color);
  dma_display->setCursor(x, y);    // start at top left, with 8 pixel of spacing
  for (int i = 0; text[i] != '\0'; i++) {
    if (text[i] == ' ') {
      dma_display->setCursor(dma_display->getCursorX() + 2, dma_display->getCursorY()); // smaller space
    } 
    else if (text[i] == ':') {
      dma_display->setTextColor(myWHITE);
      dma_display->setCursor(dma_display->getCursorX() - 2, dma_display->getCursorY());
      dma_display->print(text[i]);
      dma_display->setCursor(dma_display->getCursorX() - 2, dma_display->getCursorY());
    }
    else {
      dma_display->print(text[i]);
    }
  }
}

int getTextLength(char* text) {
  int len;
  for (int i = 0; text[i] != '\0'; i++)  {
    if (text[i] == ' ') {
      len += 2;
    }
    else if (text[i] == ':') {
      len += 4;
    }
    else {
      len += 6;
    }
  }
  return len;
}

void truncateToFit(char* text, int maxPixels) {
  int length = strlen(text);
  char buffer[256]; // temp buffer, adjust size accordingly
  buffer[0] = '\0';

  int totalWidth = 0;
  int pos = 0;

  while (pos < length) {
    // Find next space or end of string
    int start = pos;
    while (pos < length && text[pos] != ' ') pos++;
    int wordLen = pos - start;

    // Extract the current word
    char word[64]; // assuming words aren't longer than 64 chars
    strncpy(word, &text[start], wordLen);
    word[wordLen] = '\0';

    // Calculate pixel width if we add this word (plus a space if buffer not empty)
    int spaceWidth = (buffer[0] != '\0') ? getTextLength(" ") : 0;
    int wordWidth = getTextLength(word);

    if (totalWidth + spaceWidth + wordWidth <= maxPixels) {
      // Append space if needed
      if (buffer[0] != '\0') strcat(buffer, " ");
      strcat(buffer, word);
      totalWidth += spaceWidth + wordWidth;
    } else {
      // Can't fit this word, stop here
      break;
    }

    // Skip the space after the word
    while (pos < length && text[pos] == ' ') pos++;
  }

  // Copy truncated string back to original buffer
  strcpy(text, buffer);
}

void printTimestamp(time_t t) {
  struct tm *timeinfo = localtime(&t);  // Convert to local time
  char buffer[25];
  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", timeinfo);
  Serial.println(buffer);
}

void printWiFiStatus(int status) {
  switch (status) {
    case WL_NO_SHIELD:      Serial.println("🔌 No WiFi shield."); break;
    case WL_IDLE_STATUS:    Serial.println("💤 Idle status."); break;
    case WL_NO_SSID_AVAIL:  Serial.println("📡 No SSID available."); break;
    case WL_SCAN_COMPLETED: Serial.println("🔍 Scan complete."); break;
    case WL_CONNECTED:      Serial.println("✅ Connected."); break;
    case WL_CONNECT_FAILED: Serial.println("❌ Connection failed."); break;
    case WL_CONNECTION_LOST:Serial.println("🔌 Connection lost."); break;
    case WL_DISCONNECTED:   Serial.println("🔌 Disconnected."); break;
    default:                Serial.print("❓ Unknown status: ");
                            Serial.println(status); break;
  }
}

void connectToWifi() {
  Serial.println("🔍 Scanning for networks...");
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.println(" dBm)");
    Serial.print(" dBm)  Channel: ");
    Serial.println(WiFi.channel(i));
  }

  WiFi.begin(ssid, password);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    delay(1000);
    wl_status_t status = WiFi.status();
    Serial.print("Status: ");
    Serial.println((int)status);
    printWiFiStatus(status);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ Failed to connect to WiFi.");
  }
}

void synchronizeTime() {
  // Set time via NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  while (time(nullptr) < 100000) {
    delay(500);
    Serial.print("⏳ Syncing time...\n");
  }
  Serial.println("⏰ Time synchronized!");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  connectToWifi();

  synchronizeTime();

  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);  // Normal operation

  for (int i = 0; i < NUM_BUS_TARGETS; i++) {
    int stpid = get_stop(bus_lines[i]);
    bus_lines[i].stpid = stpid;  // Modify actual struct in the array
  }

  // Module configuration
  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   // module width
    PANEL_RES_Y,   // module height
    PANEL_CHAIN    // Chain length
  );

  mxconfig.gpio.a = 23;
  mxconfig.gpio.b = 22;
  mxconfig.gpio.c = 5;
  mxconfig.gpio.d = 17;
  mxconfig.gpio.e = 32;
  mxconfig.clkphase = false;
  //mxconfig.driver = HUB75_I2S_CFG::FM6126A;

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(30); //0-255
  dma_display->setTextWrap(false);
  dma_display->clearScreen();

  // fill the screen with 'black'
  dma_display->fillScreen(myBLACK);
  drawSimpleText("Set up done!", 2, 5, myWHITE);
  
}

void loop() {
  time_t now = time(nullptr); // Current time from NTP

  clear_dict();
  get_arrival_times(now); // get train times
  for (Line line : bus_lines) { // get bus times
    get_prediction(line, now);
  }

  Serial.print("Querying at ");
  printTimestamp(now);

  int x_pos = 3;
  int y_pos = 4;

  dma_display->clearScreen();

  for (int i = 0; i < NUM_TRAIN_TARGETS + NUM_BUS_TARGETS; i++) {

    Line currentLine;
    if (i < NUM_TRAIN_TARGETS) {
      currentLine = lines[i];
    } else {
      currentLine = bus_lines[i - NUM_TRAIN_TARGETS];
    }

    print_values(currentLine);
    String value = get_values_string(currentLine);
    char charArray[value.length() + 1];  // +1 for the null terminator
    value.toCharArray(charArray, sizeof(charArray));

    truncateToFit(charArray, 64); 


    uint16_t color;

    if (strcmp(currentLine.name, "95th") == 0 || strcmp(currentLine.name, "Hwd") == 0) {
      color = myRED;
    }
    else if  (strcmp(currentLine.name, "Loop") == 0) {
      color = myPURPLE;
    }
    else if (strcmp(currentLine.name, "22N") == 0 || strcmp(currentLine.name, "22S") == 0) {
      color = myBLUE;
    }
    else {
      color = myWHITE;
    }

    drawSimpleText(charArray, x_pos, y_pos, color);
    y_pos += 12;
  }

  Serial.println();
  delay(10000); // Wait 10 second
}
