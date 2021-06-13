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

#manually select the files
#SOURCES=

#Or add the source files automatically
SOURCES=$(notdir $(wildcard $(SOURCE_DIRECTORY)/*.c))
SOURCES_IGNORE=

#Filtering and creating the needed variables
SOURCES := $(filter-out $(SOURCES_IGNORE),$(SOURCES)) #:= is an "expansion assignement" where everything is exapnded before assignement so the reference is broken (here it is to prevent infinite recursion)
OBJECTS=$(SOURCES:%.c=$(OBJECTS_DIRECTORY)/%.o)

ifdef OPENSSL_MD5
CFLAGS += -DOPENSSL_MD5
LIBS += -lcrypto
else 
ifdef BSD_MD5
CFLAGS += -DBSD_MD5
LIBS += -lbsd
endif
endif

.PHONY: is_lib_specified build clean clean-all run run-verbose help

all: build

is_lib_specified:
ifdef OPENSSL_MD5
	@echo "Using OPENSSL_MD5"
else
ifdef BSD_MD5
	@echo "Using BSD_MD5"
else
	$(error No MD5 library specified : use either "make build OPENSSL_MD5=1" or "make build BSD_MD5=1" or to get help : "make help")
endif
endif

build: is_lib_specified $(BIN_DIRECTORY)/$(EXECUTABLE)

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
	rm -rfi $(BUILD_DIRECTORY)

run: build
	$(BIN_DIRECTORY)/$(EXECUTABLE) $(APP_ARG)

run-verbose: build
	$(BIN_DIRECTORY)/$(EXECUTABLE) $(APP_ARG) -v

help:
	@echo "To use this makefile, you have to specify the md5 library you want to use : either \"OPENSSL_MD5=1\" or \"BSD_MD5=1\". If both are specified, openssl is chosen"
	@echo "The default target is \"build\" and it will generate the executable in BUILD_DIRECTORY/bin and the object files in BUILD_DIRECTORY/objects."
	@echo "BUILD_DIRECTORY can be specified and if not is \"BUILD_DIRECTORY=./build\" by default"
	@echo "You can also remove the object files with the target \"clean\""
	@echo "Or alternatively remove the object files and the executable and optionnaly the build folder with the target \"clean-all\""
	@echo "You can run the application with the target \"run\" or \"run-verbose\" and give the arguments to APP_ARG"
	@echo "|-> By default, without arguments : the current directory is scanned, the MD5 sum is set to 0 and is not calculated and the results are saved in result_of_scan.txt in the current directory."
	@echo "Afin d'utiliser l'application, on peut donner les arguments suivants :"
	@echo "   - \"-i\" suivi du dossier à analyser, sinon le dossier actuel sera analysé."
	@echo "   - \"-s\" sans argument afin d'activer le calcul des sommes MD5, sinon la somme n'est pas calculé et la valeur sera 0."
	@echo "   - \"-o\" suivi d'un fichier dans lequel la sauvegarde de la liste de l'arborescence se fera, sinon la sauvegarde se fera dans \"./result_of_scan.txt\"."
	@echo "   - \"-v\" pour activer le mode verbose"