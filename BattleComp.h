//
//  BattleComp.h
//  Project2
//
//  Created by Austin Stamford on 2/16/19.
//  Copyright © 2019 Austin Stamford. All rights reserved.
//

#ifndef BattleComp_h
#define BattleComp_h

class BattleComp{
public:
    bool operator ()(const Deployment i, const Deployment j){
        return i.FS <= j.FS;
    }
};


#endif /* BattleComp_h */
