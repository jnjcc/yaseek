//  Copyright 2013 Google Inc. All Rights Reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef __APPLE__
 #include <sys/malloc.h>
 #include <stdlib.h>
#else
 #include <malloc.h>
#endif

const long long kMaxSize = 2000;         // max length of strings
const long long kMaxW = 50;              // max length of vocabulary entries
const float kOutOfVocab = -2.0f;         // Out-Of-Vocabulary

// word2vec model struct
typedef struct w2v_tag {
  long long words;
  long long size;
  float *M;
  char *vocab;
} w2v_t;

int load_model(const char *mfile, w2v_t *pw2v) {
  if (mfile == NULL || pw2v == NULL) {
    return -1;
  }
  FILE *f = fopen(mfile, "rb");
  if (f == NULL) {
    return -1;
  }
  fscanf(f, "%lld", &(pw2v->words));
  fscanf(f, "%lld", &(pw2v->size));
  long long words = pw2v->words;
  long long size = pw2v->size;
  pw2v->vocab = (char *)malloc((long long)words * kMaxW * sizeof(char));
  pw2v->M = (float *)malloc((long long)words * (long long)size * sizeof(float));
  if (pw2v->M == NULL) {
    printf("Cannot allocate memory: %lld MB    %lld  %lld\n",
        (long long)words * size * sizeof(float) / 1048576, words, size);
    return -1;
  }
  char *vocab = pw2v->vocab;
  float *M = pw2v->M;
  float len;
  long long a, b;
  for (b = 0; b < pw2v->words; b++) {
    a = 0;
    while (1) {
      vocab[b * kMaxW + a] = fgetc(f);
      if (feof(f) || (vocab[b * kMaxW + a] == ' ')) break;
      if ((a < kMaxW) && (vocab[b * kMaxW + a] != '\n')) a++;
    }
    vocab[b * kMaxW + a] = 0;
    for (a = 0; a < size; a++) fread(&M[a + b * size], sizeof(float), 1, f);
    len = 0;
    for (a = 0; a < size; a++) len += M[a + b * size] * M[a + b * size];
    len = sqrt(len);
    for (a = 0; a < size; a++) M[a + b * size] /= len;
  }
  fclose(f);
  return 1;
}

void free_model(w2v_t *pw2v) {
  if (pw2v) {
    if (pw2v->M) {
      free(pw2v->M);
      pw2v->M = NULL;
    }
    if (pw2v->vocab) {
      free(pw2v->vocab);
      pw2v->vocab = NULL;
    }
  }
}

float cosine(const w2v_t *pw2v, const char *st1, const char *st2) {
  float dist, len, vec1[kMaxSize], vec2[kMaxSize];
  long long words = pw2v->words;
  long long size = pw2v->size;
  long long b, c, d;
  const float *M = pw2v->M;
  const char *vocab = pw2v->vocab;
  for (b = 0; b < words; ++b) if (!strcmp(&vocab[b * kMaxW], st1)) break;
  if (b == words) b = -1;
  for (c = 0; c < words; ++c) if (!strcmp(&vocab[c * kMaxW], st2)) break;
  if (c == words) c = -1;
  if (b == -1 || c == -1) {
    return kOutOfVocab;
  } else {
    for (d = 0; d < size; ++d) {
      vec1[d] = M[d + b * size];
      vec2[d] = M[d + c * size];
    }

    len = 0;
    for (d = 0; d < size; ++d) len += vec1[d] * vec1[d];
    len = sqrt(len);
    for (d = 0; d < size; ++d) vec1[d] /= len;

    len = 0;
    for (d = 0; d < size; ++d) len += vec2[d] * vec2[d];
    len = sqrt(len);
    for (d = 0; d < size; ++d) vec2[d] /= len;

    dist = 0.0f;
    for (d = 0; d < size; ++d) dist += vec1[d] * vec2[d];
    return dist;
  }
}

int main(int argc, char **argv) {
  w2v_t w2v;
  if (load_model(argv[1], &w2v) < 0) {
    return -1;
  }

  char st1[kMaxSize], st2[kMaxSize];
  while (fscanf(stdin, "%s\t%s", st1, st2) == 2) {
    fprintf(stdout, "%s\t%s\t%f\n", st1, st2, cosine(&w2v, st1, st2));
  }

  free_model(&w2v);
  return 0;
}
