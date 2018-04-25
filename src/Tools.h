#ifndef DEVELOP_TOOLS_H
#define DEVELOP_TOOLS_H

#include <iostream>
#include <vector>
#include <algorithm>

class Constants{
public:

    //char* cant be init static
    const char* MONEY_DEFINE = "Money";
    const char* LIVES_DEFINE = "Lives";
    const char* TOWER_DEFINE = "Towers";
    const char* ENEMIES_DEFINE = "Enemies";
    const char* QUEUE_DEFINE = "Queue";
    const char* MAP_DEFINE = "Map";
    const char* ENEMIES_IN_MAP = "EnemiesInMap";
    const char* START_END = "StartEnd";
    const char FREE_PLACE_CHAR = ' ';
    const char WALL_CHAR = '#';
    const char QUEUE_IN_CHAR = 'i';
    const char QUEUE_OUT_CHAR = 'o';
    const unsigned int MAX_TOWERS = 8;
};


namespace Tools {

    template <typename _T>
    bool find(const std::vector<_T*>& list, _T& item, typename std::vector<_T*>::const_iterator& resultPosition) {
        if (list.empty()){
            return false;
        }

        resultPosition = std::lower_bound(list.begin(), list.end(), &item);
        if (resultPosition == list.end()){
            return false;
        }
        return item < *resultPosition;
    }

    template <typename _T>
    bool contains(const std::vector<_T*>& list, _T& item, bool (*compare)(const _T*, const _T*)) {
        if (list.empty()){
            return false;
        }

        auto resultPosition = std::lower_bound(list.begin(), list.end(), &item, compare);
        if (resultPosition == list.end()){
            return false;
        }
        return !compare( &item, *resultPosition);
    }

    template <typename _T>
    bool find(const std::vector<_T*>& list, _T& item, typename std::vector<_T*>::const_iterator& resultPosition,
                         bool (*compare)(const _T*, const _T*)) {
        if (list.empty()){
            return false;
        }

        resultPosition = std::lower_bound(list.begin(), list.end(), &item, compare);
        if (resultPosition == list.end()){
            return false;
        }
        return !compare( &item, *resultPosition);
    }

    class IlegalStateException{

    };
}

#endif //DEVELOP_TOOLS_H
