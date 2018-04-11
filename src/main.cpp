#include <iostream>
#include "UiItems.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

int m_WindowWidth, m_WindowHeight;

void displayMe(void) {

    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < 5; ++i) {
        Button btn( i * 100, 400, 50, 200);
        btn.draw();
    }




    glEnd();
    glFlush();
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
            break;
        case GLUT_KEY_DOWN:
            break;
        case GLUT_KEY_LEFT:
            break;
        case GLUT_KEY_RIGHT:
            break;
    }
    cout << key << endl;
}

void mouseClicks(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    }
    cout << "MousClick "  << x  << ", " << y << endl;
}



void resize(int width, int height){
    m_WindowWidth = width;
    m_WindowHeight = height;
    displayMe();
}

int main(int argc, char **argv) {
    m_WindowWidth = 800;
    m_WindowHeight = 600;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(m_WindowWidth, m_WindowHeight);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Hello world :D");
    glutDisplayFunc(displayMe);
    glutMouseFunc(mouseClicks);
    glutSpecialUpFunc(specialInput);
    glutKeyboardUpFunc(keyboardFunc);
    glutReshapeFunc(resize);
    glutMainLoop();
    return 0;
}
