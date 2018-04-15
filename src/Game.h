#ifndef DEVELOP_GAME_H
#define DEVELOP_GAME_H


#include <vector>
#include <typeinfo>
#include "UiItems.h"
#include "MapCreator.h"
#include "PathFindingAStar.h"

class Game {
private:
    const unsigned int itemWidth = 15;
    const unsigned int itemHeight = 20;

    MapItem* startPoint = nullptr,* endPoint = nullptr;
    std::vector<std::vector<MapItem*>> m_map;
    std::vector<Enemy*> enemiesQueue;


public:

    Game();
    void initGame(const MapCreator& mapCreator);
    void gameTick();
    void enemyMove();
    void findEnemies(std::vector<MapItem*>& result);
    void swapMapPosition(const MapItem& oldPosition, const MapItem& newPosition);
    void swapMapPosition(int oldMapX, int oldMapY, int newMapX, int newMapY);
    void addFromQueue();
    void drawMap();
};


#endif //DEVELOP_GAME_H
