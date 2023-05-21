APP_NAME = sheet
LIB_NAME = libsheet

CC = g++

CFLAGS = -Wall -Werror
CPPFLAGS = -I src
GTKFLAGS1 = `pkg-config gtk+-3.0 --cflags`
GTKFLAGS2 = `pkg-config gtk+-3.0 --libs`
SRC_EXT = cpp

BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src

APP_PATH = $(BIN_DIR)/$(APP_NAME)
LIB_PATH = $(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME)/$(LIB_NAME).a

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
	$(CC) $(GTKFLAGS1) $(CFLAGS) $(CPPFLAGS) -o $@ $^ $(GTKFLAGS2)

$(LIB_PATH): $(LIB_OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: %.cpp
	$(CC) $(GTKFLAGS1) -c $(CFLAGS) $(CFLAGS_TEST) $< -o $@ $(GTKFLAGS2)

.Shee: clean
clean:
	rm -f $(APP_PATH) $(TEST_PATH) $(LIB_PATH) 
	rm -rf $(DEPS) $(APP_OBJECTS) $(LIB_OBJECTS)
	
.Shee: run
run: $(APP_RUN)
	$(APP_RUN)

.Shee: start
start:
	make clean
	make
	make run