#include "defs.h"

unsigned long __SEQ_RAND = 0;
unsigned char __PROOF_MAX_DIFF = 4;
unsigned char __PROOF_DIFF = 4;
pthread_mutex_t __PROOF_MUTEX;