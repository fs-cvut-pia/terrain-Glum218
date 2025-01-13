#ifndef SHIP_H
#define SHIP_H

#include "Path.h"

class ship : public Path {
public:
    ship(TerrainMap& m, std::string name, Point start, Point finish);

    bool find() override; 
};


#endif