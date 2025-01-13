#include "ferry.h"
#include "TerrainMap.h"
#include "Path.h"
#include <iostream>
#include <deque>
#include <algorithm>

ferry::ferry(TerrainMap& m, std::string name, Point start, Point finish)
    : Path(m, name, start, finish) {}

bool ferry::find() {

    std::deque<Point> pointQueue;                           // queue for processing
    Matrix<int> checkMatrix(map.nx,map.ny);                 // processed point evidence
    Matrix<int> queueMatrix(map.nx,map.ny);                 // queued point evidence
    Matrix<double> distanceMatrix(map.nx,map.ny);           // current shortest path evidence; meaning modified to fictional travel time
    for (int x = 0; x < map.nx; ++x) {                      
        for (int y = 0; y < map.ny; ++y) {
            distanceMatrix(Point(x,y)) = std::numeric_limits<double>::infinity();
        }
    }
    distanceMatrix(start) = 0;                      
    Matrix<Point> previousPointMatrix(map.nx,map.ny);       // current shortest path previous point

    pointQueue.push_front(start);
    checkMatrix(start) = 1;

    while(!pointQueue.empty()) {
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
            if (nearbyPoints[i].x >= 0 && nearbyPoints[i].y >= 0 && nearbyPoints[i].x < map.nx && nearbyPoints[i].y < map.ny ) {
            // checking for valid coordinates                   
                if (queueMatrix(nearbyPoints[i]) == 0 && checkMatrix(nearbyPoints[i]) == 0) {
                // checking for points already processed or set for processing
                    pointQueue.push_front(nearbyPoints[i]);
                    queueMatrix(nearbyPoints[i]) = 1;
                    // adding point to queue                                                                                   
                }
                if (map.alt(nearbyPoints[i]) < 0) {
                // checking for water
                    if ( distanceMatrix(nearbyPoints[i]) > (distanceMatrix(current_point) + 4*(nearbyPoints[i] - current_point).length())){
                    // checking if better travel time can be achieved through current_point
                        distanceMatrix(nearbyPoints[i]) = distanceMatrix(current_point) + 4*(nearbyPoints[i] - current_point).length();
                        // changing best path
                        previousPointMatrix(nearbyPoints[i]) = current_point;
                        // saving information about new best path previous point
                        if (queueMatrix(nearbyPoints[i]) == 0) {
                        // checking if point is already set for recalculation
                            pointQueue.push_front(nearbyPoints[i]);
                            // adding point for recalculation
                        }      
                    }
                }                                                                 
                else if (abs(map.alt(nearbyPoints[i]) - map.alt(current_point))/(1000*((nearbyPoints[i] - current_point).length())) < 0.06) {
                // checking for incline on land
                    if ( distanceMatrix(nearbyPoints[i]) > (distanceMatrix(current_point) + (nearbyPoints[i] - current_point).length())){
                    // checking if better travel time can be achieved through current_point
                        distanceMatrix(nearbyPoints[i]) = distanceMatrix(current_point) + (nearbyPoints[i] - current_point).length();
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