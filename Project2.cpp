//
//  main.cpp
//  Project2
//
//  Created by Austin Stamford on 2/14/19.
//  Copyright © 2019 Austin Stamford. All rights reserved.
//

#include <iostream>
#include <getopt.h>
#include "xcode_redirect.hpp"
#include "P2random.h"
#include "Universe.h"

using namespace std;

void get_options(int argc, char** argv);
void get_input(Universe &universe);
void DL(Universe &universe);
void PR(Universe &universe);
void match(Planet &p);

char INPUT_MODE;
unsigned int CURRENT_TIMESTAMP;

int main(int argc, char** argv) {
    /******DONT*TOUCH*********/
    xcode_redirect(argc, argv);
    get_options(argc, argv);
    /*************************/
    Universe universe;
    //get input
    get_input(universe);
    
    return 0;
}

//let's make a function that deals with input
void get_options(int argc, char** argv) {
    int option_index = 0, option = 0;
    
    // Don't display getopt error messages about options
    opterr = false;
    
    // use getopt to find command line options
    struct option longOpts[] = { { "verbose", no_argument, nullptr, 'v' },
        { "median", no_argument, nullptr, 'm' },
        { "general-eval", no_argument, nullptr, 'g' },
        { "watcher", no_argument, nullptr, 'w' },
        { nullptr, 0, nullptr, '\0' }
    };
    
    //process options
    while ((option = getopt_long(argc, argv, "vmgw", longOpts, &option_index)) != -1) {
        switch (option) {
            case 'v':
                //output = 'v';
                break;
                
            case 'm':
                //output = 'm';
                break;
                
            case 'g':
                // output = 'g';
                break;
                
            case 'w':
                // output = 'w';
                break;
                
            default:
                std::cerr << "Yikes" << std::endl;
                exit(1);
        }
    }
    
}

void get_input(Universe &universe){
    CURRENT_TIMESTAMP = 0;
    //input occurs in the following way
    string newline, str;
    char c = 'x';
    //COMMENT: <COMMENT> ignore
    getline(cin, newline);
     //MODE: <INPUT_MODE> DL or PR
    while(c != 'D' && c != 'P'){
        cin >> c;
    }
    INPUT_MODE = c;
    //NUM_GENERALS: <NUM_GENERALS>
    while(!(c >= '0' && c <= '9')){
        cin >> c;
    }
    
    newline = "";
    
    while(c >= '0' && c <= '9'){
        newline += c;
        cin >> c;
    }
    universe.num_generals = stoi(newline);
    
    getline(cin, newline);
    str = "";
    
    for(auto i : newline){
        if(i >= '0' && i <= '9') str += i;
    }
    universe.num_planets = stoi(str);
    
    //now deal with actual input - let's implement DL first
    if(INPUT_MODE == 'D'){
        DL(universe);
    }
    else PR(universe);
}

void DL(Universe &universe){
    int i = 0;
    int count = 0;
    string newline;
    string temp = "";
    unsigned int time, gen, planet, FS, quantity;
    bool j;
    //read in all lines
    while(getline(cin, newline)){
        //read in timestamp
        count++;
        cout << "LOOP : " << count << endl;
        while(newline[i] != ' '){
            temp += newline[i];
            i++;
        }
        //MEDIAN HERE
        time = stoi(temp);
        if(time != CURRENT_TIMESTAMP) CURRENT_TIMESTAMP = time;
        temp = "";
        //read in jedi
        i++;
        if(newline[i] == 'S') j = false;
        else j = true;
        //advance to next info
        while(newline[i] != 'G') i++;
        i++;
        //General
        while(newline[i] != ' '){
            temp += newline[i];
            i++;
        }
        gen = stoi(temp);
        temp = "";
        //advance to Planet
        while(newline[i] != 'P') i++;
        i++;
        //Planet
        while(newline[i] != ' '){
            temp += newline[i];
            i++;
        }
        planet = stoi(temp);
        
        universe.add_planet(planet);
        
        temp = "";
        //advance to FS
        while(newline[i] != 'F') i++;
        i++;
        //Force Senitivity
        while(newline[i] != ' '){
            temp += newline[i];
            i++;
        }
        FS = stoi(temp);
        temp = "";
        
        //advance to NUM_TROOPS
        while(newline[i] != '#') i++;
        i++;
        //NUM_TROOPS
        while(newline[i] != ' '){
            temp += newline[i];
            i++;
        }
        quantity = stoi(temp);
        temp = "";
        
        //push back deployment
        i = universe.find_planet(planet);
        universe.planets[i].add_depl(Deployment(time, gen, planet, j, FS, quantity), j);
        match(universe.planets[i]);
        i = 0;
    }
}

void PR(Universe &universe){
    //UNDER CONSTRUCTION
}

void match(Planet &p){
    if(p.jedi_depls.empty() || p.sith_depls.empty()) return;
    SithComp s;
    Deployment d;
    int diff = 0;
    //check whether a battle will happen
    while(s(p.jedi_depls.top(), p.sith_depls.top())){
        //positive denotes JEDI win, neg denotes SITH win
        diff = p.jedi_depls.top().quantity - p.sith_depls.top().quantity;
        
        cout << "BATTLE! diff : " << diff << endl;
        
        //JEDI WIN
        if(diff > 0){
            p.sith_depls.pop();
            d = p.jedi_depls.top();
            p.jedi_depls.pop();
            d.quantity -= diff;
            p.jedi_depls.push(d);
        }
        //SITH WIN
        else if(diff < 0){
            p.jedi_depls.pop();
            d = p.sith_depls.top();
            p.sith_depls.pop();
            d.quantity += diff;
            p.sith_depls.push(d);
        }
        //DRAW
        else{
            p.jedi_depls.pop();
            p.sith_depls.pop();
        }
        if(p.jedi_depls.empty() || p.sith_depls.empty()) return;
    }
    cout << "No more battles today" << endl;
    cout << "TIME : " << CURRENT_TIMESTAMP << endl;
}
