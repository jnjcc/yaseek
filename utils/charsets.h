// Copyright (c) 2012 - 2014 Yste.org
// All Rights Reserved.
#ifndef UTILS_CHARSETS_H_
#define UTILS_CHARSETS_H_

#include <stdint.h>
#include <string.h>

#include <string>
#include <vector>

namespace yaseek {

// Unicode code point
typedef uint32_t UCS4CodePoint;
typedef uint16_t UCS2CodePoint;
typedef std::vector<UCS4CodePoint> UCS4String;
typedef std::vector<UCS2CodePoint> UCS2String;

// UCS-4 code point range for UTF-8
const UCS4CodePoint kUCS4CodeRange[] = {
  0x00000000,  // U+0000 : U+007F, 1 byte sequence
  0x00000080,  // U+0080 : U+07FF, 2 bytes sequence
  0x00000800,  // U+0800 : U+FFFF, 3 bytes sequence
  0x00010000,  // U+10000 : U+1FFFFF, 4 bytes sequence
  0x00200000,  // U+200000 : U+3FFFFFF, 5 bytes sequence
  0x04000000   // U+4000000 : U+7FFFFFFF, 6 bytes sequence
};

// UTF-8 max byte sequence
// UTF-8 was restricted by `RFC 3629' to end at U+10FFFF
// i.e., no quintuple bytes or sextuple bytes
const uint8_t kUTF8MaxBytes = 4;

// UTF-8 leading byte
const uint8_t kUTF8LeadMask[] = {
  0x80,  // 1000 0000, single byte mask
  0xC0,  // 110x xxxx, double bytes mark
  0xE0,  // 1110 xxxx, triple bytes mark & double bytes mask
  0xF0,  // 1111 0xxx
  0xF8,  // 1111 10xx
  0xFC,  // 1111 110x
  0xFE   // 1111 1110, sextuple bytes mask
};

// UTF-8 continuation bytes: 10xx xxxx
const uint8_t kUTF8ContMark = 0x80;  // 10xx xxxx
const uint8_t kUTF8ContMask = 0xC0;  // 1100 0000

// Character conversion: UTF8 vs. UCS4
char *GetNextUTF8(const char utfs[], int len, int *bytes);
int UTF8ToU4Code(const char utfs[], int bytes, UCS4CodePoint *u4cp);
// Number of bytes needed for UTF8 to encode U4CP
int GetUTF8Bytes(UCS4CodePoint u4cp);
// Requires: UTFS with enough space
int U4CodeToUTF8(UCS4CodePoint u4cp, char utfs[]);
int U4CodeToUTF8(UCS4CodePoint u4cp, std::string *utfs);

// String conversion: UTF8Str vs. UCS4String
// Decode a UTF8 string into UCS4String
int DecodeUTF8Str(const char *utfstr, UCS4String *u4str);
int DecodeUTF8Str(const std::string &utfstr, UCS4String *u4str);
// Encode UCS4String into UTF8
int EncodeUTF8Str(const UCS4String &ucs, std::string *utfs);
int EncodeUTF8Str(UCS4String::const_iterator beg,
    UCS4String::const_iterator end, std::string *utfs);

// GBK vs. UCS2
const UCS2CodePoint kNaUni = 0xFFFD;  // Not-a-Unicode marker
typedef uint16_t GBKCodePoint;
const GBKCodePoint kNaGBK = 0xFFFD;  // No-a-GBK marker
// single-byte GBK
const uint8_t kSingleGBKMask = 0x80;
// double-byte GBK
const uint8_t kDoubleGBKLow = 0x81;
const uint8_t kDoubleGBKHigh = 0xFE;

const GBKCodePoint kGBKMapHighMask = 0xFF80;
const GBKCodePoint kGBKMapLowMask  = 0x007F;
const uint8_t kGBKMapLowBits = 7;

// Character conversion: GBK vs. UCS2
char *GetNextGBK(const char gbks[], int len, int *bytes);
int GBKToU2Code(const char gbk[], int bytes, UCS2CodePoint *u2cp);
int U2CodeToGBK(UCS2CodePoint u2c, char gbk[]);

// String conversion: GBKStr vs. UCS2String
// Decode a GBK string into UCS2String
int DecodeGBKStr(const char *gbks, UCS2String *u2str);
int EncodeGBKStr(const UCS2String &ucs, std::string *gbks);
int EncodeGBKStr(UCS2String::const_iterator beg,
    UCS2String::const_iterator end, std::string *gbks);

// We used UCS4CodePoint for UTF8 functions...
int EncodeUTF8Str(const UCS2String &ucs, std::string *utfs);
int EncodeUTF8Str(UCS2String::const_iterator beg,
    UCS2String::const_iterator end, std::string *utfs);
int EncodeGBKStr(const UCS4String &ucs, std::string *gbks);
int EncodeGBKStr(UCS4String::const_iterator beg,
    UCS4String::const_iterator end, std::string *gbks);

}  // namespace yaseek

#endif  // UTILS_CHARSETS_H_
