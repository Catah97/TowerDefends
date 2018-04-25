//
// Created by Martin Beran on 21.04.18.
//

#ifndef DEVELOP_MAPEXPORT_H
#define DEVELOP_MAPEXPORT_H


#include <fstream>
#include "Tools.h"
#include "Item.h"

class MapExport {
private:
    Constants constants;
    const char* FILENAME = "MapSave";

    int m_money;
    int m_lives;
    FreePlace* m_startPoint,* m_endPoint;
    std::vector<std::vector<MapItem*>> m_map;
    std::vector<Tower*> m_defineTowers;
    std::vector<Enemy*> m_defineEnemies;

    std::vector<Enemy*> m_enemiesQueue;
    std::vector<Enemy*> m_enemiesInMap;


    bool fileExist(const std::string& path);
    std::string generateFilePath();
    void writeTowerDefine(std::ofstream& mapFile);
    void writeEnemyDefine(std::ofstream& mapFile);
    void writeEnemyQuque(std::ofstream& mapFile);
    void writeMap(std::ofstream& mapFile);
    void writeStartEnd(std::ofstream& mapFile);
    void writeEnemiesInMap(std::ofstream &ofstream);
    char getDelmiter();

public:

    MapExport(int money, int lives, FreePlace *startPoint, FreePlace *endPoint,
              std::vector<std::vector<MapItem *>> &map, std::vector<Tower *> &defineTowers,
              std::vector<Enemy *> &defineEnemies, std::vector<Enemy *> &enemiesQueue,
              std::vector<Enemy *> &enemiesInMap);
    void saveGame();
};


#endif //DEVELOP_MAPEXPORT_H
