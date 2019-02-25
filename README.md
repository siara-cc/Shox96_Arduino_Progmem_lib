# Compression and decompression Library for use with Arduino Flash Memory (PROGMEM)

[Shox96](https://github.com/siara-cc/Shox96) is a technique used for compressing and decompressing short strings. This library allows storing of compressed text strings in Arduino Program Memory and provides API to retrieve each string individually using index.

Shox96 can achieve upto 60% compression depending on the composition of text.  This can be especially useful in constrained environments like Arduino Uno where only 32kb of Flash memory is available.

To find out how Shox96 works, read [this article](Shox96_Article_0_2_0.pdf?raw=true).

## Applications

- Displaying descriptive error or warning messages
- Storing text file content (such as html)
- Transferring compressed content over low speed networks

## Usage

First, the strings that need to compressed should to be listed in a file (usually with .txt extension).

Download [Shox96](https://github.com/siara-cc/Shox96) and compile it using instructions provided in the repository. Then create compressed PROGMEM header using following command:

```
./shox96 g <text_file> <output_header_name>
```

or

```
./shox96 G <text_file> <output_header_name>
```

The command with capital G provides more compression taking into account repeating texts across the file.

For example, if we use the file `proverbs.txt` given in the repository and the command `./shox96 g proverbs.txt proverbs`, a file by name `proverbs.h` will be generated.

This file can be copied alongside any `.ino` file and included.  Then the `shox96_0_2_pgm_decompress()` API from `shox96_progmem_0_2.h` in this library can be called to uncompress the strings.

Please try the examples provided along with this library to see how it works.

## Compatibility

This library can be used with Arduino Duemilanove, Uno upwards.  It is useful only if saving by compressing text content is over 3000 bytes since the decompressor takes as much space.

## Limitations

- Only text data (ASCII 32 to 126, 13, 10, 9) can be compressed using Shox96
- The capital G parameter requires more memory allocated for decompression. Please see `Html` example
- It is useful only if saving by compressing text content is over 3000 bytes since the decompressor takes as much space.

## Examples and screenshots

### Html

This example demonstrates de-compression of Html content. Source of Html content is https://github.com/modularcode/modular-admin-html, which is released under MIT License.

![](ss_html.png?raw=true)

### Error Messages

This example demonstrates de-compression of Error messages. Source: `include/errno.h` of GCC.

![](ss_err_msgs.png?raw=true)

### Proverbs

This example demonstrates de-compression of compressed proverbs. Source: https://en.wikipedia.org/wiki/List_of_proverbial_phrases.

![](ss_proverbs.png?raw=true)

## Issues

If you face any problems, please create issue on GitHub.
