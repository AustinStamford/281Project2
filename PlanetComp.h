//
//  PlanetComp.h
//  Project2
//
//  Created by Austin Stamford on 2/14/19.
//  Copyright © 2019 Austin Stamford. All rights reserved.
//

#ifndef PlanetComp_h
#define PlanetComp_h

class Planetcomp{
    public:
    bool operator ()(const Planet i, const Planet j){
        return i.ID < j.ID;
    }
};

#endif /* PlanetComp_h */
