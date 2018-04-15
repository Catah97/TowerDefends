#include "MapCreator.h"


//----------------MapCreator-------------------------------

MapCreator::~MapCreator() {
    for (auto tower : defineTowers){
        delete tower;
    }
    for (auto enemy : defineEnemies){
        delete enemy;
    }
}

bool MapCreator::loadTowers(const std::string& towerDefines) {
    char towerChar;
    int towerAttack, towerRange;

    int readResult;
    readResult = sscanf(&towerDefines.at(0), " %c , %d , %d  ", &towerChar, &towerAttack, &towerRange);
    if (readResult != 3) {
        std::cerr << "Parse tower error" << std::endl;
        return false;
    }
    if (!checkMainCharMatch("Tower", towerChar)){
        return false;
    }
    auto tower = new Tower();
    tower->m_mapItemChar = towerChar;
    tower->m_attack = towerAttack;
    tower->m_range = towerRange;
    std::vector<Tower *>::const_iterator position;
    if (defineTowers.empty()){
        defineTowers.push_back(tower);
    }
    else if (!Tools::find(defineTowers, *tower, position, compareTower)) {
        defineTowers.insert(position, tower);
    } else{
        std::cerr << "Tower with char " << tower->m_mapItemChar << " is already defined" << std::endl;
        delete tower;
        return false;
    }
    return true;
}

bool MapCreator::loadEnemies(const std::string &enemyDefine) {
    char enemyChar;
    int enemyHp;

    int readResult;
    readResult = sscanf(&enemyDefine.at(0), " %c , %d ", &enemyChar, &enemyHp);
    if (readResult != 2) {
        std::cerr << "Parse enemies error" << std::endl;
        return false;
    }
    if (!checkMainCharMatch("Tower", enemyChar)){
        return false;
    }
    auto enemy = new Enemy();
    enemy->m_mapItemChar = enemyChar;
    enemy->m_hp = enemyHp;
    std::vector<Enemy*>::const_iterator position;
    if (defineEnemies.empty()){
        defineEnemies.push_back(enemy);
    }
    else if (!Tools::find(defineEnemies, *enemy, position, compareEnemy)) {
        defineEnemies.insert(position, enemy);
    } else{
        std::cerr << "Enemy with char " << enemy->m_mapItemChar << " is already defined" << std::endl;
        delete enemy;
        return false;
    }
    return true;
}

bool MapCreator::loadQueue(const std::string &queue) {
    for (char c : queue) {
        Enemy searchEnemy;
        searchEnemy.m_mapItemChar = c;
        std::vector<Enemy*>::const_iterator position;
        if (Tools::find(defineEnemies, searchEnemy, position, compareEnemy)){
            auto newEnemy = new Enemy(**position);
            enemiesQueue.push_back(newEnemy);
        } else{
            std::cerr << "Undefine enemy with char " << searchEnemy.m_mapItemChar << " in Queue" << std::endl;
            return false;
        }
    }
    return true;
}

bool MapCreator::parseString(const std::string &headLine, const std::string &arg) {
    if (headLine == TOWER_DEFINE){
        return loadTowers(arg);
    } else if (headLine == ENEMIES_DEFINE){
        return loadEnemies(arg);
    } else if (headLine == QUEUE_DEFINE){
        return loadQueue(arg);
    } else if (headLine == MAP_DEFINE){
        return loadMap(arg);
    }
    return false;
}

bool MapCreator::loadGameFile(const char *gameDefinePath) {
    std::cout << "Reading file: " << gameDefinePath << std::endl;
    std::string line, headLine;
    std::ifstream mapFile (gameDefinePath);
    if (mapFile.is_open())
    {
        while ( getline (mapFile,line) )
        {
            if (line == TOWER_DEFINE || line == ENEMIES_DEFINE || line == QUEUE_DEFINE || line == MAP_DEFINE){
                if (line == MAP_DEFINE && !map.empty()){
                    std::cerr << "Only one map can be defined" << std::endl;
                    mapFile.close();
                    return false;
                }
                headLine = line;
            } else if (!headLine.empty() && !line.empty() && !parseString(headLine, line)){
                mapFile.close();
                return false;
            }
        }
        mapFile.close();
    }
    return true;
}

bool MapCreator::checkMainCharMatch(const std::string &item, const char &c) {
    if (c == WALL_CHAR){
        std::cerr << item <<" cant be defined by " << WALL_CHAR << std::endl;
        return false;
    } else if (c == QUEUE_IN_CHAR){
        std::cerr << item <<" cant be defined by " << QUEUE_IN_CHAR << std::endl;
        return false;
    } else if (c == QUEUE_OUT_CHAR){
        std::cerr << item <<" cant be defined by " << QUEUE_OUT_CHAR << std::endl;
        return false;
    }
    return true;
}

bool MapCreator::loadMap(const std::string &mapLine) {
    std::vector<MapItem*> mapRow;
    size_t y = map.size();
    for (size_t x = 0; x < mapLine.length(); x++) {
        char itemChar = mapLine.at(x);
        MapItem* mapItem;
        if (createMapItem(itemChar, mapItem)){
            mapItem->m_mapPositionX = static_cast<int>(x);
            mapItem->m_mapPositionY = static_cast<int>(y);
            mapRow.push_back(mapItem);
        } else{
            return false;
        }
    }
    if (!map.empty() && map[map.size() - 1].size() != mapRow.size()){
        std::cerr << "Map can be only square or rectangle." << std::endl;
        return false;
    }
    map.push_back(mapRow);
    return true;
}

bool MapCreator::compareTower(const Tower *t1, const Tower *t2) {
    return t1->m_mapItemChar < t2->m_mapItemChar;
}

bool MapCreator::compareEnemy(const Enemy *e1, const Enemy *e2) {
    return e1->m_mapItemChar < e2->m_mapItemChar;
}

bool MapCreator::createMapItem(const char& itemChar, MapItem*& foundItem) {
    if (itemChar == WALL_CHAR){
        foundItem = new Wall();
        return true;
    } else if (itemChar == FREE_PLACE_CHAR){
        foundItem = new FreePlace();
        return true;
    } else if (itemChar == QUEUE_IN_CHAR) {
        if (startPoint != nullptr){
            std::cerr << "Only one start point can be defined." << std::endl;
            return false;
        }
        startPoint = new FreePlace();
        foundItem = startPoint;
        return true;
    } else if (itemChar == QUEUE_OUT_CHAR){
        if (endPoint != nullptr){
            std::cerr << "Only one end point can be defined." << std::endl;
            return false;
        }
        endPoint = new FreePlace();
        foundItem = endPoint;
        return true;
    }
    Tower tower;
    tower.m_mapItemChar = itemChar;
    if (Tools::contains(defineTowers, tower, compareTower)){
        foundItem = new Tower();
        foundItem->m_mapItemChar = itemChar;
        return true;
    }

    std::cerr << "Undefined char " << itemChar << " in map." << std::endl;
    return false;
}

