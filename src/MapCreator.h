#ifndef DEVELOP_MAPCREATOR_H
#define DEVELOP_MAPCREATOR_H

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <typeinfo>
#include "Item.h"
#include "Tools.h"

/**
 * Je třída, která se stará o načtení hry ze souboru
 */
class MapCreator {
private:
    Constants constants;

    bool getCharAttribute(std::stringstream &iss, char &result);
    bool getNextAttribute(std::stringstream &iss, int &result);
    bool getNextAttribute(std::stringstream &iss, int &result, char delimiter);
    bool loadMoney(const std::string &moneyDefine);
    bool loadLives(const std::string &livesDefine);
    bool loadStartEnd(const std::string &startOrEnd);
    bool loadTowers(const std::string &towerDefine);
    bool loadEnemies(const std::string& enemyDefine);
    bool loadEnemiesInMap(const std::string& enemyInMapDefine);
    bool loadQueue(const std::string& queue);
    bool loadMap(const std::string& mapLine);
    bool finalCheck();

    bool createMapItem(const char& itemChar, MapItem*& foundItem);

    bool parseString(const std::string &headLine, std::string &arg);

    bool checkMainCharMatch(const std::string& item, const char& c);
    bool createStartOrEnd(const MapItem& item);

    static bool compareTower(const Tower* t1, const Tower* t2);
    static bool compareEnemy(const Enemy* e1, const Enemy* e2);



public:
    FreePlace* m_startPoint = nullptr,* m_endPoint = nullptr;
    std::vector<std::vector<MapItem*>> m_map;
    std::vector<Tower*> m_defineTowers;
    std::vector<Enemy*> m_defineEnemies;

    std::vector<Enemy*> m_enemiesQueue;
    std::vector<Enemy*> m_enemiesInMap;
    std::vector<Tower*> m_towersInMap;
    int m_money = 5000;
    int m_lives = 5;

    bool loadGameFile(const std::string &gameDefinePath);

};


#endif //DEVELOP_MAPCREATOR_H
