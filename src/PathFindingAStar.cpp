//
// Created by Martin Beran on 14.04.18.
//

#include "PathFindingAStar.h"

MapNode::MapNode(int x, int y, int startDistance, int endDistance, MapNode* parent) : m_x(x),
                                                                                      m_y(y),
                                                                                      m_startDistance(startDistance),
                                                                                      m_endDistance(endDistance),
                                                                                      m_parent(parent) {}

int MapNode::getMetrict() {
    return m_startDistance + m_endDistance;
}

int MapNode::getEndDistance() {
    return m_endDistance;
}

bool MapNode::operator == (const MapItem &mapItem) {
    return m_x == mapItem.m_mapPositionX && m_y == mapItem.m_mapPositionY;
}

bool MapNode::operator == (const MapNode &mapNode) {
    return m_x == mapNode.m_x && m_y == mapNode.m_y;
}


PathFindingAStar::PathFindingAStar(const std::vector<std::vector<MapItem *>>&map, const MapItem *endPoint) : m_map(map),
                                                                                                               m_endPoint(endPoint) {}

MapNode*& PathFindingAStar::createMapNode(const MapItem &mapItem, MapNode* parentNode) {
    int x = mapItem.m_mapPositionX;
    int y = mapItem.m_mapPositionY;
    int startDistance = parentNode == nullptr ? 0 : parentNode->m_startDistance + 1;
    int endDistance = abs( x - m_endPoint->m_mapPositionX) + abs( y - m_endPoint->m_mapPositionY);
    auto * mapNode = new MapNode(x, y, startDistance, endDistance, parentNode);
    return mapNode;
}

bool PathFindingAStar::findBestPath(const MapItem &startPoint, MapNode*& result) {
    MapNode* startNode = createMapNode(startPoint);
    std::vector<MapNode*> availableNodes, closedNodes;
    availableNodes.push_back(startNode);
    std::vector<MapNode*> neighbours;
    while (true){
        std::vector<MapNode*>::const_iterator position;
        MapNode* currentNode = findBestNode(availableNodes, position);
        closedNodes.push_back(currentNode);
        availableNodes.erase(position);
        if (*currentNode == *m_endPoint){
            while (true){
                if (startNode == currentNode->m_parent){
                    result = currentNode;
                    return true;
                }
                currentNode = currentNode->m_parent;
            }
        }
        neighbours.clear();
        getAvailableNeighbourNodes(*startNode, *currentNode, neighbours);
        for (auto neighbour : neighbours){
            if (isInList(*neighbour, closedNodes)){
                continue;
            } else if (!isInList(*neighbour, availableNodes)){
                availableNodes.push_back(neighbour);
            }
        }
    }
}

void PathFindingAStar::findBestPath(const std::vector<MapItem*> &enemies, std::vector<MapNode *> &result) {
    for (const auto enemy : enemies){
        MapNode* mapNode;
        findBestPath(*enemy, mapNode);
        result.push_back(mapNode);
    }
}

MapNode*& PathFindingAStar::findBestNode(const std::vector<MapNode*>& availableNodes,
                                        std::vector<MapNode*>::const_iterator& position) {
    MapNode* result = *availableNodes.begin();
    position = availableNodes.begin();
    for (auto it = availableNodes.begin(); it < availableNodes.end(); ++it) {
        MapNode* node = *it;
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


void PathFindingAStar::getAvailableNeighbourNodes(const MapNode& startNode, MapNode &item, std::vector<MapNode*> &result) {
    int x = item.m_x;
    int y = item.m_y;
    getAvailableNeighbourNodes(startNode, item, x - 1, y, result);
    getAvailableNeighbourNodes(startNode, item, x + 1, y, result);
    getAvailableNeighbourNodes(startNode, item, x, y - 1, result);
    getAvailableNeighbourNodes(startNode, item, x, y + 1, result);
}

bool PathFindingAStar::getAvailableNeighbourNodes(const MapNode& startNode, MapNode &parentNode, int x, int y, std::vector<MapNode*> &result) {
    if (x >= 0 && y >= 0 && x < m_map[0].size() && y < m_map.size()){
        MapItem* mapItem = m_map[y][x];
        if (mapItem->m_isFree){
            MapNode* mapNode = createMapNode(*mapItem, &parentNode);
            result.push_back(mapNode);
        }
    }
    return false;
}

bool PathFindingAStar::isInList(const MapNode& item, std::vector<MapNode*> &list) {
    for (const MapNode* closeItem : list){
        if (item.m_x == closeItem->m_x && item.m_y == closeItem->m_y){
            return true;
        }
    }
    return false;
}

void PathFindingAStar::printListOfNotes(const std::vector<MapNode *>& list) {
    std::cout << " printListNotes " << std::endl;
    for (const auto item : list){
        std::cout << "Item position: " << item->m_x << ", " << item->m_y << std::endl;
        std::cout << "Item values: " << item->getMetrict() << ", " << item->getEndDistance() << std::endl;

    }
}

