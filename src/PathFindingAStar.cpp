//
// Created by Martin Beran on 14.04.18.
//

#include "PathFindingAStar.h"

//---------------------------PathFindingAStar--------------------------------

void PathFindingAStar::setMap(const std::vector<std::vector<MapItem *>> &map) {
    m_map = map;
}

void PathFindingAStar::setMap(const std::vector<std::vector<MapItem *>> &map, const MapItem *endPoint) {
    m_map = map;
    m_endPoint = endPoint;
}

MapPath* PathFindingAStar::createMapNode(const MapItem &mapItem, MapPath* parentNode) {
    int x = mapItem.m_mapPositionX;
    int y = mapItem.m_mapPositionY;
    //startDistance has diff 10 endDistance 1
    int startDistance = parentNode == nullptr ? 0 : parentNode->m_startDistance + 10;
    int endDistance = abs( x - m_endPoint->m_mapPositionX) * 10 + abs( y - m_endPoint->m_mapPositionY) * 10;
    auto mapNode = new MapPath(x, y, startDistance, endDistance, parentNode);
    return mapNode;
}


bool PathFindingAStar::findBestPath(const MapItem &startPoint, MapPath*& result) {
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
        if (*currentNode == *m_endPoint){
            createPath(currentNode, result);
            freeLists(availableNodes);
            freeLists(closedNodes);
            std::cout << "Cycles: " << count << std::endl;
            return true;
        }
        neighbours.clear();
        getAvailableNeighbourNodes(*currentNode, neighbours);
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
        currentNode = currentNode->m_before;
    }
    newStartNode = lastNode;
}

MapPath* PathFindingAStar::findBestNode(const std::vector<MapPath*>& availableNodes,
                                          std::vector<MapPath*>::const_iterator& position) {
    MapPath* result = *availableNodes.begin();
    position = availableNodes.begin();
    for (auto it = availableNodes.begin(); it < availableNodes.end(); ++it) {
        MapPath* node = *it;
        if (result->getMetrict() > node->getMetrict()){
            result = node;
            position = it;
        } else if (result->getMetrict() == node->getMetrict() && result->getEndDistance() > node->getEndDistance()){
            result = node;
            position = it;
        }
    }
    return result;
}


void PathFindingAStar::getAvailableNeighbourNodes(MapPath &item, std::vector<MapPath *> &result) {
    int x = item.m_x;
    int y = item.m_y;
    getAvailableNeighbourNodes(item, x - 1, y, result);
    getAvailableNeighbourNodes(item, x + 1, y, result);
    getAvailableNeighbourNodes(item, x, y - 1, result);
    getAvailableNeighbourNodes(item, x, y + 1, result);
}

bool PathFindingAStar::getAvailableNeighbourNodes(MapPath &parentNode, int x, int y, std::vector<MapPath *> &result) {
    if (x >= 0 && y >= 0){
        //Aby kompilator neřval, jelikož x zde již nemůže být zaporné tak je cast validní
        unsigned int u_x = static_cast<unsigned int>(x);
        unsigned int u_y = static_cast<unsigned int>(y);
        if (u_x < m_map[0].size() && u_y < m_map.size()) {
            MapItem *mapItem = m_map[y][x];
            if (mapItem->m_isFree) {
                MapPath *mapNode = createMapNode(*mapItem, &parentNode);
                result.push_back(mapNode);
            }
        }
    }
    return false;
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
        std::cout << "Item values: " << item->getMetrict() << ", " << item->getEndDistance() << std::endl;

    }
}

void PathFindingAStar::freeLists(const std::vector<MapPath *> &list) {
    for (const auto item : list){
        delete item;
    }
}


