APP_NAME = sheet
LIB_NAME = libsheet

CC = g++

CFLAGS = -Wall -Werror
CPPFLAGS = -I src
PACKAGE = `pkg-config gtk+-3.0 --cflags`
LIBS = `pkg-config gtk+-3.0 --libs`

BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src

APP_PATH = $(BIN_DIR)/$(APP_NAME)
LIB_PATH = $(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME)/$(LIB_NAME).a

SRC_EXT = cpp
APP_RUN = $(BIN_DIR)/./$(APP_NAME)

APP_SOURCES = $(shell find $(SRC_DIR)/$(APP_NAME) -name '*.$(SRC_EXT)')
APP_OBJECTS = $(APP_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

LIB_SOURCES = $(shell find $(SRC_DIR)/$(LIB_NAME) -name '*.$(SRC_EXT)')
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

DEPS = $(APP_OBJECTS:.o=.d) $(LIB_OBJECTS:.o=.d)

.Shee: all
all: $(APP_PATH)

-include $(DEPS)

$(APP_PATH): $(APP_OBJECTS) $(LIB_PATH)
	$(CC) $(CFLAGS) $(PACKAGE) $(CPPFLAGS) -o $@ $^ $(LIBS)

$(LIB_PATH): $(LIB_OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) $(PACKAGE) -c $< -o $@

.Geom: clean
clean:
	rm -f $(APP_PATH) $(LIB_PATH) 
	rm -rf $(DEPS) $(APP_OBJECTS) $(LIB_OBJECTS)
	
.Geom: run
run: $(APP_RUN)
	$(APP_RUN)