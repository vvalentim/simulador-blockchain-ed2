#include "structs.h"

TRawBlock initBlock(TMinedBlock *prevBlock, MTRand *rng) {
  TRawBlock block;
  block.num = prevBlock ? prevBlock->raw.num + 1 : 0;
  block.nonce = 0;

  /* Fill data vector with zeroes to prevent trash on it. */
  for (unsigned int i = 0; i < sizeof(block.data); i++) {
    block.data[i] = 0;
  }
  
  if (prevBlock) {
    for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
      block.prevHash[i] = prevBlock->hash[i];
    }
  } else {
    for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
      block.prevHash[i] = 0;
    }
  }

  /* Generates a random number of transactions for the block */
  unsigned char transactions = randChar(rng, 1, 61);

  printf("Initializing Block nº %u, transactions generated: %d\n", block.num, transactions);

  /* Iterate to generate data for each transaction (send, receive, value) */
  for (int t = 0; t < transactions; t++) {
    /* Calculate the next memory block for the current transaction */
    int i = t * 3;

    unsigned char send = randChar(rng, 0, 255);
    unsigned char receive = randChar(rng, 0, 255);
    unsigned char value = randChar(rng, 1, 50);

    /* Check for duplicate addresses for sender and receiver */
    while (send == receive) {
      receive = randChar(rng, 0, 255);
    }

    block.data[i] = send;
    block.data[i + 1] = receive;
    block.data[i + 2] = value;
  }

  return block;
}

int authBlock(unsigned char *prevHash, unsigned char *minedHash, TRawBlock *block) {
  int flag = memcmp(prevHash, block->prevHash, SHA256_DIGEST_LENGTH);

  if (flag == 0) {
    unsigned char bufHash[SHA256_DIGEST_LENGTH];

    SHA256((unsigned char *)(block), sizeof(TRawBlock), bufHash);
    flag = memcmp(minedHash, bufHash, SHA256_DIGEST_LENGTH);
  }

  return !flag;
}

void* workerGetProof(void *args) {
  void **p_vec = (void **)args;
  unsigned int *v_key = (unsigned int *)(p_vec[0]);
  unsigned char *proofHash = (unsigned char *)(p_vec[1]);
  unsigned char *proofDone = (unsigned char *)(p_vec[2]);
  TRawBlock *block = (TRawBlock *)(p_vec[3]);
  
  if (v_key && proofHash && proofDone && block) {
    /* Each worker thread should have their own block of bytes so it can hash and alter it's nonce independently. */
    TRawBlock *dummy = (TRawBlock *)malloc(sizeof(TRawBlock));
    /* The maximum number of iterations that a thread can try hashing. */
    unsigned int iteration_limit = (UINT_MAX - *v_key) / (__MAX_THREADS);

    if (dummy) {
      memcpy(dummy, block, sizeof(TRawBlock));
      /* Set nonce for the first iteration based on the number of this thread. */
      dummy->nonce = *v_key;
      unsigned char bufHash[SHA256_DIGEST_LENGTH];
      SHA256((unsigned char *)dummy, sizeof(TRawBlock), bufHash);

      while (!checkProof(bufHash)) {
        /* Check if the current thread has a higher valued nonce in case a nonce was found. If so drops it, as we want the lesser value nonce. */
        if (*proofDone && block->nonce < dummy->nonce) {
          break;
        }

        if (iteration_limit == 0) {
          printf("Thread %u hits the limit of iterations with nonce %u.\n", *v_key, dummy->nonce);
          break;
        }

        iteration_limit--;
        dummy->nonce += __MAX_THREADS;
        SHA256((unsigned char *)dummy, sizeof(TRawBlock), bufHash);
      }

      pthread_mutex_lock(&__PROOF_MUTEX);
      printf("Joining thread %u.\n", *v_key);
      /* One last check before dropping the thread, so if a lower value nonce hash was found we still get to swap it. */
      if (checkProof(bufHash)) {
        *proofDone = 1;
        block->nonce = dummy->nonce;
        memcpy(proofHash, bufHash, SHA256_DIGEST_LENGTH);

        printf("Found hash on thread %u with nonce %u: ", *v_key, dummy->nonce);
        printSHA256(bufHash);
        printf("\n");
        free(dummy);
      }
      pthread_mutex_unlock(&__PROOF_MUTEX);
    }

    free(v_key);
  }

  free(args);
  return NULL;
}

void mineNextBlock(TMinedBlock **prevBlock, MTRand *rng) {
  if (!prevBlock || !rng) {
    return;
  }

  TMinedBlock *newBlock = (TMinedBlock *)malloc(sizeof(TMinedBlock));
  struct timespec start, finish;
  double elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

  if (newBlock) {
    unsigned char proofDone = 0;
    newBlock->raw = initBlock(*prevBlock, rng);
    pthread_t * tids = (pthread_t *)malloc(sizeof(pthread_t) * __MAX_THREADS);
    pthread_mutex_init(&__PROOF_MUTEX, NULL);

    while (!proofDone && __PROOF_DIFF >= 1) {
      printf("Creating threads to mine block nº %u with difficulty %u...\n", newBlock->raw.num, __PROOF_DIFF);
      for (unsigned int i = 0; i < __MAX_THREADS; i++) {
        unsigned int *v_key = (unsigned *)malloc(sizeof(unsigned int));
        void **args = (void **)malloc(sizeof (void *) * 4);
        
        if (v_key && args) {
          *v_key = i;
          args[0] = v_key;
          args[1] = &(newBlock->hash);
          args[2] = &proofDone;
          args[3] = &(newBlock->raw);
          
          pthread_create(tids + i, NULL, workerGetProof, args);
        } else {
          printf("Failed to allocate memory for thread (v_key/args).\n");
          break;
        }
      }

      printf("Threads created: %d\n", __MAX_THREADS);
      for (unsigned int i = 0; i < __MAX_THREADS; i++) {
        if (pthread_join(tids[i], NULL)) {
          printf("Failed to join thread %u.\n", i);
        }
      }

      if (!proofDone) {
        __PROOF_DIFF--;
        printf("Difficutly adjusted to %u...\n", __PROOF_DIFF);
      }
    }

    if (proofDone) {
      if (*prevBlock) {
        free(*prevBlock);
      }

      __PROOF_DIFF = __PROOF_MAX_DIFF;
      printf("RNG iterations: %lu\n", __SEQ_RAND);
      saveToBin((void *)(newBlock), sizeof(TMinedBlock), 1, __FILE_BLOCKS, "ab");
      saveToBin((void *)(&__SEQ_RAND), sizeof(unsigned long), 1, __FILE_RAND, "wb");
      *prevBlock = newBlock;
    }

    pthread_mutex_destroy(&__PROOF_MUTEX);
    free(tids);
  }
  
  clock_gettime(CLOCK_MONOTONIC, &finish);
  elapsed = (finish.tv_sec - start.tv_sec);
  elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
  printf("All done, elapsed: %f\n", elapsed);
}