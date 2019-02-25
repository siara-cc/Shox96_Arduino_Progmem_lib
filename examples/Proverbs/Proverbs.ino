/*
  This example demonstrates de-compression of compressed proverbs
  Source: https://en.wikipedia.org/wiki/List_of_proverbial_phrases.
  Decompresses data from Proverbs.h, which was generated using Shox96 compression utility
  found at https://github.com/siara-cc/Shox96
  Command to generate Html.h: `./shox96 g proverbs.txt Proverbs`

  Original size : 25244 bytes
  Compressed size: 14556 bytes
  Savings: 42.34%

  How Shox96 works:
  https://github.com/siara-cc/Shox96/blob/master/Shox96_Article_0_2_0.pdf?raw=true

  Other projects using Shox96:
  Compression library for Arduino - https://github.com/siara-cc/Shox96_Arduino_lib
  As SQLite loadable extension - https://github.com/siara-cc/Shox96_Sqlite_UDF
  Sqlite3 Library for ESP32 - https://github.com/siara-cc/esp32_arduino_sqlite3_lib
  Sqlite3 Library for ESP8266 - https://github.com/siara-cc/esp_arduino_sqlite3_lib
  Sqlite3 Library for ESP-IDF - https://github.com/siara-cc/esp32-idf-sqlite3
*/
#include "shox96_progmem_0_2.h"
#include "Proverbs.h"

void setup() {

  Serial.begin(115200);
  randomSeed(analogRead(0));

}

void loop() {

  // buffer for decompression
  // since g option was used to generate Proverbs.h, the buffer can be
  // just Proverbs0_2_max_len size.
  // The call to shox96_0_2_decompress() can be kept inside a function with this buffer
  // to release it immediately.
  char out[Proverbs0_2_max_len];

  Serial.write("Welcome\n");
  Serial.write("-------\n");

  Serial.write("1. Print all Proverbs\n");
  Serial.write("2. Print random proverb\n\n");
  Serial.write("Enter choice:\n");
  int ch = 0;
  while (true) {
      if (Serial.available()) {
          ch = Serial.read();
          if (ch == '1' || ch == '2')
            break;
      }
  }

  if (ch == '1') {
    for (int i=0; i < Proverbs0_2_line_count; i++) {
        int len = shox96_0_2_pgm_decompress(Proverbs0_2, i, out, 0);
        out[len] = 0;
        Serial.write(out);
        Serial.write("\n");
    }
  } else if (ch == '2') {
    int len = shox96_0_2_pgm_decompress(Proverbs0_2, random(0, Proverbs0_2_line_count - 1), out, 0);
    out[len] = 0;
    Serial.write(out);
    Serial.write("\n\n");
  }

}
