//
// Created by Martin Beran on 21.04.18.
//

#include <fstream>
#include "MapExport.h"

MapExport::MapExport(int money,
                     FreePlace* startPoint,
                     FreePlace* endPoint,
                     std::vector<std::vector<MapItem *>> &map,
                     std::vector<Tower *> &defineTowers,
                     std::vector<Enemy *> &defineEnemies,
                     std::vector<Enemy *> &enemiesQueue) : m_money(money),
                                                           m_startPoint(*startPoint),
                                                           m_endPoint(*endPoint),
                                                           m_map(map),
                                                           m_defineTowers(defineTowers),
                                                           m_defineEnemies(defineEnemies),
                                                           m_enemiesQueue(enemiesQueue) {

}

void MapExport::saveGame() {
    std::string path = generateFilePath();
    std::ofstream mapFile;
    mapFile.open (path, std::ios::out);
    mapFile << constants.MONEY_DEFINE << std::endl;
    mapFile << std::to_string(m_money) << std::endl;
    mapFile << std::endl;

    mapFile << constants.TOWER_DEFINE << std::endl;
    writeTowerDefine(mapFile);
    mapFile << constants.ENEMIES_DEFINE << std::endl;
    writeEnemyDefine(mapFile);
    mapFile << constants.QUEUE_DEFINE << std::endl;
    writeEnemyQuque(mapFile);
    mapFile << constants.MAP_DEFINE << std::endl;
    writeMap(mapFile);
    if (mapFile.good()){
        std::cout << "Game saved to: " << path << std::endl;
    } else{
        std::cerr << "Game save error" << std::endl;
    }
    mapFile.close();
}

bool MapExport::fileExist(const std::string &path) {
    std::ifstream fileStream(path);
    return fileStream.good();
}

std::string MapExport::generateFilePath() {
    std::string fileNameComplete;
    int i = 0;
    while ( true ){
        fileNameComplete = FILENAME + std::to_string(i) + ".txt";
        if (!fileExist(fileNameComplete)){
            return fileNameComplete;
        }
        i++;
    }
}

void MapExport::writeTowerDefine(std::ofstream &mapFile) {
    for (auto tower : m_defineTowers){
        tower->writeToFile(mapFile, getDelmiter());
    }
    mapFile << std::endl;
}

void MapExport::writeEnemyDefine(std::ofstream &mapFile) {
    for (auto enemy : m_defineEnemies){
        enemy->writeToFile(mapFile, getDelmiter());
    }
    mapFile << std::endl;
}

void MapExport::writeEnemyQuque(std::ofstream &mapFile) {
    for (auto enemy : m_enemiesQueue){
        mapFile << enemy->m_mapItemChar;
    }
    mapFile << std::endl;
    mapFile << std::endl;
}

void MapExport::writeMap(std::ofstream &mapFile) {
    for (auto mapRow : m_map){
        for (auto mapItem : mapRow){
            mapFile << mapItem->m_mapItemChar;
        }
        mapFile << std::endl;
    }
    mapFile << std::endl;
    mapFile << std::endl;
}

char MapExport::getDelmiter() {
    return ',';
}


