#ifndef TOWER_DEFENDS_UIITEMS_H
#define TOWER_DEFENDS_UIITEMS_H

#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif



extern int m_WindowWidth, m_WindowHeight;

class UiItems {
private:
    static float getGlFloat(int position, const int& size);

protected:
    int m_x, m_y;
    int m_width, m_height;
    float m_vectorTop, m_vectorLeft, m_vectorRight, m_vectorBottom;

    void drawAsSquare();

public:

    explicit UiItems(int x = -1, int y = -1, int width = -1, int height = -1);

    static float getGlFloatX(const int& position);
    static float getGlFloatY(int position);
    bool contains(int x, int y) const;
    bool contains(float x, float y) const;
    virtual void setPosition(int x, int y, int width, int height);
    void calPosition();

    virtual void draw() = 0;
};

class Button : public UiItems{
public:

    virtual void draw();
};

class MapItem : public UiItems{
public:
    char m_mapItemChar;
    bool m_isFree;
    int m_mapPositionX, m_mapPositionY;

    explicit MapItem(bool isFree);
    virtual std::string getMapItemType();
    virtual void setPosition(int x, int y);
    virtual void setPosition(int x, int y, int width, int height);

};

class FreePlace : public MapItem{
public:
    explicit FreePlace();
    explicit FreePlace(const FreePlace &mapItem);


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
public:
    int m_hp;

    explicit Enemy();
    explicit Enemy(const Enemy& enemy);

    virtual void draw();
    virtual std::string getMapItemType();

};

class Tower : public MapItem{
public:
    int m_attack, m_range;

    explicit Tower();

    virtual void draw();
    virtual std::string getMapItemType();


};

#endif //TOWER_DEFENDS_UIITEMS_H
