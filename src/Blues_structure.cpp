//
//  Blues_structure.cpp
//  
//
//  Created by Christodoulos Aspromallis on 04/03/2016.
//
//

#include "Blues_structure.h"
#include "Sequencer.h"
//#include "Grammar_parser.h"


void Blues_structure::setup(){
    
    //seq.parser.gr_pop = 0;
    seq.r_comp.parser.initiate_cycle();
    
    seq.setup();
    
    t = {0, 0, 0, 0, 0};
    //fin_t = {0};
    //cad_t = {0};
    
    stopping = false;
    
    cadencing = false;
    finishing = false;
    
    //gr1 channels
    ch_chords = 1;
    /*swing drums: *///ch_drums = 2;//bass drum, snare, tom1 (floor), tom2, tom 3, high-hat(open?), ride, crash
    drum_notes = {36, 38, 43, 45, 47, 48, 51, 49};//respectively..
    ch_egypt_cymbal = 4;
    ch_egypt_mel = 3;
    
    //gr2 channels
    ch_jazz_organ = 11;
    ch_drums = 12;
    ch_bass = 13;//train
    ch_train_mel = 14;
    
    
    
    QN_dur = seq.QN_subdivision;//quarternote duration in its subdivisions
    
    if (seq.r_comp.parser.gr_pop==0){
    
        vel_gr1 = 100;
        vel_gr2 = 100;
    }
    else {
        
        vel_gr1 = 0;
        vel_gr2 = 100;
    }

    //vel_gr2 = 100;
    controller_counter = 0;
    upwards = 1;
    //scan_file();
    //seq.r_comp.parser.setup();
    //play_chord_sequence();
    
    //for (int i=0; i<5; i++) curr_t[i] = 0;
    //better in constructor..??
    
    //randomised transitions..
    //trans_bars = {7, 21, 14, 12, 25, 13, 11, 10, 8, 20, 14};
    //FORTESTING
    //1, 1, 2, 2, 3, 3, 4, 4, 5, 5,
    trans_bars = {6, 6, 7, 7 ,8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31};
    //trans_bars = {4, 4};
    trans_pop = 0;
}


void Blues_structure::update(){
    
    //different value every second (for orchestration variety)
    elapsed_mins = (ofGetElapsedTimeMillis() / 5000) % 5;
    //cout << elapsed_mins << endl;
    
    //trying to set up volume automations for DAW
    /*
    if (controller_counter == 0) upwards = 1;
    if (controller_counter == 127) upwards = 0;
    
    if (upwards) controller_counter += 1;
    else controller_counter -= 1;
    //seq.midiOut::sendControlMessage(1, 1, controller_counter);
    seq.controls(1, 1, controller_counter);
     */
    
    //seq.velocity = (seq.velocity + 10) % 127;
    
    //cout << "controller counter: " << controller_counter << endl;
    
    //seq.update();
    //seq.metronome(t);
    
    //debugging for finishing etc..??
    /*
    if (fin_t.size()==0) t = seq.timer();
    else if (!(fin_t[3]==t[3] && fin_t[4]==t[4] && t[1]==2 && t[2]==3 && t[0]==7))
     */
    
    seq.update();//only for cycle_len for now..
    t = seq.timer();
    
    
    if (seq.only_on("beat", t)){
     
        //cout << "BEAT!!!!!!" << endl;
        //cout << "beat: " << t[2] << endl;
        //if (fin_t.size()!=0) cout << "fin_t: " << fin_t[3] << " " << fin_t[4] << endl;
    }
    
    if (seq.only_on("beat", t)) {
     
        //cout << "ending: " << ending << ", goal_reached: " << goal_reached << endl;
        seq.r_comp.parser.ending = ending;
        seq.r_comp.parser.goal_reached = goal_reached;
    }
    
    //find_rule only on every bar (exclude ticks & )
    if (seq.only_on("bar", t)){
        
        if (t[3]==0 && t[4]==1){
        
        }
        
        if (!seq.r_comp.trans_incomplete) seq.r_comp.parser.start_cycle(t);//places "S" at start of cycle (if not ending etc..) OR if grammar just changed //HERE or in seq.r_comp.parser.update_cycle();
        
        //A_B_change();
        ordered_change();
        //beacon_change();
    
        if(seq.r_comp.parser.transitioning && !seq.r_comp.rules_combined){
        
            seq.r_comp.parser.comb_setup = 0;
            seq.r_comp.combine_rules(t);
            
            recovered = 0;
        }
        //else seq.r_comp.parser.find_rule(t);
        //seq.r_comp.parser.find_rule(t);
        
        //seq.r_comp.parser.transitioning = 1;
        //seq.r_comp.rules_combined = 0;
        
        //JUMP to new form optimal point when g_p is reached..
        if ((t[3] == seq.r_comp.g_p) && seq.r_comp.rules_combined && !recovered){// && seq.r_comp.parser.transitioning){
            //"S"
            
            seq.r_comp.parser.logger.rt_log.append("MORPH ENDED, Goal Point Reached\n");
        
            seq.r_comp.build_next_form();

            //update times
            t[3] = seq.r_comp.n_s_pos;
            seq.bar = seq.r_comp.n_s_pos;
            seq.cycle = -1;
            
            //end _transitioning();
            seq.r_comp.parser.transitioning = 0;
            seq.r_comp.trans_incomplete = 0;
            seq.r_comp.rules_combined = 0;
            //seq.r_comp.parser.gr_pop = !seq.r_comp.parser.gr_pop;
            seq.r_comp.clearance();
        }
        
        seq.r_comp.parser.find_rule(t);
        
        //if (seq.r_comp.trans_complete) seq.r_comp.un_dist_found = 0;//to get unrewritten funcs till g_p only once in a transition..
        
        //if(!seq.r_comp.parser.transitioning || transition_complete) seq.r_comp.parser.find_rule(t);
        //else seq.r_comp.combine_rules(t);
        string terminal = seq.r_comp.parser.return_terminal(t);
        chord = terminal_to_midi(terminal);
        
        //logging
        seq.r_comp.parser.logger.bar();
        seq.r_comp.parser.logger.final_cycle_names.push_back(terminal);
        seq.r_comp.parser.logger.final_cycle_times.push_back(t[3]);
    }
    
    //if (automate_vel) {
        
        //fade_to_gr1 = seq.r_comp.parser.gr_pop;//fade to the opposite of the currect grammar
        
    //update_velocities();
    if (((t[3]>=trans_bars[trans_pop]) && (t[3]<=trans_bars[trans_pop]+9)) && (t[4]==3)){
        
        //update_velocities_once();
    }
    
    if(!vel_aut_complete) ;//update_velocities_once();
    
    //}
    
    //outside the if only on bar to have smoother vel automation
    //automate based on goal distance in actual version fo the algorithm
    
    /*
    update_state();
    
    string s = r_comp.parser.curr_cycle[t[3]].name;
    //cout << "time bar: " << blues.t[3] << ", s: " << s << endl;
    //bool finished = goal_reached && (s=="i"||s=="i6"||s=="i7");
    bool last_chord = (s=="i"||s=="i6"||s=="i7");
    */
    /*
    if (!finished && !stopping){

        play_main(chord);
    }
    else
    */
    if (seq.r_comp.parser.stop_seq){//finishing && goal_reached && last_chord)
    
        stopping = true;
        if (fin_t.size()==0) fin_t = t;//keeping constant value
        if (fin_t[3]==t[3] && fin_t[4]==t[4]) play_finale(chord);//play only for one bar more and only on current cycle
    }
    else play_main(chord);
}


void Blues_structure::A_B_change(){

    //RE-STARTING CYCLE (disrupting from top for new grammar)
    if (!seq.r_comp.parser.gr_changed){
        
        //automate_vel = 1;
        vel_aut_complete = 0;
        seq.r_comp.parser.logger.rt_log.append("\nvelocity crossfade START (unintelligent) to gr2 - [timestamp : " + ofGetTimestampString("[%M:%S.%i] \n"));
        
        //check if bar is an end_time
        vector<int> e_t = seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].end_times;
        vector<int>::iterator it_e_t = find(e_t.begin(), e_t.end(), t[3]);
        
        if (it_e_t!=e_t.end()){
            
            seq.r_comp.parser.initiate_cycle();
            //resetting timer values
            seq.bar = 0;
            t[3] = 0;
            
            seq.r_comp.parser.gr_pop = !seq.r_comp.parser.gr_pop;
            
            seq.r_comp.parser.gr_changed = 1;
        }
    }
}


void Blues_structure::beacon_change(){

    if (do_transition){//(t[3]==trans_bars[trans_pop]) && (t[4]==1)){
        
        //logging
        seq.r_comp.parser.logger.rt_log.append("\ndo_transition: - [timestamp : " + ofGetTimestampString("[%M:%S.%i] \n"));
        
        seq.r_comp.parser.transitioning = 1;
        seq.r_comp.trans_incomplete = 1;
        seq.r_comp.rules_combined = 0;
        //seq.r_comp.parser.gr_changed = 1;
        //update_velocities_once();
        
        //trans_pop = (trans_pop + 1) % trans_bars.size();
        
        vel_aut_complete = 0;
        //logging
        seq.r_comp.parser.logger.rt_log.append("\nvelocity crossfade START (intelligent - beacon) to gr2 - [timestamp : " + ofGetTimestampString("[%M:%S.%i] \n"));
        //*/
    }
}


void Blues_structure::ordered_change(){

    //transition
    if ((seq.r_comp.parser.gr_pop == 0) || (seq.r_comp.parser.gr_pop == 1) || (seq.r_comp.parser.gr_pop == 2)){
        
        if ((t[3]==4) && ((t[4]==1))){//(t[3]==trans_bars[trans_pop]) && (t[4]==1)){
            
            seq.r_comp.parser.transitioning = 1;
            seq.r_comp.trans_incomplete = 1;
            seq.r_comp.rules_combined = 0;
            //seq.r_comp.parser.gr_changed = 1;
            //update_velocities_once();
            
            trans_pop = (trans_pop + 1) % trans_bars.size();
            
            vel_aut_complete = 0;
            seq.r_comp.parser.logger.rt_log.append("\nvelocity crossfade START (intelligent) to " + ofToString(seq.r_comp.next_gr) + " - [timestamp : " + ofGetTimestampString("[%M:%S.%i] \n"));
             //*/
        }
    }
    //*/
    
    //transition
    /*
    if (seq.r_comp.parser.gr_pop == 1){
    
        if ((t[3]==4) && (t[4]==0)){//(t[3]==trans_bars[trans_pop]) && (t[4]==0)){//trans_bars[trans_pop]
            
            seq.r_comp.parser.transitioning = 1;
            seq.r_comp.trans_incomplete = 1;
            seq.r_comp.rules_combined = 0;
            
            trans_pop = (trans_pop + 1) % trans_bars.size();
            
            vel_aut_complete = 0;
            seq.r_comp.parser.logger.rt_log.append("\nvelocity crossfade START (intelligent) to " + ofToString(seq.r_comp.next_gr) + " - [timestamp : " + ofGetTimestampString("[%M:%S.%i] \n"));
        }
        
    }
     */
    
    //recovery
    //if ((t[3]==10) && (t[4]==0)) recovering = 1;
    if (recovering) {
     
        update_recovery();////(recovering && !recovered)
        //recovered = 1;
    }
    
    //reset_time
}


void Blues_structure::update_recovery(){
    
    //logging
    seq.r_comp.parser.logger.rt_log.append("\nrecovering - [timestamp : " + ofGetTimestampString("[%M:%S.%i] \n"));
    
    seq.r_comp.parser.transitioning = 0;
    seq.r_comp.trans_incomplete = 0;
    seq.r_comp.rules_combined = 1;
    
    //find grammar to be recovered
    seq.r_comp.parser.gr_pop = seq.r_comp.curr_gr;
    
    //place optimal functions
    int _f_l = seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length;
    //int optimal_bars_len = _f_l - t[3] - 1;// - 1 for offsetting to make space for "rec" non-T
    for (int i = (t[3]+2); i < _f_l; i++){
    
        seq.r_comp.parser.curr_cycle[i].name = seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].optimal_form_functions[i];
    }
    //place "rec" non-T
    seq.r_comp.parser.curr_cycle[(t[3]+1)].name = "rec";
    seq.r_comp.parser.logger.rt_log.append("'rec' placed in bar: " + ofToString((t[3]+1)) + "\n");
    
    //find which grammar to see!!!
    //prev_gr = seq.r_comp.prev_gr  //i.e. grammar being left.. this is non-dynamic, unlike curr_gr
    
    recovering = 0;
    recovered = 1;
    
    seq.r_comp.clearance();
}


void Blues_structure::update_velocities_once(){
    
    //bool vel_aut_complete;//velocity automation complete
    //bool fade_to_gr1;
    /*
     if (vel_gr1 <= 100 || vel_gr1 >= 0){
     
     fade_to_gr1 = !fade_to_gr1;
     //automate_vel = 0;
     }
     //if (vel_gr1 >= 0) fade_to_gr1 = 1;
     */
    
    if (!(vel_smoothener % 10)){//less than 5 means less smooth
        
        /*
        //this version may be useful for transition-based step-by-step automation control
        //but for 'A' - 'B' version it does abrupt step on end time (especially if 'A/B' near end time..)
        if (seq.r_comp.parser.gr_changed){//(!seq.r_comp.parser.transitioning){
            
            if(seq.r_comp.parser.gr_pop == 0){
                
                vel_gr1 = 100;//not transitioning and gr1
                vel_gr2 = 0;
            }
            else {
                
                vel_gr1 = 0;//not transitioning and gr2
                vel_gr2 = 100;
            }
        }
         */
        if(false){}
        else {
            
            if(seq.r_comp.parser.gr_pop == 1){
                
                if (vel_gr1 != 0) vel_gr1--;//fade out till 0
                if (vel_gr2 != 100) vel_gr2++;//fade in till 100
            }
            else {
                
                if (vel_gr2 != 0) vel_gr2--;//fade out till 0
                if (vel_gr1 != 100) vel_gr1++;//fade in till 100
            }
        }
        
        //vel_aut_complete = 0;
        
        if (vel_gr1 == 0 || vel_gr2 == 100){
            
            vel_aut_complete = 1;
            seq.r_comp.parser.logger.rt_log.append("\nvelocity crossfade END - [timestamp : " + ofGetTimestampString("[%M:%S.%i] \n"));
        }
        else if (vel_gr1 == 100 || vel_gr2 == 0){
            
            vel_aut_complete = 1;
            seq.r_comp.parser.logger.rt_log.append("\nvelocity crossfade END - [timestamp : " + ofGetTimestampString("[%M:%S.%i] \n"));
        }
        
        /*
         if (!fade_to_gr1){
         
         vel_gr1--;//fade out till 0
         vel_gr2++;//fade in till 100
         //vel_aut_complete = 0;
         
         if (vel_gr1 == 0 || vel_gr2 == 100) automate_vel = 0;
         }
         else {
         
         vel_gr2--;//fade out till 0
         vel_gr1++;//fade in till 100
         //vel_aut_complete = 0;
         
         if (vel_gr2 == 0 || (vel_gr1 == 100)) automate_vel = 0;
         }
         */
        
        vel_smoothener = 0;
    }
    
    vel_smoothener++;
}


void Blues_structure::update_velocities(){

    //bool vel_aut_complete;//velocity automation complete
    //bool fade_to_gr1;
    /*
    if (vel_gr1 <= 100 || vel_gr1 >= 0){
    
        fade_to_gr1 = !fade_to_gr1;
        //automate_vel = 0;
    }
    //if (vel_gr1 >= 0) fade_to_gr1 = 1;
     */
     
    if (!(vel_smoothener % 5)){//less than 5 means less smooth
        
        //this version may be useful for transition-based step-by-step automation control
            //but for 'A' - 'B' version it does abrupt step on end time (especially if 'A/B' near end time..)
        if (seq.r_comp.parser.gr_changed){//(!seq.r_comp.parser.transitioning){
            
            if(seq.r_comp.parser.gr_pop == 0){
         
                //vel_gr1 = 100;//not transitioning and gr1
                vel_gr2 = 100;
            }
            else {
            
                vel_gr1 = 0;//not transitioning and gr2
                vel_gr2 = 100;
            }
        }
        else {
        
            if(seq.r_comp.parser.gr_pop == 0){
                
                if (vel_gr1 != 0) vel_gr1--;//fade out till 0
                if (vel_gr2 != 100) vel_gr2++;//fade in till 100
            }
            else {
                
                if (vel_gr2 != 0) vel_gr2--;//fade out till 0
                if (vel_gr1 != 100) vel_gr1++;//fade in till 100
            }
        }
        
        /*
         if (!fade_to_gr1){
         
         vel_gr1--;//fade out till 0
         vel_gr2++;//fade in till 100
         //vel_aut_complete = 0;
         
         if (vel_gr1 == 0 || vel_gr2 == 100) automate_vel = 0;
         }
         else {
         
         vel_gr2--;//fade out till 0
         vel_gr1++;//fade in till 100
         //vel_aut_complete = 0;
         
         if (vel_gr2 == 0 || (vel_gr1 == 100)) automate_vel = 0;
         }
         */
        
        vel_smoothener = 0;
    }
    
    vel_smoothener++;
}


void Blues_structure::update_state(){

    string term = seq.r_comp.parser.return_terminal(t);
    
    if (term == "cad"){
        
        if (cadencing == false) cad_t = t;//keep temporary time for second bar of cad (drum closing in last bar of cadence for now..)
        cadencing = true;
        finishing = false;
    }
    if (term == "rec") {
        
        cadencing = false;
        finishing = false;
    }
    if (term == "fin"){
        
        cout << "FINISHING!" << endl;
        finishing = true;
        cadencing = false;
    }
}


void Blues_structure::play_main(vector<int>& chord){
    
    play_chords(chord);
    play_train_bass(chord);
    play_train_mel(chord);
    play_egypt_mel(chord);
    play_drums();
    
    if (elapsed_mins == 5){
        //seq.controls(1, 10, 0);
        //cout << "ch 1, control 1, 0" << endl;
    }
    if (elapsed_mins == 1){
        //seq.controls(1, 10, 50);
        //cout << "ch 1, control 1, 50" << endl;
    }
    
    /*
    if (elapsed_mins == 1 || elapsed_mins == 2) seq.controls(1, 10, 0);
    if (elapsed_mins == 1 || elapsed_mins == 4 || elapsed_mins == 2)
    if (elapsed_mins == 0 || elapsed_mins == 3)
     */
}


void Blues_structure::play_finale(vector<int>& chord){
    //2 bars with rock enging.. (+"thank you goodnight :P")
    
    play_chords(chord);
    play_bass_finale(chord);
    play_drums_finale();
}


//TO BLUES
void Blues_structure::play_drums(){
    
    //if (term == "cad") drum_pattern = drums_closing;
    //else if (term == "fin") drum_pattern = drums_end;
    //else drum_pattern = drums_shullfe;
    
    if (cadencing && !finishing && cad_t[3]+1==t[3]) play_drums_cad();
    else if (finishing) play_drums_fin();
    //else play_drums_normal();
    play_egypt_cymbal();
    play_drums_normal();
    
    //apo fin se rec pws pame sto grammar??? me rec pali??
    
}


void Blues_structure::play_egypt_cymbal(){
    
    //printf ("cymbal");
    seq.velocity = vel_gr1;
    
    notes_v = {55, 60, 94};
    for (int i=0; i<seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1; i++) multiple_bars.push_back(i);
    
    ///*
    for (int i=0; i<multiple_bars.size(); i++){
        
        pos_drums = {0, 0, 0, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_egypt_cymbal);
        pos_drums = {0, 1, 1, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_egypt_cymbal);
        pos_drums = {0, 0, 2, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_egypt_cymbal);
    }
     //*/
    multiple_bars.clear();
}


void Blues_structure::play_drums_normal(){
    
    seq.velocity = vel_gr2;
    
    //BASS DRUM
    notes_v = {drum_notes[0]};
    for (int i=0; i<seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1; i++) multiple_bars.push_back(i);
    
    for (int i=0; i<multiple_bars.size(); i++){
    
        pos_drums = {0, 0, 0, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 2, 0, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 2, 1, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 0, 2, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //bar 12 XXX LAST BAR??
    for (int i=0; i<seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length; i+=2){
    
        pos_drums = {0, i%3, i/3, seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }

    //SNARE
    notes_v = {drum_notes[1]};//{drum_notes[0]};
    //for (int i=0; i<11; i++) multiple_bars.push_back(i);
    
    for (int i=0; i<seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1; i++){
    
        pos_drums = {0, 0, 1, i};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 0, 3, i};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    multiple_bars = {1, 5, 9};
    
    for (int i=0; i<multiple_bars.size(); i++){
        
        //pos_drums = {0, 0, 3, multiple_bars[i]};//covered above..
        //seq.time_placement(t, notes_v, pos_drums, 6, ch_drums);//covered above..
        pos_drums = {0, 2, 3, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    multiple_bars = {3, 7};
    
    for (int i=0; i<multiple_bars.size(); i++){
        
        pos_drums = {0, 1, 2, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 0, 3, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 1, 3, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 2, 3, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //bar 12
    for (int i=0; i<3; i++){
    
        pos_drums = {0, i, 0, seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
     
    //RIDE
    notes_v = {drum_notes[6]};//{drum_notes[0]};
    for (int i=0; i < seq.r_comp.parser.all_gr[1].form_length; i++) multiple_bars.push_back(i);
    //multiple_bars = {0, 1, 2, 4, 5, 6, 8, 9, 10};//to avoid playing when e.g. snare is too busy (drummer had only two arms..)
    
    for (int i=0; i<multiple_bars.size(); i++){
        
        pos_drums = {0, 0, t[2], multiple_bars[i]};//if all bars then t[3] instead of multiple bars..
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 2, t[2], multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    multiple_bars = {3, 7};
    
    for (int i=0; i<multiple_bars.size(); i++){
        
        pos_drums = {0, 0, 0, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 2, 0, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 0, 1, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 2, 1, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 0, 2, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
        pos_drums = {0, 2, 2, multiple_bars[i]};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //TOM(s)
    //bar 12
    notes_v = {drum_notes[4]};
    for (int i=0; i<3; i++){

        pos_drums = {0, i, 1, seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    notes_v = {drum_notes[3]};
    for (int i=0; i<3; i++){
        
        pos_drums = {0, i, 2, seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    notes_v = {drum_notes[2]};
    for (int i=0; i<3; i++){
        
        pos_drums = {0, i, 3, seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length-1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }

    //CRASH
    notes_v = {drum_notes[7]};//{drum_notes[drum_notes.size()-1]};
    pos_drums = {0, 0, 0, 0};
    if (t[4] != 0) seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    pos_drums = {0, 0, 0, 4};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    
    multiple_bars.clear();
}


void Blues_structure::play_drums_cad(){
    
    //BASS DRUM
    notes_v = {drum_notes[0]};
    //bar 12
    for (int i=0; i<seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length; i+=2){
        
        pos_drums = {0, i%3, i/3, cad_t[3]+1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //SNARE
    notes_v = {drum_notes[1]};
    //bar 12
    for (int i=0; i<3; i++){
        
        pos_drums = {0, i, 0, cad_t[3]+1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //TOM(s)
    //bar 12
    //tom high
    notes_v = {drum_notes[4]};
    for (int i=0; i<3; i++){
        
        pos_drums = {0, i, 1, cad_t[3]+1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //tom mid
    notes_v = {drum_notes[3]};
    for (int i=0; i<3; i++){
        
        pos_drums = {0, i, 2, cad_t[3]+1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //tom low
    notes_v = {drum_notes[2]};
    for (int i=0; i<3; i++){
        
        pos_drums = {0, i, 3, cad_t[3]+1};
        seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    }
    
    //CRASH
    notes_v = {drum_notes[7]};//{drum_notes[drum_notes.size()-1]};
    pos_drums = {0, 0, 0, (cad_t[3]+1)%seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
}


void Blues_structure::play_drums_fin(){

    //if very last bar, then no drums
    //else very open/simple counting
}


void Blues_structure::play_drums_finale(){
    
    //if very last bar, then no drums
    //else very open/simple counting
    
    //BASS DRUM
    notes_v = {drum_notes[0]};
    pos_drums = {0, 1, fin_t[2], fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    pos_drums = {0, 0, fin_t[2]+1, fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    pos_drums = {0, 2, fin_t[2]+1, fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    
    //TOM HIGH
    notes_v = {drum_notes[4]};
    pos_drums = {0, 0, fin_t[2], fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    //TOM MID
    notes_v = {drum_notes[3]};
    pos_drums = {0, 1, fin_t[2], fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    
    //SNARE
    notes_v = {drum_notes[1]};
    pos_drums = {0, 2, fin_t[2], fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    pos_drums = {0, 0, fin_t[2]+1, fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
    
    //CRASH
    notes_v = {drum_notes[7]};
    pos_drums = {0, 2, fin_t[2] + 1, fin_t[3]};
    seq.time_placement(t, notes_v, pos_drums, QN_dur, ch_drums);
}

void Blues_structure::play_egypt_mel(vector<int>& chord){
    
    seq.velocity = vel_gr1;
    //seq.velocity = 100;
    
    vector<int> b_note;
    vector<int> pos_bass;
    int dur_bass = QN_dur;
    
    vector<int> bass_patt;
    int count = 0;
    
    if (chord.size()==4) bass_patt = {0, 1, 2, 1, 3, 2, 1, 2};
    else if (chord.size()==3) bass_patt = {0, 1, 2, 1, 0, 2, 1, 2};
    
    if (seq.only_on("beat", t)){
        
        b_note = {chord[bass_patt[t[2]*2]]};
        pos_bass = {0, 0, t[2], t[3]};
        seq.time_placement(t, b_note, pos_bass, dur_bass, ch_egypt_mel);
        b_note = {chord[bass_patt[t[2]*2+1]]};
        pos_bass = {0, 2, t[2], t[3]};
        seq.time_placement(t, b_note, pos_bass, dur_bass, ch_egypt_mel);
    }
}


void Blues_structure::play_train_bass(vector<int>& chord){
    
    seq.velocity = vel_gr2;
    //seq.velocity = 100;
    
    vector<int> b_note;
    vector<int> pos_bass;
    int dur_bass = 1;//QN_dur;
    
    vector<int> bass_patt;
    int count = 0;
    
    if (chord.size()==4) bass_patt = {0, 1, 2, 1, 3, 2, 1, 2};
    else if (chord.size()==3) bass_patt = {0, 1, 2, 1, 0, 2, 1, 2};
        
    //if (seq.only_on("beat", t)){
        
        b_note = {chord[bass_patt[t[2]*2]] - 12};//-12 for an octave down..
        pos_bass = {0, 0, t[2], t[3]};
        seq.time_placement(t, b_note, pos_bass, dur_bass, ch_bass);
    
        b_note = {chord[bass_patt[t[2]*2+1]] - 12};
        pos_bass = {0, 2, t[2], t[3]};
        seq.time_placement(t, b_note, pos_bass, dur_bass, ch_bass);
         //*/
    //}
}

void Blues_structure::play_train_mel(vector<int>& chord){
    
    seq.velocity = vel_gr2;
    //seq.velocity = 100;
    
    vector<int> m_note;
    vector<int> pos_mel;
    int dur_mel;//
    
    //Melody of A
    for (int i=0; i < seq.r_comp.parser.all_gr[1].form_length; i+=8){

        //play B mel
        if (i==16){

            //bar 16
            m_note = {69};
            pos_mel = {0, 0, 0, i};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            m_note = {72};
            pos_mel = {0, 2, 0, i};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 10, ch_train_mel);
            
            //bar 17
            m_note = {76};
            pos_mel = {0, 0, 0, i+1};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            m_note = {65};
            pos_mel = {0, 2, 0, i+1};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 4, ch_train_mel);
            m_note = {69};
            pos_mel = {0, 0, 2, i+1};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            m_note = {72};
            pos_mel = {0, 0, 3, i+1};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);

            //bar 18 - 19
            m_note = {76};
            pos_mel = {0, 0, 0, i+2};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            m_note = {69};
            pos_mel = {0, 2, 0, i+2};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 21, ch_train_mel);

            //bar 20
            m_note = {69};
            pos_mel = {0, 0, 0, i+4};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            m_note = {72};
            pos_mel = {0, 2, 0, i+4};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 10, ch_train_mel);
            
            //bar 21
            m_note = {76};
            pos_mel = {0, 0, 0, i+5};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            m_note = {66};
            pos_mel = {0, 2, 0, i+5};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 4, ch_train_mel);
            m_note = {69};
            pos_mel = {0, 0, 2, i+5};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            m_note = {72};
            pos_mel = {0, 0, 3, i+5};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            
            //bar 22
            m_note = {76};
            pos_mel = {0, 0, 0, i+6};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            m_note = {69};
            pos_mel = {0, 2, 0, i+6};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 15, ch_train_mel);

            //bar 23
            m_note = {68};
            pos_mel = {0, 0, 2, i+7};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 6, ch_train_mel);
        }
        else {

            //bars 1-2
            m_note = {55 + 12};
            pos_mel = {0, 0, 0, i};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 13, ch_train_mel);
            m_note = {64 + 12};
            pos_mel = {0, 1, 0, i+1};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 5, ch_train_mel);
            m_note = {55 + 12};
            pos_mel = {0, 0, 2, i+1};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            m_note = {60 + 12};
            pos_mel = {0, 0, 3, i+1};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            
            //bars 3-4
            m_note = {76};
            pos_mel = {0, 0, 0, i+2};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            m_note = {68};
            pos_mel = {0, 2, 0, i+2};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 20, ch_train_mel);
            
            //bars 5-8
            m_note = {69};
            pos_mel = {0, 0, 0, i+4};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 11, ch_train_mel);
            m_note = {69};
            pos_mel = {0, 0, 0, i+5};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            m_note = {70};
            pos_mel = {0, 2, 0, i+5};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 1, ch_train_mel);
            m_note = {71};
            pos_mel = {0, 0, 1, i+5};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            m_note = {76};
            pos_mel = {0, 2, 1, i+5};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 1, ch_train_mel);
            m_note = {67};
            pos_mel = {0, 0, 2, i+5};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            m_note = {66};
            pos_mel = {0, 2, 2, i+5};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 1, ch_train_mel);
            m_note = {65};
            pos_mel = {0, 0, 3, i+5};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            m_note = {73};
            pos_mel = {0, 2, 3, i+5};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 1, ch_train_mel);
            m_note = {72};
            pos_mel = {0, 0, 0, i+6};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 2, ch_train_mel);
            m_note = {64};
            pos_mel = {0, 2, 0, i+6};//tick, semiq, beat, bar
            seq.time_placement(t, m_note, pos_mel, 20, ch_train_mel);

        
        }
    }
    
    //automation - itarative melody management try..
     /*
     vector<int> pitches_A = {67, 76, 67, 72, 76, 68,c69, 69, 70, 71, 76, 67, 66, 65, 73, 72, 67};
     vector<int> pitches_B;
     vector<int> durs_A = {14, 4, 3, 3, 2, 22, 12, 2, 1, 2, 1, 2, 1, 2, 1, 2, 22};//use this as full duration for melody itrative building AND manually subtract duration where necessary
     vector<int> durs_B;
     
     int tick, semiq, beat, bar;//need to be global in class?s
     for (int j=0; j < pitches_A.size(); j++){
     
     //filter durations
     
     m_note = {pitches_A[j]};
     if (j==0) {
     
     tick=0;
     semiq=0;
     beat=0;
     bar=0;
     }
     
     else {
     
     durs_A
     
     pos_mel = {0, 0, 0, i};//tick, semiq, beat, bar
     seq.time_placement(t, m_note, pos_mel, 12, ch_train_mel);
     }
     }
     */
    //*/
    //}
}

void Blues_structure::play_bass_fin(vector<int>& chord){
}


void Blues_structure::play_bass_finale(vector<int>& chord){

    vector<int> b_note;
    int tonic = chord[0];
    vector<int> pos_bass;
    int dur_bass = QN_dur;
        
    b_note = {tonic};
    pos_bass = {0, 0, fin_t[2], fin_t[3]};
    seq.time_placement(t, b_note, pos_bass, dur_bass, ch_bass);
    b_note = {tonic-2};
    pos_bass = {0, 2, fin_t[2], fin_t[3]};
    seq.time_placement(t, b_note, pos_bass, dur_bass, ch_bass);
    b_note = {tonic-1};
    pos_bass = {0, 0, fin_t[2]+1, fin_t[3]};
    seq.time_placement(t, b_note, pos_bass, dur_bass, ch_bass);
    b_note = {tonic};
    pos_bass = {0, 2, fin_t[2]+1, fin_t[3]};
    seq.time_placement(t, b_note, pos_bass, dur_bass, ch_bass);
}


void Blues_structure::play_chords(vector<int>& chord){
    
    seq.velocity = vel_gr1;
    
    vector<int> pos_chord = {0, 0, 0, t[3]};
    seq.time_placement(t, chord, pos_chord, QN_dur*4, ch_chords);
    //can't' accept get_chord() by reference
    
    seq.velocity = vel_gr2;
    seq.time_placement(t, chord, pos_chord, QN_dur*4, ch_jazz_organ);
}


vector<int> Blues_structure::terminal_to_midi(string& terminal){
    
    int oct_off = 1; //octave offset in chord_translation
    
    int root_pitch = chord_translation[terminal][0] + oct_off * 12;
    //cout << "root_pitch: " << root_pitch << endl;
    int type = chord_translation[terminal][1];
    //cout << "type: " << type << endl;
    
    return get_chord(root_pitch, type);
}


map<string, vector<int>> Blues_structure::chord_translation{//no static here

    {"i", {48, 0}},
    {"isus4", {48, 9}},
    {"im", {48, 1}},
    {"immaj", {48, 8}},
    {"i6", {48, 3}},
    {"i7", {48, 6}},
    {"ii7", {50, 6}},
    {"bii", {49, 0}},
    {"biisus2", {49, 10}},
    {"iim7", {50, 4}},
    {"iiim", {52, 1}},
    {"iiim7", {52, 4}},
    {"iiio", {52, 7}},
    {"iv", {53, 0}},
    {"ivsus4", {53, 9}},
    {"iv6", {53, 3}},
    {"iv7", {53, 6}},
    {"ivm", {53, 1}},
    {"v7", {55, 6}},
    {"vo", {55, 7}},
    {"bvi", {44, 0}},
    {"bviim", {46, 1}},
    
};


vector<int> Blues_structure::get_chord(int root_pitch, int type, int invert, bool position){
    
    //chord structure
    vector<int> chord_set;
    if (type == 0) chord_set = {0, 4, 7};
    else if (type == 1) chord_set = {0, 3, 7};
    else if (type == 2) chord_set = {0, 4, 7, 11};
    else if (type == 3) chord_set = {0, 4, 7, 9};
    else if (type == 4) chord_set = {0, 3, 7, 10};
    else if (type == 5) chord_set = {0, 3, 7, 9};
    else if (type == 6) chord_set = {0, 4, 7, 10};
    else if (type == 7) chord_set = {0, 3, 6};//chord_set.resize()?
    else if (type == 8) chord_set = {0, 3, 7, 11};
    else if (type == 9) chord_set = {0, 5, 7};//sus4
    else if (type == 10) chord_set = {0, 2, 7};//sus2
    //else if (more chord types)
    
    /*
    int invert = 0;
    //for root position leave inv as is..
    if (inversion == "1st") invert = 1;
    else if (inversion == "2nd") invert = 2;
    else if (inversion == "3rd" && chord_set.size()>=4) invert = 3;
    */
     
    //appending first element(s) to vector end
    for (int i=0; i<invert; i++){
        int first_elem = chord_set[0];
        chord_set.erase(chord_set.begin());
        chord_set.push_back(first_elem + seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length);
    }
    
    //3rd inversion 1 octave lower (better register)
    if (invert == 2 || invert == 3){
        for (int i=0; i<chord_set.size(); i++){
            chord_set[i] -= seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length;
        }
    }
    
    if (position) chord_set[1] += seq.r_comp.parser.all_gr[seq.r_comp.parser.gr_pop].form_length;
    
    //make chord pitches
    vector<int> chord;
    //{0, 4, 7} ==> {4, 0, 7} ?? --> {4, 7, 8}
    for (int i=0; i<chord_set.size(); i++){
        chord.push_back(chord_set[i]+root_pitch);
    }
    //needs clearing? (chord)
    
    return chord;
}


//TO BLUES
void Blues_structure::time_misplace_chord(){
    //compute 16th/8th/QN misplacement back or forth (based on style..)
    //a little grammar for it?
    //simple markov..?!
}

/*
 int blues_structure::chord_array(int harmony_type, int tonal_centre){
 //rnr type
 int tc = tonal_centre;
 if (harmony_type == 1){ // <----- <-----
 chord_list_1[0] = tc;
 chord_list_1[4] = ;
 triad_pool =
 
 }
 //jazz(-er) type
 else if (harmony_type == 2){
 for (int i=0; i<12; i++){
 7th_chords = ;
 chord_list_2
 }
 }
 //alice blues
 else if (harmony_type == 3){
 
 }
 }
 */
