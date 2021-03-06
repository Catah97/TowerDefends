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
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut_std.h>
#include <GL/freeglut_ext.h>
#endif

/**
 * The class is a manager who draws a map, handles instructions from a user, and initializing enemies moves
 * First of all you have to initializing class. After initializing you should call gameTick(). Function make game run. Every call is one tick in game.
 * After gameTick() call you should call drawScene() to redraw scene.
 */
class Game : public BottomToolbarCommunicator, public PathFindingAStarCommunicator {
private:
    Constants constants;

    BottomToolbar bottomToolbar;
    PathFindingAStar m_pathFindingAStar;

    /**
     * Min view of window
     */
    static const int s_minWidth = 400;


    static const int s_itemWidth = 15;
    static const int s_itemHeight = 20;

    /**
     * Height of BottomToolbar
     */
    static const unsigned int s_bottoBarHeight = 50;


    int m_mapWidth;
    int m_mapHeight;

    /**
     * Start of path. This MapNode is added to all new enemies from the queue.
     */
    MapPath* m_startPathNode;

    /**
     * Map item last selected mouse cursor.
     */
    MapItem* m_lastSelectedItem;
    bool m_isRunning;
    bool m_findAsBestPath;
    int m_lives;
    int m_selectedTower;
    int m_money;

    /**
     * Start point on map for enemies
     */
    FreePlace* m_startPoint = nullptr;

    /**
     * Finish for all enemies
     */
    FreePlace* m_endPoint = nullptr;

    std::vector<std::vector<MapItem*> > m_map;
    std::vector<Tower*> m_defineTowers;
    std::vector<Enemy*> m_defineEnemies;

    std::vector<Enemy*> m_enemiesInMap;
    std::vector<Tower*> m_towersInMap;


    std::vector<Enemy*> m_enemiesQueue;

    /**
     * initializing BottomToolbar
     */
    void initBottomBar();

    /**
     * Get x position in map. But it can return bad position also. You should check result of this function
     * @param x X position in window
     * @return X position in map
     */
    unsigned int getMapXPos(int x);

    /**
     * Get y position in map. But it can return bad position also. You should check result of this function
     * @param y Y position in window
     * @return Y position in map
     */
    unsigned int getMapYPos(int y);

    /**
     * Get Tower that is selected in BottomToolbar
     * @return definition of the selected tower
     */
    Tower* getSelectedTower();

    /**
     * Check if is possible to find path from m_startPoint to m_endPoint
     * @return TRUE if is possible to found path, else FALSE
     */
    bool checkPathAvailable();

    /**
     * Static function that compare distance of enemies to m_endPoint
     * @param e1 Enemy 1
     * @param e2 Enemy 2
     * @return TRUE if Enemy 1 is closer to m_endPoint then Enemy 2, else FALSE
     */
    static bool compareEnemiesDistance(const Enemy* e1, const Enemy* e2);

    /**
     * Request to add new tower to map
     * @param x X position in window where we want to add tower
     * @param y Z position in window where we want to add tower
     * @return TRUE if tower was added, else FALSE
     */
    bool addTower(int x, int y);

    /**
     * Sort enemies by distance to end
     */
    void sortEnemiesByDistance();

    /**
     * Calculate path from start to finish.
     * Set path for all enemies on map.
     * @return TRUE if is possible to found path from START to FINISh, else FALSE
     */
    bool resetEnemyPath();

    /**
     * Detect if enemy has default path or not
     * @param enemy
     * @return TRUE enemy can use default path
     */
    bool isInDefaultPath(Enemy &enemy);

    /**
     * Move with all enemies on next position
     */
    void enemyMove();

    /**
     * Towers attack enemies. Each tower attack only on one enemy.
     */
    void towersAttack();

    /**
     * Delete dead enemies from map and replace them with FreePlaces
     */
    void clearDeadEnemy();

    /**
     * Add MapItem to by on position save in MapItem and free old MapItem.
     * @param mapItem MapItem that we want to add to map instead of MapItem on same position in map
     */
    void replaceMapItem(MapItem &mapItem);

    /**
     * Check if mapItem is m_lastSelectedItem
     * @param mapItem checked item
     * @return TRUE if it is, else FALSE
     */
    bool isLastSelectedItem(const MapItem &mapItem);

    /**
     * Check if position match with m_lastSelectedItem position
     * @param x
     * @param y
     * @return TRUE if match, else FALSE
     */
    bool isLastSelectedItem(int x, int y);

    /**
     * Set new selected Item
     * on old select set selected as false
     * @param mapItem new selected item
     */
    void setLastSelectedItem(MapItem &mapItem);

    void swapMapPosition(const MapItem& oldPosition, const MapItem& newPosition);
    void swapMapPosition(int oldMapX, int oldMapY, int newMapX, int newMapY);

    /**
     * Add Enemies defined in Queue to map, if is Queue empty do nothing
     */
    void addFromQueue();

    /**
     * Calculate path from m_startPoint to m_endPoint
     * @return TRUE if can found path, else FALSE
     */
    bool calStartPathNode();

    /**
     * Load all variables from MapCreator
     * @return TRUE if successful, else FALSE
     */
    bool initGame();

    /**
     * Free m_startPathNode
     */
    void freePath();

public:

    Game();
    ~Game();

    /**
     * Load input files, and check game data validation
     * @param path Path to game config file
     * @return TRUE if successful and game can start, else FALSE
     */
    bool initGame(const std::string &path);

    /**
     * Move enemies, do towers attacks, clear dead enemies and check game status
     */
    void gameTick();

    /**
     * Draw all item in game
     */
    void drawScene();

    /**
     * Check if game end or not
     * @return TRUE if game end
     */
    void checkGameEnd();
    bool isRunning();
    void startGame();
    void pauseGame();

    /**
     * Save current game status to file
     */
    void saveGame();

    /**
     * Draw mouse cursor items selection
     * @param x mouse X position in window
     * @param y mouse Y position in window
     */
    void mouseMove(int x, int y);

    /**
     * Detect mouse click in game
     * @param x mouse X position in window
     * @param y mouse Y position in window
     * @return TRUE if some item was pressed, else FALSE
     */
    bool mouseClick(int x, int y);

    //Overridden from BottomToolbarCommunicator
    virtual void onTowerSelected(int pos);
    virtual int getSelectedTowerPosition();
    virtual int getAvailableMoney();

    //Overridden from PathFindingAStarCommunicator
    virtual const std::vector<std::vector<MapItem*>>& getMap();
    virtual const std::vector<Tower*>& getTowersInMap();
    virtual const MapItem* getEndPoint();
    virtual bool getBestPathForEnemies();

};


#endif //DEVELOP_GAME_H
