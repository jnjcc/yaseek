// Copyright (c) 2012 - 2014 Yste.org
// All Rights Reserved.

#include "charsets.h"
#include "gbk2uni_in.h"
#include "uni2gbk_in.h"

namespace yaseek {

char *GetNextUTF8(const char utfs[], int len, int *bytes) {
  if (utfs == NULL || len <= 0) {
    *bytes = 0;
    return NULL;
  }

  uint8_t lead = uint8_t(utfs[0]);
  if ((lead & kUTF8LeadMask[0]) == 0x00) {
    *bytes = 1;
    return const_cast<char *>(utfs);
  }

  *bytes = 0;
  for (uint8_t i = 2; i <= kUTF8MaxBytes; ++i) {
    if ((lead & kUTF8LeadMask[i]) == kUTF8LeadMask[i - 1]) {
      if (i <= len) {
        *bytes = i;
        return const_cast<char *>(utfs);
      } else {
        *bytes = i;
        return NULL;
      }
    }
  }
  return NULL;
}
int UTF8ToU4Code(const char utfs[], int bytes, UCS4CodePoint *u4cp) {
  if (utfs == NULL || bytes <= 0) {
    return -1;
  }
  uint8_t high, mid, mow, low;
  switch (bytes) {
    case 1:
      low = uint8_t(utfs[0]) & ~kUTF8LeadMask[0];
      *u4cp = UCS4CodePoint(low);
      break;
    case 2:
      high = uint8_t(utfs[0]) & ~kUTF8LeadMask[2];  // 5 bits
      low = uint8_t(utfs[1]) & ~kUTF8ContMask;  // 6 bits
      *u4cp = (UCS4CodePoint(high) << 6) | UCS4CodePoint(low);
      break;
    case 3:
      high = uint8_t(utfs[0]) & ~kUTF8LeadMask[3];  // 4 bits
      mid = uint8_t(utfs[1]) & ~kUTF8ContMask;  // 6 bits
      low = uint8_t(utfs[2]) & ~kUTF8ContMask;  // 6 bits
      *u4cp = (UCS4CodePoint(high) << 12) | (UCS4CodePoint(mid) << 6) |
              (UCS4CodePoint(low));
      break;
    case 4:
      high = uint8_t(utfs[0]) & ~kUTF8LeadMask[4];  // 3 bits
      mid = uint8_t(utfs[1]) & ~kUTF8ContMask;  // 6 bits
      mow = uint8_t(utfs[2]) & ~kUTF8ContMask;  // 6 bits
      low = uint8_t(utfs[3]) & ~kUTF8ContMask;  // 6 bits
      *u4cp = (UCS4CodePoint(high) << 18) | (UCS4CodePoint(mid) << 12) |
              (UCS4CodePoint(mow) << 6) | UCS4CodePoint(low);
      break;
    default:
      return -1;
  }
  return 1;
}
int GetUTF8Bytes(UCS4CodePoint u4cp) {
  int bytes = -1;
  // RFC 3629
  for (int i = 1; i <= kUTF8MaxBytes; ++i) {
    if (u4cp < kUCS4CodeRange[i]) {
      bytes = i;
      break;
    }
  }
  return bytes;
}
int U4CodeToUTF8(UCS4CodePoint u4cp, char utfs[]) {
  int bytes = GetUTF8Bytes(u4cp);
  uint8_t uchr;
  switch (bytes) {
    case 1:
      *utfs = char(u4cp);
      break;
    case 2:
      uchr = (uint8_t(u4cp >> 6) & ~kUTF8LeadMask[2]) | kUTF8LeadMask[1];
      *utfs++ = char(uchr);
      uchr = (uint8_t(u4cp) & ~kUTF8ContMask) | kUTF8ContMark;
      *utfs++ = char(uchr);
      break;
    case 3:
      uchr = (uint8_t(u4cp >> 12) & ~kUTF8LeadMask[3]) | kUTF8LeadMask[2];
      *utfs++ = char(uchr);
      uchr = (uint8_t(u4cp >> 6) & ~kUTF8ContMask) | kUTF8ContMark;
      *utfs++ = char(uchr);
      uchr = (uint8_t(u4cp) & ~kUTF8ContMask) | kUTF8ContMark;
      *utfs++ = char(uchr);
      break;
    case 4:
      uchr = (uint8_t(u4cp >> 18) & ~kUTF8LeadMask[4]) | kUTF8LeadMask[3];
      *utfs++ = char(uchr);
      uchr = (uint8_t(u4cp >> 12) & ~kUTF8ContMask) | kUTF8ContMark;
      *utfs++ = char(uchr);
      uchr = (uint8_t(u4cp >> 6) & ~kUTF8ContMask) | kUTF8ContMark;
      *utfs++ = char(uchr);
      uchr = (uint8_t(u4cp) & ~kUTF8ContMask) | kUTF8ContMark;
      *utfs++ = char(uchr);
      break;
    default:
      return -1;
  }
  return bytes;
}
int U4CodeToUTF8(UCS4CodePoint u4cp, std::string *utfs) {
  char cutfs[4];
  int bytes = U4CodeToUTF8(u4cp, cutfs);
  if (bytes > 0) {
    utfs->assign(cutfs, bytes);
  }
  return bytes;
}

int DecodeUTF8Str(const char *utfstr, UCS4String *u4str) {
  u4str->clear();
  int bytes = 0;
  char *nxtchr = NULL;
  UCS4CodePoint u4cp;
  while ((nxtchr = GetNextUTF8(utfstr, strlen(utfstr), &bytes)) != NULL) {
    if (UTF8ToU4Code(nxtchr, bytes, &u4cp) > 0) {
      u4str->push_back(u4cp);
      utfstr = nxtchr + bytes;
    }
  }
  return u4str->size();
}
int DecodeUTF8Str(const std::string &utfstr, UCS4String *u4str) {
  u4str->clear();

  char *cutfs = new char[utfstr.size() + 1];
  if (cutfs == NULL) {
    return -1;
  }
  size_t len = utfstr.copy(cutfs, utfstr.size());
  cutfs[len] = '\0';

  int ret = DecodeUTF8Str(cutfs, u4str);
  delete [] cutfs;
  return ret;
}
int EncodeUTF8Str(const UCS4String &ucs, std::string *utfs) {
  return EncodeUTF8Str(ucs.begin(), ucs.end(), utfs);
}
int EncodeUTF8Str(UCS4String::const_iterator beg,
    UCS4String::const_iterator end, std::string *utfs) {
  utfs->clear();
  std::string utf8;
  UCS4String::const_iterator cit;
  for (cit = beg; cit != end; ++cit) {
    if (U4CodeToUTF8(*cit, &utf8) > 0) {
      utfs->append(utf8);
    } else {
      return 0;
    }
  }
  return 1;
}

char *GetNextGBK(const char gbks[], int len, int *bytes) {
  if (gbks == NULL || len <= 0) {
    *bytes = 0;
    return NULL;
  }

  *bytes = 0;
  uint8_t lead = uint8_t(gbks[0]);
  if ((lead & kSingleGBKMask) == 0x00) {
    *bytes = 1;
    return const_cast<char *>(gbks);
  } else if (lead >= kDoubleGBKLow && lead <= kDoubleGBKHigh) {
    *bytes = 2;
    if (len >= 2) {
      return const_cast<char *>(gbks);
    }
  }
  return NULL;
}
int GBKToU2Code(const char gbk[], int bytes, UCS2CodePoint *u2cp) {
  UCS2CodePoint src, dest;
  const UCS2CodePoint *pmap = NULL;
  switch (bytes) {
    case 1:
      src = *gbk;
      pmap = kGBK2UniMapIndex[(src & kGBKMapHighMask) >> kGBKMapLowBits];
      if (pmap != NULL) {
        dest = pmap[src & kGBKMapLowMask];
      } else {
        dest = src;
      }
      break;
    case 2:
      // NOTICE: little-endian
      src = *(UCS2CodePoint *)gbk;
      pmap = kGBK2UniMapIndex[(src & kGBKMapHighMask) >> kGBKMapLowBits];
      if (pmap != NULL) {
        dest = pmap[src & kGBKMapLowMask];
      } else {
        dest = kNaUni;
      }
      break;
    default:
      dest = kNaUni;
      break;
  }
  if (dest == kNaUni) {
    return -1;
  } else {
    *u2cp = dest;
    return bytes;
  }
}
int U2CodeToGBK(UCS2CodePoint u2cp, char gbk[]) {
  const GBKCodePoint *pmap = NULL;
  GBKCodePoint dest;
  pmap = kUni2GBKMapIndex[(u2cp & kGBKMapHighMask) >> kGBKMapLowBits];
  if (pmap != NULL) {
    dest = pmap[u2cp & kGBKMapLowMask];
  } else {
    dest = kNaGBK;
  }
  if (dest == kNaGBK) {
    return -1;
  } else if ((dest & kSingleGBKMask) == 0x00) {
    gbk[0] = char(dest);
    gbk[1] = 0x00;
    return 1;
  } else {
    *(GBKCodePoint *)gbk = dest;
    return 2;
  }
}
int U2CodeToGBK(UCS2CodePoint u2cp, std::string *gbks) {
  char cgbks[2];
  int bytes = U2CodeToGBK(u2cp, cgbks);
  if (bytes > 0) {
    gbks->assign(cgbks, bytes);
  }
  return bytes;
}

int DecodeGBKStr(const char *gbks, UCS2String *u2str) {
  u2str->clear();
  int bytes = 0;
  char *nxtchr = NULL;
  UCS2CodePoint u2cp;
  while ((nxtchr = GetNextGBK(gbks, strlen(gbks), &bytes)) != NULL) {
    if (GBKToU2Code(nxtchr, bytes, &u2cp) > 0) {
      u2str->push_back(u2cp);
      gbks = nxtchr + bytes;
    }
  }
  return u2str->size();
}
int EncodeGBKStr(const UCS2String &ucs, std::string *gbks) {
  return EncodeGBKStr(ucs.begin(), ucs.end(), gbks);
}
int EncodeGBKStr(UCS2String::const_iterator beg,
    UCS2String::const_iterator end, std::string *gbks) {
  gbks->clear();
  std::string gbk;
  UCS2String::const_iterator cit;
  for (cit = beg; cit != end; ++cit) {
    if (U2CodeToGBK(*cit, &gbk) > 0) {
      gbks->append(gbk);
    } else {
      return 0;
    }
  }
  return 1;
}

int EncodeUTF8Str(const UCS2String &ucs, std::string *utfs) {
  return EncodeUTF8Str(ucs.begin(), ucs.end(), utfs);
}
int EncodeUTF8Str(UCS2String::const_iterator beg,
    UCS2String::const_iterator end, std::string *utfs) {
  utfs->clear();
  std::string utf8;
  UCS2String::const_iterator cit;
  for (cit = beg; cit != end; ++cit) {
    if (U4CodeToUTF8(UCS4CodePoint(*cit), &utf8) > 0) {
      utfs->append(utf8);
    } else {
      return 0;
    }
  }
  return 1;
}
int EncodeGBKStr(const UCS4String &ucs, std::string *gbks) {
  return EncodeGBKStr(ucs.begin(), ucs.end(), gbks);
}
int EncodeGBKStr(UCS4String::const_iterator beg,
    UCS4String::const_iterator end, std::string *gbks) {
  gbks->clear();
  std::string gbk;
  UCS4String::const_iterator cit;
  for (cit = beg; cit != end; ++cit) {
    if (U2CodeToGBK(UCS2CodePoint(*cit), &gbk) > 0) {
      gbks->append(gbk);
    } else {
      return 0;
    }
  }
  return 1;
}

}  // namespace yaseek
