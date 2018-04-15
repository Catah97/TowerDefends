#include "UiItems.h"


//----------------------------UiItems

UiItems::UiItems(int x, int y, int width, int height) : m_x(x),
                                                        m_y(y),
                                                        m_width(width),
                                                        m_height(height){
    calPosition();
}

void UiItems::setPosition(int x, int y, int width, int height) {
    m_x = x * width;
    m_y = y * height;
    m_width = width;
    m_height = height;
    calPosition();
}

void UiItems::calPosition() {
    m_vectorLeft = getGlFloatX(m_x);
    m_vectorTop = getGlFloatY(m_y);
    m_vectorRight = getGlFloatX( m_x + m_width);
    m_vectorBottom = getGlFloatY( m_y + m_height);
}

float UiItems::getGlFloatX(const int& position) {
    return getGlFloat(position, m_WindowWidth);
}

float UiItems::getGlFloatY(int position) {
    //Reverse position to get right position
    position = m_WindowHeight - position;
    return getGlFloat(position, m_WindowHeight);
}

float UiItems::getGlFloat(int position, const int& size) {
    float middle = size / 2;
    position -= middle;
    float result = position / middle;
    return result;
}

bool UiItems::contains(int x, int y) const {
    //Y = 0 is on bottom of screen not on TOP
    y = m_WindowHeight - y;
    return x >= m_x && x <= m_x + m_width && y >= m_y && y <= m_y + m_height;
}

bool UiItems::contains(float x, float y) const {
    return x >= m_vectorLeft && x <= m_vectorRight && y >= m_vectorTop && y <= m_vectorBottom;
}

void UiItems::drawAsSquare() {

    glBegin(GL_QUADS);
    glVertex2f( m_vectorLeft, m_vectorTop);
    glVertex2f( m_vectorRight, m_vectorTop);
    glVertex2f( m_vectorRight, m_vectorBottom);
    glVertex2f( m_vectorLeft, m_vectorBottom);
    glEnd();
}


//-------------Button------------------------------------------------

void Button::draw() {

}

//-------------MapItem-----------------

MapItem::MapItem(bool isFree): m_isFree(isFree) {}

std::string MapItem::getMapItemType() {
    return "MapItem";
}

void MapItem::setPosition(int x, int y) {
    m_mapPositionX = x;
    m_mapPositionY = y;
    UiItems::setPosition(x, y, m_width, m_height);
}

void MapItem::setPosition(int x, int y, int width, int height) {
    m_mapPositionX = x;
    m_mapPositionY = y;
    UiItems::setPosition(x, y, width, height);
}


//-------------FreePlace-----------------------------

FreePlace::FreePlace() : MapItem(true) {}

FreePlace::FreePlace(const FreePlace &mapItem) : MapItem(mapItem.m_isFree) {
    m_mapItemChar = mapItem.m_mapItemChar;
    m_mapPositionX = mapItem.m_mapPositionX;
    m_mapPositionY = mapItem.m_mapPositionY;
}

void FreePlace::draw() {
    glColor3f(0.0f, 0.0f, 0.0f);
    drawAsSquare();
}

std::string FreePlace::getMapItemType() {
    return "FreePlace";
}

//-------------Wall-------------------------------

Wall::Wall() : MapItem(false) {}


void Wall::draw() {
    glColor3f(0.5f, 0.5f, 0.5f);
    drawAsSquare();
}

std::string Wall::getMapItemType() {
    return "Wall";
}

//-------------Tower-------------------------------------

Tower::Tower() : MapItem(false) {}


void Tower::draw() {

    glColor3f(1.0f, 1.0f, 1.0f);
    drawAsSquare();


    glColor3f(0.0f, 1.0f, 0.0f);
    glDisable(GL_LIGHTING);



    glRasterPos2d( m_vectorLeft , m_vectorBottom ) ;
    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, m_mapItemChar );

}

std::string Tower::getMapItemType() {
    return "Tower";
}

//-------------Enemy---------------------

Enemy::Enemy() : MapItem(true) {}
Enemy::Enemy(const Enemy &enemy) : MapItem(true), m_hp(enemy.m_hp) {
    m_mapItemChar = enemy.m_mapItemChar;
}


void Enemy::draw() {

    glColor3f(1.0f, 0.0f, 0.0f);
    drawAsSquare();

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_LIGHTING);

    glRasterPos2d( m_vectorLeft , m_vectorBottom ) ;
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, m_mapItemChar );

}

std::string Enemy::getMapItemType() {
    return "Enemy";
}
