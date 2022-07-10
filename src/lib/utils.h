#ifndef __BLOCKCHAIN_UTILS_H
#define __BLOCKCHAIN_UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "defs.h"
#include "../vendor/mtwister.h"

/**
 * Generates a random unsigned char value from a closed interval.
 * 
 * @param generator Pointer to the Mersenne Twister RNG.
 * @param min Interval minimum value.
 * @param max Interval maximum value.
 * @return An unsigned char value from a closed interval of min and max.
 */
unsigned char randChar(MTRand *generator, unsigned char min, unsigned char max);

/**
 * Checks if the proof of work for a hash is valid, by comparing the first n bytes based of the current difficulty.
 * 
 * @param hash Pointer to the SHA256 hash.
 * @return A boolean flag.
 */
int checkProof(unsigned char *hash);

/**
 * Prints the specified SHA256 hash.
 * 
 * @param hash Pointer to the SHA256 hash.
 */
void printSHA256(unsigned char *hash);

/**
 * Capture user input as an array of characters, tries to validate it as an integer.
 * 
 * @param errMsg Pointer to the vector which contains an error message, message will be printed if the pointer isn't null.
 * @return An integer.
 */
int getIntInput(char *errMsg);

/**
 * Saves a block of bytes to a binary file.
 * 
 * @param bytes Pointer to the block of bytes.
 * @param size Number of bytes for the type of data to be saved.
 * @param n Number of elements on the block.
 * @param path Path to the binary file where the data will be saved.
 * @param fp_flag Mode specifier for the fopen() call.
 */
void saveToBin(void *bytes, size_t size, unsigned int n, char *path, char *fp_flag);

/**
 * Loads the last block of data from a file, based on it's type.
 * 
 * @param ptr Pointer to pointer that will receive the block of data.
 * @param size Number of bytes for the type of data to be loaded.
 * @param path Path to the binary file where data will be loaded from.
 * @return A void pointer which points to the allocated data.
 */
void* loadLastBin(size_t size, char *path);

#endif