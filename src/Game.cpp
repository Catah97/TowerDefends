//
// Created by Martin Beran on 13.04.18.
//

#include <iostream>
#include "Game.h"

Game::Game() = default;


void Game::initGame(const MapCreator& mapCreator) {
    m_map = mapCreator.map;
    enemiesQueue = mapCreator.enemiesQueue;

    //Create deep copy of StartPoint and EndPoint
    startPoint = new FreePlace(*mapCreator.startPoint);
    endPoint = new FreePlace(*mapCreator.endPoint);

    size_t width = m_map.at(0).size();
    size_t height = m_map.size();
    m_WindowWidth = static_cast<int>(width * itemWidth);
    m_WindowHeight = static_cast<int>(height * itemHeight);

    int x_position, y_position;
    for (size_t y = 0; y < m_map.size(); ++y) {
        for (size_t x = 0; x < m_map[y].size(); ++x) {
            auto mapItem = m_map[y][x];
            x_position = static_cast<int>(x);
            y_position = static_cast<int>(y);
            mapItem->setPosition( x_position, y_position, itemWidth, itemHeight);
        }
    }
}

void Game::drawMap() {
    for (auto &y : m_map) {
        for (auto mapItem : y) {
            mapItem->draw();
        }
    }
}

void Game::gameTick() {
    std::cout << "--------------GameTick---------------------" << std::endl;
    enemyMove();
    addFromQueue();

}

void Game::addFromQueue() {
    if (!enemiesQueue.empty()){
        int startX = startPoint->m_mapPositionX;
        int startY = startPoint->m_mapPositionY;
        MapItem* mapItem = m_map[startY][startX];
        if ( typeid(*mapItem) != typeid(Enemy)){
            auto newEnemy = enemiesQueue.begin();
            (*newEnemy)->setPosition(startX, startY, itemWidth, itemHeight);
            m_map[startY][startX] = *newEnemy;
            enemiesQueue.erase(newEnemy);
        }
    }
}

void Game::enemyMove() {
    PathFindingAStar pathFindingAStar(m_map, endPoint);
    std::vector<MapItem*> enemiesInMap;
    std::vector<MapNode*> nextEnemyPaths;
    findEnemies(enemiesInMap);
    pathFindingAStar.findBestPath(enemiesInMap, nextEnemyPaths);
    //std::cout << "----------------Enemy move: " << enemiesInMap.size() << "-----------------------------" << std::endl;
    for (size_t i = 0; i < enemiesInMap.size(); i++) {
        MapItem* enemy = enemiesInMap[i];
        MapNode* nextEnemyPosition = nextEnemyPaths[i];

        auto nextPositionItem = m_map[nextEnemyPosition->m_y][nextEnemyPosition->m_x];
        if (typeid(*nextPositionItem) != typeid(Enemy)) {
            swapMapPosition(enemy->m_mapPositionX,
                            enemy->m_mapPositionY,
                            nextPositionItem->m_mapPositionX,
                            nextPositionItem->m_mapPositionY);
        } else{
            //Move with enemy to end of list if he is block by another one enemy.
            enemiesInMap.push_back(enemy);
            nextEnemyPaths.push_back(nextEnemyPosition);
        }
    }
}

void Game::swapMapPosition(const MapItem &oldPosition, const MapItem &newPosition) {
    swapMapPosition(oldPosition.m_mapPositionX, oldPosition.m_mapPositionY, newPosition.m_mapPositionX, newPosition.m_mapPositionY);
}

void Game::swapMapPosition(int oldMapX, int oldMapY, int newMapX, int newMapY) {
    MapItem* item = m_map[oldMapY][oldMapX];
    m_map[oldMapY][oldMapX] = m_map[newMapY][newMapX];
    m_map[oldMapY][oldMapX]->setPosition(oldMapX, oldMapY);
    item->setPosition(newMapX, newMapY);
    m_map[newMapY][newMapX] = item;
}

void Game::findEnemies(std::vector<MapItem *>& result) {
    for (auto mapRow : m_map){
        for (auto mapItem : mapRow){
            if (typeid(*mapItem) == typeid(Enemy)){
                result.push_back(mapItem);
            }
        }
    }
}
