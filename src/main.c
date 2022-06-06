#include "./lib/structs.h"

int main() {
  MTRand rng = seedRand(1234567);
  TMinedBlock *previous = (TMinedBlock *)loadLastBin(sizeof(TMinedBlock), __FILE_BLOCKS);
  unsigned long *seq_rand = (unsigned long *)loadLastBin(sizeof(unsigned long), __FILE_RAND);

  if (seq_rand) {
    while (__SEQ_RAND < *seq_rand) {
      randChar(&rng, 0, 2);
    }
  }

  if (previous) {
    printf("Last block: %u %u ", previous->raw.num, previous->raw.nonce);
    printSHA256(previous->hash);
    printf("\n");
  }

  printf("---------------------\n");

  while (1) {
    mineNextBlock(&previous, &rng);
    printf("----------------------\n");
  }

  return 0;
}