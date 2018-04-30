//
// Created by Martin Beran on 13.04.18.
//

#include "Game.h"

Game::Game() :  bottomToolbar(this),m_lastSelectedItem(nullptr), m_isRunning(false), m_selectedTower(0) {}

Game::~Game() {
    delete m_startPoint;
    delete m_endPoint;
    for (auto tower : m_defineTowers){
        delete tower;
    }
    for (auto enemy : m_defineEnemies){
        delete enemy;
    }
    for (const auto &mapRow : m_map){
        for (const auto &mapItem : mapRow){
            delete mapItem;
        }
    }
    for (const auto enemy : m_enemiesQueue){
        delete enemy;
    }
}

bool Game::initGame(const std::string &path) {
    MapCreator mapCreator;
    bool result = mapCreator.loadGameFile(path);

    m_map = mapCreator.m_map;
    m_money = mapCreator.m_money;
    m_lives = mapCreator.m_lives;
    m_defineTowers = mapCreator.m_defineTowers;
    m_defineEnemies = mapCreator.m_defineEnemies;

    m_enemiesQueue = mapCreator.m_enemiesQueue;
    m_enemiesInMap = mapCreator.m_enemiesInMap;
    m_towersInMap = mapCreator.m_towersInMap;

    //Create deep copy of StartPoint and EndPoint
    m_startPoint = mapCreator.m_startPoint;
    m_endPoint = mapCreator.m_endPoint;
    m_pathFindingAStar.setMap(m_map, m_endPoint);
    if (!result){
        return false;
    }
    return initGame();
}

bool Game::initGame() {

    if (!resetEnemyPath()){
        std::cerr << "No available path from start to end." << std::endl;
        return false;
    }

    size_t width = m_map.at(0).size();
    size_t height = m_map.size();
    m_mapWidth = static_cast<int>(width * s_itemWidth);
    m_mapHeight = static_cast<int>(height * s_itemHeight);

    if (m_mapWidth < s_minWidth){
        m_WindowWidth = s_minWidth;
    } else {
        m_WindowWidth = m_mapWidth;
    }
    m_WindowHeight = static_cast<int>(height * s_itemHeight + s_bottoBarHeight);
    initBottomBar();

    int x_position, y_position;
    for (size_t y = 0; y < m_map.size(); ++y) {
        for (size_t x = 0; x < m_map[y].size(); ++x) {
            auto mapItem = m_map[y][x];
            x_position = static_cast<int>(x);
            y_position = static_cast<int>(y);
            mapItem->setPosition( x_position, y_position, s_itemWidth, s_itemHeight);
        }
    }
    std::cout << "Game loaded " << std::endl << std::endl << std::endl << std::endl;
    return true;
}

void Game::initBottomBar() {
    int bottomBarY = m_mapHeight;
    bottomToolbar.setPosition(0, bottomBarY, m_WindowWidth, s_bottoBarHeight);
    bottomToolbar.setLives(m_lives);
    bottomToolbar.setDefinedTowers(m_defineTowers);
}

bool Game::checkPathAvailable() {
    MapNode* mapNode;
    bool isPathAvailable = m_pathFindingAStar.findBestPath(*m_startPoint, mapNode);
    if (isPathAvailable){
        MapNode::deletePath(mapNode);
    }
    return isPathAvailable;
}

void Game::drawScene() {
#ifdef __OPENGL__
    //Draw white background
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f( -1, 1);
    glVertex2f( 1, 1);
    glVertex2f( 1, -1);
    glVertex2f( -1, -1);
#endif //__OPENGL__

    for (auto &y : m_map) {
        for (auto mapItem : y) {
            mapItem->draw();
        }
    }
    bottomToolbar.draw();
}

void Game::gameTick() {
    if (m_isRunning) {
        enemyMove();
        addFromQueue();
        towersAttack();
        checkGameEnd();
    }
}

void Game::addFromQueue() {
    if (!m_enemiesQueue.empty()){
        int startX = m_startPoint->m_mapPositionX;
        int startY = m_startPoint->m_mapPositionY;
        MapItem* mapItem = m_map[startY][startX];
        if ( typeid(*mapItem) != typeid(Enemy)){
            auto newEnemyIt = m_enemiesQueue.begin();
            auto newEnemy = *newEnemyIt;
            newEnemy->setPosition(startX, startY, s_itemWidth, s_itemHeight);
            newEnemy->setPath(*startPathNode);
            delete m_map[startY][startX];
            m_map[startY][startX] = newEnemy;
            m_enemiesInMap.push_back(newEnemy);
            m_enemiesQueue.erase(newEnemyIt);
        }
    }
}

bool Game::calStartPathNode() {
    return m_pathFindingAStar.findBestPath(*m_startPoint, startPathNode);
}

bool Game::resetEnemyPath() {
    for (auto enemy : m_enemiesInMap){
        MapNode* mapNode;
        if (!m_pathFindingAStar.findBestPath(*enemy, mapNode)){
            return false;
        }
        enemy->setPath(*mapNode);
        MapNode::deletePath(mapNode);
    }
    return calStartPathNode();
}

void Game::sortEnemiesByDistance() {
    std::sort(m_enemiesInMap.begin(), m_enemiesInMap.end(), compareEnemiesDistance);
}

bool Game::compareEnemiesDistance(const Enemy *e1, const Enemy *e2) {
    return e1->compareDistanceToEnd(*e2);
}

void Game::enemyMove() {
    std::cout << "-----------Enemy move------------" << std::endl;
    sortEnemiesByDistance();
    std::vector<std::vector<Enemy*>::iterator> removedItems;
    for (auto it = m_enemiesInMap.begin(); it < m_enemiesInMap.end(); ++it){
        Enemy* enemy = *it;
        //enemy->printDistanceToEnd();
        MapNode* nextEnemyPosition = enemy->getNextPosition();
        //Enemy finish his journey
        if (nextEnemyPosition == nullptr){
            removedItems.push_back(it);
            auto nexItem = new FreePlace(*m_endPoint);
            nexItem->setPosition(m_endPoint->m_mapPositionX,m_endPoint->m_mapPositionY, s_itemWidth, s_itemHeight);
            m_map[m_endPoint->m_mapPositionY][m_endPoint->m_mapPositionX] = nexItem;
            m_lives--;
            bottomToolbar.setLives(m_lives);
            continue;
        }
        auto nextPositionItem = m_map[nextEnemyPosition->m_y][nextEnemyPosition->m_x];
        //Check if is next mapPosition block by another one enemy
        if (!nextPositionItem->isBlock()) {
            swapMapPosition(enemy->m_mapPositionX,
                            enemy->m_mapPositionY,
                            nextPositionItem->m_mapPositionX,
                            nextPositionItem->m_mapPositionY);
            enemy->enemyMove();
        }
    }
    for (auto it : removedItems){
        delete *it;
        m_enemiesInMap.erase(it);
    }
}

void Game::swapMapPosition(const MapItem &oldPosition, const MapItem &newPosition) {
    swapMapPosition(oldPosition.m_mapPositionX, oldPosition.m_mapPositionY, newPosition.m_mapPositionX, newPosition.m_mapPositionY);
}

void Game::swapMapPosition(int oldMapX, int oldMapY, int newMapX, int newMapY) {
    MapItem* item = m_map[oldMapY][oldMapX];
    m_map[oldMapY][oldMapX] = m_map[newMapY][newMapX];
    m_map[oldMapY][oldMapX]->setPosition(oldMapX, oldMapY);
    item->setPosition(newMapX, newMapY);
    m_map[newMapY][newMapX] = item;
}
void Game::checkGameEnd() {
    if (m_lives <= 0) {
        std::cout << "Game over" << std::endl;
        exit(0);
    } else if (m_enemiesQueue.empty() && m_enemiesInMap.empty()) {
        std::cout << "You won with " << m_lives << " lives" << std::endl;
        exit(0);
    }
}

/*
void Game::removeEnemyFromMap(const Enemy &enemy) {
    for (auto it =  m_enemiesInMap.begin(); it < m_enemiesInMap.end(); ++it) {
        auto enemyInMap = *it;
        if (enemyInMap->m_mapPositionX == enemy.m_mapPositionX && enemyInMap->m_mapPositionY == enemy.m_mapPositionY){
            m_enemiesInMap.erase(it);
            delete *it;
            break;
        }
    }
}
 */

void Game::towersAttack() {
    for (auto tower : m_towersInMap){
        tower->attack(m_enemiesInMap);
    }
    clearDeadEnemy();
}

void Game::clearDeadEnemy() {
    for (auto it = m_enemiesInMap.begin(); it < m_enemiesInMap.end(); ++it) {
        auto enemy = *it;
        if (enemy->isDead()){
            int posX = enemy->m_mapPositionX;
            int posY = enemy->m_mapPositionY;
            auto freeItem = new FreePlace();
            freeItem->setPosition(posX, posY, s_itemWidth, s_itemHeight);
            freeItem->m_mapItemChar = constants.FREE_PLACE_CHAR;
            m_map[posY][posX] = freeItem;
            m_enemiesInMap.erase(it);
            delete enemy;
        }
    }
}

void Game::startGame() {
    m_isRunning = true;
}

void Game::pauseGame() {
    m_isRunning = false;
}

bool Game::isRunning() {
    return m_isRunning;
}

void Game:: mouseMove(int x, int y) {
    if (x > 0 && y > 0 && x < m_mapWidth && y < m_mapWidth) {
        unsigned int u_x = getMapXPos(x);
        unsigned int u_y = getMapYPos(y);
        if (u_x < m_map[0].size() && u_y < m_map.size()) {
            auto selectedItem = m_map[u_y][u_x];
            selectedItem->m_isSelected = true;
            if (m_lastSelectedItem != nullptr && m_lastSelectedItem != selectedItem) {
                m_lastSelectedItem->m_isSelected = false;
            }
            if (m_lastSelectedItem != selectedItem) {
                m_lastSelectedItem = m_map[u_y][u_x];
            }
        }
    } else if (m_lastSelectedItem != nullptr){
        m_lastSelectedItem->m_isSelected = false;
    }
}

bool Game::addTower(int x, int y) {
    unsigned int u_x = getMapXPos(x);
    unsigned int u_y = getMapYPos(y);
    MapItem* mapItem = m_map[u_y][u_x];
    if (!mapItem->m_isFree || mapItem->isBlock()){
        return false;
    } else if (*mapItem == *m_startPoint){
        return false;
    }
    auto selectedTower = getSelectedTower();
    if (m_money < selectedTower->getPrice()){
        return false;
    }

    mapItem->m_isFree = false;
    m_pathFindingAStar.setMap(m_map);
    if (checkPathAvailable()){
        auto newTower = new Tower(*selectedTower);
        newTower->setPosition(u_x, u_y, s_itemWidth, s_itemHeight);
        delete mapItem;
        m_map[u_y][u_x] = newTower;
        m_towersInMap.push_back(newTower);
        m_money -= newTower->getPrice();
        resetEnemyPath();
        return true;
    } else{
        m_map[u_y][u_x]->m_isFree = true;
        m_pathFindingAStar.setMap(m_map);
        return false;
    }
}

unsigned int Game::getMapXPos(int x) {
    return static_cast<unsigned int>(x / s_itemWidth);
}

unsigned int Game::getMapYPos(int y) {
    return static_cast<unsigned int>(y / s_itemHeight);
}

Tower* Game::getSelectedTower() {
    return m_defineTowers[m_selectedTower];
}

//Overrided from BottomCommunicator
void Game::onTowerSelected(int pos) {
    m_selectedTower = pos;
}

//Overrided from BottomCommunicator
int Game::getSelectedTowerPosition() {
    return m_selectedTower;
}

//Overrided from BottomCommunicator
int Game::getAvailableMoney() {
    return m_money;
}

bool Game::mouseClick(int x, int y) {
    if (x <= m_mapWidth && y <= m_mapHeight){
        return addTower(x, y);
    } else{
        return bottomToolbar.onMouseClick(x, y);
    }
}

void Game::saveGame() {
    pauseGame();
    MapExport mapExport(m_money, m_lives,m_startPoint, m_endPoint, m_map, m_defineTowers, m_defineEnemies, m_enemiesQueue, m_enemiesInMap);
    mapExport.saveGame();
}






