#ifndef __BLOCKCHAIN_ESTRUTURAS_H
#define __BLOCKCHAIN_ESTRUTURAS_H

// Inclui as bibliotecas necessárias para executar as funções dessa biblioteca.
#include <openssl/crypto.h>
#include <openssl/sha.h>
#include "../vendor/mtwister.h"
#include "utils.h"

// Contém as informações do bloco (transações), vetor "data" deve ser inicializado com zeros em todas as posições.
// Tamanho da TAD deve ser de 224 bytes
typedef struct BlocoNaoMinerado {
  unsigned int numero; // 4 bytes
  unsigned int nonce;// 4 bytes
  unsigned char data[184]; // 184 bytes
  unsigned char hashAnterior[SHA256_DIGEST_LENGTH]; // 32 bytes
} BlocoNaoMinerado;

// Contém o bloco não minerado e a hash com o resultado da mineração.
// A hash deve cumprir a condição de iniciar com o valor 0 nas primeiras quatro posições.
// Tamanho da TAD deve ser de 256 bytes.
typedef struct BlocoMinerado {
  BlocoNaoMinerado bloco; // 224 bytes
  unsigned char hash[SHA256_DIGEST_LENGTH]; // 32 bytes
} BlocoMinerado;

// TAD do tipo lista simples para encadeamento de blocos minerados.
typedef struct TNoBloco {
  BlocoMinerado *ref;
  struct TNoBloco * prox;
} TNoBloco;

// Instancia e retorna a TAD BlocoNaoMinerado de acordo com as condições definidas.
// Caso o bloco seja o primeiro (numero == 1), o vetor da hash anterior deve ser preenchido com 0, caso contrário faz uma cópia da hash do bloco anterior.
// MTRand *g é a referencia ao Mersenne Twister (deve ser inicializado na main com a seed 1234567).
BlocoNaoMinerado novoBloco(int numero, unsigned char *hashAnterior, MTRand *gerador);

BlocoMinerado * simularMineracao(TNoBloco *lista, MTRand *gerador);

#endif