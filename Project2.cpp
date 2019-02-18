//
//  main.cpp
//  Project2
//
//  Created by Austin Stamford on 2/14/19.
//  Copyright © 2019 Austin Stamford. All rights reserved.
//

#include <iostream>
#include <getopt.h>
#include <math.h>
#include "xcode_redirect.hpp"
#include "P2random.h"
#include "Universe.h"
#include "BattleComp.h"
#include "MovieComp.h"

using namespace std;

void get_options(int argc, char** argv);
void get_input(Universe &universe);
void DL(Universe &universe);
void PR(Universe &universe);
void match(Planet &p, Universe &u);
void verbose_out(int sithgen, int jedigen, int troops, int pl);
void EOD_output(Universe &universe);
void median_pq(vector<int> &v, unsigned int p);
void median_output(Universe &u);
void general_eval_output(Universe &u);
void movie_watcher_output(Universe &u);
void ambush(Planet &p, unsigned int i);
void attack(Planet &p, unsigned int i);
void ambush_assess(Planet &p, const Deployment &d);
void attack_assess(Planet &p, const Deployment &d);
void PR_input(Universe &u, stringstream& ss);

void print(Planet &p);

char INPUT_MODE;
unsigned int CURRENT_TIMESTAMP;

bool VERBOSE = false;
bool MEDIAN = false;
bool GENEVAL = false;
bool MOVIE = false;

int main(int argc, char** argv) {
    /******DONT*TOUCH*********/
    xcode_redirect(argc, argv);
    get_options(argc, argv);
    cout << "Deploying troops...\n";
    /*************************/
    Universe universe;
    //get input
    get_input(universe);
    
    EOD_output(universe);
    if(GENEVAL) general_eval_output(universe);
    if(MOVIE) movie_watcher_output(universe);
    
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
                //verbose
                VERBOSE = true;
                break;
                
            case 'm':
                //median
                MEDIAN = true;
                break;
                
            case 'g':
                //gen-eval
                GENEVAL = true;
                break;
                
            case 'w':
                //movie-watcher
                MOVIE = true;
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
    while(c != ':'){
        cin >> c;
    }
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
    universe.generalstats.resize(universe.num_generals);
    
    getline(cin, newline);
    str = "";
    
    for(auto i : newline){
        if(i >= '0' && i <= '9') str += i;
    }
    universe.num_planets = stoi(str);
    universe.trooplosses.resize(universe.num_planets);
    
    //now deal with actual input - let's implement DL first
    if(INPUT_MODE == 'D'){
        DL(universe);
    }
    else PR(universe);
}

void DL(Universe &universe){
    int i = 0;
    unsigned int count = 0;
    string newline;
    string temp = "";
    unsigned int time, gen, planet, FS, quantity;
    bool j;
    //read in all lines
    while(getline(cin, newline)){
        //read in timestamp
        count++;
        //cout << "LOOP : " << count << endl;
        while(newline[i] != ' '){
            temp += newline[i];
            i++;
        }
        //MEDIAN HERE
        time = stoi(temp);
        
        if(time != CURRENT_TIMESTAMP){
            if(MEDIAN) median_output(universe);
            CURRENT_TIMESTAMP = time;
        }
        
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
        
        if(universe.generalstats[gen].empty()){
            if(j){
                universe.generalstats[gen] = {(int)quantity, 0, 0};
            }
            else{
                universe.generalstats[gen] = {0, (int)quantity, 0};
            }
        }
        else{
            if(j) universe.generalstats[gen][0] += quantity;
            else universe.generalstats[gen][1] += quantity;
        }
        
        //push back deployment
        i = universe.find_planet(planet);
        universe.planets[i].add_depl(Deployment(time, gen, planet, j, FS, quantity, count), j);
        match(universe.planets[i], universe);
        
        
        ambush_assess(universe.planets[i], Deployment(time, gen, planet, j, FS, quantity, count));
        attack_assess(universe.planets[i], Deployment(time, gen, planet, j, FS, quantity, count));
        //        if(j) universe.planets[i].jedi_m.push_back(Deployment(time, gen, planet, j, FS, quantity, count));
        //        else universe.planets[i].sith_m.push_back(Deployment(time, gen, planet, j, FS, quantity, count));
        
        i = 0;
    }
    if(MEDIAN) median_output(universe);
}

void PR(Universe &universe){
    unsigned int seed, num_depl, rate;
    string newline, temp;
    stringstream ss;
    getline(cin, newline);
    for(auto i : newline){
            if(i >= '0' && i <= '9') temp += i;
    }
    seed = stoi(temp);
    temp = "";
    
    getline(cin, newline);
    for(auto i : newline){
        if(i >= '0' && i <= '9') temp += i;
    }
    num_depl = stoi(temp);
    temp = "";
    
    getline(cin, newline);
    for(auto i : newline){
        if(i >= '0' && i <= '9') temp += i;
    }
    rate = stoi(temp);
    temp = "";
    
    P2random::PR_init(ss, seed, universe.num_generals, universe.num_planets, num_depl, rate);
    PR_input(universe, ss);
}

void match(Planet &p, Universe &u){
    if(p.jedi_depls.empty() || p.sith_depls.empty()) return;
    BattleComp s;
    Deployment d;
    int diff = 0;
    int sithgen, jedigen, troopslost;
    //check whether a battle will happen
    while(s(p.jedi_depls.top(), p.sith_depls.top())){
        
        /****DEBUG HELP**/
        //print(p);
        /****************/
        //Battle happens, so increment count
        u.num_battles++;
        //positive denotes JEDI win, neg denotes SITH win
        diff = p.jedi_depls.top().quantity - p.sith_depls.top().quantity;
        
        sithgen = p.sith_depls.top().gen;
        jedigen = p.jedi_depls.top().gen;
        
        
        
        //JEDI WIN
        if(diff > 0){
            troopslost = 2 * p.sith_depls.top().quantity;
            p.sith_depls.pop();
            d = p.jedi_depls.top();
            p.jedi_depls.pop();
            d.quantity -= (troopslost / 2);
            p.jedi_depls.push(d);
            if(VERBOSE) verbose_out(sithgen, jedigen, troopslost, p.ID);
        }
        //SITH WIN
        else if(diff < 0){
            troopslost = 2 * p.jedi_depls.top().quantity;
            p.jedi_depls.pop();
            d = p.sith_depls.top();
            p.sith_depls.pop();
            d.quantity -= (troopslost / 2);
            p.sith_depls.push(d);
            //need to find a way to preserve order here
            if(VERBOSE) verbose_out(sithgen, jedigen, troopslost, p.ID);
        }
        //DRAW
        else{
            troopslost = 2 * p.jedi_depls.top().quantity;
            p.jedi_depls.pop();
            p.sith_depls.pop();
            if(VERBOSE) verbose_out(sithgen, jedigen, troopslost, p.ID);
        }
        
        u.trooplosses[p.ID].push_back(troopslost);
        
        u.generalstats[jedigen][2] += (troopslost / 2);
        u.generalstats[sithgen][2] += (troopslost / 2);
        
        if(p.jedi_depls.empty() || p.sith_depls.empty()) return;
    }
}

void verbose_out(int sithgen, int jedigen, int troops, int pl){
    cout << "General " << sithgen << "'s battalion attacked General "
    << jedigen << "'s battalion on planet " << pl << ". " << troops <<
    " troops were lost.\n";
}

void EOD_output(Universe &universe){
    cout << "---End of Day---\n";
    cout << "Battles: " << universe.num_battles << "\n";
}


void print(Planet &p){
    priority_queue<Deployment, vector<Deployment>, JediComp> j(p.jedi_depls);
    priority_queue<Deployment, vector<Deployment>, SithComp> s(p.sith_depls);
    
    cout << "\nPLANET " << p.ID;
    cout << "\n JEDI BATTALIONS\n";
    
    while(!j.empty()){
        cout << "  GENERAL " << j.top().gen << " HAS " << j.top().quantity << " TROOPS AT FORCE SENSITIVITY " <<
        j.top().FS << "\n";
        j.pop();
    }
    cout << "SITH BATTALIONS\n";
    while(!s.empty()){
        cout << "  GENERAL " << s.top().gen << " HAS " << s.top().quantity << " TROOPS AT FORCE SENSITIVITY " <<
        s.top().FS << "\n";
        s.pop();
    }
}

void median_output(Universe &u){
    for(unsigned int i = 0; i < u.trooplosses.size(); i++){
        if(!u.trooplosses[i].empty()) median_pq(u.trooplosses[i], i);
    }
}

void median_pq(vector<int> &v, unsigned int p){
    priority_queue<int> pq(v.begin(), v.end());
    int temp, median;
    if(pq.size() % 2 == 0){
        for(unsigned int i = 0; i < pq.size() / 2 - 1; i++) pq.pop();
        temp = pq.top();
        pq.pop();
        median = (temp + pq.top()) / 2;
    }
    else{
        for(unsigned int i = 0; i < pq.size() / 2; i++) pq.pop();
        median = pq.top();
    }
    
    cout << "Median troops lost on planet " << p <<
    " at time " << CURRENT_TIMESTAMP << " is " << median << ".\n";
}

void general_eval_output(Universe &u){
    cout << "---General Evaluation---\n";
    unsigned int count = 0;
    for(auto v : u.generalstats){
        cout << "General " << count << " deployed " << v[0] << " Jedi troops and " <<
        v[1] << " Sith troops, and " << (v[0] + v[1] - v[2]) <<
        "/" << (v[0] + v[1]) << " troops survived.\n";
        count++;
    }
}

void movie_watcher_output(Universe &u){
    cout << "---Movie Watcher---\n";
    for(unsigned int i = 0; i < u.planets.size(); i++){
        ambush(u.planets[i], i);
        attack(u.planets[i], i);
    }
}

void ambush(Planet &p, unsigned int i){
    cout << "A movie watcher would enjoy an ambush on planet " << i << " with Sith at time " << p.sith_ambush.time << " and Jedi at time " << p.jedi_ambush.time << ".\n";
}

void attack(Planet &p, unsigned int i){
        cout << "A movie watcher would enjoy an attack on planet " << i << " with Jedi at time " << p.jedi_attack.time << " and Sith at time " << p.sith_attack.time << ".\n";
    
}

void ambush_assess(Planet &p, const Deployment &d){
    if(d.jedi){
        //if no sith there
        if(p.sith_ambush == Coord(-1, -1, -1)){
            return;
        }
        //if sith there
        else{
            //if no alt exists
            if(p.sith_ambush_alt == Coord(-1, -1, -1)){
                //if this is a better matchup than before (or no matchup before)
                if((int)d.FS < p.jedi_ambush.FS || p.jedi_ambush == Coord(-1, -1, -1)){
                    p.jedi_ambush = Coord((int) d.ID, (int)d.FS, (int)d.timestamp);
                }
                //not a better matchup
                else return;
            }
            else{
                //if alt exists
                if(p.sith_ambush.FS - p.jedi_ambush.FS < p.sith_ambush_alt.FS - (int)d.FS){
                    p.sith_ambush = p.sith_ambush_alt;
                    p.sith_ambush_alt = Coord(-1,-1,-1);
                    p.jedi_ambush = Coord((int) d.ID, (int)d.FS, (int)d.timestamp);
                }
                else return;
            }
        }
    }
    //sith deployment
    else{
        //no jedi
        if(p.jedi_ambush == Coord(-1,-1,-1)){
            //no sith either
            if(p.sith_ambush == Coord(-1,-1,-1)){
                p.sith_ambush = Coord((int) d.ID, (int)d.FS, (int)d.timestamp);
            }
            //there are sith already there
            else{
                //stronger sith
                if((int)d.FS > p.sith_ambush.FS){
                    p.sith_ambush = Coord((int) d.ID, (int)d.FS, (int)d.timestamp);
                }
                //not stronger
                else return;
            }
        }
        //there are jedi
        else{
            //new sith is stronger than old
            if((int)d.FS > p.sith_ambush.FS){
                //new sith is stronger than alt/is no alt
                if((int)d.FS > p.sith_ambush_alt.FS){
                    p.sith_ambush_alt = Coord((int) d.ID, (int)d.FS, (int)d.timestamp);
                }
                //new sith not stronger than alt
                else return;
            }
            //not stronger than old, return
            else return;
        }
    }
}

void attack_assess(Planet &p, const Deployment &d){
    if(d.jedi){
        //if no sith there
        if(p.sith_attack == Coord(-1, -1, -1)){
            //if also no jedi
            if(p.jedi_attack == Coord(-1, -1, -1)){
                p.jedi_attack = Coord((int) d.ID, (int)d.FS, (int)d.timestamp);
            }
            //else if there are jedi as well
            else{
                //if better matchup than current jedi (and still no sith)
                if((int)d.FS < p.jedi_attack.FS){
                    p.jedi_attack = Coord((int) d.ID, (int)d.FS, (int)d.timestamp);
                }
                //if not better matchup
                else return;
            }
        }
        //if sith there
        else{
            //if better matchup
            if((int)d.FS < p.jedi_attack.FS){
                //if no alt
                if(p.jedi_attack_alt == Coord(-1, -1, -1)){
                    p.jedi_attack_alt = Coord((int) d.ID, (int)d.FS, (int)d.timestamp);
                }
                //if alt exists
                else{
                    //if new is a better matchup than alt
                    if((int)d.FS < p.jedi_attack_alt.FS){
                        p.jedi_attack_alt = Coord((int) d.ID, (int)d.FS, (int)d.timestamp);
                    }
                    //if not better matchup than alt
                    else return;
                }
            }
            //if not better matchup than current
            else return;
        }
    }
    //sith deployment
    else{
        //no jedi
        if(p.jedi_attack == Coord(-1,-1,-1)){
            return;
        }
        //there are jedi
        else{
            //if alt does not exist
            if(p.jedi_attack_alt == Coord(-1, -1, -1)){
                //if stronger matchup than prev sith
                if((int)d.FS > p.sith_attack.FS){
                    p.sith_attack = Coord((int) d.ID, (int)d.FS, (int)d.timestamp);
                }
                //if not stronger matchup than prev sith
                else return;
            }
            //alt does exist
            else{
                //is new-alt a better matchup than old-old? - yes
                if((int)d.FS - p.jedi_attack_alt.FS > p.sith_attack.FS - p.jedi_attack.FS){
                    p.jedi_attack = p.jedi_attack_alt;
                    p.sith_attack = Coord((int) d.ID, (int)d.FS, (int)d.timestamp);
                    p.jedi_attack_alt = Coord(-1, -1, -1);
                }
                //not a better matchup than current
                else return;
            }
        }
    }
}

void PR_input(Universe &universe, stringstream &ss){
    int i = 0;
    unsigned int count = 0;
    string newline;
    string temp = "";
    unsigned int time, gen, planet, FS, quantity;
    bool j;
    //read in all lines
    while(getline(ss, newline)){
        //read in timestamp
        count++;
        //cout << "LOOP : " << count << endl;
        while(newline[i] != ' '){
            temp += newline[i];
            i++;
        }
        //MEDIAN HERE
        time = stoi(temp);
        
        if(time != CURRENT_TIMESTAMP){
            if(MEDIAN) median_output(universe);
            CURRENT_TIMESTAMP = time;
        }
        
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
        
        if(universe.generalstats[gen].empty()){
            if(j){
                universe.generalstats[gen] = {(int)quantity, 0, 0};
            }
            else{
                universe.generalstats[gen] = {0, (int)quantity, 0};
            }
        }
        else{
            if(j) universe.generalstats[gen][0] += quantity;
            else universe.generalstats[gen][1] += quantity;
        }
        
        //push back deployment
        i = universe.find_planet(planet);
        universe.planets[i].add_depl(Deployment(time, gen, planet, j, FS, quantity, count), j);
        match(universe.planets[i], universe);
        
        
        ambush_assess(universe.planets[i], Deployment(time, gen, planet, j, FS, quantity, count));
        //        if(j) universe.planets[i].jedi_m.push_back(Deployment(time, gen, planet, j, FS, quantity, count));
        //        else universe.planets[i].sith_m.push_back(Deployment(time, gen, planet, j, FS, quantity, count));
        
        i = 0;
    }
    if(MEDIAN) median_output(universe);
}
