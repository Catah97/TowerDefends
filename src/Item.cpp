#include "Item.h"

//----------------------------Items

UiItem::UiItem(int x, int y, int width, int height) : m_viewX(x),
                                                      m_viewY(y),
                                                      m_width(width),
                                                      m_height(height){
    calPosition();
}


void UiItem::setPosition(int x, int y, int width, int height) {
    m_viewX = x;
    m_viewY = y;
    m_width = width;
    m_height = height;
    calPosition();
}

void UiItem::calPosition() {
    m_vectorLeft = getGlFloatX(m_viewX);
    m_vectorTop = getGlFloatY(m_viewY);
    m_vectorRight = getGlFloatX( m_viewX + m_width);
    m_vectorBottom = getGlFloatY( m_viewY + m_height);
}

float UiItem::getGlFloatX(const int& position) {
    return getGlFloat(position, m_WindowWidth);
}

float UiItem::getGlFloatY(int position) {
    //Reverse position to get right position
    position = m_WindowHeight - position;
    return getGlFloat(position, m_WindowHeight);
}

float UiItem::getGlFloat(int position, const int& size) {
    float middle = size / 2;
    position -= middle;
    float result = position / middle;
    return result;
}

bool UiItem::contains(int x, int y) const {
    //Y = 0 is on bottom of screen not on TOP
    return x >= m_viewX && x <= m_viewX + m_width && y >= m_viewY && y <= m_viewY + m_height;
}

bool UiItem::contains(float x, float y) const {
    return x >= m_vectorLeft && x <= m_vectorRight && y >= m_vectorTop && y <= m_vectorBottom;
}


void UiItem::drawAsSquare(bool filled) {
    UiItem::drawAsSquare(m_vectorLeft, m_vectorTop, m_vectorRight, m_vectorBottom, filled);
}

void UiItem::drawAsSquare(const float& left,
                          const float& top,
                          const float& right,
                          const float& bottom, bool filled) {
#ifdef __OPENGL__

    if (filled) {
        glBegin(GL_QUADS);
    } else{
        glLineWidth(getLineWidth());
        glBegin(GL_LINE_LOOP);
    }
    glVertex2f( left, top);
    glVertex2f( right, top);
    glVertex2f( right, bottom);
    glVertex2f( left, bottom);
    glEnd();

#endif
}

float UiItem::getLineWidth() {
    return 1.0f;
}

//-----------------MapPath---------------------------------------------

MapPath::MapPath(const MapPath &mapNode) : m_startDistance(mapNode.m_startDistance),
                                           m_endDistance(mapNode.m_endDistance),
                                           m_towerInRange(mapNode.m_towerInRange),
                                           m_x(mapNode.m_x),
                                           m_y(mapNode.m_y),
                                           m_next(nullptr),
                                           m_previous(nullptr){}

MapPath::MapPath(int x, int y, int startDistance, int endDistance, int towerInRange, MapPath *parent) : m_startDistance(startDistance),
                                                                                                        m_endDistance(endDistance),
                                                                                                        m_towerInRange(towerInRange),
                                                                                                        m_x(x),
                                                                                                        m_y(y),
                                                                                                        m_next(nullptr),
                                                                                                        m_previous(parent){}

int MapPath::getMetric() {
    return m_startDistance + m_endDistance + m_towerInRange * 10000;
}

int MapPath::getEndDistance() {
    return m_endDistance;
}

int MapPath::getTowerInRange() {
    return m_towerInRange;
}


bool MapPath::operator == (const MapItem &mapItem) {
    return m_x == mapItem.m_mapPositionX && m_y == mapItem.m_mapPositionY;
}

bool MapPath::operator == (const MapPath &mapNode) {
    return m_x == mapNode.m_x && m_y == mapNode.m_y;
}

MapPath& MapPath::operator = (const MapPath &mapNode) {
    if (&mapNode == this){
        return *this;
    }
    m_x = mapNode.m_x;
    m_y = mapNode.m_y;
    m_startDistance = mapNode.m_startDistance;
    m_endDistance = mapNode.m_endDistance;
    m_previous = mapNode.m_previous;
    return *this;
}

void MapPath::deletePath(MapPath* start) {
    auto current = start;
    MapPath* tmp;
    do {
        tmp = current->m_next;
        delete current;
        current = tmp;
    } while (current);
}

int MapPath::getStartDistance() {
    return m_startDistance;
}

//-------------MapItem-----------------

MapItem::MapItem(bool isFree): m_isFree(isFree), m_isSelected(false) {}

MapItem::MapItem(const MapItem &mapItem) : m_isFree(mapItem.m_isFree),m_isSelected(false), m_mapItemChar(mapItem.m_mapItemChar) {}

std::string MapItem::getMapItemType() {
    return "MapItem";
}

void MapItem::setPosition(int x, int y) {
    m_mapPositionX = x;
    m_mapPositionY = y;
    UiItem::setPosition(x * m_width, y * m_height, m_width, m_height);
}

void MapItem::setPosition(int x, int y, int width, int height) {
    m_mapPositionX = x;
    m_mapPositionY = y;
    UiItem::setPosition(x * width, y * height, width, height);
}

void MapItem::drawSelectedSquare() {
    if (m_isSelected) {
#ifdef __OPENGL__

        if (canBeSelected()) {
            glColor3f(0.0f, 1.0f, 0.0f);
        } else{
            glColor3f(1.0f, 0.0f, 0.0f);
        }
        drawAsSquare(m_vectorLeft, m_vectorTop, getGlFloatX( m_viewX + m_width - 1), m_vectorBottom, false);

#endif
    }
}

bool MapItem::canBeSelected() {
    return m_isFree;
}

bool MapItem::isBlock() {
    return !isFree();
}

bool MapItem::isFree() const {
    return m_isFree;
}

//-------------FreePlace-----------------------------

FreePlace::FreePlace() : MapItem(true) {}

FreePlace::FreePlace(const MapItem &mapItem) : MapItem(mapItem.m_isFree) {
    m_mapItemChar = mapItem.m_mapItemChar;
    m_mapPositionX = mapItem.m_mapPositionX;
    m_mapPositionY = mapItem.m_mapPositionY;
}

void FreePlace::draw() {
#ifdef __OPENGL__

    glColor3f(0.0f, 0.0f, 0.0f);
    drawAsSquare();

    drawSelectedSquare();

#endif
}

std::string FreePlace::getMapItemType() {
    return "FreePlace";
}

//-------------Wall-------------------------------

Wall::Wall() : MapItem(false) {}


void Wall::draw() {
#ifdef __OPENGL__
    glColor3f(0.5f, 0.5f, 0.5f);
    drawAsSquare();

    drawSelectedSquare();
#endif
}

std::string Wall::getMapItemType() {
    return "Wall";
}

//-------------Tower-------------------------------------

Tower::Tower(const Tower &tower) : MapItem(tower),
                                   m_price(tower.m_price),
                                   m_attack(tower.m_attack),
                                   m_range(tower.m_range) {}

Tower::Tower(const int &price, const int &attack, const int &range) : MapItem(false),
                                                                        m_price(price),
                                                                        m_attack(attack),
                                                                        m_range(range){

}


void Tower::draw() {

#ifdef __OPENGL__
    glColor3f(1.0f, 1.0f, 1.0f);
    drawAsSquare();


    glColor3f(0.0f, 1.0f, 0.0f);
    glDisable(GL_LIGHTING);



    glRasterPos2d( m_vectorLeft , m_vectorBottom ) ;
    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, m_mapItemChar );

    drawSelectedSquare();
#endif

}

std::string Tower::getMapItemType() {
    return "Tower";
}

void Tower::attack(std::vector<Enemy*>& enemiesInMap) {
    for (auto enemy : enemiesInMap){
        if (isInRange(*enemy) && enemy->isAlive()){
            enemy->getHit(m_attack);
            break;
        }
    }
}

bool Tower::isInRange(const Enemy& enemy) {
    int x = enemy.m_mapPositionX;
    int y = enemy.m_mapPositionY;
    return isInRange(x, y);
}


bool Tower::isInRange(int x, int y) {
    int distanceX = abs(m_mapPositionX - x);
    int distanceY = abs(m_mapPositionY - y);
    return distanceX + distanceY <= m_range;
}


int Tower::getPrice() const {
    return m_price;
}

void Tower::writeToFile(std::ofstream &ofstream, char delimiter) {
    ofstream << m_mapItemChar << delimiter << m_price << delimiter << m_attack << delimiter << m_range << std::endl;
}


//-------------Enemy---------------------

Enemy::Enemy(const int &maxHp) : MapItem(true), m_enemyPath(nullptr), m_hp(maxHp), m_maxHp(maxHp) {}
Enemy::Enemy(const Enemy &enemy) : MapItem(true), m_enemyPath(nullptr), m_hp(enemy.m_hp), m_maxHp(enemy.m_maxHp) {
    m_mapItemChar = enemy.m_mapItemChar;
}


void Enemy::draw() {

#ifdef __OPENGL__
    //DrawMaxHp
    glColor3f(1.0f, 1.0f, 1.0f);
    drawAsSquare();

    //DrawCurrentHp
    glColor3f(1.0f, 0.0f, 0.0f);
    int hpHeight = getHpHeight();
    if (hpHeight > 0) {
        float vectorBottom = getGlFloatY(m_viewY + hpHeight);
        drawAsSquare(m_vectorLeft, m_vectorTop, m_vectorRight, vectorBottom);
    }

    glColor3f(0.0f, 0.0f, 0.0f);
    glDisable(GL_LIGHTING);

    glRasterPos2d( m_vectorLeft , m_vectorBottom ) ;
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, m_mapItemChar );

    drawSelectedSquare();
#endif

}

std::string Enemy::getMapItemType() {
    return "Enemy";
}

int Enemy::getHpHeight() {
    float hp = m_hp;
    float maxHp = m_maxHp;
    float diff = hp / maxHp;
    return  static_cast<int>(m_height * diff);
}

bool Enemy::getHit(const int &hit) {
    m_hp -= hit;
    return isDead();
}

bool Enemy::isAlive() {
    return m_hp > 0;
}

bool Enemy::isDead() {
    return !isAlive();
}

bool Enemy::canBeSelected() {
    return false;
}

bool Enemy::isBlock() {
    return true;
}

void Enemy::setPath(const MapPath &startNode) {
    if (m_enemyPath != nullptr){
        MapPath::deletePath(m_enemyPath);
    }
    m_enemyPath = new MapPath(startNode);
    MapPath* currentNode = startNode.m_next;
    MapPath* currentNewNode = m_enemyPath;
    m_mapNodesToEnd = 0;
    while ( currentNode ){
        auto newNode = new MapPath(*currentNode);
        currentNewNode->m_next = newNode;
        currentNewNode = newNode;
        currentNode = currentNode->m_next;
        m_mapNodesToEnd++;
    }
    currentNewNode->m_next = nullptr;
    m_nextPosition = m_enemyPath->m_next;
}

void Enemy::enemyMove() {
    m_mapNodesToEnd--;
    m_nextPosition = m_nextPosition->m_next;
    /*
    std::cout << "Enemy " << this << " move: " << m_mapNodesToEnd << std::endl;
    std::cout << "Enemy " << this << " nextPos: " << m_nextPosition->m_x << ", " << m_nextPosition->m_y << std::endl;
     */

}

MapPath *Enemy::getNextPosition() {
    return m_nextPosition;
}

void Enemy::printDistanceToEnd() {
    std::cout << "Distance: " << m_mapNodesToEnd << std::endl;
}

Enemy::~Enemy() {
    if (m_enemyPath != nullptr) {
        MapPath::deletePath(m_enemyPath);
    }
    m_enemyPath = nullptr;
}


bool Enemy::compareDistanceToEnd(const Enemy &e) const {
    return m_mapNodesToEnd < e.m_mapNodesToEnd;
}

void Enemy::writeToFile(std::ofstream &ofstream, char delimiter) {
    ofstream << m_mapItemChar << delimiter << m_maxHp << std::endl;
}

void Enemy::writeToFileAsEnemyInMap(std::ofstream &ofstream, char delimiter) {
    ofstream << m_mapPositionX << delimiter << m_mapPositionY << delimiter << m_hp <<  std::endl;
}
