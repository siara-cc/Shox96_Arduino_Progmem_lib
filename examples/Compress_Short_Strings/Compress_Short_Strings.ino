/*
  This example demonstrates compression and de-compression of Short Strings
  using Shox96 compression utility found at https://github.com/siara-cc/Shox96

  How Shox96 works:
  https://github.com/siara-cc/Shox96/blob/master/Shox96_Article_0_2_0.pdf?raw=true

  Other projects using Shox96:
  As SQLite loadable extension - https://github.com/siara-cc/Shox96_Sqlite_UDF
  Sqlite3 Library for ESP32 - https://github.com/siara-cc/esp32_arduino_sqlite3_lib
  Sqlite3 Library for ESP8266 - https://github.com/siara-cc/esp_arduino_sqlite3_lib
  Sqlite3 Library for ESP-IDF - https://github.com/siara-cc/esp32-idf-sqlite3
*/
#include "shox96_0_2.h"

void setup() {

  Serial.begin(115200);

}

void print_compressed(char *in, int len) {
  int l;
  byte bit;
  for (l=0; l<len; l++) {
    Serial.write((int) in[l]);
  }
  Serial.write("\n");
  for (l=0; l<len*8; l++) {
    bit = (in[l/8]>>(7-l%8))&0x01;
    Serial.print((int)bit);
    if (l%8 == 7) printf(" ");
  }
}

int input_string(char *str, int max_len) {
  max_len--;
  int ctr = 0;
  str[ctr] = 0;
  while (str[ctr] != '\n') {
    if (Serial.available()) {
        str[ctr] = Serial.read();
        if (str[ctr] >= ' ' && str[ctr] <= '~')
          ctr++;
        if (ctr >= max_len)
          break;
    }
  }
  str[ctr] = 0;
  Serial.println(str);
  return ctr;
}


void loop() {

  char str[256];
  char cbuf[300];
  char dbuf[300];
  Serial.write("Welcome\n");
  Serial.write("-------\n");

  Serial.write("Enter string to compress:\n");
  int len = input_string(str);

  if (len > 0) {
    memset(cbuf, 0, sizeof(cbuf));
    int ctot = shox96_0_2_compress(str, len, cbuf, NULL);
    print_compressed(cbuf, ctot);
    memset(dbuf, 0, sizeof(dbuf));
    int dlen = shox96_0_2_decompress(cbuf, ctot, dbuf, NULL);
    dbuf[dlen] = 0;
    Serial.write("Decompressed:");
    Serial.write(dbuf);
    float perc = (len-ctot);
    perc /= len;
    perc *= 100;
    Serial.write("\nRatio: ");
    Serial.write(ctot);
    Serial.write("/");
    Serial.write(len);
    Serial.write("=");
    Serial.write(perc);
  }

}
