// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.

// double array trie, the raw trie part test

#include "nlp/datrie/include/datrie.h"
#include "utils/strings.cc"
#include <string.h>

static int add_wds_file(datrie *dtraw, const char *fpath) {
  FILE *fin = fopen(fpath, "r");
  if (!fin) {
    return 0;
  }
  const void *pnull = NULL;
  int nwds = 0;
  const int kMaxLine = 256;
  char line[kMaxLine];
  while (fgets(line, kMaxLine, fin)) {
    yaseek::rtrims(line);
    if (dtadd(dtraw, line, &pnull, sizeof(pnull))) {
      ++nwds;
    }
  }
  fclose(fin);
  return nwds;
}

static void search_wds_file(const datrie *dtraw, const char *fpath) {
  FILE *fin = fopen(fpath, "r");
  if (!fin) {
    return;
  }

  int nlost = 0;
  const int kMaxLine = 256;
  char line[kMaxLine];
  while (fgets(line, kMaxLine, fin)) {
    yaseek::rtrims(line);
    if (dtfind(dtraw, line, NULL, 0) != 1) {
      ++nlost;
      fprintf(stderr, "{%s} not in dict...\n", line);
    }
  }
  if (nlost > 0) {
    fprintf(stderr, "Oops: %d words lost from double array trie\n", nlost);
  } else {
    fprintf(stderr, "Hooray! No words lost from double array trie\n");
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s WORD_FILE [SEARCH_FILE]\n", argv[0]);
    return 0;
  }
  const char *fword = argv[1];
  const char *fsearch = argv[1];
  if (argc > 2) {
    fsearch = argv[2];
  }
  datrie *dtre = dtalloc(ENC_UTF8, "en_US.UTF8", 10*1024, 0);
  if (dtre == NULL) {
    return 0;
  }
  add_wds_file(dtre, fword);
  int ret = dtbuild(dtre);
  if (ret == 0) {
    return 0;
  }
  search_wds_file(dtre, fsearch);
  dtfree(dtre);
  return 0;
}
