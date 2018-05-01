#ifndef DEVELOP_PATHFINDINGASTAR_H
#define DEVELOP_PATHFINDINGASTAR_H

#include "Item.h"


class PathFindingAStar {
private:

    std::vector<std::vector<MapItem*>> m_map;
    const MapItem* m_endPoint = nullptr;

    void printListOfNotes(const std::vector<MapPath*>& list);
    MapPath* createMapNode(const MapItem &mapItem, MapPath* parentNode = nullptr);
    MapPath* findBestNode(const std::vector<MapPath*>& availableNodes,
                            std::vector<MapPath*>::const_iterator& position);
    bool isInList(const MapPath& item, std::vector<MapPath*> &list);
    void createPath(MapPath* endPosition, MapPath*& newStartNode);
    void getAvailableNeighbourNodes(MapPath &item, std::vector<MapPath *> &result);
    bool getAvailableNeighbourNodes(MapPath &parentNode, int x, int y, std::vector<MapPath *> &result);


public:
    PathFindingAStar() = default;
    void setMap(const std::vector<std::vector<MapItem*>>& map);
    void setMap(const std::vector<std::vector<MapItem*>>& map, const MapItem* endPoint);
    bool findBestPath(const MapItem& startPoint, MapPath*& result);
    void freeLists(const std::vector<MapPath*>& list);



};

#endif //DEVELOP_PATHFINDINGASTAR_H
