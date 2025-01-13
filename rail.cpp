#include "rail.h"
#include "TerrainMap.h"
#include "Path.h"
#include <iostream>
#include <deque>
#include <algorithm>

rail::rail(TerrainMap& m, std::string name, Point start, Point finish)
    : Path(m, name, start, finish) {}

bool rail::find() {

    std::deque<Point> pointQueue;                           // queue for processing
    Matrix<int> checkMatrix(map.nx,map.ny);                 // processed point evidence
    Matrix<int> queueMatrix(map.nx,map.ny);                 // queued point evidence
    Matrix<double> eleMatrix(map.nx,map.ny);                // distance matrix changed to matrix of lowest possible total ascent + descent
    for (int x = 0; x < map.nx; ++x) {
        for (int y = 0; y < map.ny; ++y) {
            eleMatrix(Point(x,y)) = std::numeric_limits<double>::infinity();
        }
    }
    eleMatrix(start) = 0;
    Matrix<Point> previousPointMatrix(map.nx,map.ny);       // current best path previous point

    pointQueue.push_front(start);
    checkMatrix(start) = 1;

    while(pointQueue.size()>0) {
        Point current_point = pointQueue.back();
        pointQueue.pop_back();
        
        std::vector<Point> nearbyPoints = {
        {current_point + Point(1, 1)}, 
        {current_point + Point(1, 0)}, 
        {current_point + Point(1, -1)},
        {current_point + Point(0, 1)},
        {current_point + Point(0, -1)},
        {current_point + Point(-1, 1)},
        {current_point + Point(-1, 0)},
        {current_point + Point(-1, -1)},
        };
 
        for (int i = 0; i < 8; i++) {
            if (nearbyPoints[i].x >= 0 && nearbyPoints[i].y >= 0 && nearbyPoints[i].x < map.nx && nearbyPoints[i].y < map.ny) {
            // checking for valid coordinates
                if (map.alt(nearbyPoints[i]) > 0 && (abs(map.alt(nearbyPoints[i]) - map.alt(current_point))/(1000*((nearbyPoints[i] - current_point).length()))) < 0.04) {
                // checking for land and incline                 
                    if (queueMatrix(nearbyPoints[i]) == 0 && checkMatrix(nearbyPoints[i]) == 0) {
                    // checking for points already processed or set for processing
                        pointQueue.push_front(nearbyPoints[i]);
                        queueMatrix(nearbyPoints[i]) = 1;
                        // adding point to queue
                    }
                
                    if ( eleMatrix(nearbyPoints[i]) > (eleMatrix(current_point) + abs(map.alt(nearbyPoints[i]) - map.alt(current_point)))){
                    // checking if better path can be achieved through current_point
                        eleMatrix(nearbyPoints[i]) = eleMatrix(current_point) + abs(map.alt(nearbyPoints[i]) - map.alt(current_point));
                        previousPointMatrix(nearbyPoints[i]) = current_point;
                        // changing best path
                        if (queueMatrix(nearbyPoints[i]) == 0) {
                        // checking if point is already set for recalculation
                            pointQueue.push_front(nearbyPoints[i]);
                            // adding point for recalculation
                        }      
                    }                                                                
                } 
            checkMatrix(current_point) = 1;
            queueMatrix(current_point) = 0; 
            
            }
        }        
    }
    if (checkMatrix(finish) == 1){          // path reconstruction
        Point step = finish;
        while (step != start) {
            path.push_back(step);
            step = previousPointMatrix(step);
        }
        path.push_back(start);
        reverse(path.begin(), path.end());
    }
return true;
}