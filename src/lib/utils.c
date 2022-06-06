#include "utils.h"

unsigned char randChar(MTRand *generator, unsigned char min, unsigned char max) {
  __SEQ_RAND = __SEQ_RAND == ULONG_MAX ? 0 : __SEQ_RAND + 1;

  return min + genRandLong(generator) % max + 1 - min;
}

int checkProof(unsigned char *hash) {
  int flag = 1;

  for (unsigned char i = 0; i < __PROOF_DIFF; i++) {
    if (hash[i] != 0) {
      flag = 0;
      break;
    }
  }

  return flag;
}

void printSHA256(unsigned char *hash) {
  for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    printf("%02x", hash[i]);
  }
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

void saveToBin(void *bytes, size_t size, unsigned int n, char *path, char *fp_flag) {
  FILE *fp_bin = fopen(path, fp_flag);

  if (fp_bin == NULL) {
    printf("Failed to open binary file, couldn't save.\n");
    return;
  }

  fwrite(bytes, size, n, fp_bin);
  
  if (ferror(fp_bin)) {
    printf("Error when trying to write to binary file.\n");
  }
  
  fclose(fp_bin);
}

void* loadLastBin(size_t size, char *path) {
  FILE *fp_bin = fopen(path, "rb");
  long total_bytes = 0;

  if (fp_bin == NULL) {
    return NULL;
  }

  fseek(fp_bin, 0, SEEK_END);
  total_bytes = ftell(fp_bin);
  printf("Total bytes on '%s': %lu\n", path, total_bytes);

  if (total_bytes < (long)(size)) {
    fclose(fp_bin);
    return NULL;
  }

  unsigned char *p = (unsigned char *)malloc(sizeof(unsigned char) * size);

  if (p == NULL) {
    printf("Error when trying to allocate memory.\n");
    fclose(fp_bin);
    return NULL;
  }

  fseek(fp_bin, -size, SEEK_END);
  fread(p, size, 1, fp_bin);
  
  if (ferror(fp_bin)) {
    printf("Error when trying to read from the binary file.\n");
    free(p);
    fclose(fp_bin);
    return NULL;
  }

  fclose(fp_bin);
  return (void *)p;
}