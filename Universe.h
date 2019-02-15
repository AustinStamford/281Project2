//
//  Universe.h
//  Project2
//
//  Created by Austin Stamford on 2/14/19.
//  Copyright © 2019 Austin Stamford. All rights reserved.
//

#include "Planet.h"

using namespace std;

#ifndef Universe_h
#define Universe_h

class Universe {
    
public:
    Universe(){
        num_generals = 0;
        num_planets = 0;
    }
    
    Universe(unsigned int ng, unsigned int np){
        num_generals = ng;
        num_planets = np;
    }
    
    void add_planet(int planet){
        if(find_planet(planet) > -1) return;
        planets.push_back(Planet(planet));
    }
    
    int find_planet(int planet){
        for(int i = 0; i < planets.size(); i++){
            if(planets[i].ID == planet) return i;
        }
        return -1;
    }
    
    unsigned int num_generals;
    unsigned int num_planets;
    vector<Planet> planets;
};



#endif /* Universe_h */
