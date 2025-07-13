#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uthash.h"

#define MAX_VALUES 10

typedef struct {
    Line key;          // string key
    int values[MAX_VALUES]; // fixed-size array of ints
    int values_len;        // number of valid ints in values
    UT_hash_handle hh;     // makes this structure hashable
} DictEntry;

DictEntry *dict = NULL;

void clear_dict() {
  dict = NULL;
}

void add_entry(Line key, int val) {
    DictEntry *entry = NULL;

    HASH_FIND(hh, dict, &key, sizeof(Line), entry);

    if (entry == NULL) {
        entry = (DictEntry*)malloc(sizeof(DictEntry));
        if (!entry) {
            Serial.println("malloc failed");
            return;
        }
        memcpy(&entry->key, &key, sizeof(Line));
        entry->values_len = 0;
        HASH_ADD(hh, dict, key, sizeof(Line), entry);
    }

    if (entry->values_len < MAX_VALUES) {
        entry->values[entry->values_len++] = val;
    } else {
        Serial.print("Values array full for key: ");
        Serial.println(entry->key.name);
    }
}

// Optional: helper to print values for a key
void print_values(Line key) {
    DictEntry *entry = NULL;
    HASH_FIND(hh, dict, &key, sizeof(Line), entry);
    if (entry) {
        Serial.print(entry->key.name);
        Serial.print(": ");
        for (int i = 0; i < entry->values_len; i++) {
          if (entry->values[i] == 0) {
            Serial.print("due ");
          }
          else {
            Serial.print(entry->values[i]);
            Serial.print("m ");
          }
        }
        Serial.println();
    } else {
        Serial.print(key.name);
        Serial.println(": No times");
    }
}

String get_values_string(Line key) {
  DictEntry *entry = NULL;
  HASH_FIND(hh, dict, &key, sizeof(Line), entry);

  String result;

  if (entry) {
    result += entry->key.name;
    result += ": ";
    for (int i = 0; i < entry->values_len; i++) {
    //   if (entry->values[i] == 0) {
    //     result += "due ";
    //   } else {
        result += String(entry->values[i]);
        result += "m ";
    //   }
    }
  } else {
    result += key.name;
    result += ": None";
  }

  return result;
}
