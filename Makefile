BUILD_DIR = build
SOURCES = src/main.c
TARGET = projet

BIN_DIR = $(BUILD_DIR)
OBJ_DIR = $(BUILD_DIR)/objects
# http://www.gnu.org/software/make/manual/make.html#Substitution-Refs
OBJECTS = $(SOURCES:src/%.c=$(OBJ_DIR)/%.o)
LDFLAGS = -lm

# Base targets

.PHONY: build clean

build: $(BIN_DIR)/projet

clean:
	rm -rf $(BUILD_DIR)

$(BIN_DIR)/projet: $(OBJ_DIR)/main.o
	@[ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

# '%' (the stem) here refers to the .c's extensionless basename
$(OBJECTS): $(OBJ_DIR)/%.o: src/%.c
	@# Create the output directory if it is not present
	@[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)

	@# Build the files
	$(CC) $(CFLAGS) -c -o $@ $<
