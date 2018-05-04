#ifndef TOWER_DEFENDS_UIITEMS_H
#define TOWER_DEFENDS_UIITEMS_H

#include <fstream>
#include <iostream>
#include <vector>

#ifdef __OPENGL__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#endif //__OPENGL__

/**
 * Global value that save window width and height
 */
extern int m_WindowWidth, m_WindowHeight;

/**
 * Basic view, holds coordinates X and Y in the Cartesian system and display size. These coordinates are convert into vectors in openGl.
 * Contains an abstract function for draw all subview
 */
class UiItem {
private:

    /**
     * Convert standard coordinates into openGl vector coordinates
     * @param position to convert
     * @param size of window
     * @return vector coordinates for openGl
     */
    static float getGlFloat(int position, const int& size);

protected:

    int m_viewX, m_viewY;
    int m_width, m_height;

    /**
     * Position of item in OpenGl
     */
    float m_vectorTop, m_vectorLeft, m_vectorRight, m_vectorBottom;

    /**
     * Draw rectangle with same size as view
     * @param filled TRUE if rectangle should be filled(default is true)
     */
    void drawAsSquare(bool filled = true);

    /**
     * Draw rectangle by params
     * @param left left of rectangle
     * @param top top of rectangle
     * @param right right of rectangle
     * @param bottom bottom of rectangle
     * @param filled TRUE if rectangle should be filled(default is true)
     */
    void drawAsSquare(const float& left, const float& top, const float& right, const float& bottom, bool filled = true);

    /**
     * Get line width for draw
     * @return line width
     */
    virtual float getLineWidth();

public:

    explicit UiItem(int x = -1, int y = -1, int width = -1, int height = -1);

    /**
     * Convert X position to opengl vector
     * @param position X position in window
     * @return vektor vector in opengl
     */
    static float getGlFloatX(const int& position);

    /**
     * Convert Y position to opengl vector
     * @param position Y position in window
     * @return vektor vector in opengl
     */
    static float getGlFloatY(int position);

    /**
     * Detect if coordinates are in view or not
     * @param x
     * @param y
     * @return TRUE if yes, else FALSE
     */
    bool contains(int x, int y) const;

    /**
     * Detect if coordinates are in view or not
     * @param x
     * @param y
     * @return TRUE if yes, else FALSE
     */
    bool contains(float x, float y) const;

    /**
    * Set item position and get vectors for openGl
    * @param x X position in window
    * @param y Y position in window
    * @param width width of view
    * @param height height of view
    */
    virtual void setPosition(int x, int y, int width, int height);

    /**
     * Convert position to opengl vectors
     */
    void calPosition();

    /**
     * Abstract function to draw view
     */
    virtual void draw() = 0;
};

/**
 * View, which represents one item on the map.
 */
class MapItem : public UiItem{
protected:

    /**
     * Draw border around selected item
     */
    void drawSelectedSquare();

    /**
     * Check if item can be selected (green color) or not (red color).
     * If item can be selected, the user can build tower on position of this item.
     * @return TURE, user can build tower here
     */
    virtual bool canBeSelected();

public:

    /**
     * If TRUE, then this field can be used for path from start to finish
     */
    bool m_isFree;

    /**
     * TRUE, if is field selected by mouse cursor
     */
    bool m_isSelected;
    char m_mapItemChar;
    int m_mapPositionX, m_mapPositionY;

    explicit MapItem(bool isFree);
    explicit MapItem(const MapItem& mapItem);
    virtual ~MapItem() = default;

    /**
     * DEBUG function return item type
     * @return ITEM type
     */
    virtual std::string getMapItemType();

    /**
     * set view position and set map coordinates
     * @param x x position in window
     * @param y y position in window
     */
    virtual void setPosition(int x, int y);

    /**
     * set view position and set map coordinates
     * @param x x position in window
     * @param y y position in window
     * @param width
     * @param height
     */
    virtual void setPosition(int x, int y, int width, int height);

    /**
     * Check if field is block.
     * Block mean that field can be used to path, but now it is block by enemy and tower cannot be placed here.
     * @return TRUE if is block
     */
    virtual bool isBlock();

    /**
     * Getter for m_isFree
     * @return return m_isFree
     */
    virtual bool isFree() const;

    /**
     * Compare two MapItem by coordinates on map
     * @param mapItem
     * @return TRUE if coordinates are same, else FALSE
     */
    virtual bool operator == (const MapItem& mapItem){
        return m_mapPositionX == mapItem.m_mapPositionX && m_mapPositionY == mapItem.m_mapPositionY;
    }
};

/**
 * Class for path implementation by A*
 * To found path is used three attributes m_startDistance, m_endDistance, m_towerInRange.
 * Final path is represent by link list.
 * Every item in this list has reference on next segment of path and previous segment.
 */
class MapPath{
private:
    /**
     * Absolute distance from start point(mean how many jump you are from start)
     */
    int m_startDistance;

    /**
     * Relative distance from end
     */
    int m_endDistance;

    /**
     * How many tower can attack on this field of map
     */
    int m_towerInRange;

public:

    /**
     * Position on map
     */
    int m_x, m_y;

    /**
     * Links on next and previous segment of path
     */
    MapPath* m_next,* m_previous;

    /**
     * Copy attributes of params mapNode to current MapPath
     * @param mapNode
     */
    MapPath(const MapPath& mapNode);
    MapPath(int x, int y, int startDistance, int endDistance, int towerInRange, MapPath *m_parent);

    /**
     * Get start distance
     * @return value of m_startDistance
     */
    int getStartDistance();

    /**
     * Get how many tower can attack this MapPath
     * @return value of m_towerInRange
     */
    int getTowerInRange();

    /**
     * Get distance to end
     * @return value of m_endDistance
     */
    int getEndDistance();

    /**
     * Metric for A* algorithm. Metric is sum from (m_startDistance, m_endDistance, m_towerInRange * 10000).
     * m_towerInRange * 10000 is used to found path with least dangerous fields
     * @return sum from (m_startDistance, m_endDistance, m_towerInRange * 10000)
     */
    int getMetric();

    MapPath& operator = (const MapPath& mapNode);
    bool operator == (const MapPath& mapNode);
    bool operator == (const MapItem& mapItem);

    /**
     * Delete all items in path
     * @param start first item in link list
     */
    static void deletePath(MapPath* start);
};

/**
 * Class for free places on map
 */
class FreePlace : public MapItem{
public:
    explicit FreePlace();
    explicit FreePlace(const MapItem &mapItem);


    virtual void draw();
    virtual std::string getMapItemType();

};

/**
 * Class for walls on map
 */
class Wall : public MapItem{
public:
    explicit Wall();

    virtual void draw();
    virtual std::string getMapItemType();

};

/**
 * Class for walls on enemies
 */
class Enemy : public MapItem{
private:

    /**
     * Start of enemy path
     */
    MapPath* m_enemyPath;

    /**
     * Reference on next segment of path
     */
    MapPath* m_nextPosition;

    /**
     * Number of moves that is necessary to get to finish.
     */
    int m_mapNodesToEnd;

    /**
     * Calculate height of square of that shows enemy health
     * @return height of square
     */
    int getHpHeight();

protected:
    virtual bool canBeSelected();

public:
    int m_hp;
    int m_maxHp;

    explicit Enemy(const int& maxHp = -1);
    explicit Enemy(const Enemy& enemy);
    ~Enemy();

    /**
     * Call when enemy get hit from tower
     * @param hit hit from tower
     * @return return isDead()
     */
    bool getHit(const int& hit);
    bool isAlive();
    bool isDead();

    /**
     * Compare distance enemies from end and return which of enemy is closer to end
     * @param e second enemies
     * @return TRUE true if is current enemy closer then second one, else FALSE
     */
    bool compareDistanceToEnd(const Enemy &e) const;

    /**
     * Set enemy path to end
     * @param startNode start MapNode of path
     */
    void setPath(const MapPath& startNode);
    void printDistanceToEnd();
    MapPath* getNextPosition();
    void enemyMove();

    /**
     * Write to file as definition of enemy
     * @param ofstream stream of file
     * @param delimiter delimiter between attributes
     */
    virtual void writeToFile(std::ofstream &ofstream, char delimiter);

    /**
     * Write as enemy on map. Write coordinates of current enemy and health of enemy
     * @param ofstream stream of file
     * @param delimiter delimiter between attributes
     */
    virtual void writeToFileAsEnemyInMap(std::ofstream &ofstream, char delimiter);
    virtual void draw();
    virtual std::string getMapItemType();
    virtual bool isBlock();

};

/**
 * Class of tower on map.
 */
class Tower : public MapItem{
private:

    /**
     * Attributes of tower
     */
    int m_price, m_attack, m_range;


public:

    /**
     * Check if is enemy in range of tower attacks
     * @param enemy
     * @return TRUE if enemy is in range, else FALSE
     */
    bool isInRange(const Enemy& enemy);
    bool isInRange(int x, int y);


    explicit Tower(const Tower& tower);
    explicit Tower(const int& price = -1, const int& attack = -1,const int& range = -1);

    /**
     * Attack of Tower on enemy that is first in range of Tower
     * @param enemiesInMap list of all enemies in map
     */
    void attack(std::vector<Enemy*>& enemiesInMap);
    int getPrice() const;

    /**
     * Write definition of tower to file
     * @param ofstream stream of file
     * @param delimiter delimiter between attributes
     */
    virtual void writeToFile(std::ofstream &ofstream, char delimiter);
    virtual void draw();
    virtual std::string getMapItemType();


};

#endif //TOWER_DEFENDS_UIITEMS_H
