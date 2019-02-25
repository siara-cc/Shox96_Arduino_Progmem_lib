/*
  This example demonstrates de-compression of Html content
  Source of Html content is https://github.com/modularcode/modular-admin-html,
    which is released under MIT License.
  Decompresses data from Html.h, which was generated using Shox96 compression utility
  found at https://github.com/siara-cc/Shox96
  Command to generate Html.h: `./shox96 G html.txt html`

  Original html size: 65540 bytes
  Compressed size: 22973 bytes
  Savings: 64.95%

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
#include "Html.h"

void setup() {

  Serial.begin(115200);
  Serial.write("Hello World\n");

  // buffer for decompression
  // since G option was used to generate Html.h, the buffer needs to be 3 to 4 times max line size.
  // The call to shox96_0_2_decompress() can be kept inside a function with this buffer
  // to release it immediately.
  char out[1000];

  for (int i=0; i < html0_2_line_count; i++) {
     int len = shox96_0_2_pgm_decompress(html0_2, i, out, 0);
     out[len] = 0;
     Serial.write(out);
     Serial.write("\n");
  }

}

void loop() {

}
