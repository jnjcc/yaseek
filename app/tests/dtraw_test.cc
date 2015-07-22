// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.

// double array trie, the raw trie part test

#include "nlp/datrie/include/datrie.h"
#include "utils/strings.cc"
#include <string.h>

static void fill_extra(const char *word, char *extra) {
  strcpy(extra, word);
}
static int comp_extra(const char *word, const char *extra) {
  return strcmp(word, extra);
}

static int add_wds_file(datrie *dtraw, const char *fpath) {
  FILE *fin = fopen(fpath, "r");
  if (!fin) {
    return 0;
  }
  const int kMaxLine = 256;
  char extra[kMaxLine];
  int nwds = 0;
  char line[kMaxLine];
  while (fgets(line, kMaxLine, fin)) {
    yaseek::rtrims(line);
    fill_extra(line, extra);
    if (dtadd(dtraw, line, extra, strlen(extra) + 1)) {
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
  int ndiff = 0;
  const int kMaxLine = 256;
  char line[kMaxLine];
  char extra[kMaxLine];
  while (fgets(line, kMaxLine, fin)) {
    yaseek::rtrims(line);
    if (!get_raw_trie(dtraw, line, extra, kMaxLine)) {
      ++nlost;
      fprintf(stderr, "{%s} not in dict...\n", line);
    } else {
      if (comp_extra(line, extra) != 0) {
        ++ndiff;
        fprintf(stderr, "{%s}'s extra info{%s} wrong\n", line, extra);
      }
    }
  }
  if (nlost > 0 || ndiff > 0) {
    fprintf(stderr, "Oops: %d words lost, %d words diff in raw trie\n", nlost,
        ndiff);
  } else {
    fprintf(stderr, "Hooray! No words lost, no words diff in raw trie\n");
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
  datrie *dtraw = dtalloc(ENC_UTF8, "en_US.UTF8", 10*1024, 1);
  if (dtraw == NULL) {
    return 0;
  }
  add_wds_file(dtraw, fword);
  int ret = build_raw_trie(dtraw);
  if (ret == 0) {
    return 0;
  }
  search_wds_file(dtraw, fsearch);
  dtfree(dtraw);
  return 0;
}
