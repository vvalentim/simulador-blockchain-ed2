# Name of the project
PROJECT_NAME=blockchain

# List of files .c
C_SOURCE=$(wildcard ./src/*/*.c)

# List of files .h
H_SOURCE=$(wildcard ./src/*/*.h)

# List of object files
OBJ=$(notdir $(subst .c,.o,$(C_SOURCE)))

# Compiler
CC=gcc

# Flags for the compiler
CC_FLAGS=-c -pedantic -W -Wall -lcrypto

all: $(PROJECT_NAME)
# Rules to link and compile
# Start with the name of the project, listing each dependency for it
# '$@' represents the target of the rule.
# '$^' lists all prerequisites with a spacing character between it.
# '$<' name of the first prerequisite.

$(PROJECT_NAME): $(OBJ) main.o
	$(CC) $(addprefix ./objects/,$^) -lcrypto -pthread -o $@
	@ echo ''
	@ echo 'Linking and compiling done.'
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