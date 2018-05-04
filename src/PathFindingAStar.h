#ifndef DEVELOP_PATHFINDINGASTAR_H
#define DEVELOP_PATHFINDINGASTAR_H

#include "Item.h"

class PathFindingAStarCommunicator{
public:
    virtual const std::vector<std::vector<MapItem*>>& getMap() = 0;
    virtual const std::vector<Tower*>& getTowersInMap() = 0;
    virtual const MapItem* getEndPoint() = 0;
    virtual bool getBestPath() = 0;
};

class PathFindingAStar {
private:

    PathFindingAStarCommunicator* m_communicator;

    void printListOfNotes(const std::vector<MapPath*>& list);
    MapPath* createMapNode(const MapItem &mapItem, MapPath* parentNode = nullptr);
    MapPath* findBestNode(const std::vector<MapPath*>& availableNodes,
                            std::vector<MapPath*>::const_iterator& position);
    bool isInList(const MapPath& item, std::vector<MapPath*> &list);
    void createPath(MapPath* endPosition, MapPath*& newStartNode);
    void getAvailableNeighbourNodes(const std::vector<std::vector<MapItem *>>& map, MapPath &item, std::vector<MapPath *> &result);
    bool getAvailableNeighbourNodes(const std::vector<std::vector<MapItem *>>& map, MapPath &parentNode, int x, int y,
                                        std::vector<MapPath *> &result);
    int getTowerInRanger(int x, int y);

public:
    PathFindingAStar(PathFindingAStarCommunicator* communicator);
    bool findBestPath(const MapItem& startPoint, MapPath*& result);
    void freeLists(const std::vector<MapPath*>& list);



};

#endif //DEVELOP_PATHFINDINGASTAR_H
