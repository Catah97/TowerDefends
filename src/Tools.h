#ifndef DEVELOP_TOOLS_H
#define DEVELOP_TOOLS_H

#include <iostream>
#include <vector>
#include <algorithm>

/**
 * Class that save constants for save and define files.
 */
class Constants{
public:

    const char* BEST_PATH = "BestPath";
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

    /**
     * Generic functions find item in list by binary search
     * @tparam _T item type
     * @param list sorted list
     * @param item search item
     * @param resultPosition position of found item, or end iterator of list (item is not in list)
     * @return TRUE if item is in list, else FALSE
     */
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

    /**
     * Generic functions find item in list by binary search
     * @tparam _T item type
     * @param list sorted list
     * @param item search item
     * @param resultPosition position of found item, or end iterator of list (item is not in list)
     * @param compare compare function(should by use for sort list too)
     * @return TRUE if item is in list, else FALSE
     */
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

    /**
     * Generic functions to detect if item is in list by binary search
     * @tparam _T item type
     * @param list sorted list
     * @param item search item
     * @param compare compare function(should by use for sort list too)
     * @return TRUE if item is in list, else FALSE
     */
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
}

#endif //DEVELOP_TOOLS_H
