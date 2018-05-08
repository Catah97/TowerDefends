CC = g++
CCFLAGS = -Wall -pedantic -Wno-long-long -O0 -ggdb -g -std=c++11


#NAME = "TowerDefends"
LOGIN_NAME = beranm30
APP_NAME = $(LOGIN_NAME)
SRC_DIR = src
OBJ_DIR = obj
DOC_DIR = doc
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
EXAMPLE_DEF_FILE = "examples/MapExample.txt"

TowerDefendsDoxyfile = src/TowerDefendsDoxyfile

OS = $(shell uname)

OPENGL_ENABLE = 1
MAC_LIBS = -framework GLUT -framework OpenGL -D __OPENGL__
NORMAL_LIBS = -lGL -lglut -D __OPENGL__

all: compile doc

compile: $(OBJ_DIR) $(OBJ_FILES)
ifeq ($(OPENGL_ENABLE), 0)
	$(CC) $(CCFLAGS) $(OBJ_FILES) -o ./$(APP_NAME)
else ifeq ($(OS), Darwin)
	$(CC) $(CCFLAGS) $(OBJ_FILES) $(MAC_LIBS) -o ./$(APP_NAME)
else
	$(CC) $(CCFLAGS) $(OBJ_FILES) $(NORMAL_LIBS) -o ./$(APP_NAME)
endif

run: compile
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

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/BottomBar.o: $(SRC_DIR)/BottomBar.cpp $(SRC_DIR)/BottomBar.h $(SRC_DIR)/Item.h
ifeq ($(OPENGL_ENABLE), 0)
	$(CC) $(CCFLAGS) -c $< -o $@
else ifeq ($(OS), Darwin)
	$(CC) $(CCFLAGS) $(MAC_LIBS) -c $< -o $@
else
	$(CC) $(CCFLAGS) $(NORMAL_LIBS) -c $< -o $@
endif

$(OBJ_DIR)/Game.o: $(SRC_DIR)/Game.cpp $(SRC_DIR)/Game.h $(SRC_DIR)/Item.h $(SRC_DIR)/MapCreator.h \
$(SRC_DIR)/Tools.h $(SRC_DIR)/PathFindingAStar.h $(SRC_DIR)/BottomBar.h $(SRC_DIR)/MapExport.h
ifeq ($(OPENGL_ENABLE), 0)
	$(CC) $(CCFLAGS) -c $< -o $@
else ifeq ($(OS), Darwin)
	$(CC) $(CCFLAGS) $(MAC_LIBS) -c $< -o $@
else
	$(CC) $(CCFLAGS) $(NORMAL_LIBS) -c $< -o $@
endif

$(OBJ_DIR)/Item.o: $(SRC_DIR)/Item.cpp $(SRC_DIR)/Item.h
ifeq ($(OPENGL_ENABLE), 0)
	$(CC) $(CCFLAGS) -c $< -o $@
else ifeq ($(OS), Darwin)
	$(CC) $(CCFLAGS) $(MAC_LIBS) -c $< -o $@
else
	$(CC) $(CCFLAGS) $(NORMAL_LIBS) -c $< -o $@
endif

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/Item.h $(SRC_DIR)/MapCreator.h $(SRC_DIR)/Tools.h \
$(SRC_DIR)/Game.h $(SRC_DIR)/PathFindingAStar.h $(SRC_DIR)/BottomBar.h $(SRC_DIR)/MapExport.h
ifeq ($(OPENGL_ENABLE), 0)
	$(CC) $(CCFLAGS) -c $< -o $@
else ifeq ($(OS), Darwin)
	$(CC) $(CCFLAGS) $(MAC_LIBS) -c $< -o $@
else
	$(CC) $(CCFLAGS) $(NORMAL_LIBS) -c $< -o $@
endif

$(OBJ_DIR)/MapCreator.o: $(SRC_DIR)/MapCreator.cpp $(SRC_DIR)/MapCreator.h $(SRC_DIR)/Item.h $(SRC_DIR)/Tools.h
ifeq ($(OPENGL_ENABLE), 0)
	$(CC) $(CCFLAGS) -c $< -o $@
else ifeq ($(OS), Darwin)
	$(CC) $(CCFLAGS) $(MAC_LIBS) -c $< -o $@
else
	$(CC) $(CCFLAGS) $(NORMAL_LIBS) -c $< -o $@
endif

$(OBJ_DIR)/MapExport.o: $(SRC_DIR)/MapExport.cpp $(SRC_DIR)/MapExport.h $(SRC_DIR)/Tools.h $(SRC_DIR)/Item.h
ifeq ($(OPENGL_ENABLE), 0)
	$(CC) $(CCFLAGS) -c $< -o $@
else ifeq ($(OS), Darwin)
	$(CC) $(CCFLAGS) $(MAC_LIBS) -c $< -o $@
else
	$(CC) $(CCFLAGS) $(NORMAL_LIBS) -c $< -o $@
endif

$(OBJ_DIR)/PathFindingAStar.o: $(SRC_DIR)/PathFindingAStar.cpp $(SRC_DIR)/PathFindingAStar.h $(SRC_DIR)/Item.h
ifeq ($(OPENGL_ENABLE), 0)
	$(CC) $(CCFLAGS) -c $< -o $@
else ifeq ($(OS), Darwin)
	$(CC) $(CCFLAGS) $(MAC_LIBS) -c $< -o $@
else
	$(CC) $(CCFLAGS) $(NORMAL_LIBS) -c $< -o $@
endif

$(OBJ_DIR)/Tools.o: $(SRC_DIR)/Tools.cpp $(SRC_DIR)/Tools.h
ifeq ($(OPENGL_ENABLE), 0)
	$(CC) $(CCFLAGS) -c $< -o $@
else ifeq ($(OS), Darwin)
	$(CC) $(CCFLAGS) $(MAC_LIBS) -c $< -o $@
else
	$(CC) $(CCFLAGS) $(NORMAL_LIBS) -c $< -o $@
endif

