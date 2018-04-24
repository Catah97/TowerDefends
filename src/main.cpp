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


/*! \mainpage TowerDefends
 *  \author   Martin Beran(beranm30)
 *  \section popis Popis
 *  Program má simulovat jednoduchou grafickou tower defends hru.
 *  Načte si data o hře ze souboru, hru následně vykreslí pomocí knihovny OpenGl.
 *  Soubor pro načtení hry je třeba předat formou vstupních argumentů při spuštění programu.
 *  Hráč si hru může pozastavovat dle libosti stisknutím klávesy MEZERNÍK.
 *  Pokud hráč chce může ve hře krokovat a to levou šipkou. Hru také lze uloži klávesou S. Hra se uloží do souboru, který se nachází ve složce
 *  s hrou. Soubor s uloženou hrou se bude jmenovat MapSave.txt popřípadě MapSave(1).txt respektive MapSave(2).txt, záleží jestli jíž dený soubor existuje.
 *  Pokud uživatel chce načíst rozehranou hru, potom musí dát cestou k souboru s uloženou hrou jako argument při spuštění hry.
 *  Pokud si hráč přeje ukončit hru uděláto klávesou ESC.
 *
 */

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
