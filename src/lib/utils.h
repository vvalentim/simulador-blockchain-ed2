#ifndef __BLOCKCHAIN_UTILS_H
#define __BLOCKCHAIN_UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "../vendor/mtwister.h"

/**
 * Gera e retorna um caractere randômico baseado no intervalo definido.
 * 
 * @param generator - Endereço do gerador de números randômicos (Mersenne Twister), a função assume que esse valor não é nulo.
 * @param min - Valor mínimo do intervalo.
 * @param max - Valor máximo do intervalo.
 * @return Retorna um caractere randômico.
 */
unsigned char randChar(MTRand *generator, unsigned char min, unsigned char max);

/**
 * Imprime um vetor com a hash SHA256 (condizente com o tamanho de 32 bytes)
 * 
 * @param hash - Endereço do vetor hash para impressão, a função assume que esse valor não é nulo.
 */
void printSHA256(unsigned char *hash);

/**
 * Compara dois vetores de hash SHA256.
 * 
 * @param h1 - Endereço de um vetor com a hash SHA256, a função assume que esse valor não é nulo.
 * @param h2 - Endereço de um vetor com a hash SHA256, a função assume que esse valor não é nulo.
 * 
 * @return Retorna 0 ou 1 para falso/verdadeiro.
 */
unsigned char compSHA256(unsigned char *h1, unsigned char *h2);

/**
 * Captura o input do usuário como string, faz a validação e retorna um inteiro.
 * 
 * @param errMsg - Ponteiro contendo a mensagem de erro caso falhe a conversão do input para inteiro.
 * 
 * @return Retorna um inteiro.
 */
int getIntInput(char *errMsg);

#endif