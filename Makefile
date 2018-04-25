CC := g++
CCFLAGS := -Wall -pedantic -Wno-long-long -O0 -ggdb -g -std=c++11


NAME := "TowerDefends"
LOGIN_NAME := beranm30
SRC_DIR := src
OBJ_DIR := obj
DOC_DIR := doc
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
EXAMPLE_DEF_FILE = "examples/map.txt"

TowerDefendsDoxyfile := src/TowerDefendsDoxyfile

OS := $(shell uname)

OPENGL_ENABLE := 1
MAC_LIBS := -framework GLUT -framework OpenGL -D __OPENGL__
NORMAL_LIBS := -lGL -lglut -D __OPENGL__


all: compile doc

compile:
	@echo Compile
	@echo $(CCFLAGS)
	@mkdir -p $(OBJ_DIR)
	@make linking

run:
#@echo Run
	./$(LOGIN_NAME)/$(NAME) $(EXAMPLE_DEF_FILE)

start: clean compile run

doc: clean_doc
	@echo Creating doc
	doxygen $(TowerDefendsDoxyfile)

clean: clean_prog clean_obj clean_doc

clean_prog:
	@echo Removing $(LOGIN_NAME)
	@rm -rf $(LOGIN_NAME)

clean_obj:
	@echo Removing $(OBJ_DIR)
	@rm -rf $(OBJ_DIR)

clean_doc:
	@echo Removing $(DOC_DIR)
	@rm -rf $(DOC_DIR)

linking: $(OBJ_FILES)
	@mkdir -p $(LOGIN_NAME)
ifeq ($(OPENGL_ENABLE), 0)
	$(CC) $(CCFLAGS) $(OBJ_FILES) -o $(LOGIN_NAME)/$(NAME)
else ifeq ($(OS), Darwin)
	$(CC) $(CCFLAGS) $(OBJ_FILES) $(MAC_LIBS) -o $(LOGIN_NAME)/$(NAME)
else
	$(CC) $(CCFLAGS) $(OBJ_FILES) $(NORMAL_LIBS) -o $(LOGIN_NAME)/$(NAME)
endif

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
ifeq ($(OPENGL_ENABLE), 0)
	$(CC) $(CCFLAGS) -c $< -o $@
else ifeq ($(OS), Darwin)
	$(CC) $(CCFLAGS) $(MAC_LIBS) -c $< -o $@
else
	$(CC) $(CCFLAGS) $(NORMAL_LIBS) -c $< -o $@
endif
