//
// Created by Martin Beran on 14.04.18.
//

#include "PathFindingAStar.h"

//---------------------------PathFindingAStar--------------------------------

PathFindingAStar::PathFindingAStar(PathFindingAStarCommunicator* communicator) : m_communicator(communicator) {}


MapPath* PathFindingAStar::createMapNode(const MapItem &mapItem, MapPath* parentNode) {
    int x = mapItem.m_mapPositionX;
    int y = mapItem.m_mapPositionY;
    int startDistance = parentNode == nullptr ? 0 : parentNode->getStartDistance() + 10;
    auto endPoint = m_communicator->getEndPoint();
    int endDistance = abs( x - endPoint->m_mapPositionX) * 1 + abs( y - endPoint->m_mapPositionY) * 1;
    int towersInRange = m_communicator->getBestPathForEnemies() ? getTowerInRanger(x, y) : 0;
    auto mapNode = new MapPath(x, y, startDistance, endDistance, towersInRange, parentNode);
    return mapNode;
}


bool PathFindingAStar::findBestPath(const MapItem &startPoint, MapPath*& result) {
    auto endPoint = m_communicator->getEndPoint();
    auto map = m_communicator->getMap();
    MapPath* startNode = createMapNode(startPoint);
    std::vector<MapPath*> availableNodes, closedNodes;
    availableNodes.push_back(startNode);
    std::vector<MapPath*> neighbours;
    int count = 0;
    while (true){
        count++;
        if (availableNodes.empty()){
            //No Path found
            freeLists(availableNodes);
            freeLists(closedNodes);
            return false;
        }
        std::vector<MapPath*>::const_iterator position;
        MapPath* currentNode = findBestNode(availableNodes, position);
        closedNodes.push_back(currentNode);
        availableNodes.erase(position);
        if (*currentNode == *endPoint){
            createPath(currentNode, result);
            freeLists(availableNodes);
            freeLists(closedNodes);
            std::cout << "Cycles: " << count << std::endl;
            return true;
        }
        neighbours.clear();
        getAvailableNeighbourNodes(map, *currentNode, neighbours);
        for (auto neighbour : neighbours){

            if (isInList(*neighbour, closedNodes)){
                delete neighbour;
                continue;
            } else if (!isInList(*neighbour, availableNodes)){
                availableNodes.push_back(neighbour);
            } else{
                delete neighbour;
            }
        }
    }
}

void PathFindingAStar::createPath(MapPath* endPosition, MapPath*& newStartNode) {
    MapPath* currentNode = endPosition;
    MapPath* lastNode = nullptr;
    while ( currentNode ){
        auto newNode = new MapPath(*currentNode);
        newNode->m_next = lastNode;
        lastNode = newNode;
        currentNode = currentNode->m_previous;
    }
    newStartNode = lastNode;
}

MapPath* PathFindingAStar::findBestNode(const std::vector<MapPath*>& availableNodes,
                                        std::vector<MapPath*>::const_iterator& position) {
    MapPath* result = *availableNodes.begin();
    position = availableNodes.begin();

    //std::cout << "----------findBestNode-------------" << std::endl;
    //printListOfNotes(availableNodes);

    for (auto it = availableNodes.begin(); it < availableNodes.end(); ++it) {
        MapPath* node = *it;
        if (result->getMetric() > node->getMetric()){
            result = node;
            position = it;
        } else if (result->getMetric() == node->getMetric() && result->getEndDistance() > node->getEndDistance()){
            result = node;
            position = it;
        }
    }

    //std::cout << "Final result " << result->m_x << ", " << result->m_y << std::endl;
    return result;
}


void PathFindingAStar::getAvailableNeighbourNodes(const std::vector<std::vector<MapItem *>>& map,
                                                  MapPath &item,
                                                  std::vector<MapPath *> &result) {
    int x = item.m_x;
    int y = item.m_y;
    getAvailableNeighbourNodes(map, item, x - 1, y, result);
    getAvailableNeighbourNodes(map, item, x + 1, y, result);
    getAvailableNeighbourNodes(map, item, x, y - 1, result);
    getAvailableNeighbourNodes(map, item, x, y + 1, result);
}

void PathFindingAStar::getAvailableNeighbourNodes(const std::vector<std::vector<MapItem *>> &map, MapPath &parentNode,
                                                  int x,
                                                  int y, std::vector<MapPath *> &result) {
    if (x >= 0 && y >= 0){
        //Aby kompilator neřval, jelikož x zde již nemůže být zaporné tak je cast validní
        unsigned int u_x = static_cast<unsigned int>(x);
        unsigned int u_y = static_cast<unsigned int>(y);
        if (u_x < map[0].size() && u_y < map.size()) {
            MapItem *mapItem = map[y][x];
            if (mapItem->m_isFree) {
                MapPath *mapNode = createMapNode(*mapItem, &parentNode);
                result.push_back(mapNode);
            }
        }
    }
}

bool PathFindingAStar::isInList(const MapPath& item, std::vector<MapPath*> &list) {
    for (const MapPath* closeItem : list){
        if (item.m_x == closeItem->m_x && item.m_y == closeItem->m_y){
            return true;
        }
    }
    return false;
}

void PathFindingAStar::printListOfNotes(const std::vector<MapPath *>& list) {
    std::cout << " printListNotes " << std::endl;
    for (const auto item : list){
        std::cout << "Item position: " << item->m_x << ", " << item->m_y << std::endl;
        std::cout << "Item values: " << item->getMetric() << ", " << item->getStartDistance() << ", " << item->getEndDistance() << std::endl;
        std::cout << "TowerInRange" << item->getTowerInRange() << std::endl;
    }
}

void PathFindingAStar::freeLists(const std::vector<MapPath *> &list) {
    for (const auto item : list){
        delete item;
    }
}

int PathFindingAStar::getTowerInRanger(int x, int y) {
    int result = 0;
    auto towerInMap = m_communicator->getTowersInMap();
    for (auto tower : towerInMap){
        if (tower->isInRange(x, y)){
            result++;
        }
    }
    return result;
}



