#ifndef __DEFS_BLOCKCHAIN
#define __DEFS_BLOCKCHAIN
#include <pthread.h>

#define __MAX_THREADS 8
/* Block size refers to the storage system. */
#define __BLOCK_SIZE 4096
#define __FILE_BLOCKCHAIN "./dat/mt_blocks_a.dat"
#define __FILE_RAND "./dat/mt_rand_a.dat"
/* Number of calls to the RNG function. */
extern unsigned long __SEQ_RAND;
/* Proof of work current difficulty, should be changed back to the maximum diffculty whenever a block is successfully mined. */
extern unsigned char __PROOF_DIFF;
/* Proof of work maximum difficulty. */
extern unsigned char __PROOF_MAX_DIFF;
/* Mutex for the hashing threads */
extern pthread_mutex_t __PROOF_MUTEX;

#endif