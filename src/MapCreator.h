#ifndef DEVELOP_MAPCREATOR_H
#define DEVELOP_MAPCREATOR_H

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <stdexcept>
#include "Item.h"
#include "Tools.h"

/**
 * Manager to load game from file. File structure you can find on \ref index "Main page"
 */
class MapCreator {
private:
    Constants constants;

    /**
     * Get char attribute from iss, and move with iss.
     * @param iss string with char
     * @param result attribute
     * @return TRUE if successful
     */
    bool getCharAttribute(std::stringstream &iss, char &result);

    /**
     * Get next int attribute to EOF. Return FALSE if cant parse string to int.
     * @param iss string with attribute
     * @param result attribute
     * @return TRUE if successful
     */
    bool getNextAttribute(std::stringstream &iss, int &result);

    /**
     * Get next int attribute from string to delimiter. Return FALSE if cant parse string to int.
     * @param iss string with attribute
     * @param result attribute
     * @param delimiter delimiter between attributes
     * @return TRUE if successful
     */
    bool getNextAttribute(std::stringstream &iss, int &result, char delimiter);

    /**
     * Load findAsBestPath value to m_findAsBestPath
     * @param bestPathDefine
     * @return TRUE if successful
     */
    bool loadBestPath(const std::string &bestPathDefine);

    /**
     * Load money to m_money
     * @param moneyDefine
     * @return TRUE if successful
     */
    bool loadMoney(const std::string &moneyDefine);

    /**
     * Load lives to m_lives
     * @param livesDefine
     * @return TRUE if successful
     */
    bool loadLives(const std::string &livesDefine);

    /**
     * Load start or end to m_startPoint or m_endPoint
     * @param startOrEnd
     * @return TRUE if successful
     */
    bool loadStartEnd(const std::string &startOrEnd);

    /**
     * Load tower definition and add it to m_defineTowers
     * @param towerDefine
     * @return TRUE if successful
     */
    bool loadTowers(const std::string &towerDefine);

    /**
     * Load enemy definition and add them to m_defineEnemies
     * @param enemyDefine
     * @return TRUE if successful
     */
    bool loadEnemies(const std::string& enemyDefine);

    /**
     * Load enemy from map and add them to m_enemiesInMap
     * @param enemyInMapDefine
     * @return TRUE if successful
     */
    bool loadEnemiesInMap(const std::string& enemyInMapDefine);

    /**
     * Load define enemy and add them to m_enemiesQueue
     * @param queue
     * @return TRUE if successful, FALSE if enemy is not define in m_defineEnemies
     */
    bool loadQueue(const std::string& queue);

    /**
     * Load map line and save it to m_map. Link for all tower in map save to m_towersInMap, for enemies m_enemiesInMap.
     * When found  'i' set m_startPoint. 'o' set m_endPoint
     * @param mapLine
     * @return TRUE if successful(all char are defined)
     */
    bool loadMap(const std::string& mapLine);

    /**
     * Final check for all attribute
     * @return TRUE if successful
     */
    bool finalCheck();

    /**
     * Cream map item from definition
     * @param itemChar map item char
     * @param foundItem
     * @return TRUE if is map item defined by char
     */
    bool createMapItem(const char& itemChar, MapItem*& foundItem);

    bool parseString(const std::string &headLine, std::string &arg);

    /**
     * Char if char is not contain 'i' or 'o' or '#'.
     * @param item Item(used to print error msg)
     * @param c checking char
     * @return TRUE if successful(c is not 'i' or 'o' or '#')
     */
    bool checkMainCharMatch(const std::string& item, const char& c);

    /**
     * Set m_startPoint or m_endPoint items
     * @param item
     * @return TRUE if successful
     */
    bool createStartOrEnd(const MapItem& item);

    /**
     * Compare tower by tower char
     * @param t1
     * @param t2
     * @return TRUE if t1 char is got less value then t2 char.
     */
    static bool compareTower(const Tower* t1, const Tower* t2);

    /**
     * Compare enemies by enemy char
     * @param e1
     * @param e2
     * @return TRUE if e1 char is got less value then e2 char.
     */
    static bool compareEnemy(const Enemy* e1, const Enemy* e2);



public:
    FreePlace* m_startPoint = nullptr,* m_endPoint = nullptr;
    std::vector<std::vector<MapItem*>> m_map;
    std::vector<Tower*> m_defineTowers;
    std::vector<Enemy*> m_defineEnemies;

    std::vector<Enemy*> m_enemiesQueue;
    std::vector<Enemy*> m_enemiesInMap;
    std::vector<Tower*> m_towersInMap;
    bool m_findAsBestPath = false;
    int m_money = 5000;
    int m_lives = 5;

    /**
     * Load values from file
     * @param gameDefinePath path to file
     * @return TRUE if successful, else FALSE
     */
    bool loadGameFile(const std::string &gameDefinePath);

};


#endif //DEVELOP_MAPCREATOR_H
