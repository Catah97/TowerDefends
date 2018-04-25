#ifndef DEVELOP_GAME_H
#define DEVELOP_GAME_H


#include <vector>
#include <typeinfo>
#include "Item.h"
#include "MapCreator.h"
#include "PathFindingAStar.h"
#include "BottomBar.h"
#include "MapExport.h"
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut_std.h>
#include <GL/freeglut_ext.h>
#endif

/**
 * Třida Game se stará ho průběh hry. Je manažer, který vykresluje mapu hry zpracovává pokyny od uživatele a přesouvá nepřátele v případě jejich pochybu.
 * Stará se o uvolňování alkovaných prvků. Třidu je třeba nejprve inicialozovat. Poté stačí z cyklu volat funkci
 * gameTick(), každé volání při každém volání se přepřepočítá pozice nepřátel a věže zautočí na nepřítele v případě, že vež nepřítele zabije je z mapy odstraněn.
 * Vždy po gameTick(), by se měla volat <drawScene>"()", která překreslí celou scenu.
 */
class Game : public BottomToolbarCommunicator {
private:
    Constants constants;
    BottomToolbar bottomToolbar;

    static const int s_minWidth = 400;
    static const int s_itemWidth = 15;
    static const int s_itemHeight = 20;


    static const unsigned int s_bottoBarHeight = 50;

    int m_mapWidth;
    int m_mapHeight;

    MapNode* startPathNode;
    MapItem* m_lastSelectedItem;
    bool m_isRunning;
    int m_lives;
    int m_selectedTower;
    int m_money;

    PathFindingAStar m_pathFindingAStar;
    FreePlace* m_startPoint = nullptr,* m_endPoint = nullptr;

    std::vector<std::vector<MapItem*> > m_map;
    std::vector<Tower*> m_defineTowers;
    std::vector<Enemy*> m_defineEnemies;

    std::vector<Enemy*> m_enemiesInMap;
    std::vector<Tower*> m_towersInMap;


    std::vector<Enemy*> m_enemiesQueue;

    /**
     * Inicializuje spodní bar s UI prvkami
     */
    void initBottomBar();

    /**
     * Získá X souřadnici itemu v mapě. Může vártít i souřadnici mimo mapu je třeba ověřit vně funkce
     * @param x souřadnice v okně
     * @return x souřadnici itemu v mapě
     */
    unsigned int getMapXPos(int x);

    /**
     * Získá Y souřadnici itemu v mapě. Může vrátit i souřadnici mimo mapu je třeba ověřit vně funkce
     * @param x souřadnice v okně
     * @return y souřadnici itemu v mapě
     */
    unsigned int getMapYPos(int y);

    /**
     * Získá Tower, která je uživatelem vybraná v BottomToolbar.
     * @return pointer na definici vybrané věže.
     */
    Tower* getSelectedTower();

    /**
     * Funce, která ověřuje jestli je v aktuální mapě možné dojit do cíle
     * @return Poku je možná dojít do cíle vráti TRUE, jinak FALSE.
     */
    bool checkPathAvailable();

    /**
     * Statická funkce, která porovnáva vzdálenosti nepřátel od cíle
     * @param e1 Nepřítel 1
     * @param e2 Nepřítel 2
     * @return Vrátí TRUE pokud Nepřítel 1 je blíž než Nepřítel 2, jinak false.
     */
    static bool compareEnemiesDistance(const Enemy* e1, const Enemy* e2);

    /**
     * Žádost o přídání nové veže
     * @param x pozice x v okně, na kterou cheme věž přřidate
     * @param y pozice y v okně, na kterou cheme věž přřidate
     * @return true pokud vež byla přidána jinak false(pokud by věž blokovala cestu nepřátelům)
     */
    bool addTower(int x, int y);

    /**
     * Saředí nepřátele podle vzdálenosti od cíle
     */
    void sortEnemiesByDistance();

    /**
     * Přepočítá cestu pro všechny nepřátele na mapě plus předpočítá novou cestu ze startu do cíle
     * @return true pokud lze nají cestu ze startu do cíle, jinak false
     */
    bool resetEnemyPath();

    /**
     * Přesune nepřátele na další pozici
     */
    void enemyMove();

    /**
     * Veže zautočí na nepřátele, každé vež vždy útočí na jednoho živého nepřítele.
     */
    void towersAttack();

    /**
     * Smaže z mapy mrtvé nepřátele
     */
    void clearDeadEnemy();

    /**
     * Prohodí vstupní parametry na mapě
     * @param oldPosition
     * @param newPosition
     */
    void swapMapPosition(const MapItem& oldPosition, const MapItem& newPosition);

    /**
     * Prohodí itemy v mapě na základě souřadnic
     * @param oldMapX X souřadnice prvního itemu
     * @param oldMapY Y souřadnice prvního itemu
     * @param newMapX X souřadnice druhého itemu
     * @param newMapY Y souřadnice druhého itemu
     */
    void swapMapPosition(int oldMapX, int oldMapY, int newMapX, int newMapY);

    /**
     * Přidá nepřítele z fronty do mapy, pokud se ve frontě žádný nepřítel nenechází nic nedělá.
     */
    void addFromQueue();

    /**
     * Vypočítá cestu z m_startPoint do m_endPoint
     * @return TRUE pokud cesta existuje, jinak FALSE
     */
    bool calStartPathNode();

    /**
     * Inizializuje hru ve smyslu ověření načtených položek
     * @return TRUE pokud bylo úspěšné jinak FALSE
     */
    bool initGame();

public:

    Game();
    ~Game();
    /**
     * Načte vstupní soubor a ověří načtené položky
     * @param path cesta k konfiguračnímu souboru
     * @return TRUE pokud načtení bylo úspěšné a hru lze spustit, jinak FALSE
     */
    bool initGame(const std::string &path);

    /**
     * Přesune nepřátele po mapě nechá věže zaútočit, odstraní mrtvé nepřátele a ověří jestli hra nemá skončit.
     */
    void gameTick();

    /**
     * Vykreslí všechny prvky ve hře
     */
    void drawScene();

    /**
     * Ověří jestli hra může pokračovat nebo již skončila
     * @return TRUE pokud hra skončila
     */
    bool checkGameEnd();

    /**
     * Srací pokud hra běží nebo je pozastavená
     * @return TRUE pokud běží, FALSE pokud je pozastavená
     */
    bool isRunning();

    /**
     * Zapne hru
     */
    void startGame();

    /**
     * Pozastaví hru
     */
    void pauseGame();

    /**
     * Uloží hru
     */
    void saveGame();

    /**
     * Veřejná funkce pro vykreslování polohy kursoru na mapě
     * @param x souřadnice kursoru v okně
     * @param y souřadnice kursoru v okně
     */
    void mouseMove(int x, int y);

    /**
     * Veřejná funce pro kliknutí myši
     * @param x souřadnice kursoru v okně
     * @param y souřadnice kursoru v okně
     * @return Vrací TRUE, pokud hra nějak reagovala na tlačítko FALSE
     */
    bool mouseClick(int x, int y);

    //Overided from BottomToolbarCommunicator
    virtual void onTowerSelected(int pos);
    virtual int getSelectedTowerPosition();
    virtual int getAvailableMoney();

};


#endif //DEVELOP_GAME_H
