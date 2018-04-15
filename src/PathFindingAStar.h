#ifndef DEVELOP_PATHFINDINGASTAR_H
#define DEVELOP_PATHFINDINGASTAR_H


#include <vector>
#include "UiItems.h"

class MapNode{
public:
    int m_x, m_y;
    int m_startDistance, m_endDistance;
    MapNode* m_parent;


    MapNode(int x, int y, int startDistance, int endDistance, MapNode* m_parent);
    int getEndDistance();
    int getMetrict();

    bool operator == (const MapNode& mapNode);
    bool operator == (const MapItem& mapItem);

};

class PathFindingAStar {
private:

    const std::vector<std::vector<MapItem*>> m_map;
    const MapItem* m_endPoint = nullptr;

    void printListOfNotes(const std::vector<MapNode*>& list);
    MapNode*& createMapNode(const MapItem &mapItem, MapNode* parentNode = nullptr);
    MapNode*& findBestNode(const std::vector<MapNode*>& availableNodes,
                          std::vector<MapNode*>::const_iterator& position);
    bool isInList(const MapNode& item, std::vector<MapNode*> &list);
    void getAvailableNeighbourNodes(const MapNode& startNode, MapNode& item, std::vector<MapNode*>& result);
    bool getAvailableNeighbourNodes(const MapNode& startNode, MapNode &parentNode, int x, int y, std::vector<MapNode*>& result);


public:
    PathFindingAStar(const std::vector<std::vector<MapItem*>>& map, const MapItem* endPoint);
    void findBestPath(const std::vector<MapItem*>& enemies, std::vector<MapNode*>& result);
    bool findBestPath(const MapItem& startPoint, MapNode*& result);


};

#endif //DEVELOP_PATHFINDINGASTAR_H
