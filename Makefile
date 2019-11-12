C=g++
SRC:=$(wildcard src/*.cpp)
LIB_SRC:=$(wildcard src/engine*.cpp)
OBJ:=$(addprefix obj/,$(notdir $(SRC:.cpp=.o)))
LIB_OBJ:=$(addprefix obj/,$(notdir $(LIB_SRC:.cpp=.o)))
LDFLAGS=-lncurses -lm
CCFLAGS=-std=gnu++17 -Wall -Werror -Wextra -pedantic -m64 -c
DFLAGS=-DDEBUG -g
RFLAGS=-O3
OUT=asciiwave

BUILD_DIRS=bin obj

.PHONY: build_dirs clean test

# default adds compile flags
default: CCFLAGS += $(DFLAGS)
default: build_dirs compile
	@echo Debug Build
	@bin/$(OUT)

# release build, without debug flags and with optimizations
release: CCFLAGS += $(RFLAGS)
release: build_dirs compile
	@echo Release Build

lib: CCFLAGS += $(RFLAGS) -fpic
lib: clean-all $(LIB_OBJ)
	$(CC) $(LDFLAGS) -shared -o bin/libcarbon.so $(LIB_OBJ)

compile: $(OBJ)
	$(CC) $(LDFLAGS) -o bin/$(OUT) $^

obj/%.o: src/%.cpp
	@echo Building debug $<
	$(CC) $(CCFLAGS) -o $@ $<

test:
	bin/$(OUT)

clean:
	@echo Cleaning up
	rm -f obj/*.o readme.html

clean-all: clean
	rm -f bin/*

build_dirs:
	@mkdir -p ${BUILD_DIRS}
