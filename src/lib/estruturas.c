#include "estruturas.h"

BlocoNaoMinerado novoBloco(int numero, unsigned char *hashAnterior, MTRand *gerador) {
  BlocoNaoMinerado bloco;

  // Define o número do bloco e o nonce inicial
  bloco.numero = numero < 1 ? 1 : numero;
  bloco.nonce = 0;

  // Inicialização do vetor de dados (transações)
  for (unsigned int i = 0; i < sizeof(bloco.data); i++) {
    bloco.data[i] = 0;
  }
  
  if (bloco.numero == 1) {
    for (unsigned int i = 0; i < sizeof(bloco.hashAnterior); i++) {
      bloco.hashAnterior[i] = 0;
    }
  } else {
    for (unsigned int i = 0; i < sizeof(bloco.hashAnterior); i++) {
      bloco.hashAnterior[i] = hashAnterior[i];
    }
  }

  // Simula um número randômico de transações contidas no bloco
  unsigned char transacoes = randChar(gerador, 1, 61);

  printf("Número de transações geradas: %d\n", transacoes);

  // Iteração para gerar os dados individuais de cada transação
  for (int t = 0; t < transacoes; t++) {
    // Considerando que cada transação ocupa 3 bytes, calcula o próximo indice no vetor de dados
    int i = t * 3;

    // Simula os endereços e valor da transação
    unsigned char addr1 = randChar(gerador, 0, 255);
    unsigned char addr2 = randChar(gerador, 0, 255);
    unsigned char valor = randChar(gerador, 1, 50);

    // Caso o segundo endereço seja o mesmo que o primeiro, itera até encontrar um diferente
    while (addr2 == addr1) {
      addr2 = randChar(gerador, 0, 255);
    }

    // Popula o vetor de dados
    bloco.data[i] = addr1;
    bloco.data[i + 1] = addr2;
    bloco.data[i + 2] = valor;

    printf("%d %d %d\n", bloco.data[i], bloco.data[i + 1], bloco.data[i + 2]);
  }

  return bloco;
}

BlocoMinerado * simularMineracao(TNoBloco *lista, MTRand *gerador) {
  unsigned int numAnterior = 0;
  unsigned char * hashAnterior = NULL;

  // Condicional para checar encadeamento
  if (lista != NULL) {
    TNoBloco *no = lista;

    // Itera até chegar ao final da blockchain
    while (no->prox != NULL) {
      no = no->prox;
    }

    numAnterior = no->ref->bloco.numero;
    hashAnterior = no->ref->hash;
  }

  BlocoMinerado * minerado = (BlocoMinerado *)malloc(sizeof(BlocoMinerado));

  if (minerado != NULL) {
    printf("Iniciando mineração do bloco nº %d:", numAnterior + 1);
    printf("\n");

    minerado->bloco = novoBloco(numAnterior + 1, hashAnterior, gerador);

    SHA256((unsigned char *)&(minerado->bloco), sizeof(minerado->bloco), minerado->hash);

    while (minerado->hash[0] != 0 || minerado->hash[1] != 0 || minerado->hash[2] != 0) {
      // printf("Hash inválida: ");
      // printSHA256(minerado->hash);
      // printf("\n");

      minerado->bloco.nonce += 1;
      SHA256((unsigned char *)&(minerado->bloco), sizeof(minerado->bloco), minerado->hash);
    }
    
    printf("Hash válida: ");
    printSHA256(minerado->hash);
    printf("\n");
    printf("Nonce: %d\n", minerado->bloco.nonce);
  }

  return minerado;
}