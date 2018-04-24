#ifndef DEVELOP_PATHFINDINGASTAR_H
#define DEVELOP_PATHFINDINGASTAR_H

#include "Item.h"


class PathFindingAStar {
private:

    std::vector<std::vector<MapItem*>> m_map;
    const MapItem* m_endPoint = nullptr;

    void printListOfNotes(const std::vector<MapNode*>& list);
    MapNode* createMapNode(const MapItem &mapItem, MapNode* parentNode = nullptr);
    MapNode* findBestNode(const std::vector<MapNode*>& availableNodes,
                            std::vector<MapNode*>::const_iterator& position);
    bool isInList(const MapNode& item, std::vector<MapNode*> &list);
    void createPath(MapNode* endPosition, MapNode*& newStartNode);
    void getAvailableNeighbourNodes(const MapNode& startNode, MapNode& item, std::vector<MapNode*>& result);
    bool getAvailableNeighbourNodes(const MapNode& startNode, MapNode &parentNode, int x, int y, std::vector<MapNode*>& result);


public:
    PathFindingAStar() = default;
    void setMap(const std::vector<std::vector<MapItem*>>& map);
    void setMap(const std::vector<std::vector<MapItem*>>& map, const MapItem* endPoint);
    bool findBestPath(const MapItem& startPoint, MapNode*& result);
    void freeLists(const std::vector<MapNode*>& list);



};

#endif //DEVELOP_PATHFINDINGASTAR_H
