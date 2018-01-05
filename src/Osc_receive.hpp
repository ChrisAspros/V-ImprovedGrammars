//
//  Osc_receive.hpp
//
// blues_osc
//
// for git..
//
//  Created by Christodoulos Aspromallis on 13/06/2016.
//
// mercurial test comment

#ifndef Osc_receive_hpp
#define Osc_receive_hpp

//#include <stdio.h>
#include "ofMain.h"
#include "ofxOsc.h"

#define PORT 9000

class Osc_receive{

public:
    
    void setup();
    void update();
    void get_beacons();
    void update_transition_state();

    ofxOscReceiver receive;
    //bool _ending = true;//, goal_reched;
    bool _ending, _goal_reached;
    float oscX = 0.0;
    float oscY = 0.0;
    
    vector<string> beacon_list; //vector<STRING> ??
    vector<string> previous_read;
    vector<string> new_read;
    string beacon;
    
    vector<string> b_r1, b_r2;//beacons room 1 and 2
    bool room1, room2, do_transition, recovering;
};

#endif /* Osc_receive_hpp */
