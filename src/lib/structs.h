#ifndef __BLOCKCHAIN_STRUCTS_H
#define __BLOCKCHAIN_STRUCTS_H

#include <openssl/crypto.h>
#include <openssl/sha.h>
#include "utils.h"

typedef struct TRawBlock {
  unsigned int num; // 4 bytes
  unsigned int nonce; // 4 bytes
  unsigned char data[184]; // 184 bytes
  unsigned char prevHash[SHA256_DIGEST_LENGTH]; // 32 bytes
} TRawBlock;

typedef struct TMinedBlock {
  TRawBlock raw; // 224 bytes
  unsigned char hash[SHA256_DIGEST_LENGTH]; // 32 bytes
} TMinedBlock;

/**
 * Initialization for a raw block.
 * 
 * @param blockNum Number value for this block.
 * @param prevHash Pointer to the hash from the previous block, if this is null it will be initialized with all bytes as zero.
 * @param rng Pointer to the Mersenne Twister RNG.
 * @return An initialized struct BlocoNaoMinerado.
 */
TRawBlock initBlock(TMinedBlock *prevBlock, MTRand *rng);

/**
 * Check if a block can be linked to the blockchain.
 * First compares the previous hash with the one from the raw block, then hash the raw block and check if the mined hash matches.
 * 
 * @param prevHash Pointer to the hash from the previous block.
 * @param minedHash Pointer to the hash that has been mined for this block.
 * @param block Pointer to the block structure which needs to be authenticated.
 * @return A boolean flag.
 */
int authBlock(unsigned char *prevHash, unsigned char *minedHash, TRawBlock *block);

/**
 * Receives a vector of pointers which will contain the data necessary to hash a block of data.
 * 
 * @param args{unsigned_int*,unsigned_char*,unsigned_char*,TRawBlock*} Vector of pointers with the expected values: a key value for the worker, a pointer to set the proof hash, a flag to sign if a proof was found and the block of data to be hashed.
 * @return NULL valued pointer.
 */
void* workerGetProof(void *args);

/**
 * Initializes a raw block and assign threads to try and find a hash with a valid proof of work.
 * If a valid proof is found, it will save and set it as the previous block for future calls.
 * 
 * @param prevBlock Pointer to the previous block
 * @param rng Pointer to the Mersenne Twister RNG.
 */
void mineNextBlock(TMinedBlock **prevBlock, MTRand *rng);

#endif