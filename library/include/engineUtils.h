//
// Created by Chris on 22/06/2022.
//

#ifndef ENGINE_ENGINEUTILS_H
#define ENGINE_ENGINEUTILS_H

#include <string>
#include <vector>

namespace Amber {
    using string = std::string;

    std::vector<string> split(const string& s, char delimiter);

    std::vector<int> isplit(const string& s, char delimiter);

}


#endif //ENGINE_ENGINEUTILS_H
