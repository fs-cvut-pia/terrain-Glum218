#ifndef FERRY_H
#define FERRY_H

#include "Path.h"

class ferry : public Path {
public:
    ferry(TerrainMap& m, std::string name, Point start, Point finish);

    bool find() override; 
};


#endif