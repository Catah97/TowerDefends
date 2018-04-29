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


/*!\mainpage TowerDefends
 * \author   Martin Beran(beranm30)
 * \section description Description:
 * The program is to simulate a simple graphics tower defends game.
 * Retrieves game data from a file, then renders the game using the OpenGl library.
 * The game load file must be passed through input arguments when the program is started.
 * The player can pause game by pressing the 'SPACEBAR' key.
 * The game can also be saved with the 'S' button. The game is saved to a file located in the directory
 * with the game. The saved game file will be called MapSave.txt or MapSave(1).txt, MapSave(2).txt etc.
 * If a user wishes to load game then he must give the path to the saved game file as the argument at the start of the game.
 * If you want to exit the game, press the 'ESC' button.
 *
 *
 * \section config_file Structure of the configuration file
 * The file structure is the same for the new game as well for the saved game.
 * The file is divided into several sectors. Sector always starts with a fixed header.Lines under sector header are sector data.
 *
 *
 *
 * \section headers Headers of the
 * \subsection mandatory_headers Mandatory Headers: Towers, Enemies, Map, StartEnd
 * \li In the Towers section, the towers are defined in the following attributes: CHAR, PRICE, ATTACK, RANGE
 * \li Enemies section defines enemies: CHAR, MAX_HEALTH
 * \li Map is defined as a sequence of characters. The "#" characters are walls. The "i" character is the start for the enemy 'o' is the finish.
 * It is also possible to define the characters from the section defining towers and enemies.
 * The free field is ''. The map must always be square or rectangular.
 * \li StartEnd is used only if the end and start character is not defined on the map. The definition place in rows in the style: CHAR ('i' or 'o'), X, Y.
 * X and Y are coordinates of the start or finish in the map
 *
 *
 *
 * \subsection option_headers Optional Headers: Money, Lives, EnemiesInMap, Queue
 * \li Money section define the number of money in the game. The default value is 5000
 * \li Lives is a section where we define the player's lives. The default value is 5.
 * \li EnemiesInMap is used when saving a game after recording the lives of enemies. the first two numbers are the coordinates of the enemy, and the third is the number of health.
 * So: X, Y, HEALTH. This section must be defined after the Map section
 * Queue section is a string of characters that appear during the game on the starting position defined on the map by 'i' or in the section
 * StartEnd for an entry with an 'i' character.
 *
 *
 *
 * \section runtime Game progress:
 * Playing as real time. The objective is to prevent enemies from reaching the finish, or kill all enemies from the queue and not lose at all of their lives.
 * During the game, the player can build things to prevent the enemy from reaching the target. Towers can build in empty places that are not block by enemy.
 * Assuming that the player has enough money, he can build at any time. If the user wishes to save the game, press the 'S' key.
 * The game pauses and the file name of the game is displayed in the console.
 *
 * The game ends when the player lost are his lives or kill all enemies.
 * The window will close and a message appears in the console. In first case console write "Game over" in second "You have won:" {and number of lives that remain}
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

int main(int argc, char **argv) {
    if (argc != 2){
        cerr << "Bad input arguments" << endl;
        return 1;
    }
    std::string gameFileDefine = argv[1];
    if (!game.initGame(gameFileDefine)) {
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
