#include "utils.h"

unsigned char randChar(MTRand *generator, unsigned char min, unsigned char max) {
  return min + genRandLong(generator) % max + 1 - min;
}

void printSHA256(unsigned char *hash) {
  for(unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    printf("%02x", hash[i]);
  }
}