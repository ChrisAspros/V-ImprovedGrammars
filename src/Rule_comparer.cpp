//
//  Rule_comparer.cpp
//  Egypt_pub_2grammars
//
//  Created by Christodoulos Aspromallis on 22/08/2017.
//
//

#include "Rule_comparer.hpp"



void Rule_comparer::combine_rules(vector<int>& seq_t){
    
    //logging
    parser.logger.rt_log.append("\n\n===TRANSITION BEGINS=== [timestamp : " + ofGetTimestampString("[%M:%S.%i] \n"));
    
    //STATIC process
    if (!parser.comb_setup) setup_combination(seq_t);
    
    //logging
    parser.logger.rt_log.append("\n\n===COMBINATION SETUP=== [timestamp : " + ofGetTimestampString("[%M:%S.%i] \n"));
    
    
    //morph_
    //UPDATING - running the cycle now.. (pairnei ta hnia..)
    update_combination(seq_t);
    
    rules_combined = 1;
    parser.transitioning = 0;
    //parser.gr_pop = next_gr;//already in build_next_form()
    //place_next_sect();
    
    //logging
    parser.logger.rt_log.append("\n\n===COMBINATION UPDATED / MORPH BEGINS=== [timestamp : " + ofGetTimestampString("[%M:%S.%i] \n"));
    //logging
    //seq.r_comp.parser.logger.rt_log.append("\n\n===TRANSITION BEGINS===\n");
    
    //too early..
    /*
    int aux_gr = next_gr;
    next_gr = curr_gr;
    curr_gr = aux_gr;
     */
     
    //get engine to keep playing
    
    cout << endl << "BP:" << endl;
    //exchange curr_/next_grammars
    //necessary to empty elements in find_best_rule?? e.g. S_rule_next, aux cycle, hist, etc. etc.etc....?
    
    //((((
    
    //start parallel aux_cycle for incoming GR(_2)
    
    //decide which is the best section to be placed at goal point..
    
    //keep curr_gr previous until transition if complete.. then exchange
    
    
    //optimal transition functions will be returned here (2 function vectors)
    //look at GR1 OR GR2 for choice FUNCTION choice and production
        //weighted random, based on transition stage..
    //keep producing GR1 (according to forced compatible rule&production to the extent it plays a role) in case we go back..
    //stop producing GR1 if total transition to GR2 (and vice-versa..)
        //on full transition curr_gr = all_gr[1] & next_gr = all_gr[0]
    
    
    //prediction
        //if ending at specific bars: map bars to goal points
        //for smoothness map distance to weighted selection (FUNC->terminal GR1 || GR2)..
            //if transition exceeds goal point follow both GRs for as long as necessary
            //transition is cancelled only if we go outside it.. distance-wise in space..thus, a smooth weighted de-transition back to GR1
        // - instead of mere smooth stages, we aim at a goal point as a formal reference point (i.e. preserving better both form 1 and form 2) for introduction of sect of GR2, so that a better sense of form is preserved
            //SO PREDICTION and GOAL POINT are NECESSARY...!!!
    
    //return production, i.e. vector<G_parser::elem_ID> best_prod
    //best_r = find_best_rule(seq_t);//necessary??
    
    //Towards function-level restriction of space (optimal rules & productions, both locally and formally, i.e. historically)
}


void Rule_comparer::setup_combination(vector<int>& seq_t){

    //must be managed dynamically
    curr_gr = parser.gr_pop;//= 0;
    //prev_gr = curr_gr;
    if (curr_gr==0) next_gr = 1;
    else if (curr_gr==1) next_gr = 2;
    else next_gr = 0;
    //next_gr = !curr_gr;//cuz we only hae 2 grammars for now..
    
    //dist only ONCE??
    //int dist;
    dist = get_distance_to_goal(seq_t);
    //cout << "DIST_TO_GOAL: " << dist << endl;
    //logging
    parser.logger.rt_log.append("DIST_TO_GOAL: " + ofToString(dist) + ", Goal Point: " + ofToString(g_p) + "\n");
    parser.logger.rt_log.append("curr_gr: " + ofToString(curr_gr) + ", next_gr: " + ofToString(next_gr) + "\n");
    
    find_best_rule(seq_t);
    
    parser.comb_setup = 1;
    
    //run possible FUNC productions till goal (GR_1)
    //run possible FUNC tail productions of (GR_2)
    //keep best scores
    //check past
    //run pre-tail productions of (GR_2)
    //compare with function History
    //get optimal functions of GR1 AND GR2 until g_p
}


int Rule_comparer::get_distance_to_goal(vector<int>& seq_t){
    
    int distance, curr_pos;//, g_p;
    
    //find current position
    curr_pos = seq_t[3];
    
    //calcuate goal point (we are transitioning)
    //every 8 bars now.. - CAN CHANGE, based on form significance.., e.g. blues IV significance..!!!
    if (curr_gr == 0 || curr_gr == 1 || curr_gr == 2){
    
        if (curr_pos > 3 & curr_pos <= 11) g_p = 16;//includes -1
        if (curr_pos > 11 & curr_pos <= 19) g_p = 24;
        if (curr_pos > 19 & curr_pos <= 27) g_p = 0;
        if (curr_pos > 27 || curr_pos <= 3) g_p = 8;
    }
    else {//if gr_2 - for blues form??
        
        if (curr_pos > 3 & curr_pos <= 11) g_p = 0;//includes -1
        if (curr_pos > 11 || curr_pos <= 3) g_p = 8;
    }
    
    //make more dynamic later, based on tempo, player speed, form morphology etc..
    
    if (curr_pos > g_p) distance = (parser.all_gr[curr_gr].form_length - curr_pos) + g_p;//+-1??
    else distance = g_p - curr_pos;//+-1??
    
    return distance;//watch out it may return the previously help number OR returning it one bar too late..
}



void Rule_comparer::find_best_rule(vector<int>& seq_t){
    
    //check all sects tails for enough length
    
    //find/store the S rule (next_gr)
    S_rule_curr = get_S_rule(curr_gr);
    S_rule_next = get_S_rule(next_gr);
    
    //find sect lengths
   sect_lengths_curr = get_sect_lengths(S_rule_curr, curr_gr);
    sect_lengths_next = get_sect_lengths(S_rule_next, next_gr);
    
    //logging
    parser.logger.rt_log.append("S_rule_curr: \n");
    for (int i=0; i < S_rule_curr.right_side[0].right_str.size(); i++){
        
        parser.logger.rt_log.append(S_rule_curr.right_side[0].right_str[i]);
        parser.logger.rt_log.append(", time: " + ofToString(S_rule_curr.prod_times[i]) + "\n");
    }
    parser.logger.rt_log.append("S_rule_next: \n");
    for (int i=0; i < S_rule_next.right_side[0].right_str.size(); i++){
        
        parser.logger.rt_log.append(S_rule_next.right_side[0].right_str[i]);
        parser.logger.rt_log.append(", time: " + ofToString(S_rule_next.prod_times[i]) + "\n");
    }
    parser.logger.rt_log.append("sect_lengths_curr: ");
    for (int i=0; i < sect_lengths_curr.size(); i++) parser.logger.rt_log.append(ofToString(sect_lengths_curr) + ", ");
    parser.logger.rt_log.append("\nsect_lengths_next: ");
    for (int i=0; i < sect_lengths_next.size(); i++) parser.logger.rt_log.append(ofToString(sect_lengths_next) + ", ");
    parser.logger.rt_log.append("\n");
    
    
    
    /*
    //check Sect lengths
    //compare prod times of Sects
    for (int i=0; i < S_rule_next.prod_times.size(); i++){
        
        //calculate sect lengths
        int len;
        if (i == S_rule_next.prod_times.size() - 1) len = parser.all_gr[next_gr].form_length - S_rule_next.prod_times[i];
        else len = S_rule_next.prod_times[i+1] - S_rule_next.prod_times[i];
        sect_lengths.push_back(len);
    }
    /*
    cout << endl << "sect_lengths: " << endl;
    for (int i=0; i < sect_lengths.size(); i++) cout << sect_lengths[i] << endl;
    cout << endl;
     */
    
    //find/store the sect rules
    vector<string> sect_names_aux;
    vector<G_parser::rule> sect_rules;
    //aug_sect_rule asr;//augmented section rule
        //get sect names
    for (int i=0; i < S_rule_next.right_side.size(); i++){
        
        for (int j=0; j < S_rule_next.right_side[i].right_str.size(); j++){
            
            string sect_name;//isolated from time sing, i.e. e.g. (5).
            int k=0;
            while (S_rule_next.right_side[i].right_str[j][k] != '('){
            
                sect_name.push_back(S_rule_next.right_side[i].right_str[j][k]);
                k++;
            }
            sect_names_aux.push_back(sect_name);
            aug_sect_rules.push_back(aug_sect_rule());
            aug_sect_rules[j].sect_name = sect_name;
            aug_sect_rules[j].position = S_rule_next.prod_times[j];
        }
    }
    
        //search sect_names (in general rules) - EXPAND for TIMED RULES ALSO!!
    for (int i=0; i < sect_names_aux.size(); i++){
        
        //search in general rules
        for (int j=0; j < parser.all_gr[next_gr].general_rules.size(); j++){
        
            for (int k=0; k < parser.all_gr[next_gr].general_rules[j].left_str.size(); k++){
            
                if (sect_names_aux[i] == parser.all_gr[next_gr].general_rules[j].left_str[k]){
                
                    aug_sect_rules[i].s_rule = parser.all_gr[next_gr].general_rules[j];
                    break;//because only 1 "Sect" on the left_str of a sect rule
                }
            }
            aug_sect_rules[i].r_len = sect_lengths_next[i];
            aug_sect_rules[i].next_sect = sect_names_aux[(i+1) % sect_names_aux.size()];
        }
        
        //what if SectA is in two points in the form, i.e. preceeded by 2 different sections..?
        //both must be found and recorded
        //for now just keep the first one - it may be more definitive of next_gr (earlier in the form..), but maybe not.. - mention?
        
        //search in timed_rules
        //- EXPAND for TIMED RULES ALSO!! - Sect rules need to be general_rules (a.o.t. timed_) for now..
    }
    
    //vector<G_parser::rule> long_rules;
    vector<vector<G_parser::rule>> prolonged_rules;
    
    initiate_aux_cycle();//size of longest form of the grammars
    
    rewrite_funcs();//curr_gr
    //now we have parser.func_chunks (vector<elem_ID>)
        //curr_func_chunks & next_func_chunks
    
    //check if func chunks OK
        //make them multiple productions of FUNCTIONS in gr2 (for testing..)
    
    //work from func chuncs - no lines??
    
    //compare next func chunks to curr func chunks till goal
    //keep N best
    //check scores (all, regardless of rewrite probability for now..) of next_func_chunks with history
        //e.g. for another section..
        //this is simpler cuze history is static..
    //keep the one best FUNCs combo (curr_gr - next_gr)
    //place Sect* on g_p
    //mix transition FUNCs in time
    //manage cycle placemebt of Sect after transition..
    
    //make amendments for 'T' instead of 'A'-'B'
    
        //(NOT FOR NOW)for looking at the future just make curr func chunks get further than g_p in the first place..
    
    //maybe 1 twice with different input??
    
    //to get scores, isolate the next_func lines portions I need..
    curr_func_lines = construct_lines (curr_func_chunks, curr_gr, un_dist, undist_bar);
    next_func_lines = construct_lines (next_func_chunks, next_gr, parser.all_gr[next_gr].form_length, 0);
    
    //logging
    parser.logger.transition_data_log.append("\n\n\ncurr_func_lines, [name:time]\n");
    for (int i=0; i < curr_func_lines.size(); i++){
        
        parser.logger.transition_data_log.append("number:" + ofToString(i) + " ");
        
        for (int j=0; j < curr_func_lines[i].size(); j++) parser.logger.transition_data_log.append("[" + curr_func_lines[i][j].name + ":" + ofToString(curr_func_lines[i][j].time[1]) + "], ");
        
        parser.logger.transition_data_log.append("\n");
    }
    parser.logger.transition_data_log.append("next_func_lines, [name:time]\n");
    for (int i=0; i < next_func_lines.size(); i++){
        
        parser.logger.transition_data_log.append("number:" + ofToString(i) + " ");
        
        for (int j=0; j < next_func_lines[i].size(); j++) parser.logger.transition_data_log.append("[" + next_func_lines[i][j].name + ":" + ofToString(next_func_lines[i][j].time[1]) + "], ");
        
        parser.logger.transition_data_log.append("\n");
    }
    
    int _l_filt = 400;
    
    filter_func_lines_pop(curr_func_lines, _l_filt);
    filter_func_lines_pop(next_func_lines, _l_filt);
    
    
    //logging
    parser.logger.transition_data_log.append("curr_func_lines (FILTERED at " + ofToString(_l_filt) + ", [name:time]\n");
    for (int i=0; i < curr_func_lines.size(); i++){
        
        parser.logger.transition_data_log.append("number:" + ofToString(i) + " ");
        
        for (int j=0; j < curr_func_lines[i].size(); j++) parser.logger.transition_data_log.append("[" + curr_func_lines[i][j].name + ":" + ofToString(curr_func_lines[i][j].time[1]) + "], ");
        
        parser.logger.transition_data_log.append("\n");
    }
    parser.logger.transition_data_log.append("next_func_lines (FILTERED at " + ofToString(_l_filt) + ", [name:time]\n");
    for (int i=0; i < next_func_lines.size(); i++){
        
        parser.logger.transition_data_log.append("number:" + ofToString(i) + " ");
        
        for (int j=0; j < next_func_lines[i].size(); j++) parser.logger.transition_data_log.append("[" + next_func_lines[i][j].name + ":" + ofToString(next_func_lines[i][j].time[1]) + "], ");
        
        parser.logger.transition_data_log.append("\n");
    }
    
    
    compare_t_g();
    
    /*
    qsort()
    qsort_b(void *__base, <#size_t __nel#>, <#size_t __width#>, <#^int(const void *, const void *)__compar#>)
    qsort_r(<#void *__base#>, <#size_t __nel#>, <#size_t __width#>, <#void *#>, <#int (* _Nonnull __compar)(void *, const void *, const void *)#>)
    qsort
     */
    
    compare_include_history(form_pc);
    
    //compare_include_future(form_pc);
    //cout << "return";
    //mix_in_rt();
    //implement_recovery();//end of version_A!! - version b with constraint / diminishing of the musical space (style/chord et.c) at the note level.. - set theory.. (instead of constraint at the function level..)
    
    //look ahead??
    
    //get total scores of combinations
    //place best combination in 2 cycles
    
    //place next sect on g_p
    
    //mix 2 cycles into the main (playing)
        //based on transition phase
    
    //if g_p reached end transition (for now..) OR re do the proces..
    
    
//VHMA VHMA na vrw poses fores xreiazetai kai pou to trans_update()
    
    //next_gr: make vector of all possible functions till goal.. (irrespective of probabilities)
    
    //make map of all possible func_chunkcs.. linearly..
    //--expand all possible sects gr2
    //get scores..
    //mix in real time..
    //till goal
    //till updating goal
        //if g_p reached and still transitioning:
            //re do the test process of BEST TILL GOAL + what has been played..
                //nested..!!
    //finalise transitions / introduce next_gr
    //presentation
    //better grammars
        //update goal points..
        //rhythm changes - https://en.wikipedia.org/wiki/Rhythm_changes
            //https://www.freejazzlessons.com/4-jazz-turnarounds/
        //https://www.jazzadvice.com/7-killer-turnarounds-for-jazz-improvisation/
    //orchestrations
    //mix orchestrations

    
    
    /*
     //checking grammar elements arrive...
     for(int i=0; i<_all_gr.size(); i++){
     
     cout << endl << endl << endl << "RULE COMPARER:" << endl;
     cout << "grammar number: " << parser.all_gr[i].gr_pop << endl;
     cout << "GR1 form length: " << parser.all_gr[i].form_length << endl << endl;
     }
     */
    
    //logic
    /*
    compares:
			most likely (/ an average) upcoming
			most likely (/ an average) of GR2 rule
			N best rules are compared by further length.. - why not from start?
				in order to control / stress (by N) local VS formal compatibility
				UI: (impacting N) local smoothness VS large-scale smoothness
		keeps history for comparison
			run separate cycle: 
				if in function vector, then keep / replace.. 
				else don’t
     */
}


vector<int> Rule_comparer::get_sect_lengths(G_parser::rule _S_rule, int _gr_pop){

    vector<int> sect_lengths;
    
    for (int i=0; i < _S_rule.prod_times.size(); i++){
        
        //calculate sect lengths
        int len;
        if (i == _S_rule.prod_times.size() - 1) len = parser.all_gr[_gr_pop].form_length - _S_rule.prod_times[i];
        else len = _S_rule.prod_times[i+1] - _S_rule.prod_times[i];
        sect_lengths.push_back(len);
    }
    /*
     cout << endl << "sect_lengths: " << endl;
     for (int i=0; i < sect_lengths.size(); i++) cout << sect_lengths[i] << endl;
     cout << endl;
     */
    
    return sect_lengths;
}


G_parser::rule Rule_comparer::get_S_rule(int _gr_pop){

    G_parser::rule _S_rule;
    
    for (int i=0; i<parser.all_gr[_gr_pop].general_rules.size(); i++){
        
        for (int j=0; j<parser.all_gr[_gr_pop].general_rules[i].left_str.size(); j++){
            
            if (parser.all_gr[_gr_pop].general_rules[i].left_str[j] == "S"){
                
                _S_rule = parser.all_gr[_gr_pop].general_rules[i];
                break;
            }
        }
    }
    
    return _S_rule;
}


void Rule_comparer::compare_t_g(){//compares unrewritten functions till goal
    

    //get unrewritten funcs area (distance) t_g, i.e. un_dist
    cout << endl << "un_dist: " << un_dist << endl;
    
    //get_un_dist_scores (withou histories)
    //compare curr_-next_func_lines for all Sect endings..
        //(for necessary length - could even be longer than a sect)
    
    //scores of non-rewritten functions till goal (without histories..)
    un_dist_scores = get_un_dist_scores();
    formed_local_scores = form_scores();
    
    //logging
    parser.logger.transition_data_log.append("\nformed_local_scores:\n");
    for (int i=0; i < formed_local_scores.size(); i++){
        
        parser.logger.transition_data_log.append("number: " + ofToString(i) + ":, score: " + ofToString(formed_local_scores[i][0]) + ", i: " + ofToString(formed_local_scores[i][1]) + ", j: " + ofToString(formed_local_scores[i][2]) + ", l: " + ofToString(formed_local_scores[i][3]) + "\n");
    }
    
    //get percentage of best scores (depending how local vs form-aware we want the transition to be..)
    //1 for 10%, 2 for 20%,... 5 for 50%.
    best_local_scores = get_best_local_scores_percentage();//score, i, j, l
    //best_local_scores = get_best_local_scores_singleBest();//score, i, j, l
    
    //logging
    parser.logger.transition_data_log.append("\nbest_local_scores:\n");
    for (int i=0; i < best_local_scores.size(); i++){
        
        parser.logger.transition_data_log.append("number: " + ofToString(i) + ":, score: " + ofToString(best_local_scores[i][0]) + ", i: " + ofToString(best_local_scores[i][1]) + ", j: " + ofToString(best_local_scores[i][2]) + ", l: " + ofToString(best_local_scores[i][3]) + "\n");
    }
}


vector<vector<vector<int>>> Rule_comparer::get_un_dist_scores(){

    vector<vector<vector<int>>> _un_dist_scores;
    
    for (int i=0; i < aug_sect_rules.size(); i++){
        
        _un_dist_scores.push_back(vector<vector<int>>());
        
        for (int j=0; j < curr_func_lines.size(); j++){
            
            _un_dist_scores[i].push_back(vector<int>());
            
            for (int l=0; l < next_func_lines.size(); l++){
                
                int score = 0;
                int m = abs(aug_sect_rules[(i+1)%aug_sect_rules.size()].position - un_dist) % parser.all_gr[next_gr].form_length;
                
                for (int k=0; k < curr_func_lines[j].size(); k++){
                    
                    if (next_func_lines[l][m].name == curr_func_lines[j][k].name) score++;
                    
                    m++;
                }
                
                _un_dist_scores[i][j].push_back(score);
            }
        }
    }
    
    return _un_dist_scores;
}


vector<vector<int>> Rule_comparer::form_scores(){
//set score in: score, i, j, l form.

    vector<vector<int>> _formed_scores;
    int s_pop = 0; //scores population
    
    //store un_dist_scores in _sorted_scores
    for (int i=0; i < un_dist_scores.size(); i++){
        
        for (int j=0; j < un_dist_scores[i].size(); j++){
            
            for (int l=0; l < un_dist_scores[i][j].size(); l++){
                
                _formed_scores.push_back(vector<int>());//storing them to get them descending later..
                //score, i, j, l
                _formed_scores[s_pop].push_back(un_dist_scores[i][j][l]);
                _formed_scores[s_pop].push_back(i);
                _formed_scores[s_pop].push_back(j);
                _formed_scores[s_pop].push_back(l);
                
                s_pop ++;
            }
        }
    }
    
    return _formed_scores;
}


vector<vector<int>> Rule_comparer::bubble_sort_scores(vector<vector<int>> _unsorted_scores){

    //logging
    parser.logger.transition_data_log.append("BubbleSort used\n");
    
    vector<vector<int>> _sorted_scores = _unsorted_scores;
    
    //SORT _sorted_scores
    for (int n=0; n < _sorted_scores.size() - 1; n++){
        
        if ((n%5)==0){
        
            cout << endl << "workload n" << endl;
        }
        
        for (int a=0; a < _sorted_scores.size() - 1; a++){
            
            if ((a%10000)==0){
                
                cout << endl << "workload a" << endl;
            }
            
            if (_sorted_scores[a][0] < _sorted_scores[a+1][0]){
                
                vector<int> aux_score = _sorted_scores[a];
                _sorted_scores[a] = _sorted_scores[a+1];
                _sorted_scores[a+1] = aux_score;
            }
        }
        
    }
    
    return _sorted_scores;
}


vector<vector<int>> Rule_comparer::merge_sort_scores(vector<vector<int>> _unsorted_scores){

    //logging
    parser.logger.transition_data_log.append("MergeSort used\n");
    
    //http://interactivepython.org/runestone/static/pythonds/SortSearch/TheMergeSort.html
    vector<vector<int>> _sorted_scores = _unsorted_scores;
    
    if (_unsorted_scores.size() > 1){
    
        //_unsorted_scores = merge_sort_scores(_unsorted_scores);
        
        //SPLIT VECTOR
        vector<vector<int>> half_1, half_2;
        int split = _unsorted_scores.size() / 2;
        
        for (int i=0; i < _unsorted_scores.size(); i++){
        
            if (i<split) half_1.push_back(_unsorted_scores[i]);
            else half_2.push_back(_unsorted_scores[i]);
        }
        
        //ITERATE SPLIT SORT MERGE..
        half_1 = merge_sort_scores(half_1);
        half_2 = merge_sort_scores(half_2);
        
        //MERGE 2 VECTORS
        int a = 0;
        int b = 0;
        int c = 0;
        //Compare the two - one-by-one
        while ((a < half_1.size()) && (b < half_2.size())){
        
            if (half_1[a][0] > half_2[b][0]){
            
                _sorted_scores[c] = half_1[a];
                a++;
                c++;
            }
            else {//else if (half_1[a][0] < half_2[b][0]){
            
                _sorted_scores[c] = half_2[b];
                b++;
                c++;
            }
        }
        //Push back the remaining
        while (a < half_1.size()){
        
            _sorted_scores[c] = half_1[a];
            a++;
            c++;
        }
        while (b < half_2.size()){
            
            _sorted_scores[c] = half_2[b];
            b++;
            c++;
        }
    }
    
    return _sorted_scores;
}


vector<vector<int>> Rule_comparer::insertion_sort_scores(vector<vector<int>> _unsorted_scores){
    
    //logging
    parser.logger.transition_data_log.append("InsertionSort used\n");
    
    //http://www.geeksforgeeks.org/insertion-sort/
    
    int key, j;
    
    for (int i=1; i < _unsorted_scores.size(); i++){
    
        key = _unsorted_scores[i][0];
        j = i-1;
        
        /* Move elements of arr[0..i-1], that are
         greater than key, to one position ahead
         of their current position */
        while (j >= 0 && _unsorted_scores[j][0] < key)
        {
            _unsorted_scores[j+1][0] = _unsorted_scores[j][0];
            j = j-1;
        }
        _unsorted_scores[j+1][0] = key;
    }
    
    vector<vector<int>> _sorted_scores = _unsorted_scores;
    return _sorted_scores;
}


vector<vector<int>> Rule_comparer::TimSort_scores(vector<vector<int>> _unsorted_scores){

    //logging
    parser.logger.transition_data_log.append("TimSort used\n");
    
    //http://www.geeksforgeeks.org/timsort/
    
    int RUN = 1000;
    int n = _unsorted_scores.size();
    
    for (int i = 0; i < n; i+=RUN) _unsorted_scores = insertion_sort_scores(_unsorted_scores);
    
    // start merging from size RUN (or 32). It will merge
    // to form size 64, then 128, 256 and so on ....
    for (int size = RUN; size < n; size = 2*size)
    {
        // pick starting point of left sub array. We
        // are going to merge arr[left..left+size-1]
        // and arr[left+size, left+2*size-1]
        // After every merge, we increase left by 2*size
        for (int left = 0; left < n; left += 2*size)
        {
            // find ending point of left sub array
            // mid+1 is starting point of right sub array
            
            //int mid = left + size - 1;
            //int right = min((left + 2*size - 1), (n-1));
            
            // merge sub array arr[left.....mid] &
            // arr[mid+1....right]
            _unsorted_scores = TimMerge_scores(_unsorted_scores);
        }
    }
    
    vector<vector<int>> _sorted_scores = _unsorted_scores;
    return _sorted_scores;
}


vector<vector<int>> Rule_comparer::TimMerge_scores(vector<vector<int>> _unsorted_scores){//NON-ITERATIVE
    
    vector<vector<int>> _sorted_scores = _unsorted_scores;
    
    if (_unsorted_scores.size() > 1){
        
        //_unsorted_scores = merge_sort_scores(_unsorted_scores);
        
        //SPLIT VECTOR
        vector<vector<int>> half_1, half_2;
        int split = _unsorted_scores.size() / 2;
        
        for (int i=0; i < _unsorted_scores.size(); i++){
            
            if (i<split) half_1.push_back(_unsorted_scores[i]);
            else half_2.push_back(_unsorted_scores[i]);
        }
        
        //ITERATE SPLIT SORT MERGE..
        //half_1 = merge_sort_scores(half_1);
        //half_2 = merge_sort_scores(half_2);
        
        //MERGE 2 VECTORS
        int a = 0;
        int b = 0;
        int c = 0;
        //Compare the two - one-by-one
        while ((a < half_1.size()) && (b < half_2.size())){
            
            if (half_1[a][0] > half_2[b][0]){
                
                _sorted_scores[c] = half_1[a];
                a++;
                c++;
            }
            else {//else if (half_1[a][0] < half_2[b][0]){
                
                _sorted_scores[c] = half_2[b];
                b++;
                c++;
            }
        }
        //Push back the remaining
        while (a < half_1.size()){
            
            _sorted_scores[c] = half_1[a];
            a++;
            c++;
        }
        while (b < half_2.size()){
            
            _sorted_scores[c] = half_2[b];
            b++;
            c++;
        }
    }
    
    return _sorted_scores;
}



vector<vector<int>> Rule_comparer::get_best_hist_scores(vector<vector<int>> _scores){

    //int top_score = _scores[0][0];
    vector<vector<int>> _best_hist_scores;
    
    int n=0;
    
    while(_scores[0][0] == _scores[n][0]){
    
        _best_hist_scores.push_back(_scores[n]);
        n++;
        if (n > _scores.size()-1) break;
    }
    
    return _best_hist_scores;
}


vector<vector<int>> Rule_comparer::get_top_curr_func_line_scores(vector<vector<int>> _scores){

    //int top_score = _scores[0][0];
    vector<vector<int>> _t_f_l_scores;
    
    int n=0;
    
    while(_scores[0][2] == _scores[n][2]){
        
        _t_f_l_scores.push_back(_scores[n]);
        n++;
        if (n > _scores.size()-1) break;
    }
    
    return _t_f_l_scores;
}

vector<vector<int>> Rule_comparer::get_sort_n_best_scores(vector<vector<int>> _scores){

    //int top_score = _scores[0][0];
    vector<vector<int>> _s_n_b_scores;
    
    //SORT based on 'l'
    for (int n=0; n < _scores.size() - 1; n++){
        
        for (int a=0; a < _scores.size() - 1; a++){
            
            if (_scores[a][3] > _scores[a+1][3]){
                
                vector<int> aux_score = _scores[a];
                _scores[a] = _scores[a+1];
                _scores[a+1] = aux_score;
            }
        }
    }
    
    
    int n=0;
    
    //keep only the ones with the best 'l'
    while(_scores[0][3] == _scores[n][3]){
        
        _s_n_b_scores.push_back(_scores[n]);
        n++;
        if (n > _scores.size()-1) break;
    }
    
    return _s_n_b_scores;
}


vector<vector<int>> Rule_comparer::get_earliest_next_form_scores(vector<vector<int>> _scores){

    //get earliest BUT PRIORITISE i == S_rule_next.times[-1]
    
    vector<vector<int>> aux_scores;
    
    for (int n=0; n < _scores.size(); n++){
    
        if (_scores[n][1] == (S_rule_next.prod_times.size() - 1)) aux_scores.push_back(_scores[n]);
    }
    
    if (aux_scores.size() > 0) return aux_scores;
    else return get_sort_n_best_scores(_scores);
}


vector<vector<int>> Rule_comparer::get_best_local_scores_percentage(){

    vector<vector<int>> _best_scores;
    
    //logging
    parser.logger.transition_data_log.append("Sort local timestamp (start): " + ofGetTimestampString("[%M:%S.%i] \n"));
    
    //ofGetTimestampString("[%M:%S.%i] \n"));
    
    //sorted_local_scores = bubble_sort_scores(formed_local_scores);
    //sorted_local_scores = merge_sort_scores(formed_local_scores);
    //sorted_local_scores = insertion_sort_scores(formed_local_scores);
    sorted_local_scores = TimSort_scores(formed_local_scores);
    
    //logging
    parser.logger.transition_data_log.append("Sort local timestamp (end): " + ofGetTimestampString("[%M:%S.%i] \n"));
    
    //get percentage of sorted scores for best scores (depending how local vs form-aware we want the transition to be..)
    //1 for 10%, 2 for 20%,... 5 for 50%..
    //int b_s_pop = int(_sorted_scores.size() * (score_pc / 100));//best_scores population
    
    
    b_s_pop = int(float(sorted_local_scores.size()) * (float(score_pc) / 100.0));//best_scores population
    
    //logging
    parser.logger.transition_data_log.append("score_pc (percentage): " + ofToString(score_pc) + "b_s_pop (percentage): " + ofToString(b_s_pop));


    if (b_s_pop == 0) b_s_pop = 2;
    
    for (int n=0; n < b_s_pop; n++){
    
        _best_scores.push_back(sorted_local_scores[n]);
    }
    
    //keep the first cause they may be the most distinct (earlier in the form)
    
    return _best_scores;
}


void Rule_comparer::compare_include_history(int form_pc){//history with N best (next_gr side of the pair)
    
    //capture history
    //vector<G_parser::elem_ID> history;
    history = parser.function_cycle;
    
    //logging
    parser.logger.transition_data_log.append("History function_cycle: [function_name : time] \n");
    for (int i=0; i < history.size(); i++) parser.logger.transition_data_log.append("[" + history[i].name + " : " + ofToString(history[i].time[1]) + "]");
    parser.logger.transition_data_log.append("\n");
    
    parser.function_cycle.clear();//to avoid memory leak..
    
    //captures between history and first curr_un_dist
    for (int i=0; i < intermediate_functions.size(); i++){
    
        history.push_back(intermediate_functions[i]);
    }
    
    intermediate_functions.clear();
    
    //logging
   parser.logger.transition_data_log.append("History function_cycle (INCL.intermediate): [function_name : time] \n");
    for (int i=0; i < history.size(); i++) parser.logger.transition_data_log.append("[" + history[i].name + " : " + ofToString(history[i].time[1]) + "]");
    parser.logger.transition_data_log.append("\n");
    
    
    //calculate new scores including history
    //whole section before g_p
    int t_s = S_rule_curr.prod_times.size();//times size
    int prev_sect_start, s_len;
    
    //iterate this as necessary..
    for (int i=0; i < t_s; i++){
    
        if (g_p == S_rule_curr.prod_times[i]){
            
            prev_sect_start = S_rule_curr.prod_times[((i-form_pc) + t_s) % t_s];
            s_len = sect_lengths_curr[((i-form_pc) + t_s) % t_s];
        }
    }
    
    //compare with best_local_scores - score, i, j, l
    
    //int curr_t = (undist_bar - intermediate_functions.size()) - 1;//current bar wwhen the transition occurs - else pass seq_t in function
    
    
    
    int curr_f_l = parser.all_gr[curr_gr].form_length;
    int next_f_l = parser.all_gr[next_gr].form_length;
    int add_hist_length = ((undist_bar - prev_sect_start) + curr_f_l ) % curr_f_l;
    //int aux_time = prev_sect_start;
    
    //logging
    parser.logger.transition_data_log.append("hist_length added: " + ofToString(add_hist_length) + "\ns");
    
    //make new scores
    for (int n=0; n < best_local_scores.size(); n++){
    
        //compare each gr_2 tail of tail with history
        //best_local_scores[n]
        
        //for s_len
        
        int _score, _i, _j, _l;
        _score = best_local_scores[n][0];
        _i = best_local_scores[n][1];
        _j = best_local_scores[n][2];
        _l = best_local_scores[n][3];
        
        int aux_score = 0;
        
        //get start position in next_lines[_l] : distance of prev_sect_start from g_p.. : befpor the sect end..
        int prev_dist_g_p = ((g_p + curr_f_l) - prev_sect_start) % curr_f_l;
        int next_lines_pos = ((S_rule_next.prod_times[(_i + 1) % S_rule_next.prod_times.size()] + next_f_l) - prev_dist_g_p) % next_f_l;
        
        //get start position in history
        int hist_pos = history.size() - add_hist_length;
        
        for (int k = 0; k < add_hist_length; k++){
            
            //compare preceding of next_func_lines with history for this lenght..
            if (next_func_lines[_l][next_lines_pos].name == history[hist_pos].name) aux_score ++;
         
            next_lines_pos = (next_lines_pos + 1) % next_f_l;
            hist_pos ++;
        }
        
        hist_scores.push_back(best_local_scores[n]);
        hist_scores[n][0] += aux_score;//adds history score to local score..
        
        //keep new scores of combos..
    }
    
    //logging
    parser.logger.transition_data_log.append("Sort hist timestamp (start): " + ofGetTimestampString("[%M:%S.%i] \n"));
    
    //sorted_hist_scores = bubble_sort_scores(hist_scores);
    //sorted_hist_scores = merge_sort_scores(hist_scores);
    //sorted_hist_scores = insertion_sort_scores(hist_scores);
    sorted_hist_scores = TimSort_scores(hist_scores);
    
    parser.logger.transition_data_log.append("Sort hist timestamp (end): " + ofGetTimestampString("[%M:%S.%i] \n"));
    
    best_hist_scores = get_best_hist_scores(sorted_hist_scores);//keep all of no1 scores..
    top_curr_func_line_scores = get_top_curr_func_line_scores(best_hist_scores);//keep scores of the most likely (i.e. smallest pop number) of curr_func_lines, i.e. 'j'..
    top_next_func_line_scores = get_sort_n_best_scores(top_curr_func_line_scores);//best of 'l'
    
    //earliest_next_form_scores = get_sort_n_best_scores(top_next_func_line_scores);
    //earliest_next_form_scores = (top_next_func_line_scores);
    earliest_next_form_scores = get_sort_n_best_scores(top_next_func_line_scores);
    
    final_best_score = earliest_next_form_scores[0];//in case (even though unlikely) theres is more than one, keep the 1st (enough refinement till here anyway..)
    
    //logging
    parser.logger.transition_data_log.append("\nbest_hist_scores:\n");
    for (int i=0; i < best_hist_scores.size(); i++) parser.logger.transition_data_log.append("number: " + ofToString(i) + ":, score: " + ofToString(best_hist_scores[i][0]) + ", i: " + ofToString(best_hist_scores[i][1]) + ", j: " + ofToString(best_hist_scores[i][2]) + ", l: " + ofToString(best_hist_scores[i][3]) + "\n");
    parser.logger.transition_data_log.append("\ntop_curr_func_line_scores:\n");
    for (int i=0; i < top_curr_func_line_scores.size(); i++) parser.logger.transition_data_log.append("number: " + ofToString(i) + ":, score: " + ofToString(top_curr_func_line_scores[i][0]) + ", i: " + ofToString(top_curr_func_line_scores[i][1]) + ", j: " + ofToString(top_curr_func_line_scores[i][2]) + ", l: " + ofToString(top_curr_func_line_scores[i][3]) + "\n");
    parser.logger.transition_data_log.append("\ntop_next_func_line_scores:\n");
    for (int i=0; i < top_next_func_line_scores.size(); i++) parser.logger.transition_data_log.append("number: " + ofToString(i) + ":, score: " + ofToString(top_next_func_line_scores[i][0]) + ", i: " + ofToString(top_next_func_line_scores[i][1]) + ", j: " + ofToString(top_next_func_line_scores[i][2]) + ", l: " + ofToString(top_next_func_line_scores[i][3]) + "\n");
    parser.logger.transition_data_log.append("\nearliest_next_form_scores:\n");
    for (int i=0; i < earliest_next_form_scores.size(); i++) parser.logger.transition_data_log.append("number: " + ofToString(i) + ":, score: " + ofToString(earliest_next_form_scores[i][0]) + ", i: " + ofToString(earliest_next_form_scores[i][1]) + ", j: " + ofToString(earliest_next_form_scores[i][2]) + ", l: " + ofToString(earliest_next_form_scores[i][3]) + "\n");
    parser.logger.transition_data_log.append("\nFINAL_BEST_SCORE: \nscore: " + ofToString(final_best_score[0]) + ", i: " + ofToString(final_best_score[1]) + ", j: " + ofToString(final_best_score[2]) + ", l: " + ofToString(final_best_score[3]) + "\n");
    
    
    parser.logger.transition_data_log.append("Sort hist timestamp (end): " + ofGetTimestampString("[%M:%S.%i] \n"));
    /*
    //The classicness (high probability in the style) of curr_/next_line are far more important than the position in the form, i.e. recognisabiity) the most classic of the style (the most probable lines..)
        this way I know how much I am in/out of style (of curr/next gr) for in favour of compatibility.. (include in LOG)
     */
    
    //start mixing..
    
    //make sure parser.function_cycle restarts fine after delete.. - should be fine as is!!
}


void Rule_comparer::rewrite_funcs(){

    rewrite_curr_gr_t_g();//till funcs till g_p (goal point)
    
    rewrite_next_all();//expand all next form till funcs
    
    //cout << endl << "for b.p." << endl;
}


void Rule_comparer::rewrite_curr_gr_t_g(){
    //curr_gr: make vector of all possible functions till goal.. (irrespective of probabilities)
    
    cout << endl << "entering rewrite_curr_gr_t_g()" << endl;
    
    parser.rewriting_curr = 1;
    
    //expand to functions
    parser.till_function = 1;
    //parser.gr_pop = curr_gr;//not necessary here but might become..
    
    int f_l = parser.all_gr[curr_gr].form_length;
    
    
    int curr_bar = (f_l + g_p - (dist)) % f_l;//17
    //int curr_bar =
    
    cout << endl << "f_l: " << f_l << endl;
    cout << endl << "curr_bar is: " << curr_bar << endl;
    
    int to_g_p = g_p;
    if (to_g_p==0) to_g_p = f_l;
    if (curr_bar > to_g_p) to_g_p = to_g_p + f_l;
    
    //for (int j=curr_bar; j < to_g_p - 1; j++){//g_p - 1 to avoid rewriting on g_p
    for (int j=0; j < dist - 1; j++){//g_p - 1 to avoid rewriting on g_p
        
        //find rule with
        //dynamic assignment of aux_cycle
        //& is_function
        //& ALL productions (irrespective of production probabilities), i.e. musical space constraint
        
        vector<int> aux_t = {0, 0, 0, curr_bar % f_l, 0};
        //parser.setup_t = {0, 0, 0, j, 0};
        
        cout << endl << "curr_bar it:: " << curr_bar << endl;
        
        cout << endl << "aux_cycle: ";
        for (int k=0; k<parser.aux_cycle.size(); k++) cout << parser.aux_cycle[k].name << " ";
        cout << endl << "(keeps the last right_side(s?))" << endl;
        
        //going forward in cycle as long as functions already there..
        while (parser.is_function(aux_t)){ //CHECK THIS--> //&& !un_dist_found) {
            
            if (curr_bar == g_p){
                
                parser.till_function = 0;
                cout << endl << "breaking in while" << endl;
                break;
            }
            
            //so that it doesn't get the next func even when it's not pre-rewritten..
            //AND THIS--> if (!un_dist_found) //IT COMES I LATER AS WELL and it SHOULDN'T
            if (!un_dist_found) intermediate_functions.push_back(parser.aux_cycle[curr_bar]);
            
            curr_bar = (curr_bar + 1) % f_l;
            j++;
            
            //int ti = j % f_l;
            //parser.setup_t = {0, 0, 0, j, 0};
            aux_t = {0, 0, 0, curr_bar, 0};
            
            //manually restard aux_cycle (place S on top, else it loops looking for func in isus4
            if (curr_bar==0) parser.aux_cycle[0].name = "S";
            
            cout << endl << "g_p in wh: " << g_p;
            cout << endl << "curr_bar in while: " << curr_bar << endl;
            cout << "j in while: " << j << endl;
        }
        
        //only for the first time, to get the distance of unrewritten funcs to g_p
        if (!un_dist_found){
        
            un_dist = dist - j;
            undist_bar = curr_bar;
            un_dist_found = 1;
        }
        
        
        if (curr_bar==0) parser.aux_cycle[0].name = "S";
        
        //UPDATE DIST IN HERE for updated g_p
        
        //if ((j % f_l) == (to_g_p % f_l)) break;//to avoid rewriting on g_p due to manual j++
        if (curr_bar == g_p){
            
            parser.till_function = 0;
            cout << endl << "breaking outside while" << endl;
            break;
        }
        
        cout << endl << "g_p in before find_rule: " << g_p;
        cout << endl << "curr_bar before find_rule: " << curr_bar << endl;
        cout << "j before find rule: " << j << endl;
        
        parser.find_rule(aux_t);
        
        curr_bar = (curr_bar + 1) % f_l;
        /*
        if (j==curr_bar){//filter with is_function only the first time..
        
            if (!parser.is_function(aux_t)) parser.find_rule(aux_t);//i.e. in order to avoid rewriting the next FUNCTION to type_level - then loop in (!is_function) find_rule())
        }
        else parser.find_rule(aux_t);
        */
         
        //else //aug_sect_rule. / a_s_r. = oti exei o curr_cycle ekei..
        
        /*
         cout << endl << "aux_cycle: ";
         for (int k=0; k<parser.aux_cycle.size(); k++) cout << parser.aux_cycle[k].name << " ";
         cout << endl;
         */
    }
    
    curr_func_chunks = parser.func_chunks;
    parser.func_chunks.clear();
    cout << endl << "curr_func_chunks size: " << curr_func_chunks.size() << endl;
    
    parser.rewriting_curr = 0;
}


void Rule_comparer::rewrite_next_all(){
    //if rewriting_next (r.left_str[0]!="S" || rewriting_next) do only once the whole S rule (in rewrite()), i.e. the whole form and that's it..
    
    cout << endl << "entering rewrite_next_all()" << endl;
    
    parser.rewriting_next = 1;
    
    //expand to functions
    parser.till_function = 1;
    parser.gr_pop = next_gr;//WATCH IT!! - make more dynamic??
    
    int f_l = parser.all_gr[next_gr].form_length;
    int curr_bar = 0;
    parser.aux_cycle[0].name = "S";
    
    vector<int> aux_t = {0, 0, 0, 0, 0};
    parser.find_rule(aux_t);
    
    next_func_chunks = parser.func_chunks;
    parser.func_chunks.clear();
    cout << endl << "next_func_chunks size: " << next_func_chunks.size() << endl;
    
    parser.rewriting_next = 0;
}


void Rule_comparer::initiate_aux_cycle(){
    
    //find longest form of the 2 grammars
    int longest_f_l;// = 800;
    for (int i=0; i < parser.all_gr.size(); i++){
    
        if (longest_f_l < parser.all_gr[i].form_length) longest_f_l = parser.all_gr[i].form_length;
    }
    
    //make size of aux cycle
    for (int i=0; i < longest_f_l; i++){
    
        parser.aux_cycle.push_back(G_parser::elem_ID());
    }
        
        //assign curr_cycle to aux_cycle
    parser.aux_cycle = parser.curr_cycle;
}



//einai form-aware mexri edw?? - context-aware
//check paper suggestion.. - will work??
    //exei nohma to expansion?? EINAI form-aware??
//go through method
//implement algor thought

//does it cover 2 vs 4-bar long phrases? -  does it include all?

/*
 merge..
gitsave
get to construct these in the beginning?
get score
mix in r/t only this..
 expand score to past
 expand score to future
 mix in r/t
construct func_lines on start? beginning?
*/

//make dynamic
vector<vector<G_parser::elem_ID>> Rule_comparer::construct_lines(vector<vector<G_parser::elem_ID>> func_chunks, int gr_num, int length, int init_curr_bar){//make all possible curr/next_lines
    
    vector<vector<G_parser::elem_ID>> prelast_lines;
    vector<vector<G_parser::elem_ID>> last_lines;
    
    prelast_lines.push_back(vector<G_parser::elem_ID>());
    
    for (int i=0; i < length; i++){//un_dist; i++){
        
        for (int j=0; j < func_chunks.size(); j++){
            
            if (func_chunks[j][0].time[1] == init_curr_bar) {
                
                for (int l=0; l < prelast_lines.size(); l++){
                    
                    last_lines.push_back(vector<G_parser::elem_ID>());
                    last_lines[last_lines.size()-1] = prelast_lines[l];//push existing
                    
                    for (int k=0; k < func_chunks[j].size(); k++){
                        
                        last_lines[last_lines.size()-1].push_back(func_chunks[j][k]);
                        
                    }
                }
            }
            else if (last_lines.size() > 0){
                
                prelast_lines = last_lines;
                last_lines.erase(last_lines.begin(), last_lines.end());
            }
            //else break; //???
            
        }
        
        init_curr_bar = (init_curr_bar + 1) % parser.all_gr[gr_num].form_length;
        
    }
    
    //manual restriction for less func_lines (to shorten the search space...)
    /*
    vector<vector<G_parser::elem_ID>> aux_prelast_lines = prelast_lines;
    prelast_lines.clear();
    
    for (int l=0; ((l < 2) && (l < aux_prelast_lines.size())); l++){
    
        prelast_lines.push_back(aux_prelast_lines[l]);
    }
     //*/
    
    return prelast_lines;
}


void Rule_comparer::filter_func_lines_pop(vector<vector<G_parser::elem_ID>>& func_lines, int max_pop){

    if (func_lines.size() > max_pop){
    
        vector<vector<G_parser::elem_ID>> _lines_aux;
        
        for (int i=0; i<400; i++) _lines_aux.push_back(func_lines[i]);
        func_lines.clear();
        func_lines = _lines_aux;
    }
}


void Rule_comparer::update_combination(vector<int>& seq_t){

    //if still transitioning
    //before g_p
    
    //get start and end of (initially needed) morph
    int start_t = curr_func_lines[0][0].time[1];
    int end_t = curr_func_lines[0][curr_func_lines[0].size()-1].time[1];
    int morph_len = (((end_t - start_t) + 1) + parser.all_gr[next_gr].form_length) % parser.all_gr[next_gr].form_length;
    
    //translate score to func members
    //final_best_score[2]//score, i, j, l;
    int _i = final_best_score[1];
    int _j = final_best_score[2];
    int _l = final_best_score[3];
    
    //get best vector<elem_ID> of both sides..
    vector<G_parser::elem_ID> curr_best = curr_func_lines[_j];
    vector<G_parser::elem_ID> next_best;
    int pts = S_rule_next.prod_times.size();
    int n_f_l_pos  = ((S_rule_next.prod_times[(_i + 1) % pts] - morph_len) + parser.all_gr[next_gr].form_length) % parser.all_gr[next_gr].form_length;//next _func_line position for selected '_i'
        //here _i - 1 because by form_pc we are looking at one section back..
        /*
         _i is at the previous section of g_p (and optimum start Sect of next_gr)
         the search expands to one sect earlier (due to form_pc = 2)
         so last _i is ok for beginning of form 2
         */
    for (int m=0; m < morph_len; m++){
        
        next_best.push_back(next_func_lines[_l][n_f_l_pos]);
        n_f_l_pos = (n_f_l_pos + 1) % parser.all_gr[next_gr].form_length;
    }
    
    //chooses functions/terminals probabilistically - weighted (distrubuted probabilities based on morph length)
        //& places them in cycles..
    weight_choose_morph(curr_best, next_best);
    
    //if stage (4 or) 5
    //place chosen sect non-T of GR_2 / restor cycle with offset bar count
    //produce based on GR_2
    
    //if not yet
    //set g_p = next ent_time
    //do same production search (return vector of functions) till there
    
    //if transition cancelled
    
    //(having kept next upcoming sect of curr_gr)
    //place next upcoming sect (of GR_1) in g_p
    //step back stages gradually (normalise till sect/g_p)
    //i.e. like reconciliation microgrammar, but more clever here..
}


void Rule_comparer::weight_choose_morph(vector<G_parser::elem_ID> _curr_best, vector<G_parser::elem_ID> _next_best){

    //logging
    parser.logger.transition_data_log.append("\ncurr_best (line): [name : time]\n");
    for (int i=0; i < _curr_best.size(); i++) parser.logger.transition_data_log.append("[" + _curr_best[i].name + " : " + ofToString(_curr_best[i].time[1]) + "], ");
    parser.logger.transition_data_log.append("\nnext_best (line): [name : time]\n");
    for (int i=0; i < _next_best.size(); i++) parser.logger.transition_data_log.append("[" + _next_best[i].name + " : " + ofToString(_next_best[i].time[1]) + "], ");
    
    int l = _curr_best.size();
    float percentage_unit = 100.0 / (l + 1);
    float percentage = percentage_unit;
    
    //for testing of weighted probabilistic mixing
    /*
    _curr_best[0].name = "GR1";
    _curr_best[1].name = "GR1";
    _curr_best[2].name = "GR1";
    _curr_best[3].name = "GR1";
    _next_best[0].name = "GR2";
    _next_best[1].name = "GR2";
    _next_best[2].name = "GR2";
    _next_best[3].name = "GR2";
     */
    
    int aux_gr_pop = parser.gr_pop;
    
    //logging
    parser.logger.transition_data_log.append("WEIGHT-CHOOSE FUNCTIONS (for undist): [name : time]\n");
    
    //WEIGHT-CHOOSE FUNCTIONS (for undist part of cycle, i.e. for functions that have yet to be generated)
    vector<G_parser::elem_ID> _chosen_functions;
    
    for (int i=0; i < l; i++){
        
        float random = (rand()%100);
        if (random >= percentage){
            _chosen_functions.push_back(_curr_best[i]);
            //logging
            parser.logger.transition_data_log.append("(from curr_gr) [" + _curr_best[i].name + " : " + ofToString(_curr_best[i].time[1]) + "], \n");
        }
        else{
            
            _chosen_functions.push_back(_next_best[i]);//might break if transition on first 4 bars.. (_next_best is empty..)
            //logging
            parser.logger.transition_data_log.append("(from next_gr) [" + _next_best[i].name + " : " + ofToString(_next_best[i].time[1]) + "], \n");
        }
        //restore elem_ID times to match parser.morph_cycle times
        //_chosen_functions[i].time[1] = i;
        _chosen_functions[i].time[1] = _curr_best[i].time[1];
        
        //build parser.morph_cycle
        parser.morph_cycle.push_back(_chosen_functions[i]);
        parser.aux_cycle[_curr_best[i].time[1]] = _chosen_functions[i];
        
        percentage += percentage_unit;
    }
    
    
    //logging
    parser.logger.transition_data_log.append("WEIGHT-CHOOSE TERMINALS (intermediate): [_t] (of find_rule(_t))\n");
    
    //WEIGHT-CHOOSE TERMINALS - INTERMEDIATE: weigthed rewrite of the INTERMEDIATE FUNCTIONS..
    percentage = percentage_unit;//restarting percentage count
    
    parser.updating_morph = 1;
    parser.till_function = 0;
    
    //logging
    parser.logger.transition_data_log.append("Morph update: [symbol : time]\n");
    
    
    int _f_l = parser.all_gr[curr_gr].form_length;
    
    int curr_bar_morph = ((g_p - dist) + _f_l) % _f_l;
    while (curr_bar_morph != _curr_best[0].time[1]){
    
        float random = (rand()%100);
        if (random >= percentage) {
         
            parser.gr_pop = curr_gr;
            //logging
            parser.logger.transition_data_log.append("(from curr_gr) [" + ofToString(curr_bar_morph) + "], \n");
        }
        else {
         
            parser.gr_pop = next_gr;
            //logging
            parser.logger.transition_data_log.append("(from next_gr) [" + ofToString(curr_bar_morph) + "], \n");
        }
        
        vector<int> _t = {0, 0, 0, curr_bar_morph, 0};
        //LAST ZERO-WHAT IF WE ARE IN further cycles?? - check also for transitions...!!!!! + aux_t further up..!!
        //probably not a problem (especially if aux_cycle deleted / reinitiated for the next transition)
        
        parser.find_rule(_t);
        
        curr_bar_morph = (curr_bar_morph + 1) % _f_l;
        percentage += percentage_unit;
    }
    
    //logging
    parser.logger.transition_data_log.append("WEIGHT-CHOOSE TERMINALS (undist): [_t] (of find_rule(_t))\n");
    
    //WEIGHT-CHOOSE TERMINALS - UNDIST: weighted rewrite of functions of curr_func_lines size.. (also called local here..)
    percentage = percentage_unit;//restarting percentage count
    
    for (int i=0; i < l; i++){
        
        float random = (rand()%100);
        if (random >= percentage) {
            
            parser.gr_pop = curr_gr;
            //logging
            parser.logger.transition_data_log.append("(from curr_gr) [" + ofToString(curr_bar_morph) + "], \n");
        }
        else {
        
            parser.gr_pop = next_gr;
            //logging
            parser.logger.transition_data_log.append("(from next_gr) [" + ofToString(curr_bar_morph) + "], \n");
        }
        
        vector<int> _t = {0, 0, 0, _curr_best[i].time[1], 0};
        //LAST ZERO-WHAT IF WE ARE IN further cycles?? - check also for transitions...!!!!! + aux_t further up..!!
            //probably not a problem (especially if aux_cycle deleted / reinitiated for the next transition)
        parser.find_rule(_t);
        
        percentage += percentage_unit;
    }
    
    parser.gr_pop = aux_gr_pop;
    
    parser.updating_morph = 0;

    
    //logging
    parser.logger.transition_data_log.append("\n:Morph update end");
    
    
    //give to morph_cycle (gets only the last in its first position..?)
    
    cout << endl << "BP1:" << endl;
    
    //vector<string> _term_morph;
    //return _term_morph;
}


void Rule_comparer::build_next_form(){

    parser.building_next_form = true;
    
    parser.initiate_cycle();
    
    //get intro point to next form, i.e. n_s_pos
    int _i = final_best_score[1];
    n_s_pos = S_rule_next.prod_times[(_i+1) % S_rule_next.prod_times.size()];//next sect position
    
    vector<int> aux_t = {0, 0, 0, 0, 0};
    
    //SET GR_pop??
    //parser.gr_pop = next_gr;
    
    parser.find_rule(aux_t);//under building_next_form, i.e. rewrite S_rule till sects
    
    parser.building_next_form = false;//to put find_rule in normal iterative mode for future..
    
    //unfold S_rule_next in curr_cycle
        //manually OR through find_rule (with stop at Sect)
    
    /*
    string sect = S_rule_next.right_side[0].right_str[(_i+1) % S_rule_next.right_side[0].right_str.size()];//optimal sect start
    
    string sect_naked;//take the '(' out of SectA(*)
    for (int c=0; c < sect.size(); c++){
    
        if (sect[c] != '(') sect_naked.push_back(sect[c]);
        else break;
    }
    
    //making sure it starts with "S" if top of the form..
    if (n_s_pos==0) sect_naked = "S";
    
    parser.curr_cycle[n_s_pos].name = sect_naked;
    
    //restart from point of next_gr in the form (like in the A-B way kind of..)
        //maybe when g_p reached??
    cout << endl << "BP:" << endl;
     */
}


void Rule_comparer::build_next_sects(){

    
}


void Rule_comparer::clearance(){

    g_p = -1;
    
    un_dist_found = false;
    //un_dist = -1;
    //undist_bar = -1;
    
    n_s_pos = -1;
    
    parser.aux_cycle.clear();
    parser.morph_cycle.clear();
    //curr_gr = 0;
    //next_gr = 0;
    sect_lengths_curr.clear();
    sect_lengths_next.clear();
    aug_sect_rules.clear();
    //S_rule_curr
    //S_rule_next
    curr_func_chunks.clear();
    next_func_chunks.clear();
    hist_scores.clear();
    sorted_hist_scores.clear();
    best_hist_scores.clear();
    top_curr_func_line_scores.clear();
    top_next_func_line_scores.clear();
    earliest_next_form_scores.clear();
    final_best_score.clear();
    curr_func_lines.clear();
    next_func_lines.clear();
    un_dist_scores.clear();
    formed_local_scores.clear();
    
    sorted_local_scores.clear();
    best_local_scores.clear();

    intermediate_functions.clear();
    history.clear();
}


/*COMMENTS
 //get func_chunks of next_gr
 //test fitness
 //in 2 cycles (curr_gr & next_gr)
 //keep N best (2 sides: curr & next) - up to N^2 combinations..
 //higher N - less stress on locality / more on large scale form
 //look back
 //functions already played combined with
 //next_gr previous possible functions (for each decided point)
 
 //(((why not the MOST LIKELY till goal?? - because compatibility and smoothness is the major focus here..)))
 
 //look ahead??
 
 //get total scores of combinations
 //place best combination in 2 cycles
 
 //place next sect on g_p
 
 //mix 2 cycles into the main (playing)
 //based on transition phase
 
 //if g_p reached end transition (for now..) OR re do the proces..
 
 
 //get scores up to goal point
 //(((
 //of most compatible productions (on both sides..)
 //force the most compatible productions????
 //)))
 
 //search gr1 up to goal (existing + to-be-expaned)
 //search gr2 (for a sect start at goal) distance-to-goal-long before THAT sect, i.e. its TAIL!
 
 //keep rules&productions with 2/3 best scores.. (based on N, input etc..)
 
 //check history
 //compare function_cycle with MOST LIKELY production preceding the GR2 rule..
 //if same scores ---> check more history (bars one-by-one)
 //if no more available then chose at random (or 1st..)
 
 //??check future??
 
 //return
 //chosen (rule&)production of GR1 to goal point
 //chosen rule&production (i.e. sect & production) of GR2 around goal point (back and forth)
 
 //return the FUNCTION production(vector<string> OR vector<G_parser::elem_ID>)
 
 
 
 
 //VHMA VHMA na vrw poses fores xreiazetai kai pou to trans_update()
 
 //next_gr: make vector of all possible functions till goal.. (irrespective of probabilities)
 
 //make map of all possible func_chunkcs.. linearly..
 //--expand all possible sects gr2
 //get scores..
 //mix in real time..
 //till goal
 //till updating goal
 //if g_p reached and still transitioning:
 //re do the test process of BEST TILL GOAL + what has been played..
 //nested..!!
 //finalise transitions / introduce next_gr
 //presentation
 //better grammars
 //update goal points..
 //rhythm changes - https://en.wikipedia.org/wiki/Rhythm_changes
 //https://www.freejazzlessons.com/4-jazz-turnarounds/
 //https://www.jazzadvice.com/7-killer-turnarounds-for-jazz-improvisation/
 //orchestrations
 //mix orchestrations
 
 
 
 /*
 //checking grammar elements arrive...
 for(int i=0; i<_all_gr.size(); i++){
 
 cout << endl << endl << endl << "RULE COMPARER:" << endl;
 cout << "grammar number: " << parser.all_gr[i].gr_pop << endl;
 cout << "GR1 form length: " << parser.all_gr[i].form_length << endl << endl;
 }
 */

//logic
/*
 compares:
 most likely (/ an average) upcoming
 most likely (/ an average) of GR2 rule
 N best rules are compared by further length.. - why not from start?
 in order to control / stress (by N) local VS formal compatibility
 UI: (impacting N) local smoothness VS large-scale smoothness
 keeps history for comparison
 run separate cycle:
 if in function vector, then keep / replace..
 else don’t
 */
