// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#ifndef UTILS_STRINGS_H_
#define UTILS_STRINGS_H_

#include <ctype.h>
#include <string.h>

#include <string>
#include <vector>

namespace yaseek {

// split by SEP _string_
int Split(const std::string &line, const std::string &sep,
    std::vector<std::string> *pvec);
// MAXSIZE: including the extra '\0' character
int split(const char *line, const char *sep, char **items, int maxitems,
    int maxsize);
// char items[m][n]; split2d(line, sep, (char *)items, m, n);
int split2d(const char *line, const char *sep, char *item2d, int maxitems,
    int maxsize);

// split by SEP _characters_
// NOTICE: will modify LINE; thread-unsafe due to strtok()
int tokenize(char *line, const char *delim, char **items, int maxitems);

// string equivalent of is* functions for char
int isalnums(const char *str);
int isalphas(const char *str);
int isdigits(const char *str);
int islowers(const char *str);
int isspaces(const char *str);
int isuppers(const char *str);

std::string &LTrims(std::string &str);
std::string &RTrims(std::string &str);
std::string &Trims(std::string &str);
std::string &TrimCmnt(std::string &str, int cmnt = '#');

char *ltrims(const char *str);
// NOTICE: does not work for <const char *>
char *rtrims(char *str);
char *trims(char *str);
// trim comment
char *trimcmnt(char *str, int cmnt = '#');

}  // namespace yaseek

#endif  // UTILS_STRINGS_H_
