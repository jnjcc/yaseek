// Copyright (c) 2012 - 2014 Yste.org
// All Rights Reserved.

// darray test

#include "cstruct/darray/include/darray.h"
#include "utils/strings.cc"
#include <stdio.h>

void printstr(const void *pstr, uint32_t sze) {
  fprintf(stdout, "  {%s}\n", (const char *)pstr);
}

int vstrcmp(const void *lhs, const void *rhs) {
  return strcmp((const char *)lhs, (const char *)rhs);
}

int vrstrcmp(const void *lhs, const void *rhs) {
  return (-1) * vstrcmp(lhs, rhs);
}

void printda(const darray *pda, const char *header) {
  fprintf(stdout, "%s:\n>>>\n", header);
  dwalk(pda, printstr);
  fprintf(stdout, "<<<\n\n");
}

int main(int argc, char *argv[]) {
  const int kMaxLine = 1024;
  char line[kMaxLine];
  
  darray *pda = ddlloc(kMaxLine * 10);
  while (fgets(line, kMaxLine, stdin)) {
    yaseek::rtrims(line);
    dpushstr(pda, line);
  }

  darray *pcda = dclone(pda);

  // set elements
  printda(pda, "original");

  daddstr(pda, 0, "added before 0");
  printda(pda, "add before 0");

  dget(pda, 1, line, kMaxLine);
  dsetstr(pda, 1, "very very very very very very very very very long string");
  printda(pda, "very long element");

  dsetstr(pda, 1, line);
  printda(pda, "restore element 1");

  ddel(pda, dsize(pda) - 1);
  printda(pda, "delete last");

  // sort elements
  dhsort(pcda, vstrcmp);
  printda(pcda, "sort");

  dqsort(pcda, vrstrcmp);
  printda(pcda, "reverse sort");

  dfree(pda);
  dfree(pcda);
  return 0;
}
