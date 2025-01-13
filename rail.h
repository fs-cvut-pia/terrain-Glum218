#ifndef RAIL_H
#define RAIL_H

#include "Path.h"

class rail : public Path {
public:
    rail(TerrainMap& m, std::string name, Point start, Point finish);

    bool find() override; 
};


#endif