#ifndef PLANE_H
#define PLANE_H

#include "Path.h"

class plane : public Path {
public:
    plane(TerrainMap& m, std::string name, Point start, Point finish);

    bool find() override; 
};


#endif