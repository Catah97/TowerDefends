CC := g++
CCFLAGS := -Wall -pedantic -Wno-long-long -O0 -ggdb -g -std=c++11
NAME := "TowerDefends"
LOGIN_NAME := beranm30
SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
EXAMPLE_DEF_FILE = "examples/map.txt"

OS := $(shell uname)
MAC_LIBS := -framework OpenGL -framework GLUT -Wno-deprecated-declarations
NORMAL_LIBS := -lGL -lglut


all: compile doc

compile:
#@echo Compile
	@mkdir -p $(OBJ_DIR)
	@make linking



run:
#@echo Run
	@./$(LOGIN_NAME)/$(NAME) $(EXAMPLE_DEF_FILE)

start: clean compile run

clean:
	@echo Removing $(LOGIN_NAME)
	@rm -rf $(LOGIN_NAME)
	@echo Removing $(OBJ_DIR)
	@rm -rf $(OBJ_DIR)


doc:
#@echo Creating doc

linking: $(OBJ_FILES)
	@mkdir -p $(LOGIN_NAME)
ifeq ($(OS), Darwin)
	$(CC) $(CCFLAGS) $(OBJ_FILES) $(MAC_LIBS) -o $(LOGIN_NAME)/$(NAME)
else
	$(CC) $(CCFLAGS) $(OBJ_FILES) $(NORMAL_LIBS) -o $(LOGIN_NAME)/$(NAME)
endif

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
ifeq ($(OS), Darwin)
	$(CC) $(CCFLAGS) $(MAC_LIBS) -c $< -o $@
else
	$(CC) $(CCFLAGS) $(NORMAL_LIBS) -c $< -o $@
endif
