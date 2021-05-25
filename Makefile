SOURCE_DIRECTORY=src
HEADER_DIRECTORY=include

BUILD_DIRECTORY = build
BIN_DIRECTORY = $(BUILD_DIRECTORY)/bin
OBJECTS_DIRECTORY = $(BUILD_DIRECTORY)/objects

EXECUTABLE=projet

CC=gcc
CFLAGS=-I./$(HEADER_DIRECTORY) -std=c11 -Wall -pedantic
LDFLAGS=
LIBS=-lm

ifdef OPENSSL_MD5
CFLAGS += -DOPENSSL_MD5
LIBS += -lcrypto
else
CFLAGS += -DBSD_MD5
LIBS += -lbsd
endif

#manually select the files
#SOURCES=main.c polynom_fct.c

#Or add the source files automatically
SOURCES=$(notdir $(wildcard $(SOURCE_DIRECTORY)/*.c))
SOURCES_IGNORE=

#Filtering and creating the needed variables
SOURCES := $(filter-out $(SOURCES_IGNORE),$(SOURCES)) #:= is an "expansion assignement" where everything is exapnded before assignement so the reference is broken (here it is to prevent infinite recursion)
OBJECTS=$(SOURCES:%.c=$(OBJECTS_DIRECTORY)/%.o)

.PHONY: clean clean-all

all: $(BIN_DIRECTORY)/$(EXECUTABLE)

$(BIN_DIRECTORY)/$(EXECUTABLE): $(OBJECTS)
	[ -d $(BIN_DIRECTORY) ] || mkdir -p $(BIN_DIRECTORY)
	$(CC) $(LDFLAGS) -o $(BIN_DIRECTORY)/$(EXECUTABLE) $^ $(LIBS)

$(OBJECTS): $(OBJECTS_DIRECTORY)/%.o : $(SOURCE_DIRECTORY)/%.c #static pattern rule
	[ -d $(OBJECTS_DIRECTORY) ] || mkdir -p $(OBJECTS_DIRECTORY)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJECTS_DIRECTORY)

clean-all: clean
	rm -rf $(BIN_DIRECTORY)
