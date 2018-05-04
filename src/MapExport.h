//
// Created by Martin Beran on 21.04.18.
//

#ifndef DEVELOP_MAPEXPORT_H
#define DEVELOP_MAPEXPORT_H


#include <fstream>
#include "Tools.h"
#include "Item.h"

/**
 * Manager to save game in progress to file
 * File name start with value in FILENAME. IF file exist add postfix (0) or (1) or (2) etc.
 * File structure you can find on \ref index "Main page"
 */
class MapExport {
private:
    Constants constants;

    /**
     * Save file preName
     */
    const char* FILENAME = "MapSave";

    bool m_findAsBestPath;
    int m_money;
    int m_lives;
    FreePlace* m_startPoint,* m_endPoint;
    std::vector<std::vector<MapItem*>> m_map;
    std::vector<Tower*> m_defineTowers;
    std::vector<Enemy*> m_defineEnemies;

    std::vector<Enemy*> m_enemiesQueue;
    std::vector<Enemy*> m_enemiesInMap;

    /**
     * Check if file exists
     * @param path path to file
     * @return TRUE if exists
     */
    bool fileExist(const std::string& path);

    /**
     * Generate path to file. Generated path is to file that not exists.
     * @return path to file
     */
    std::string generateFilePath();

    /**
     * Write values from m_defineTowers to file
     * @param mapFile stream to file
     */
    void writeTowerDefine(std::ofstream& mapFile);

    /**
     * Write value from m_defineEnemies to file
     * @param mapFile stream to file
     */
    void writeEnemyDefine(std::ofstream& mapFile);

    /**
     * Write value from m_enemiesQueue to file
     * @param mapFile stream to file
     */
    void writeEnemyQuque(std::ofstream& mapFile);

    /**
     * Write value from m_map to file
     * @param mapFile stream to file
     */
    void writeMap(std::ofstream& mapFile);

    /**
     * Write start and end point saved in m_startPoint and m_endPoint
     * @param mapFile stream to file
     */
    void writeStartEnd(std::ofstream& mapFile);

    /**
     * Write all enemies in map from m_enemiesInMap with their health
     * @param mapFile stream to file
     */
    void writeEnemiesInMap(std::ofstream &ofstream);

    /**
     * Get delimiter between attributes
     * @return delimiter
     */
    char getDelimiter();

public:

    /**
     * Set values for MapExport to save
     * @param findAsBestPath
     * @param money
     * @param lives
     * @param startPoint
     * @param endPoint
     * @param map
     * @param defineTowers
     * @param defineEnemies
     * @param enemiesQueue
     * @param enemiesInMap
     */
    MapExport(bool findAsBestPath, int money, int lives, FreePlace *startPoint, FreePlace *endPoint,
                  std::vector<std::vector<MapItem *>> &map, std::vector<Tower *> &defineTowers,
                  std::vector<Enemy *> &defineEnemies, std::vector<Enemy *> &enemiesQueue,
                  std::vector<Enemy *> &enemiesInMap);

    /**
     * Save game to file
     */
    void saveGame();
};


#endif //DEVELOP_MAPEXPORT_H
