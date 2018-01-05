//
//  Osc_receive.cpp
//  blues_osc
//
//  Created by Christodoulos Aspromallis on 13/06/2016.
//
//

#include "Osc_receive.hpp"

void Osc_receive::setup(){

    receive.setup(PORT);
    
    b_r1 = {"2.0", "5.0"};
    b_r2 = {"1.0", "4.0"};
    room1 = 1;
    room2 = 0;
}

void Osc_receive::update(){
    
    if (beacon_list.size() > 0) previous_read = beacon_list;
    beacon_list.clear();
    
    //get_beacons();
    new_read = beacon_list;
    
    update_transition_state();
    
    
    cout << endl << "PREVIOUS READ: " << endl;
    for (int i=0; i < previous_read.size(); i++){
    
        cout << previous_read[i] << " ";
    }
    
    cout << endl << "NEW READ: " << endl;
    for (int i=0; i < new_read.size(); i++){
        
        cout << new_read[i] << " ";
    }
    cout << endl << endl;
    
    /*
    //print beacons list
    if (beacon_list.size() == 0) cout << endl << "NO BEACONS VISIBLE" << endl;
    else {
        
        //update last seen
        
        
        //print beacons
        cout << endl << "Beacons seen: " << endl;
        for (int i=0; i < beacon_list.size(); i++){
            
            cout << beacon_list[i] << " " << endl;
        }
        cout << endl << endl << endl;
    }
     */
}


void Osc_receive::get_beacons(){

    //vector<string> beacon_list;
    
    while(receive.hasWaitingMessages()){
        
        ofxOscMessage m;
        receive.getNextMessage(&m);
        
        if (m.getAddress() == "beacon_list"){//"/vertical/x"){
            
            //cout << "MESSAGE is THROUGH" << endl;
            beacon = m.getArgAsString(0);
            //_ending = m.getArgAsBool(1);
            //cout << "KEY: " << oscX << endl;
            
            beacon_list.push_back(beacon);
        }
    }

    //cout << oscX << " " << oscY << endl;
    //cout << "ending: " << _ending << ", g_r: " << _goal_reached << endl;
}


void Osc_receive::update_transition_state(){
    
    //check if room1
    for (int i=0; i < b_r1.size(); i++){
    
        vector<string>::iterator it = find(previous_read.begin(), previous_read.end(), b_r1[i]);
        if (it!=previous_read.end()){
            room1 = 1;
            room2 = 0;
        }
    }

    //check if room2
    for (int i=0; i < b_r2.size(); i++){
        
        vector<string>::iterator it = find(previous_read.begin(), previous_read.end(), b_r2[i]);
        if (it!=previous_read.end()){
            room1 = 0;
            room2 = 1;
        }
    }
    
    if (new_read.size() > 0 && previous_read.size() > 0){
        
        if (((new_read[0] == "4.0" || new_read[0] == "1.0") && room1) || ((new_read[0] == "1.0" || new_read[0] == "2.0" || new_read[0] == "5.0") && room2)){
        
            cout << endl << endl << endl << "DOES TRANSITION" << endl;
            do_transition = 1;
        }
        //else if (previous_read[0] == "4.0" && room1) transitioning = 1;
        else {
        
            cout << endl << endl << endl << "DON'T DO TRANSITION" << endl;
            do_transition = 0;
        }
    }
}

//BEACON MAP - updated
/*
    all 0.3 update rate
 5: next to 2nd socket (left of table..) - power (5 update)
 2: on high cabinet, in front of perideraia - power medium (2 update)
 1: pottery room, on top, on the left of ptolemaic/roman sign - power medium (10 update)
 4: sto klasiko gri cabinedaki - power medium (rethink? for inside cabinet) (1 update)
 */

//BEACON MAP
/*
    all 0.3 update rate
 5: on my desk - power low
 1: under the middle funriture (corner near transition) - power low
 2: on the begginning of the 2nd high cabinet as we walk from pottery to main room and turn left right away - power medium
 4: on the grey switch of pottery room (right hand as we enter pottery room) - power medium
 
 all broadcast rates at 0.3 sec
*/
