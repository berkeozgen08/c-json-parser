CC=gcc
CFLAGS=-ansi -W -Wextra -Wall -Wno-unused-variable -Wno-unused-parameter
LFLAGS=

SRC=src
OBJ=obj
BIN=bin
INC=inc inc/json

DIRS:=$(OBJ) $(BIN)
SRCS:=$(wildcard $(SRC)/*.c)
OBJS:=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
OUTPUT:=$(BIN)/json-parser

ifeq ($(OS),Windows_NT)
  RM=del /Q
  FixPath=$(subst /,\,$1)
  OUTPUT:=$(OUTPUT).exe
else
  RM=rm -f
  FixPath=$1
endif

.PHONY: all
all: $(DIRS) $(OUTPUT)

$(OBJ)/%.o: $(SRC)/%.c $(INC)/*.h
	$(CC) -c -o $@ $(CFLAGS) $< $(patsubst %,-I%,$(INC))

$(OUTPUT): $(OBJS)
	$(CC) -o $@ $(LFLAGS) $^

$(DIRS): $@
	@mkdir $@

.PHONY: debug
debug: CFLAGS+=-g
debug: clean $(OUTPUT) $(COPY_TO)

.PHONY: clean
clean:
	$(RM) $(call FixPath,$(BIN)/* $(OBJ)/*)
