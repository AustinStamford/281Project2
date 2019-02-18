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
        num_battles = 0;
    }
    
    Universe(unsigned int ng, unsigned int np){
        num_generals = ng;
        num_planets = np;
        num_battles = 0;
        trooplosses.resize((int)np);
    }
    
    void add_planet(int planet){
        if(find_planet(planet) > -1) return;
        planets.push_back(Planet(planet));
    }
    
    int find_planet(int planet){
        for(unsigned int i = 0; i < planets.size(); i++){
            if(planets[i].ID == planet) return i;
        }
        return -1;
    }
    
    unsigned int num_generals;
    unsigned int num_planets;
    unsigned int num_battles;
    vector<Planet> planets;
    vector<vector<int> > trooplosses;
    //index general
    //[0] jedi [1] sith [2] lost
    vector<vector<int> > generalstats;
};



#endif /* Universe_h */
