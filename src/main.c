#include "./lib/structs.h"

int showMenu(TAddress *addresses, unsigned int *nonceMax);
void initMining();
void printBlockHash(unsigned int n);
void printAddresses(TAddress *addresses);
void parseAll();

int main() {
  // parseAll();
  initMining();
  
  return 0;
}

int showMenuPT(TAddress *addresses, unsigned int *nonceMax) {
  printf("0. Encerrar programa\n");
  printf("1. Minerar blocos\n");
  printf("2. Buscar hash de um bloco\n");
  printf("3. Buscar quantidade de bitcoins de um endereco\n");
  printf("4. Buscar endereco com maior quantidade de bitcoins\n");
  printf("5. Listar enderecos com quantidade de bitcoins em ordem crescente\n");
  printf("6. Buscar bloco com maior valor nonce\n\n");
  printf("Informe o numero de acordo com a opcao desejada: ");

  int opt = -1;

  while (opt < 0) {
    opt = getIntInput("Opcao invalida: ");
  }

  switch (opt) {
    case 1: initMining(); break;
    
    case 2: {
      int n = 0;
      
      printf("Informe o número do bloco: ");

      while (n <= 0) {
        n = getIntInput("Número inválido: ");
      }

      printf("--------------------\n");
      printBlockHash(n);
      break;
    }

    case 3: {
      int n = 0;
      
      printf("Informe o número do endereço: ");

      while (n <= 0 || n > 255) {
        n = getIntInput("Número inválido: ");
      }

      printf("--------------------\n");
      printf("Total bitcoins: %d\n", (addresses + n)->coins);
      break;
    }

    case 4: {
      TAddress *sorted = sortedAddrByCoins(addresses);

      printf("--------------------\n");

      if (sorted) {
        printf("Os endereços que possuem a maior quantidade de bitcoin são: [ ");
      
        for (int i = 255; i > 0; i--) {
          if (i == 255) {
            printf("%d ", (sorted + i)->addr);
          } else if ((sorted + i)->coins == (sorted + i + 1)->coins) {
            printf("%d ", (sorted + i)->addr);
          } else {
            break;
          }
        }

        printf("]\n");

        free(sorted);
      } else {
        printf("Falha ao tentar ordenar o vetor de endereços.\n");
      }

      break;
    }

    case 5: {
      TAddress *sorted = sortedAddrByCoins(addresses);

      printf("--------------------\n");

      if (sorted) {
        printAddresses(sorted);

        free(sorted);
      } else {
        printf("Falha ao tentar ordenar o vetor de endereços.\n");
      }

      break;
    }

    case 6: {
      printf("--------------------\n");
      printf("O bloco %u foi o bloco com maior nonce: %u\n", nonceMax[1], nonceMax[0]);
      break;
    }
  }

  return opt;
}

void initMining() {
  MTRand rng = seedRand(1234567);
  TMinedBlock *previous = (TMinedBlock *)loadLastBin(sizeof(TMinedBlock), __FILE_BLOCKCHAIN);
  unsigned long *seq_rand = (unsigned long *)loadLastBin(sizeof(unsigned long), __FILE_RAND);

  if (seq_rand) {
    while (__SEQ_RAND < *seq_rand) {
      randChar(&rng, 0, 2);
    }
  }

  if (previous) {
    printf("Last block: %u %u ", previous->raw.num, previous->raw.nonce);
    printSHA256(previous->hash);
    printf("\n");
  }

  printf("---------------------\n");

  while (1) {
    mineNextBlock(&previous, &rng);
    printf("----------------------\n");
  }
}

void printAddresses(TAddress *addresses) {
  printf("Addresses [addr, coins, total transactions]\n");
  printf("------------------------------\n");

  if (addresses) {
    for (int i = 0; i < 256; i++) {
      printf("%d %d %d\n", (addresses + i)->addr, (addresses + i)->coins, (addresses + i)->numTransactions);
    }
  }

  printf("------------------------------\n");
}

void printBlockHash(unsigned int n) {
  TMinedBlock* block = searchBlock(n);

  if (block) {
    printf("%u %u ", block->raw.num, block->raw.nonce);
    printSHA256(block->hash);
    printf("\n");

    free(block);
  }
}

void parseAll() {
  FILE *fp = fopen(__FILE_BLOCKCHAIN, "rb");

  if (!fp) {
    printf("Failed to open blockchain file.\n");
    return;
  }

  unsigned int countElements = __BLOCK_SIZE / sizeof(TMinedBlock);
  TMinedBlock *blockBuffer = (TMinedBlock *)malloc(sizeof(TMinedBlock) * countElements);

  if (!blockBuffer) {
    printf("Failed to load and parse all blocks, couldn't allocate memory to blockBuffer.\n");
  }

  TAddress *addresses = initAddresses();

  if (!addresses) {
    printf("Failed to initialize addresses.\n");
    fclose(fp);
    free(blockBuffer);
    return;
  }
  
  /* Greatest block nonce and it's number in order */
  unsigned int nonceMax[2] = { 0, 0 };

  printf("Parsing all blocks...\n");
  int results = 0;

  /* Keep reading till it hits the end of the file */
  while (!feof(fp)) {
    /* Reads a block of the file into the buffer, returns the amount of elements which were read */
    results = fread(blockBuffer, sizeof(TMinedBlock), countElements, fp);

    if (results) {
      /* Parse the blocks of the blockchain */
      for (int i = 0; i < results; i++) {
        parseBlock(blockBuffer + i, addresses, nonceMax);
      }
    }
  }
  
  free(blockBuffer);
  fclose(fp);

  printf("Parsing blocks done.\n");

  /* Show menu before unload */
  int open = 1;

  while (open) {
    printf("--------------------\n");
    open = showMenuPT(addresses, nonceMax);
  }
  
  free(addresses);
}