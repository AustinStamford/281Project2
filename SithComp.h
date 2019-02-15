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
        return i.FS < j.FS;
    }
};

#endif /* SithComp_h */
