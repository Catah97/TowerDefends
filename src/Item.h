#ifndef TOWER_DEFENDS_UIITEMS_H
#define TOWER_DEFENDS_UIITEMS_H

#include <fstream>
#include <iostream>
#include <vector>

#ifdef __OPENGL__

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <fstream>

#else
#include <GL/glut.h>
#endif

#endif //__OPENGL__



extern int m_WindowWidth, m_WindowHeight;

class UiItem {
private:
    static float getGlFloat(int position, const int& size);

protected:
    int m_viewX, m_viewY;
    int m_width, m_height;
    float m_vectorTop, m_vectorLeft, m_vectorRight, m_vectorBottom;

    void drawAsSquare(bool filled = true);
    void drawAsSquare(const float& left, const float& top, const float& right, const float& bottom, bool filled = true);
    virtual float getLineWidth();

public:

    explicit UiItem(int x = -1, int y = -1, int width = -1, int height = -1);

    static float getGlFloatX(const int& position);
    static float getGlFloatY(int position);
    bool contains(int x, int y) const;
    bool contains(float x, float y) const;
    /**
    * Nastaví polohu prvku a vypočítá i hodnoty pro OpenGl
    * @param x souřadnice na ose x
    * @param y souřadnice na ose y
    * @param width šířka objektu
    * @param height výška objektu
    */
    virtual void setPosition(int x, int y, int width, int height);

    /**
    * Funkce, která přepočítává souřednice x a y do reprezentace opengl
    */
    void calPosition();

    virtual void draw() = 0;
};

class Button : public UiItem{
public:

    virtual void draw();
};

class MapItem : public UiItem{
protected:

    void drawSelectedSquare();
    virtual bool canBeSelected();

public:
    bool m_isFree;
    bool m_isSelected;
    char m_mapItemChar;
    //True if this MapItem important to path from end to start. Is set only when m_isSelected is set as true
    int m_mapPositionX, m_mapPositionY;

    explicit MapItem(bool isFree);
    explicit MapItem(const MapItem& mapItem);
    virtual ~MapItem() = default;
    virtual std::string getMapItemType();
    virtual void setPosition(int x, int y);
    virtual void setPosition(int x, int y, int width, int height);
    virtual bool isBlock();
    virtual bool isFree() const;
    virtual bool operator == (const MapItem& mapItem){
        return m_mapPositionX == mapItem.m_mapPositionX && m_mapPositionY == mapItem.m_mapPositionY;
    }
};

class MapNode{
public:
    int m_x, m_y;
    int m_startDistance, m_endDistance;
    MapNode* m_next,* m_before;

    MapNode(const MapNode& mapNode);
    MapNode(int x, int y, int startDistance, int endDistance, MapNode* m_parent);
    int getEndDistance();
    int getMetrict();

    MapNode& operator = (const MapNode& mapNode);
    bool operator == (const MapNode& mapNode);
    bool operator == (const MapItem& mapItem);

    static void deletePath(MapNode* start);
};

class FreePlace : public MapItem{
public:
    explicit FreePlace();
    explicit FreePlace(const MapItem &mapItem);


    virtual void draw();
    virtual std::string getMapItemType();

};

class Wall : public MapItem{
public:
    explicit Wall();

    virtual void draw();
    virtual std::string getMapItemType();

};

class Enemy : public MapItem{
private:

    MapNode* m_enemyPath;
    MapNode* m_nextPosition;
    int m_mapNodesToEnd;

    int getHpHeight();

protected:
    virtual bool canBeSelected();

public:
    int m_hp;
    int m_maxHp;

    explicit Enemy(const int& maxHp = -1);
    explicit Enemy(const Enemy& enemy);
    ~Enemy();

    bool getHit(const int& hit);
    bool isAlive();
    bool isDead();
    bool compareDistanceToEnd(const Enemy &e) const;
    void setPath(const MapNode& startNode);
    void printDistanceToEnd();
    MapNode* getNextPosition();
    void enemyMove();
    virtual void writeToFile(std::ofstream &ofstream, char delimiter);
    virtual void writeToFileAsEnemyInMap(std::ofstream &ofstream, char delimiter);
    virtual void draw();
    virtual std::string getMapItemType();
    virtual bool isBlock();

};

class Tower : public MapItem{
private:
    int m_price, m_attack, m_range;
    bool isInRange(const Enemy& enemy);

public:

    explicit Tower(const Tower& tower);
    explicit Tower(const int& price = -1, const int& attack = -1,const int& range = -1);

    void attack(std::vector<Enemy*>& enemiesInMap);
    int getPrice() const;
    virtual void writeToFile(std::ofstream &ofstream, char delimiter);
    virtual void draw();
    virtual std::string getMapItemType();


};

#endif //TOWER_DEFENDS_UIITEMS_H
