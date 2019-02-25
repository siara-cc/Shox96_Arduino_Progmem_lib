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
#ifndef __SHOX96_0_2_COMPRESS__
#define __SHOX96_0_2_COMPRESS__

typedef unsigned char byte;

struct lnk_lst {
  char *data;
  struct lnk_lst *previous;
};

int shox96_0_2_compress(const char *in, int len, char *out, struct lnk_lst *prev_lines);
int shox96_0_2_decompress(const byte * const in_list[], int idx, char *out, int end_len);
int shox96_0_2_decompress(const char *in, int len, char *out);

#endif
