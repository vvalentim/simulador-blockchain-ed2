#include "utils.h"

unsigned char randChar(MTRand *generator, unsigned char min, unsigned char max) {
  __SEQ_RAND__ = __SEQ_RAND__ == ULONG_MAX ? 0 : __SEQ_RAND__ + 1;
  return min + genRandLong(generator) % max + 1 - min;
}

void printSHA256(unsigned char *hash) {
  for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    printf("%02x", hash[i]);
  }
}

unsigned char compSHA256(unsigned char *h1, unsigned char *h2) {
  unsigned char flag = 1;

  for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    if (h1[i] != h2[i]) {
      flag = 0;
      break;
    }
  }

  return flag;
}

int getIntInput(char *errMsg) {
  char *end = NULL;
	char buf[255];
	int n = 0;
  
	while (fgets(buf, sizeof(buf), stdin)) {
		n = strtol(buf, &end, 10);

		if (end == buf || *end != '\n') {
			if (errMsg != NULL) {
        printf("%s", errMsg);
      }
		} else { 
      break; 
    }
	}
  
  return n;
}