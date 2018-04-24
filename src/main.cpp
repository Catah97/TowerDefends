#include <iostream>
#include "Item.h"
#include "MapCreator.h"
#include "Game.h"
#include <vector>

#ifdef __OPENGL__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#endif //__OPENGL__

using namespace std;

int m_WindowWidth, m_WindowHeight;
Game game;

#ifdef __OPENGL__

void refreshView(void) {

    game.drawScene();

    glFlush();


    glutSwapBuffers();
}


void keyboardFunc(unsigned char input_key, int x, int y) {
    switch(tolower(input_key))
    {
        case 27: //Esc
            exit(0);
        case 32: //SpaceBar
            if (game.isRunning()){
                game.pauseGame();
            } else{
                game.startGame();
            }
            break;
        case 'l':
            break;
        case 's':
            game.saveGame();
            break;
        default:
            break;
    };
}

void specialInput(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            break;
        case GLUT_KEY_DOWN:
            break;
        case GLUT_KEY_LEFT:
            break;
        case GLUT_KEY_RIGHT:
            game.startGame();
            game.gameTick();
            game.pauseGame();
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void glutTimer(int value) {
    game.gameTick();
    glutPostRedisplay();
    glutTimerFunc(100, glutTimer, 1);
}

void mouseMove(int x, int y) {
    game.mouseMove(x, y);
    glutPostRedisplay();
}

void mouseClicks(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if (game.mouseClick(x, y)) {
            glutPostRedisplay();
        }
    }
}



void resize(int width, int height){
    glutReshapeWindow( m_WindowWidth, m_WindowHeight);
}

#endif //__OPENGL__

bool loadFileAndMap(MapCreator& mapCreator, int argc, char ***argv){
    if (argc != 2){
        cerr << "Bad input arguments" << endl;
        return false;
    }
    //argv[0] is current file
    char* mapDefinePath = (*argv)[1];
    if (!mapCreator.loadGameFile(mapDefinePath)){
        cerr << "Load m_map error" << endl;
        return false;
    }
    return true;
}


/**
 * Zahajovací funkce programu.
 * Program očekvá vstupní parametr cestu k souboru s hrou
 * @param argc Počet vstupních argumetnů
 * @param argv Vstupní argumenty
 * @return
 */
int main(int argc, char **argv) {
    MapCreator mapCreator;
    std::string rootPath = argv[0];
    if (!loadFileAndMap(mapCreator, argc, &argv) || !game.initGame(rootPath, mapCreator)) {
        return 1;
    }

#ifdef __OPENGL__
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(m_WindowWidth, m_WindowHeight);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Hello world :D");
    glutDisplayFunc(refreshView);
    glutMouseFunc(mouseClicks);
    glutPassiveMotionFunc(mouseMove);
    glutSpecialUpFunc(specialInput);
    glutKeyboardUpFunc(keyboardFunc);
    glutReshapeFunc(resize);
    glutTimerFunc(1, glutTimer, 1);
    glutMainLoop();
#else
    game.startGame();
    while (!game.checkGameEnd()){
        game.gameTick();
    }
#endif //__OPENGL__

    return 0;
}
