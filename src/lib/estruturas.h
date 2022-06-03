#ifndef __BLOCKCHAIN_ESTRUTURAS_H
#define __BLOCKCHAIN_ESTRUTURAS_H

#include <openssl/crypto.h>
#include <openssl/sha.h>
#include "../vendor/mtwister.h"
#include "utils.h"

/**
 * TAD para armazenar os dados (número, nonce, transações e hash do bloco anterior) de um bloco não minerado.
 */
typedef struct BlocoNaoMinerado {
  unsigned int numero; // 4 bytes
  unsigned int nonce;// 4 bytes
  unsigned char data[184]; // 184 bytes
  unsigned char hashAnterior[SHA256_DIGEST_LENGTH]; // 32 bytes
} BlocoNaoMinerado;

/**
 * TAD para armazenar o valor de um bloco não minerado e o vetor com a hash que comprova a validade de um bloco minerado.
 * A hash deverá possuir o valor 0 no primeiros 2 bytes (4 zeros na representação hexadecimal).
 */
typedef struct BlocoMinerado {
  BlocoNaoMinerado bloco; // 224 bytes
  unsigned char hash[SHA256_DIGEST_LENGTH]; // 32 bytes
} BlocoMinerado;

/**
 * Verifica um bloco para garantir que pode ser ligado ao bloco anterior.
 * 
 * @param hashAnterior - Endereço da hash minerada do bloco anterior, a função assume que esse valor não é nulo.
 * @param hashMinerada - Endereço da hash que foi obtida ao minerar o bloco que está sendo verificado, a função assume que esse valor não é nulo.
 * @param bloco - Endereço do bloco não minerado, a função assume que esse valor não é nulo.
 * @return Retorna 0 ou 1 para falso/verdadeiro.
 */
unsigned char autenticarBloco(unsigned char *hashAnterior, unsigned char *hashMinerada, BlocoNaoMinerado *bloco);

/**
 * Instancia e retorna a TAD BlocoNaoMinerado.
 * 
 * @param numero - O número do novo bloco.
 * @param hashAnterior - Endereço da hash minerada do bloco anterior, caso o valor seja nulo um novo vetor será inicializado com 0 em todas as posições.
 * @param gerador - Endereço do gerador de números randômicos (Mersenne Twister), a função assume que esse valor não é nulo.
 * @return Retorna o valor inicializado da TAD BlocoNaoMinerado.
 */
BlocoNaoMinerado novoBloco(int numero, unsigned char *hashAnterior, MTRand *gerador);

/**
 * Instancia a TAD BlocoNaoMinerado e aloca dinamicamente BlocoMinerado, executa o algoritmo SHA256 sob o bloco não minerado até encontrar uma hash que atenda ao requisito de mineração.
 * Retorna o endereço da alocação dinâmica.
 * 
 * @param pb - Endereço de um bloco que já foi minerado, caso o valor seja nulo supõe-se que novo bloco será o primeiro bloco.
 * @param gerador - Endereço do gerador de números randômicos (Mersenne Twister), a função assume que esse valor não é nulo.
 * @return Retorna um endereço TAD BlocoMinerado.
 */
BlocoMinerado * simularMineracao(BlocoMinerado *pb, MTRand *gerador);

#endif