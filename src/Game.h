#ifndef DEVELOP_GAME_H
#define DEVELOP_GAME_H


#include <vector>
#include <typeinfo>
#include "Item.h"
#include "MapCreator.h"
#include "PathFindingAStar.h"
#include "BottomBar.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut_std.h>
#include <GL/freeglut_ext.h>
#endif

class Game : public BottomToolbarCommunicator {
private:
    BottomToolbar bottomToolbar;

    static const int s_minWidth = 400;
    static const int s_itemWidth = 15;
    static const int s_itemHeight = 20;


    static const unsigned int s_bottoBarHeight = 50;

    std::string m_rootPath;

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

    void initBottomBar();

    int getMapXPos(int x);
    int getMapYPos(int y);
    Tower* getSelectedTower();
    bool checkPathAvailable();
    static bool compareEnemiesDistance(const Enemy* e1, const Enemy* e2);
    bool addTower(int x, int y);
    void sortEnemiesByDistance();
    bool resetEnemyPath();
    void enemyMove();
    void towersAttack();
    void clearDeadEnemy();
    void removeEnemyFromMap(const Enemy& enemy);
    void swapMapPosition(const MapItem& oldPosition, const MapItem& newPosition);
    void swapMapPosition(int oldMapX, int oldMapY, int newMapX, int newMapY);
    void addFromQueue();
    bool calStartPathNode();

public:

    Game();
    ~Game();
    bool initGame(const std::string &rootPath, const MapCreator &mapCreator);
    void gameTick();
    void drawScene();
    bool checkGameEnd();
    bool isRunning();
    void startGame();
    void pauseGame();
    void saveGame();
    void mouseMove(int x, int y);
    bool mouseClick(int x, int y);

    //Overided from BottomToolbarCommunicator
    virtual void onTowerSelected(int pos);
    virtual int getSelectedTowerPosition();
    virtual int getAvailableMoney();

};


#endif //DEVELOP_GAME_H
