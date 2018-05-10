#include "MapCreator.h"


//----------------MapCreator-------------------------------

bool MapCreator::loadBestPath(const std::string &bestPathDefine) {
    int bestPath;
    std::stringstream iss(bestPathDefine);
    if (!getNextAttribute(iss, bestPath)) {
        std::cerr << "Bad BestPath value" << std::endl;
        return false;
    }
    if (bestPath == 0 || bestPath == 1){
        m_findAsBestPath = bestPath == 1;
        return true;
    }
    std::cerr << "Bad BestPath value" << std::endl;
    return false;
}

bool MapCreator::loadMoney(const std::string &moneyDefine) {
    int money;
    std::stringstream iss(moneyDefine);
    if (!getNextAttribute(iss, money)) {
        std::cerr << "Bad Money value" << std::endl;
        return false;
    }
    m_money = money;
    return true;
}

bool MapCreator::loadLives(const std::string &livesDefine) {
    int lives;
    std::stringstream iss(livesDefine);
    if (!getNextAttribute(iss, lives)) {
        std::cerr << "Bad lives value" << std::endl;
        return false;
    }
    m_lives = lives;
    return true;
}


bool MapCreator::loadStartEnd(const std::string & startOrEnd) {
    char c;
    int x, y;
    std::stringstream iss(startOrEnd);
    if (!getCharAttribute(iss, c)){
        return false;
    } if (!getNextAttribute(iss, x, ',')){
        return false;
    } if (!getNextAttribute(iss, y)){
        return false;
    }
    auto item = new FreePlace();
    item->m_mapItemChar = c;
    item->m_mapPositionX = x;
    item->m_mapPositionY = y;
    if (c == constants.QUEUE_IN_CHAR){
        delete m_startPoint;
        m_startPoint = item;
        return true;
    } if (c == constants.QUEUE_OUT_CHAR){
        delete m_endPoint;
        m_endPoint = item;
        return true;
    }
    delete item;
    return false;
}

bool MapCreator::loadTowers(const std::string &towerDefines) {
    std::stringstream iss(towerDefines);

    char towerChar;
    int towerPrice, towerAttack, towerRange;
    if (!getCharAttribute(iss, towerChar)){
        std::cerr << "Tower name can by only one char." << std::endl;
        return false;
    } if (!getNextAttribute(iss, towerPrice, ',')){
        std::cerr << "Bad Tower value" << std::endl;
        return false;
    } if (!getNextAttribute(iss, towerAttack, ',')){
        std::cerr << "Bad Tower value" << std::endl;
        return false;
    } if (!getNextAttribute(iss, towerRange)){
        std::cerr << "Bad Tower value" << std::endl;
        return false;
    } if (!checkMainCharMatch("Tower", towerChar)){
        std::cerr << "Bad Tower value" << std::endl;
        return false;
    }
    auto tower = new Tower(towerPrice, towerAttack, towerRange);
    tower->m_mapItemChar = towerChar;
    std::vector<Tower *>::const_iterator position;
    if (m_defineTowers.empty()){
        m_defineTowers.push_back(tower);
    } else if (m_defineTowers.size() >= constants.MAX_TOWERS){
        std::cerr << "You can define max " << constants.MAX_TOWERS << " towers." << std::endl;
        delete tower;
        return false;
    }
    else if (!Tools::find(m_defineTowers, *tower, position, compareTower)) {
        m_defineTowers.insert(position, tower);
    } else{
        std::cerr << "Tower with char " << tower->m_mapItemChar << " is already defined" << std::endl;
        delete tower;
        return false;
    }
    return true;
}

/*
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
    auto tower = new Tower(towerAttack, towerRange);
    tower->m_mapItemChar = towerChar;
    std::vector<Tower *>::const_iterator position;
    if (m_defineTowers.empty()){
        m_defineTowers.push_back(tower);
    } else if (m_defineTowers.size() >= MAX_TOWERS){
        std::cerr << "You can define max " << MAX_TOWERS << " towers." << std::endl;
        delete tower;
        return false;
    }
    else if (!Tools::find(m_defineTowers, *tower, position, compareTower)) {
        m_defineTowers.insert(position, tower);
    } else{
        std::cerr << "Tower with char " << tower->m_mapItemChar << " is already defined" << std::endl;
        delete tower;
        return false;
    }
    return true;
}
 */

bool MapCreator::loadEnemies(const std::string &enemyDefine) {
    char enemyChar;
    int enemyHp;

    std::stringstream iss(enemyDefine);

    if (!getCharAttribute(iss, enemyChar)){
        std::cerr << "Enemy name can by only one char." << std::endl;
        return false;
    } if (!getNextAttribute(iss, enemyHp)){
        std::cerr << "Bad Enemy value" << std::endl;
        return false;
    } if (!checkMainCharMatch("Enemy", enemyChar)){
        std::cerr << "Bad Enemy value" << std::endl;
        return false;
    }
    auto enemy = new Enemy(enemyHp);
    enemy->m_mapItemChar = enemyChar;
    std::vector<Enemy*>::const_iterator position;
    if (m_defineEnemies.empty()){
        m_defineEnemies.push_back(enemy);
    }
    else if (!Tools::find(m_defineEnemies, *enemy, position, compareEnemy)) {
        m_defineEnemies.insert(position, enemy);
    } else{
        std::cerr << "Enemy with char " << enemy->m_mapItemChar << " is already defined" << std::endl;
        delete enemy;
        return false;
    }
    return true;
}

bool MapCreator::loadEnemiesInMap(const std::string &enemyInMapDefine) {
    if (m_map.empty()){
        std::cerr << "Map has to be defined before " << constants.ENEMIES_IN_MAP << std::endl;
        return false;
    }
    int x, y;
    int currentHp;
    std::stringstream iss(enemyInMapDefine);
    if (getNextAttribute(iss, x, ',') && getNextAttribute(iss, y, ',') && getNextAttribute(iss, currentHp)){
        try {
            unsigned int u_x = static_cast<unsigned int>(x);
            unsigned int u_y = static_cast<unsigned int>(y);
            MapItem *mapItem = m_map.at(u_y).at(u_x);
            //Je rychlejší najít nepřítele přes mapu
            if (typeid(*mapItem) == typeid(Enemy)) {
                Enemy *enemy = dynamic_cast<Enemy *>(mapItem);
                enemy->m_hp = currentHp;
                return true;
            } else {
                std::cerr << constants.ENEMIES_IN_MAP << " bad item position" << std::endl;
                return false;
            }
        } catch (const std::out_of_range& e){
            std::cerr << constants.ENEMIES_IN_MAP << " bad item position" << std::endl;
            return false;
        }
    }
    std::cerr << constants.ENEMIES_IN_MAP << " load error" << std::endl;
    return false;
}

bool MapCreator::loadQueue(const std::string &queue) {
    for (char c : queue) {
        Enemy searchEnemy;
        searchEnemy.m_mapItemChar = c;
        std::vector<Enemy*>::const_iterator position;
        if (Tools::find(m_defineEnemies, searchEnemy, position, compareEnemy)){
            auto newEnemy = new Enemy(**position);
            m_enemiesQueue.push_back(newEnemy);
        } else{
            std::cerr << "Undefine enemy with char " << searchEnemy.m_mapItemChar << " in Queue" << std::endl;
            return false;
        }
    }
    return true;
}

bool MapCreator::parseString(const std::string &headLine, std::string &arg) {
    if (headLine == constants.MAP_DEFINE){
        return loadMap(arg);
    }

    //Remove white spaces
    arg.erase(std::remove_if(arg.begin(), arg.end(), isspace), arg.end());
    if (headLine == constants.TOWER_DEFINE){
        return loadTowers(arg);
    } else if (headLine == constants.ENEMIES_DEFINE){
        return loadEnemies(arg);
    } else if (headLine == constants.QUEUE_DEFINE){
        return loadQueue(arg);
    } else if (headLine == constants.MONEY_DEFINE){
        return loadMoney(arg);
    } else if (headLine == constants.LIVES_DEFINE){
        return loadLives(arg);
    } else if (headLine == constants.START_END){
        return loadStartEnd(arg);
    } else if (headLine == constants.ENEMIES_IN_MAP){
        return loadEnemiesInMap(arg);
    } else if (headLine == constants.BEST_PATH){
        return loadBestPath(arg);
    }
    return false;
}

bool MapCreator::loadGameFile(const std::string &gameDefinePath) {
    std::cout << "Loading game from file: " << gameDefinePath << std::endl;
    std::string line, headLine;
    std::ifstream mapFile (gameDefinePath);
    if (mapFile.is_open()) {
        {
            try {
                while (getline(mapFile, line)) {
                    if (line == constants.TOWER_DEFINE ||
                        line == constants.ENEMIES_DEFINE ||
                        line == constants.QUEUE_DEFINE ||
                        line == constants.MAP_DEFINE ||
                        line == constants.BEST_PATH ||
                        line == constants.MONEY_DEFINE ||
                        line == constants.LIVES_DEFINE ||
                        line == constants.START_END ||
                        line == constants.ENEMIES_IN_MAP) {
                        if (line == constants.MAP_DEFINE && !m_map.empty()) {
                            std::cerr << "Only one m_map can be defined" << std::endl;
                            mapFile.close();
                            return false;
                        }
                        headLine = line;
                    } else if (!headLine.empty() && !line.empty() && !parseString(headLine, line)) {
                        mapFile.close();
                        std::cerr << "Illegal config file structure" << std::endl;
                        return false;
                    } else if (headLine.empty() && !line.empty()){
                        mapFile.close();
                        std::cerr << "Illegal config file structure" << std::endl;
                        return false;
                    }
                }
            } catch (std::invalid_argument& e){
                std::cerr << "Load define file error" << std::endl;
                return false;
            }
            mapFile.close();
        }
        return finalCheck();
    }
    std::cerr << "Cannot open input file." << std::endl;
    return false;
}

bool MapCreator::checkMainCharMatch(const std::string &item, const char &c) {
    if (c == constants.WALL_CHAR){
        std::cerr << item <<" cant be defined by " << constants.WALL_CHAR << std::endl;
        return false;
    } else if (c == constants.QUEUE_IN_CHAR){
        std::cerr << item <<" cant be defined by " << constants.QUEUE_IN_CHAR << std::endl;
        return false;
    } else if (c == constants.QUEUE_OUT_CHAR){
        std::cerr << item <<" cant be defined by " << constants.QUEUE_OUT_CHAR << std::endl;
        return false;
    }
    return true;
}

bool MapCreator::loadMap(const std::string &mapLine) {
    std::vector<MapItem*> mapRow;
    size_t y = m_map.size();
    for (size_t x = 0; x < mapLine.length(); x++) {
        char itemChar = mapLine.at(x);
        MapItem* mapItem = nullptr;
        if (createMapItem(itemChar, mapItem)){
            mapItem->m_mapItemChar = itemChar;
            mapItem->m_mapPositionX = static_cast<int>(x);
            mapItem->m_mapPositionY = static_cast<int>(y);
            mapRow.push_back(mapItem);
            if (!createStartOrEnd(*mapItem)){
                m_map.push_back(mapRow);
                return false;
            }
        } else{
            m_map.push_back(mapRow);
            return false;
        }
    }
    m_map.push_back(mapRow);
    if (!m_map.empty() && m_map[m_map.size() - 1].size() != mapRow.size()){
        std::cerr << "Map can be only square or rectangle." << std::endl;
        return false;
    }
    return true;
}

bool MapCreator::createStartOrEnd(const MapItem &item) {
    char itemChar = item.m_mapItemChar;
    if (itemChar == constants.QUEUE_IN_CHAR) {
        if (m_startPoint != nullptr){
            std::cerr << "Only one start point can be defined." << std::endl;
            return false;
        }
        m_startPoint = new FreePlace(item);
        return true;
    } else if (itemChar == constants.QUEUE_OUT_CHAR){
        if (m_endPoint != nullptr){
            std::cerr << "Only one end point can be defined." << std::endl;
            return false;
        }
        m_endPoint = new FreePlace(item);
        return true;
    }
    return true;
}

bool MapCreator::compareTower(const Tower *t1, const Tower *t2) {
    return t1->m_mapItemChar < t2->m_mapItemChar;
}

bool MapCreator::compareEnemy(const Enemy *e1, const Enemy *e2) {
    return e1->m_mapItemChar < e2->m_mapItemChar;
}

bool MapCreator::createMapItem(const char& itemChar, MapItem*& foundItem) {
    if (itemChar == constants.WALL_CHAR){
        foundItem = new Wall();
        return true;
    } else if (itemChar == constants.FREE_PLACE_CHAR ||
               itemChar == constants.QUEUE_IN_CHAR ||
               itemChar == constants.QUEUE_OUT_CHAR){
        foundItem = new FreePlace();
        return true;
    }
    Tower tower;
    tower.m_mapItemChar = itemChar;
    std::vector<Tower*>::const_iterator resultTower;
    if (Tools::find(m_defineTowers, tower, resultTower, compareTower)){
        auto mapTower = new Tower(**resultTower);
        foundItem = mapTower;
        m_towersInMap.push_back(mapTower);
        return true;
    }

    Enemy enemy;
    enemy.m_mapItemChar = itemChar;
    std::vector<Enemy*>::const_iterator resultEnemy;
    if (Tools::find(m_defineEnemies, enemy, resultEnemy, compareEnemy)){
        auto mapEnemy = new Enemy(**resultEnemy);
        foundItem = mapEnemy;
        m_enemiesInMap.push_back(mapEnemy);
        return true;
    }

    std::cerr << "Undefined char " << itemChar << " in m_map." << std::endl;
    delete foundItem;
    return false;
}

bool MapCreator::finalCheck() {
    if (m_startPoint == nullptr){
        std::cerr << "Start point is not set." << std::endl;
        return false;
    } if (m_endPoint == nullptr){
        std::cerr << "End point is not set." << std::endl;
        return false;
    } if (m_defineEnemies.empty()){
        std::cerr << "No enemies defined." << std::endl;
        return false;
    } if (m_defineTowers.empty()){
        std::cerr << "No enemies defined." << std::endl;
        return false;
    }
    return true;
}

bool MapCreator::getCharAttribute(std::stringstream &iss, char &result) {
    std::string temp;
    std::getline(iss, temp, ',');
    if (temp.size() != 1){
        return false;
    }
    result = temp.at(0);
    return true;
}

bool MapCreator::getNextAttribute(std::stringstream &iss, int &result) {
    return getNextAttribute(iss, result, ' ');
}

bool MapCreator::getNextAttribute(std::stringstream &iss, int &result, char delimiter) {
    std::string temp;
    std::string::size_type sz;
    std::getline(iss, temp, delimiter);
    if (temp.empty()){
        return false;
    }
    if (temp.size() > 8){
        std::cerr << "Attribute is too big" << std::endl;
        return false;
    }
    int attribute = std::stoi(temp, &sz);
    if (attribute < 0){
        return false;
    }
    if (!temp.substr(sz).empty()){
        return false;
    }
    result = attribute;
    return true;
}


