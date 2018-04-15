#ifndef DEVELOP_MAPCREATOR_H
#define DEVELOP_MAPCREATOR_H

#include <iostream>
#include <vector>
#include <fstream>
#include "UiItems.h"
#include "Tools.h"

class MapCreator {
private:

    const char* TOWER_DEFINE = "Towers";
    const char* ENEMIES_DEFINE = "Enemies";
    const char* QUEUE_DEFINE = "Queue";
    const char* MAP_DEFINE = "Map";
    const char FREE_PLACE_CHAR = ' ';
    const char WALL_CHAR = '#';
    const char QUEUE_IN_CHAR = 'i';
    const char QUEUE_OUT_CHAR = 'o';



    std::vector<Tower*> defineTowers;
    std::vector<Enemy*> defineEnemies;


    bool loadTowers(const std::string& towerDefine);
    bool loadEnemies(const std::string& enemyDefine);
    bool loadQueue(const std::string& queue);
    bool loadMap(const std::string& mapLine);

    bool createMapItem(const char& itemChar, MapItem*& foundItem);

    bool parseString(const std::string& headLine, const std::string& arg);

    bool checkMainCharMatch(const std::string& item, const char& c);

    static bool compareTower(const Tower* t1, const Tower* t2);
    static bool compareEnemy(const Enemy* e1, const Enemy* e2);



public:
    FreePlace* startPoint = nullptr,* endPoint = nullptr;
    std::vector<std::vector<MapItem*>> map;
    std::vector<Enemy*> enemiesQueue;


    ~MapCreator();
    bool loadGameFile(const char *gameDefinePath);

};


#endif //DEVELOP_MAPCREATOR_H
