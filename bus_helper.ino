
#include "constants.h"
#include <stdlib.h>

void insert_date_dashes(const char* input, char* output, size_t out_size) {
    // Expect input format: YYYYMMDD HH:MM (length 13 including space)
    // Output format: YYYY-MM-DD HH:MM

    if (strlen(input) < 13 || out_size < 16) { // Ensure output buffer is big enough
        output[0] = '\0';
        return;
    }

    // Copy year
    strncpy(output, input, 4);
    output[4] = '-';

    // Copy month
    strncpy(output + 5, input + 4, 2);
    output[7] = '-';

    // Copy day
    strncpy(output + 8, input + 6, 2);

    // Copy rest " HH:MM"
    strcpy(output + 10, input + 8);
}


int get_stop(Line line) {
  int route = atoi(line.route);
  char* direction = line.destination;
  char url[200];
  sprintf(url, "%s/getstops?key=%s&rt=%d&dir=%s&format=json", BUS_URL, BUS_KEY, route, direction);
  // Serial.println(url);
  
  DynamicJsonDocument doc = send_request(url);
  if (doc != NULL) {
      // Access the "ctatt" -> "eta" array
      JsonArray stops = doc["bustime-response"]["stops"].as<JsonArray>();

      for (JsonObject stop : stops) {
        const char* stpnm = stop["stpnm"];
        if (strcmp(stpnm, line.stop_name) == 0) {
          return atoi(stop["stpid"]);
        }
      }
      return -1;
  }
  else {
    return -1;
  }
}

int get_prediction(Line line, time_t now) {
  int route = atoi(line.route);
  int stpid = line.stpid;
  char url[200];
  sprintf(url, "%s/getpredictions?key=%s&rt=%d&stpid=%d&format=json", BUS_URL, BUS_KEY, route, stpid);
  // Serial.println(url);

  DynamicJsonDocument doc = send_request(url);
  if (doc != NULL) {
      // Access the "ctatt" -> "eta" array
      JsonArray predictions = doc["bustime-response"]["prd"].as<JsonArray>();

      for (JsonObject pred : predictions) {
        const char* prdtime = pred["prdtm"];
        struct tm tm_arrival = {};
        char formatted[20];  // enough space for "YYYY-MM-DD HH:MM\0"
        insert_date_dashes(prdtime, formatted, sizeof(formatted));
        strptime(formatted, "%Y-%m-%d %H:%M", &tm_arrival);
        time_t arrival_time = mktime(&tm_arrival);

        double seconds_to_arrival = difftime(arrival_time, now);
        int minutes_to_arrival = max(0, (int)(seconds_to_arrival / 60.0));  // Floor to minutes, no rounding
        add_entry(line, minutes_to_arrival);

      }
      return 1;
  } else {
    return -1;
  }
}