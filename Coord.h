//
//  Coord.h
//  Project2
//
//  Created by Austin Stamford on 2/17/19.
//  Copyright © 2019 Austin Stamford. All rights reserved.
//

#ifndef Coord_h
#define Coord_h

class Coord {
    
public:
    Coord(const int x, const int y, const int z){
        ID = x;
        FS = y;
        time = z;
    }
    
    Coord(){
        ID = -1;
        FS = -1;
        time = -1;
    }
    
    int ID;
    int FS;
    int time;
};

bool operator==(const Coord p, const Coord q){
    return p.ID == q.ID && p.FS == q.FS && p.time == q.time;
}

#endif /* Coord_h */
