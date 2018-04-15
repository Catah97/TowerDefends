#include <iostream>
#include "UiItems.h"
#include "MapCreator.h"
#include "Game.h"
#include <vector>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

int m_WindowWidth, m_WindowHeight;
Game game;



void refreshView(void) {
    cout << "refreshView" << endl;


    game.drawMap();

    glFlush();


    glutSwapBuffers();
}


void keyboardFunc(unsigned char input_key, int x, int y) {
    switch(tolower(input_key))
    {
        case 'q':
            exit(0);
        case 'l':
            cout << "Load" << endl;
            break;
        case 's':
            cout << "Save" << endl;
            break;

    };
    cout << input_key << endl;
}

void specialInput(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            game.addFromQueue();
            break;
        case GLUT_KEY_DOWN:
            break;
        case GLUT_KEY_LEFT:
            break;
        case GLUT_KEY_RIGHT:
            game.gameTick();
            break;
    }
    glutPostRedisplay();
}

void mouseClicks(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    }
}



void resize(int width, int height){
    glutReshapeWindow( m_WindowWidth, m_WindowHeight);
}

bool loadFileAndMap(MapCreator& mapCreator, int argc, char ***argv){
    if (argc != 2){
        cerr << "Bad input arguments" << endl;
        return false;
    }
    //argv[0] is current file
    char* mapDefinePath = (*argv)[1];
    if (!mapCreator.loadGameFile(mapDefinePath)){
        cerr << "Load map error" << endl;
        return false;
    }
    return true;
}


int main(int argc, char **argv) {
    MapCreator mapCreator;
    if (!loadFileAndMap(mapCreator, argc, &argv)){
        return 1;
    }
    game.initGame(mapCreator);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(m_WindowWidth, m_WindowHeight);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Hello world :D");
    glutDisplayFunc(refreshView);
    glutMouseFunc(mouseClicks);
    glutSpecialUpFunc(specialInput);
    glutKeyboardUpFunc(keyboardFunc);
    glutReshapeFunc(resize);
    glutMainLoop();
    return 0;
}
