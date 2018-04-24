//
// Created by Martin Beran on 21.04.18.
//

#ifndef DEVELOP_MAPEXPORT_H
#define DEVELOP_MAPEXPORT_H


#include "Tools.h"
#include "Item.h"

class MapExport {
private:
    Constants constants;
    const char* FILENAME = "MapSave";

    FreePlace m_startPoint, m_endPoint;
    std::vector<std::vector<MapItem*>> m_map;
    std::vector<Tower*> m_defineTowers;
    std::vector<Enemy*> m_defineEnemies;

    std::vector<Enemy*> m_enemiesQueue;
    int m_money = 5000;

    bool fileExist(const std::string& path);
    std::string generateFilePath();
    void writeTowerDefine(std::ofstream& mapFile);
    void writeEnemyDefine(std::ofstream& mapFile);
    void writeEnemyQuque(std::ofstream& mapFile);
    void writeMap(std::ofstream& mapFile);
    char getDelmiter();

public:

    MapExport(int money,
              FreePlace* startPoint,
              FreePlace* endPoint,
              std::vector<std::vector<MapItem*>> &map,
              std::vector<Tower*>& defineTowers,
              std::vector<Enemy*>& defineEnemies,
              std::vector<Enemy*>& enemiesQueue);
    void saveGame();
};


#endif //DEVELOP_MAPEXPORT_H