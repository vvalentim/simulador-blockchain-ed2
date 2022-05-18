# Name of the project
PROJECT_NAME=blockchain

# Lista de arquivos .c
C_SOURCE=$(wildcard ./src/*/*.c)

# Lista de arquivos .h
H_SOURCE=$(wildcard ./src/*/*.h)

# Lista de arquivos objeto
OBJ=$(notdir $(subst .c,.o,$(C_SOURCE)))

# Compilador
CC=gcc

# Flags para o compilador
CC_FLAGS=-c -W -Wall -lcrypto

all: $(PROJECT_NAME)
# Regras para compilar e linkar arquivos
# Inicia-se com o nome o nome do projeto, listando todas as dependencias (objetos) necessários para o projeto
# '$@' representa o nome do arquivo do "alvo" da regra.
# '$^' lista o nome de todos os pré-requisitos com espaço entre eles.
# '$<' nome do primeiro pré-requisito.
# '@' Omite o output de uma linha de comando

$(PROJECT_NAME): $(OBJ) main.o
	$(CC) $(addprefix ./objects/,$^) -lcrypto -o $@
	@ echo ''
	@ echo 'Ligação e compilação finalizada.'
	@ echo ''

%.o: ./src/vendor/%.c ./src/vendor/%.h
	$(CC) $(CC_FLAGS) $< -o $(addprefix ./objects/,$@)

%.o: ./src/lib/%.c ./src/lib/%.h
	$(CC) $(CC_FLAGS) $< -o $(addprefix ./objects/,$@)

main.o: ./src/main.c $(H_SOURCE)
	$(CC) $(CC_FLAGS) $< -o $(addprefix ./objects/,$@)

clean:
	@ rm -rf ./objects/*.o $(PROJECT_NAME)

.PHONY: all clean