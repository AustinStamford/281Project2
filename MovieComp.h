//
//  MovieComp.h
//  Project2
//
//  Created by Austin Stamford on 2/17/19.
//  Copyright © 2019 Austin Stamford. All rights reserved.
//

#ifndef MovieComp_h
#define MovieComp_h

class MovieComp{
public:
    bool operator ()(const Deployment i, const Deployment j){
        if(i.FS != j.FS) return i.FS < j.FS;
        else return i.ID > j.ID;
    }
};

#endif /* MovieComp_h */
