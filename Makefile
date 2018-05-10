CC := g++
CCFLAGS := -Wall -pedantic -Wno-long-long -O0 -ggdb -g -std=c++11

APP_NAME := TowerDefends
SRC_DIR := src
OBJ_DIR := obj
DOC_DIR := doc
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
EXAMPLE_DEF_FILE = "examples/map.txt"

TowerDefendsDoxyfile := src/TowerDefendsDoxyfile

OS := $(shell uname)

MAC_LIBS := -framework GLUT -framework OpenGL -D __OPENGL__
NORMAL_LIBS := -lGL -lglut -D __OPENGL__


all: compile doc

compile: program

run:
#@echo Run
	./$(APP_NAME) $(EXAMPLE_DEF_FILE)

start: clean compile run

doc: clean_doc
	@echo Creating doc
	doxygen $(TowerDefendsDoxyfile)

clean: clean_prog clean_obj clean_doc

clean_prog:
	@echo Removing $(APP_NAME)
	@rm -rf $(APP_NAME)

clean_obj:
	@echo Removing $(OBJ_DIR)
	@rm -rf $(OBJ_DIR)

clean_doc:
	@echo Removing $(DOC_DIR)
	@rm -rf $(DOC_DIR)

compile_dir:
	@mkdir -p $(OBJ_DIR)

program: compile_dir $(OBJ_FILES)
ifeq ($(OS), Darwin)
	$(CC) $(CCFLAGS) $(OBJ_FILES) $(MAC_LIBS) -o ./$(APP_NAME)
else
	$(CC) $(CCFLAGS) $(OBJ_FILES) $(NORMAL_LIBS) -o ./$(APP_NAME)
endif

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
ifeq ($(OS), Darwin)
	$(CC) $(CCFLAGS) $(MAC_LIBS) -c $< -o $@
else
	$(CC) $(CCFLAGS) $(NORMAL_LIBS) -c $< -o $@
endif


BottomBar.o: src/BottomBar.cpp src/BottomBar.h src/Item.h
Game.o: src/Game.cpp src/Game.h src/Item.h src/MapCreator.h src/Tools.h \
  src/PathFindingAStar.h src/BottomBar.h src/MapExport.h
Item.o: src/Item.cpp src/Item.h
MapCreator.o: src/MapCreator.cpp src/MapCreator.h src/Item.h src/Tools.h
MapExport.o: src/MapExport.cpp src/MapExport.h src/Tools.h src/Item.h
PathFindingAStar.o: src/PathFindingAStar.cpp src/PathFindingAStar.h \
  src/Item.h
Tools.o: src/Tools.cpp src/Tools.h
main.o: src/main.cpp src/Item.h src/MapCreator.h src/Tools.h src/Game.h \
  src/PathFindingAStar.h src/BottomBar.h src/MapExport.h
