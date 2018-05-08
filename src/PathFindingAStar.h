#ifndef DEVELOP_PATHFINDINGASTAR_H
#define DEVELOP_PATHFINDINGASTAR_H

#include "Item.h"

/**
 * Communication interface between PathFindingAStar and Game
 */
class PathFindingAStarCommunicator{
public:
    /**
     * Function return map from game
     * @return map from game
     */
    virtual const std::vector<std::vector<MapItem*>>& getMap() = 0;

    /**
     * Funcation return all towers in map
     * @return
     */
    virtual const std::vector<Tower*>& getTowersInMap() = 0;

    /**
     * Call to get end point
     * @return endPoint (finish for enemies)
     */
    virtual const MapItem* getEndPoint() = 0;

    /**
     * Detect if PathFindingAStar should used best path for enemies algorithm.
     * That mean PathFindingAStar will find path less dangerous for enemies
     * @return find less dangerous path
     */
    virtual bool getBestPathForEnemies() = 0;
};

/**
 * Class that search for best path from between two points in map.
 * To find this path use A* start algorithm. Path can be found two ways.
 * First: algorithm find the shortest path to finish.
 * Second: algorithm find path less dangerous for enemies. Typically out of range of tower.
 */
class PathFindingAStar {
private:

    /**
     * Game and PathFindingAStar communicator
     */
    PathFindingAStarCommunicator* m_communicator;

    /**
     * Print all MapPath. Use for debug
     * @param list
     */
    void printListOfNotes(const std::vector<MapPath*>& list);

    /**
     * Create new MapPath instance that represent node in map
     * @param mapItem node in map
     * @param parentNode parent of create MapPath
     * @return new MapPath
     */
    MapPath* createMapNode(const MapItem &mapItem, MapPath* parentNode = nullptr);

    /**
     * Find bestNode from A*. Typically MapPath with loves getMetric() value.
     * @param availableNodes List of nodes
     * @param position result position in list
     * @return MapPath with loves getMetric()
     */
    MapPath* findBestNode(const std::vector<MapPath*>& availableNodes,
                            std::vector<MapPath*>::const_iterator& position);

    /**
     * Detect if item is in list
     * @param item
     * @param list
     * @return TRUE item is in list, else FALSE
     */
    bool isInList(const MapPath& item, std::vector<MapPath*> &list);

    /**
     * Create link list between endPosition and newStartNode
     * @param endPosition
     * @param newStartNode
     */
    void createPath(MapPath* endPosition, MapPath*& newStartNode);

    /**
     * Create neighbour nodes list of item in map
     * @param map current map
     * @param item
     * @param result neighbours list create by neighbours of item
     */
    void getAvailableNeighbourNodes(const std::vector<std::vector<MapItem *>>& map, MapPath &item, std::vector<MapPath *> &result);

    /**
     * Create neighbour nodes list of item in map by map coordinates
     * @param map current map
     * @param parentNode parent item of new neighbors
     * @param x
     * @param y
     * @param result list create by neighbours of item with position x and y
     */
    void getAvailableNeighbourNodes(const std::vector<std::vector<MapItem *>> &map, MapPath &parentNode, int x, int y,
                                    std::vector<MapPath *> &result);

    /**
     * Return number of tower that can attack on item in map place at x and y position.
     * Used only when PathFindingAStar finding bestPath for enemies algorithm
     * @param x
     * @param y
     * @return number of tower
     */
    int getTowerInRanger(int x, int y);

public:
    explicit PathFindingAStar(PathFindingAStarCommunicator* communicator);

    /**
     * Find best path between startPoint and end point from m_communicator
     * @param startPoint
     * @param result Link list that create Path
     * @return TRUE, can find path
     */
    bool findBestPath(const MapItem& startPoint, MapPath*& result);

    /**
     * Free instance of all item in link list
     * @param list list that we want to free
     */
    void freeLists(const std::vector<MapPath*>& list);



};

#endif //DEVELOP_PATHFINDINGASTAR_H
