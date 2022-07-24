//
// Created by Chris on 22/06/2022.
//

#include "engineUtils.h"
#include <sstream>

namespace Amber {
    std::vector<string> split(const string& s, char delimiter) {
        std::stringstream ss(s);
        std::vector<string> elems;
        string item;
        while (std::getline(ss, item, delimiter)) {
            elems.push_back(item);
        }
        return elems;
    }

    std::vector<int> isplit(const string& s, char delimiter) {
        std::stringstream ss(s);
        std::vector<int> elems;
        string item;
        while (std::getline(ss, item, delimiter)) {
            elems.push_back(std::stoi(item));
        }
        return elems;
    }

}