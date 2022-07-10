#ifndef __BLOCKCHAIN_STRUCTS_H
#define __BLOCKCHAIN_STRUCTS_H

#include <openssl/crypto.h>
#include <openssl/sha.h>
#include "utils.h"

typedef struct TRawBlock {
  unsigned int num;
  unsigned int nonce;
  unsigned char data[184];
  unsigned char prevHash[SHA256_DIGEST_LENGTH];
} TRawBlock;

typedef struct TMinedBlock {
  TRawBlock raw;
  unsigned char hash[SHA256_DIGEST_LENGTH];
} TMinedBlock;

typedef struct TAddress {
  unsigned char addr;
  unsigned char coins;
  unsigned int numTransactions;
} TAddress;

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
 * Initialization for a vector of addresses from 0 to 255.
 * 
 * @return An initialized vector of the struct TAddress.
 */
TAddress* initAddresses();

/**
 * Receives the original address vector and returns a copy of it, sorted by the number of coins (ascending).
 * 
 * @param blockNum TAddress vector to be sorted.
 * @return An initialized vector of the struct TAddress.
 */
TAddress* sortedAddrByCoins(TAddress *ref);

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
 * @param prevBlock Reference to the pointer to the previous block
 * @param rng Pointer to the Mersenne Twister RNG.
 */
void mineNextBlock(TMinedBlock **prevBlock, MTRand *rng);

/**
 * Search for a block by it's number.
 * 
 * @param num - The number of the block to be searched.
 * @return A pointer to the retrieved block.
 */
TMinedBlock* searchBlock(unsigned int num);

/**
 * Parse the data from a block.
 * 
 * @param block The block to be parsed.
 * @param addresses The addresses reference which will be processed.
 */
void parseBlock(TMinedBlock *block, TAddress *addresses, unsigned int *nonceMax);


#endif