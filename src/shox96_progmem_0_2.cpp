/*
 * Copyright (C) 2019 Siara Logics (cc)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author Arundale R.
 *
 */
#include <Arduino.h>
#include "shox96_progmem_0_2.h"

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "shox96_0_2.h"

typedef unsigned char byte;

#define NICE_LEN_FOR_PRIOR 7
#define NICE_LEN_FOR_OTHER 12

// Decoder is designed for using less memory, not speed
// Decode lookup table for code index and length
// First 2 bits 00, Next 3 bits indicate index of code from 0,
// last 3 bits indicate code length in bits
//                        0,            1,            2,            3,            4,
char vcode_0_2_pgm[32] = {2 + (0 << 3), 3 + (3 << 3), 3 + (1 << 3), 4 + (6 << 3), 0,
//                5,            6,            7,            8, 9, 10
                  4 + (4 << 3), 3 + (2 << 3), 4 + (8 << 3), 0, 0,  0,
//                11,          12, 13,            14, 15
                  4 + (7 << 3), 0,  4 + (5 << 3),  0,  5 + (9 << 3),
//                16, 17, 18, 19, 20, 21, 22, 23
                   0,  0,  0,  0,  0,  0,  0,  0,
//                24, 25, 26, 27, 28, 29, 30, 31
                   0, 0,  0,  0,  0,  0,  0,  5 + (10 << 3)};
//                        0,            1,            2, 3,            4, 5, 6, 7,
char hcode_0_2_pgm[32] = {1 + (1 << 3), 2 + (0 << 3), 0, 3 + (2 << 3), 0, 0, 0, 5 + (3 << 3),
//                8, 9, 10, 11, 12, 13, 14, 15,
                  0, 0,  0,  0,  0,  0,  0,  5 + (5 << 3),
//                16, 17, 18, 19, 20, 21, 22, 23
                   0, 0,  0,  0,  0,  0,  0,  5 + (4 << 3),
//                24, 25, 26, 27, 28, 29, 30, 31
                   0, 0,  0,  0,  0,  0,  0,  5 + (6 << 3)};

enum {SHX_SET1 = 0, SHX_SET1A, SHX_SET1B, SHX_SET2, SHX_SET3, SHX_SET4, SHX_SET4A};
char sets_0_2_pgm[][11] = {{' ', ' ', 'e', 't', 'a', 'o', 'i', 'n', 's', 'r', 'l'},
                 {'c', 'd', 'h', 'u', 'p', 'm', 'b', 'g', 'w', 'f', 'y'},
                 {'v', 'k', 'q', 'j', 'x', 'z', ' ', ' ', ' ', ' ', ' '},
                 {' ', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8'},
                 {'.', ',', '-', '/', '=', '+', ' ', '(', ')', '$', '%'},
                 {'&', ';', ':', '<', '>', '*', '"', '{', '}', '[', ']'},
                 {'@', '?', '\'', '^', '#', '_', '!', '\\', '|', '~', '`'}};

int getBitVal(const byte *in, int bit_no, int count) {
   return (pgm_read_byte(in + (bit_no >> 3)) & (0x80 >> (bit_no % 8)) ? 1 << count : 0);
}

int getCodeIdx(char *code_type, const byte *in, int len, int *bit_no_p) {
  int code = 0;
  int count = 0;
  do {
    if (*bit_no_p >= len)
      return 199;
    code += getBitVal(in, *bit_no_p, count);
    (*bit_no_p)++;
    count++;
    if (code_type[code] &&
        (code_type[code] & 0x07) == count) {
      return code_type[code] >> 3;
    }
  } while (count < 5);
  return 1; // skip if code not found
}

int getNumFromBits(const byte *in, int bit_no, int count) {
   int ret = 0;
   while (count--) {
     ret += getBitVal(in, bit_no++, count);
   }
   return ret;
}

int readCount(const byte *in, int *bit_no_p, int len) {
  const byte bit_len[7]   = {5, 2,  7,   9,  12,   16, 17};
  const uint16_t adder[7] = {4, 0, 36, 164, 676, 4772,  0};
  int idx = getCodeIdx(hcode_0_2_pgm, in, len, bit_no_p);
  if (idx > 6)
    return 0;
  int count = getNumFromBits(in, *bit_no_p, bit_len[idx]) + adder[idx];
  (*bit_no_p) += bit_len[idx];
  return count;
}

int shox96_0_2_pgm_decompress(const byte * const in_list[], int idx, char *out, int end_len) {

  int dstate;
  int bit_no;
  byte is_all_upper;
  int ol = 0;
  bit_no = 0;
  dstate = SHX_SET1;
  is_all_upper = 0;

  byte *in = pgm_read_word(&(in_list[idx]));
  int len = pgm_read_byte(&(in[0]));
  in++;

  len <<= 3;
  out[ol] = 0;
  while (bit_no < len) {
    if (end_len && ol >= end_len)
      return ol;
    int h, v;
    char c;
    byte is_upper = is_all_upper;
    int orig_bit_no = bit_no;
    v = getCodeIdx(vcode_0_2_pgm, in, len, &bit_no);
    if (v == 199) {
      bit_no = orig_bit_no;
      break;
    }
    h = dstate;
    if (v == 0) {
      h = getCodeIdx(hcode_0_2_pgm, in, len, &bit_no);
      if (h == 199) {
        bit_no = orig_bit_no;
        break;
      }
      if (h == SHX_SET1) {
         if (dstate == SHX_SET1) {
           if (is_all_upper) {
             is_upper = is_all_upper = 0;
             continue;
           }
           v = getCodeIdx(vcode_0_2_pgm, in, len, &bit_no);
           if (v == 199) {
             bit_no = orig_bit_no;
             break;
           }
           if (v == 0) {
              h = getCodeIdx(hcode_0_2_pgm, in, len, &bit_no);
              if (h == 199) {
                bit_no = orig_bit_no;
                break;
              }
              if (h == SHX_SET1) {
                 is_all_upper = 1;
                 continue;
              }
           }
           is_upper = 1;
         } else {
            dstate = SHX_SET1;
            continue;
         }
      } else
      if (h == SHX_SET2) {
         if (dstate == SHX_SET1)
           dstate = SHX_SET2;
         continue;
      }
      if (h != SHX_SET1) {
        v = getCodeIdx(vcode_0_2_pgm, in, len, &bit_no);
        if (v == 199) {
          bit_no = orig_bit_no;
          break;
        }
      }
    }
    if (h < 64 && v < 32)
      c = sets_0_2_pgm[h][v];
    if (c >= 'a' && c <= 'z') {
      if (is_upper)
        c -= 32;
    } else {
      if (is_upper && dstate == SHX_SET1 && v == 1)
        c = '\t';
      if (h == SHX_SET1B) {
         switch (v) {
           case 6:
             out[ol++] = '\r';
             c = '\n';
             break;
           case 7:
             c = is_upper ? '\r' : '\n';
             break;
           case 8:
             if (getBitVal(in, bit_no++, 0)) {
               int dict_len = readCount(in, &bit_no, len) + NICE_LEN_FOR_PRIOR;
               int dist = readCount(in, &bit_no, len) + NICE_LEN_FOR_PRIOR - 1;
               memcpy(out + ol, out + ol - dist, dict_len);
               ol += dict_len;
             } else {
               int dict_len = readCount(in, &bit_no, len) + NICE_LEN_FOR_OTHER;
               int dist = readCount(in, &bit_no, len);
               int ctx = readCount(in, &bit_no, len);
               shox96_0_2_pgm_decompress(in_list, idx - 1 - ctx, out + ol + dict_len, dist + dict_len + 1);
               memcpy(out + ol, out + ol + dict_len + dist, dict_len);
               ol += dict_len;
             }
             continue;
           case 9: {
             int count = readCount(in, &bit_no, len);
             count += 4;
             char rpt_c = out[ol - 1];
             while (count--)
               out[ol++] = rpt_c;
             continue;
           }
           case 10:
             continue;
         }
      }
    }
    out[ol++] = c;
  }

  return ol;

}
