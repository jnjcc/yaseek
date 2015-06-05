// Copyright (c) 2012 - 2014 Yste.org
// All Rights Reserved.

#include "utils/strings.cc"
#include "utils/charsets.cc"

#include <stdio.h>
#include <stdlib.h>

using namespace yaseek;

int main(int argc, char *argv[]) {
  int opt = 0;  // 0: utf8 -> unicode; 1: unicode -> utf8
                // 2: utf8 -> gbk;     3: gbk     -> utf8
  if (argc < 2) {
    fprintf(stderr, "Usage: %s OPT\n", argv[0]);
    fprintf(stderr, "  OPT = 0: UTF8 (string) -> Unicode\n");
    fprintf(stderr, "  OPT = 1: Unicode (hex) -> UTF8\n");
    fprintf(stderr, "  OPT = 2: UTF8 (string) -> GBK\n");
    fprintf(stderr, "  OPT = 3: GBK (string) -> UTF8\n");
    return -1;
  }
  opt = strtol(argv[1], NULL, 0);
  if (opt < 0 || opt > 3) {
    opt = 0;
  }
  const int kMaxLine = 1024;
  char line[kMaxLine];
  while (fgets(line, kMaxLine, stdin) > 0) {
    rtrims(line);
    switch (opt) {
      case 0: {
        UCS4String u4str;
        if (DecodeUTF8Str(line, &u4str) > 0) {
          UCS4String::const_iterator cit;
          for (cit = u4str.begin(); cit != u4str.end(); ++cit) {
            fprintf(stdout, "%#x ", *cit);
          }
          fprintf(stdout, "\n");
        }
        break;
      }
      case 1: {
        UCS4CodePoint u4cp = strtol(line, NULL, 16);
        std::string utf8;
        if (U4CodeToUTF8(u4cp, &utf8) > 0) {
          fprintf(stdout, "%s\n", utf8.c_str());
        }
        break;
      }
      case 2: {
        UCS4String u4str;
        std::string gbks;
        if (DecodeUTF8Str(line, &u4str) > 0) {
          if (EncodeGBKStr(u4str, &gbks) > 0) {
            fprintf(stdout, "%s\n", gbks.c_str());
          }
        }
        break;
      }
      case 3: {
        UCS2String u2str;
        std::string utf8s;
        if (DecodeGBKStr(line, &u2str) > 0) {
          if (EncodeUTF8Str(u2str, &utf8s) > 0) {
            fprintf(stdout, "%s\n", utf8s.c_str());
          }
        }
        break;
      }
      default:
        break;
    }
  }
  return 0;
}
