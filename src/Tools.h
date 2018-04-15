#ifndef DEVELOP_TOOLS_H
#define DEVELOP_TOOLS_H

#include <iostream>
#include <vector>
#include "UiItems.h"

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

}

#endif //DEVELOP_TOOLS_H
