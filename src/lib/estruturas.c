#include "estruturas.h"

unsigned char autenticarBloco(unsigned char *hashAnterior, unsigned char *hashMinerada, BlocoNaoMinerado *bloco) {
  /* Compara se a hash do bloco anterior coincide com a hash usada como "dado" na estrutura do bloco não minerado. */
  unsigned char flag = compSHA256(hashAnterior, bloco->hashAnterior);

  if (flag) {
    /* Se a hash de referência for igual, executa o algoritmo SHA256 sobre o bloco não minerado e compara com a hash que já foi minerada. */
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)bloco, sizeof(BlocoNaoMinerado), hash);
    flag = compSHA256(hashMinerada, hash);
  }

  return flag;
}

BlocoNaoMinerado novoBloco(int numero, unsigned char *hashAnterior, MTRand *gerador) {
  BlocoNaoMinerado bloco;
  bloco.numero = numero < 1 ? 1 : numero;
  bloco.nonce = 0;

  /* Inicialização o vetor de dados com as transações */
  for (unsigned int i = 0; i < sizeof(bloco.data); i++) {
    bloco.data[i] = 0;
  }
  
  if (hashAnterior != NULL) {
    for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
      bloco.hashAnterior[i] = hashAnterior[i];
    }
  } else {
    for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
      bloco.hashAnterior[i] = 0;
    }
  }

  /* Simula um número randômico de transações contidas no bloco */
  unsigned char transacoes = randChar(gerador, 1, 61);

  printf("Bloco criado, número de transações geradas: %d\n", transacoes);

  /* Iteração para gerar os dados individuais de cada transação */
  for (int t = 0; t < transacoes; t++) {
    /* Calcula o próximo indice no vetor de dados */
    int i = t * 3;

    /* Simula os endereços e valor da transação */
    unsigned char addr1 = randChar(gerador, 0, 255);
    unsigned char addr2 = randChar(gerador, 0, 255);
    unsigned char valor = randChar(gerador, 1, 50);

    /* Caso o segundo endereço seja o mesmo que o primeiro, itera até encontrar um diferente */
    while (addr2 == addr1) {
      addr2 = randChar(gerador, 0, 255);
    }

    bloco.data[i] = addr1;
    bloco.data[i + 1] = addr2;
    bloco.data[i + 2] = valor;

    // printf("%d %d %d\n", bloco.data[i], bloco.data[i + 1], bloco.data[i + 2]);
  }

  return bloco;
}

BlocoMinerado * simularMineracao(BlocoMinerado *pb, MTRand *gerador) {
  unsigned int numAnterior = 0;
  unsigned char *hashAnterior = NULL;

  if (pb != NULL) {
    numAnterior = pb->bloco.numero + 1;
    hashAnterior = pb->hash;
  }

  BlocoMinerado *minerado = (BlocoMinerado *)malloc(sizeof(BlocoMinerado));

  if (minerado != NULL) {
    minerado->bloco = novoBloco(numAnterior, hashAnterior, gerador);
    
    printf("Iniciando mineração do bloco nº %d:", minerado->bloco.numero);
    printf("\n");

    SHA256((unsigned char *)&(minerado->bloco), sizeof(BlocoNaoMinerado), minerado->hash);
    while (minerado->hash[0] != 0 || minerado->hash[1] != 0) {
      // printf("Hash inválida: ");
      // printSHA256(minerado->hash);
      // printf("\n");
      minerado->bloco.nonce += 1;
      SHA256((unsigned char *)&(minerado->bloco), sizeof(BlocoNaoMinerado), minerado->hash);
    }
    
    printf("Hash válida: ");
    printSHA256(minerado->hash);
    printf("\n");
    printf("Nonce: %d\n", minerado->bloco.nonce);
  }

  return minerado;
}

TNoBloco * ultimoBloco(TNoBloco *ini) {
  TNoBloco *ult = ini;

  if (ini != NULL) {
    while (ult->prox != NULL) {
      ult = ult->prox;
    }
  }

  return ult;
}

TNoBloco * inserirBloco(TNoBloco **ini, BlocoMinerado *pb) {
  TNoBloco *no = NULL;
  no = (TNoBloco *)malloc(sizeof(TNoBloco));

  if (no != NULL) {
    no->ref = pb;
    no->prox = NULL;

    /* Condicional para checar lista vazia e/ou buscar último nó da lista. */
    if (*ini == NULL) {
      unsigned char hash[SHA256_DIGEST_LENGTH];

      for (int i= 0; i < SHA256_DIGEST_LENGTH; i++) {
        hash[i] = 0;
      }

      if (!autenticarBloco(hash, pb->hash, &(pb->bloco))) {
        free(no);
        no = NULL;
      } else {
        *ini = no;
      }
    } else {
      TNoBloco *ult = ultimoBloco(*ini);

      if (!autenticarBloco(ult->ref->hash, pb->hash, &(pb->bloco))) {
        free(no);
        no = NULL;
      } else {
        ult->prox = no;
      }
    }
  }

  return no;
}