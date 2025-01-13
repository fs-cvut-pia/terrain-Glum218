#ifndef CAR_H
#define CAR_H

#include "Path.h"

class car : public Path {
public:
    car(TerrainMap& m, std::string name, Point start, Point finish);

    bool find() override; 
};


#endif