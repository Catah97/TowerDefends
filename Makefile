CC := g++
CCFLAGS := -Wall -pedantic -Wno-long-long -O0 -ggdb
NAME := "TowerDefends"
LOGIN_NAME := beranm30
SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))


all: compile doc

compile:
	@echo $(SRC_FILES)
	@echo $(OBJ_FILES)
	@echo Compile
	@mkdir -p $(OBJ_DIR)
	@make linking


run:
	@echo Run
	@./$(LOGIN_NAME)/$(NAME)

start: compile run

clean:
	@echo Cleaning
	@rm -rf $(LOGIN_NAME)
	@rm -rf $(OBJ_DIR)


doc:
	@echo Creating doc

linking: $(OBJ_FILES)
	@echo Linking File $(OBJ_FILES)
	@mkdir -p $(LOGIN_NAME)
	$(CC) $(CCFLAGS) $(OBJ_FILES) -o $(LOGIN_NAME)/$(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CCFLAGS) -c $< -o $@

