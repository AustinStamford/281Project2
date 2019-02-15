//
//  Deployment.h
//  Project2
//
//  Created by Austin Stamford on 2/14/19.
//  Copyright © 2019 Austin Stamford. All rights reserved.
//

#ifndef Deployment_h
#define Deployment_h

class Deployment {
    public:
        Deployment(unsigned int t, unsigned int g, unsigned int p,
                bool j, unsigned int f, unsigned int q){
            timestamp = t;
            gen = g;
            planet = p;
            jedi = j;
            FS = f;
            quantity = q;
        }
    
        Deployment(){
            timestamp = 0;
            gen = 0;
            planet = 0;
            jedi = true;
            FS = 0;
            quantity = 0;
        }
    
    unsigned int timestamp;
    unsigned int gen;
    unsigned int planet;
    bool jedi;
    unsigned int FS;
    unsigned int quantity;
};

#endif /* Deployment_h */
