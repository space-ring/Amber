//
// Created by croissant on 25/02/2022.
//

#include "io.h"
#include <string>
#include <iostream>
#include <fstream>

using string = std::string;

string readFile(const string &path) {
    string file;
    string line;
    std::ifstream openfile(path);
    while (std::getline(openfile, line)) {
        file += line + "\n";
    }
    openfile.close();
    return file;
}

void writeFile(const string &path, string file) {
    std::ofstream openfile(path);
    openfile << file;
    openfile.close();
}

Shader *getShader(string vertex, string tessControl, string tessEval, string geometry, string fragment, string compute) {
    return nullptr;
}

Shader *getShader(string vertex, string geometry, string fragment) {
    return nullptr;
}

Shader *getShader(string vertex, string fragment) {
    return nullptr;
}

Texture *getTexture(string path) {
    return nullptr;
}
