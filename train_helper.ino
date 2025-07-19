int get_arrival_times(time_t now) {
  char url[200];
  sprintf(url, "%s?key=%s&mapid=%d&outputType=JSON", TRAIN_URL, TRAIN_KEY, MAP_ID);
  // Serial.println(url);
  DynamicJsonDocument doc = send_request(url);
  if (doc != NULL) {
      // Access the "ctatt" -> "eta" array
      JsonArray eta = doc["ctatt"]["eta"].as<JsonArray>();

      for (JsonObject train : eta) {
        const char* rt = train["rt"];         // Route (line)
        const char* destNm = train["destNm"]; // Destination name
        const char* arrT = train["arrT"];     // Arrival time

        // Filter for Red line trains to Howard
        for (Line line : lines) {
          if (strcmp(rt, line.route) == 0 && strcmp(destNm, line.destination) == 0) {
            // Parse arrival time string to struct tm
            struct tm tm_arrival = {};
            strptime(arrT, "%Y-%m-%dT%H:%M:%S", &tm_arrival);
            time_t arrival_time = mktime(&tm_arrival);

            double seconds_to_arrival = difftime(arrival_time, now);
            int minutes_to_arrival = max(0, (int)(seconds_to_arrival / 60.0));  // Floor to minutes, no rounding
            add_entry(line, minutes_to_arrival);
          }
        }
      }
      return 1;
  } else {
    return -1;
  }
}