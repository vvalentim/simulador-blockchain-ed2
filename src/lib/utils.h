#ifndef __BLOCKCHAIN_UTILS_H
#define __BLOCKCHAIN_UTILS_H

// Inclui as bibliotecas necessárias para executar as funções dessa biblioteca.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "../vendor/mtwister.h"

// Gera e retorna um caractere randômico baseado no intervalo definido.
unsigned char randChar(MTRand *generator, unsigned char min, unsigned char max);

// Imprime um vetor com a hash SHA256 (condizente com o tamanho de 32 bytes)
void printSHA256(unsigned char *hash);

#endif