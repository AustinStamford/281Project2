//
//  Planet.h
//  Project2
//
//  Created by Austin Stamford on 2/14/19.
//  Copyright © 2019 Austin Stamford. All rights reserved.
//
#include <queue>
#include "Deployment.h"
#include "JediComp.h"
#include "SithComp.h"
#include "Coord.h"

using namespace std;

#ifndef Planet_h
#define Planet_h

class Planet {
    
public:
    Planet(){
        ID = 0;
    }
    
    Planet(unsigned int x){
        ID = x;
    }
    
    void add_depl(Deployment d, bool jedi){
        if(jedi){
            jedi_depls.push(d);
        }
        else sith_depls.push(d);
    }
    
    int ID;
    priority_queue<Deployment, vector<Deployment>, JediComp> jedi_depls;
    priority_queue<Deployment, vector<Deployment>, SithComp> sith_depls;
    //ID, FS
    //init to -1,-1
    Coord jedi_attack;
    Coord sith_attack;
    Coord jedi_attack_alt;
    
    Coord jedi_ambush;
    Coord sith_ambush;
    Coord sith_ambush_alt;
    
};
#endif /* Planet_h */
