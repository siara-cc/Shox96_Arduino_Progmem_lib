/*
  This example demonstrates de-compression of Error messages
  Source: include/errno.h
  Decompresses data from errno.h, which was generated using Shox96 compression utility
  found at https://github.com/siara-cc/Shox96
  Command to generate Errno.h: `./shox96 G errno.txt errno`

  Original size: 2122 bytes
  Compressed size: 1212 bytes
  Savings: 42.88%

  The savings do not justify compression as the decompressor itself takes around 3500 bytes.
  This example has been given to show the use case.

  How Shox96 works:
  https://github.com/siara-cc/Shox96/blob/master/Shox96_Article_0_2_0.pdf?raw=true

  Other projects using Shox96:
  As SQLite loadable extension - https://github.com/siara-cc/Shox96_Sqlite_UDF
  Sqlite3 Library for ESP32 - https://github.com/siara-cc/esp32_arduino_sqlite3_lib
  Sqlite3 Library for ESP8266 - https://github.com/siara-cc/esp_arduino_sqlite3_lib
  Sqlite3 Library for ESP-IDF - https://github.com/siara-cc/esp32-idf-sqlite3
*/
#include "shox96_0_2.h"
#include "errno.h"

void setup() {

  Serial.begin(115200);
  Serial.write("Hello World\n");

  char out[100];

  for (int i=0; i < errno0_2_line_count; i++) {
     int len = shox96_0_2_decompress(errno0_2, i, out, 0);
     out[len] = 0;
     Serial.write(out);
     Serial.write("\n");
  }

}

void loop() {

}
