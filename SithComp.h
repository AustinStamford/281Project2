//
//  SithComp.h
//  Project2
//
//  Created by Austin Stamford on 2/14/19.
//  Copyright © 2019 Austin Stamford. All rights reserved.
//

#ifndef SithComp_h
#define SithComp_h
class SithComp{
public:
    bool operator ()(const Deployment i, const Deployment j){
        if(i.FS == j.FS){
            if(i.timestamp != j.timestamp) return i.timestamp > j.timestamp;
            else return i.ID > j.ID;
        }
        else return i.FS < j.FS;
    }
};

#endif /* SithComp_h */
