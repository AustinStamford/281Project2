//
//  JediComp.h
//  Project2
//
//  Created by Austin Stamford on 2/14/19.
//  Copyright © 2019 Austin Stamford. All rights reserved.
//

#ifndef JediComp_h
#define JediComp_h
class JediComp{
public:
    bool operator ()(const Deployment i, const Deployment j){
        return i.FS > j.FS;
    }
};

#endif /* JediComp_h */
