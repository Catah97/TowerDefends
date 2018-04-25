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
 * \section description Popis:
 * Program má simulovat jednoduchou grafickou tower defends hru.
 * Načte si data o hře ze souboru, hru následně vykreslí pomocí knihovny OpenGl.
 * Soubor pro načtení hry je třeba předat formou vstupních argumentů při spuštění programu.
 * Hráč si hru může pozastavovat dle libosti stisknutím klávesy 'MEZERNÍK'.
 * Pokud hráč chce může ve hře krokovat a to levou šipkou. Hru také lze uloži klávesou 'S'. Hra se uloží do souboru, který se nachází ve složce
 * s hrou. Soubor s uloženou hrou se bude jmenovat MapSave.txt popřípadě MapSave(1).txt respektive MapSave(2).txt, záleží jestli jíž dený soubor existuje.
 * Pokud uživatel chce načíst rozehranou hru, potom musí dát cestou k souboru s uloženou hrou jako argument při spuštění hry.
 * Pokud si hráč přeje ukončit hru uděláto klávesou 'ESC'.
 *
 * \section config_file Struktura konfiguračního souboru
 * Stuktura souboru je stejná, jak pro novou hru tak pro uloženou hru.
 * Soubor je členěn do několika sektorů. Sektro vždycky začíná pevně danou hlavičkou. A poté na řádkách pod sektorem defunejeme data sektoru.
 *
 * \subsection headers Povinné hlavičky: Towers, Enemies, Map, StartEnd
 * \li V sekci Towers se po řádkách definují věže v pořadí: ZNAK,CENA,UTOK,DOSAH
 * \li V sekci Enemies se po řádkách definují nepřátelé: ZNAK,MAX_ZIVOTY
 * \li Map se definuje jako posloupnost znaků. Znaky '#' vyjadřují neprůchodné zdi. Znak 'i' je start pro nepřátele 'o' je cíl.
 * Dále je zde možne definovat znaky ze sekci, které definují věže a nepřátele Je tedy nezbytné sekci mapa definovat až po Towers a Enemies.
 * Volné pole je ' '. Mapa vždy musí být čtvercová nebo obdelníková.
 * \li StartEnd se používa jenom v případě, že na mapě neni definován znak pro konec a začátek. Definice probíhá po řádkách stylem: ZNAK('i' nebo 'o'),X,Y.
 * X a Y označují startu respektive cíle.
 *
 * \subsection option_headers Nepovinné hlavičky: Money, Lives, EnemiesInMap, Queue
 * \li Money sekce ve které definujeme počet peněz, který bude moci hrát utratit za věže. Defaultní hodnota je 5000
 * \li Lives sekce, kde definujeme životy hráče. Defaultní hodnota je 5.
 * \li EnemiesInMap používá se v případě uložení hry po zaznamenání životů nepřátel. první dvě čísla jsou souřadnice nepřátele a třetí je počet životů.
 * Tedy: X,Y,ZIVOTY. Tato sekce musí být definována až po sekci Map
 * \li Queue je řetezec znaků nepřátel, které se v průběhu hry budou objevovat na startovní pozici definované buď na mapě pomocí 'i' a nebo v sekci
 * StartEnd u položky se znakem 'i'.
 *
 * \section runtime Průběh hry:
 * Hra probíhá jako realtime. Cílem je zabránit nepřátelům dostat se do cíle, respektive zabít všechny nepřátele z fronty a neztratit u toho všechny životy.
 * V průběhu hry si hráč může stavět věce, aby nepřátelům zabránil dojít do cíle. Věže může stavět na prázdné místa, které nejsou obsazená.
 * Za předpokladu, že hráč má dostatek financí, tak může veže stavět v jakýkoliv čas. Pokud si uživatel přeje hru uložit stiskne klávesu 'S'.
 * Hra se pozastaví a v konzoly se objeví název souboru kam se hra uložila.
 *
 * Hra končí v okamži, kdy hráč zabije všechny nepřátele a nebo přijde o všechny životy. V případě prohry (hráč přišel o všechny životy)
 * se okno ukončí a v konzoly se zobrazí hlaška "Game over". V případě výhry (hráč zabil všechny nepřátele) se okno ukončí
 * a v konzoli se zobrazí hláška "You won with" a počet životů, které hráči zbyly.
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
