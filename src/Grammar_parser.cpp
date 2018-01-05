//
//  Grammar_parser.cpp
//  
//
//  Created by Christodoulos Aspromallis on 25/04/2016.
//
//

#include "Grammar_parser.h"

G_parser::G_parser() : grammars(grammars_path, std::ifstream::in){

    //"int main(){}"
    
    //grammar_path = "/Users/christodoulosaspromallis/Desktop/UCL/PhD Year 2/Development/OF/of_v0.9.1_osx_release/apps/midiWorkspace/blues/parser_project/bin/data/blues_grammar.txt";
    //cout << "CONTRUCTOR" << endl;
    
    grammars_path = "/Users/christodoulosaspromallis/Documents/UCL/PhD_Y_3/OF/of_v0.9.8_osx_release/apps/myApps/V-ImprovedGrammars/bin/data/ImprovedGr-Unity3rdGrammarRecovery.py";
    
    //ifstream grammar(grammar_path, ifstream::in);
    grammars.open(grammars_path);
    
    if (grammars.fail()){
        cerr << "Error Opening File";
        exit(1);
    };
    
    //cout << "setting up" << endl;
    
    //get main grammar elements
    while(!grammars.fail()){//z<12//!grammar.eof() //or while(grammar)
        nc = get_nc();
        //c = grammar.get();
        //cout << z << "_1: " << nc << endl;
        
        get_new_grammar(nc);
    }
    
    //logging (all grammars..)
    for (int i=0; i < all_gr.size(); i++) log_curr_gr_elements(all_gr[i]);
    
    //cout << "GRAMMAR_POP: " << gr_pop << endl;
    
    //gr_pop = 0;//restricitng to read only gr 1
    
    //restart reading from top (for next loop)
    grammars.clear();
    grammars.seekg(0, ios::beg);
    
    /*
    //initialise curr_cycle with tonics (rootpitch level)
    for(int i=0; i<all_gr[gr_pop].harm_rh * all_gr[gr_pop].form_length; i++) {
        
        curr_cycle.push_back(elem_ID());
        curr_cycle[i].time = {0, i};// {beat, harm_rh next..}
        //curr_cycle[i].name = "i";
        //curr_cycle[0].name = "S";
    }
     */
    
    grammars.close();
}


void G_parser::get_new_grammar(string& nc){
    
    if (nc=="NEW_GRAMMAR"){
        
        rule_pop = 0;//restart rule count for next grammar
        all_rules.clear();//empty auxiliary rule vector of previous grammar (if existing)
        
        //initialise one grammar
        all_gr.push_back(gr());
        
        //get its population
        nc = get_nc();
        gr_pop = atoi(nc.c_str()) - 1;
        
        //necessary?? perhaps for neatness when searching both grammars..
        all_gr[gr_pop].gr_pop = gr_pop;
        
        //the rest taken will be in the next grammar
        // next gr intialised so rest materials (e.g. form_length) are stored with grammar 2..
        
        //logging
        logger.grammars_log.append("\n======== NEW GRAMMAR: " + ofToString(gr_pop) + " ========\n");
        
        //get_grammar_elements();
    }
    
    get_grammar_elements();
    
    /*
    if (all_gr.size() != 0){
     
        log_curr_gr_elements(all_gr[all_gr.size()-1]);
    }
     */
}


void G_parser::get_grammar_elements(){

    get_time_signature(nc);
    //cout << z << "_2: " << nc << endl;
    get_form_length(nc);
    //cout << z << "_3: " << nc << endl;
    get_harm_rh(nc);
    //cout << z << "_4: " << nc << endl;
    get_optimal_form_functions(nc);
    
    get_end_times(nc);
    
    get_decs(nc);
    
    get_functions(nc);
    
    get_terminals(nc);
    //get_basic_vectors(nc);
    //cout << z << "_5: " << nc << endl;
    store_rules(nc);
    //cout << z << "_6: " << nc << endl;
    
    //log_curr_gr_elements(all_gr[all_gr.size()-1]);
}


void G_parser::get_time_signature(string& nc){
    if (nc=="time_signature"){
        nc = get_nc();
        t_sign = atoi(nc.c_str());
        //cout << "t_sign is: " << nc << endl;
        all_gr[gr_pop].t_sign = t_sign;
        
        //logging
        logger.grammars_log.append("time signature: " + ofToString(t_sign) + "\n");
    }
}


void G_parser::get_form_length(string& nc){
    if (nc=="form_length"){
        nc = get_nc();
        form_length = atoi(nc.c_str());
        //cout << "form_length is: " << form_length << endl;
        all_gr[gr_pop].form_length = form_length;
        
        //logging
        logger.grammars_log.append("form length: " + ofToString(form_length) + "\n");
    }
}


void G_parser::get_harm_rh(string& nc){
    if (nc=="harmonic_rhythm"){
        nc = get_nc();
        harm_rh = atoi(nc.c_str());
        //cout << "harm_rh is: " << harm_rh << endl;
        all_gr[gr_pop].harm_rh = harm_rh;
        
        //logging
        logger.grammars_log.append("harm rhythm: " + ofToString(harm_rh) + "\n");
    }
}


void G_parser::get_optimal_form_functions(string& nc){
    vector<string> _optimal_functions;
    
    if (nc=="optimal_form_functions"){
        
        nc = get_nc();
        if (nc=="{"){
            
            nc = get_nc();
            while(nc!="}"){
                
                _optimal_functions.push_back(nc);
                nc = get_nc();
            }
        }
        all_gr[gr_pop].optimal_form_functions = _optimal_functions;
        
        //logging
        logger.grammars_log.append("optimal_form_functions: ");
        for (int i=0; i < _optimal_functions.size(); i++) logger.grammars_log.append(_optimal_functions[i] + ", ");
        logger.grammars_log.append("\n");
    }
}


void G_parser::get_end_times(string& nc){
    vector<int> end_times;
    
    if (nc=="end_times"){
        
        nc = get_nc();
        if (nc=="{"){
            
            nc = get_nc();
            while(nc!="}"){
                
                int end_time_aux;
                end_time_aux = atoi(nc.c_str()) - 1;
                end_times.push_back(end_time_aux);
                nc = get_nc();
            }
        }
        all_gr[gr_pop].end_times = end_times;
        
        //logging
        logger.grammars_log.append("end times: ");
        for (int i=0; i < end_times.size(); i++) logger.grammars_log.append(ofToString(end_times[i]) + ", ");
        logger.grammars_log.append("\n");
    }
}


void G_parser::get_decs(string& nc){
    vector<string> _decs;
    
    if (nc=="decs"){
        
        nc = get_nc();
        if (nc=="{"){
            
            nc = get_nc();
            while(nc!="}"){
                
                _decs.push_back(nc);
                nc = get_nc();
            }
        }
        all_gr[gr_pop].decs = _decs;
        
        //logging
        logger.grammars_log.append("decs: ");
        for (int i=0; i < _decs.size(); i++) logger.grammars_log.append(_decs[i] + ", ");
        logger.grammars_log.append("\ndecs population size: " + ofToString(_decs.size()) + "\n");
        logger.grammars_log.append("\n");
    }
}


void G_parser::get_functions(string& nc){
    vector<string> functions;
    
    if (nc=="functions"){
        
        nc = get_nc();
        if (nc=="{"){
            
            nc = get_nc();
            while(nc!="}"){
                
                functions.push_back(nc);
                nc = get_nc();
            }
        }
        all_gr[gr_pop].functions = functions;
        
        //logging
        logger.grammars_log.append("functions: ");
        for (int i=0; i < functions.size(); i++) logger.grammars_log.append(functions[i] + ", ");
        logger.grammars_log.append("\nfunctions population size: " + ofToString(functions.size()) + "\n");
        logger.grammars_log.append("\n");
    }
}


void G_parser::get_terminals(string& nc){
    vector<string> terminals;
    
    if (nc=="terminals"){
        
        nc = get_nc();
        if (nc=="{"){
        
            nc = get_nc();
            while(nc!="}"){
                
                terminals.push_back(nc);
                nc = get_nc();
            }
        }
        all_gr[gr_pop].terminals = terminals;
        
        //logging
        logger.grammars_log.append("terminals: ");
        for (int i=0; i < terminals.size(); i++) logger.grammars_log.append(terminals[i] + ", ");
        logger.grammars_log.append("\nterminals population size: " + ofToString(terminals.size()) + "\n");
        logger.grammars_log.append("(for note-set comparison of chords, i.e. support for style difference?\n");
    }
}

/*
void G_parser::get_basic_vectors(string& nc){//perhaps get all namespace and categorize/throw ERROR if unrecognised?
    bool is_basic_vect = (nc=="decision_bars"||nc=="cadence_bars"||nc=="optimal_form"||nc=="rootpitch"||nc=="type"||nc=="inversion"||nc=="position"||nc=="terminals");
    if (is_basic_vect){
        string v_name = nc;
        nc = get_nc();
        if (nc=="{"){
            nc = get_nc();
            while(nc!="}"){
                if (v_name=="decision_bars"){
                    int curr_bar = atoi(nc.c_str()) - 1;//only if nc=="decision_bars"
                    dec_bars.push_back(curr_bar);
                }
                else if (v_name=="cadence_bars"){
                    int curr_bar = atoi(nc.c_str()) - 1;
                    cad_bars.push_back(curr_bar);
                }
                else basic_vector[v_name].push_back(nc);
                nc = get_nc();
            }
            sort(dec_bars.begin(), dec_bars.end());
            sort(cad_bars.begin(), cad_bars.end());
        }
    }
}
 *///basic vectors


/*
void G_parser::empty_gr_aux_elements(){
    
    all_rules.clear();

    //t_sign, form_ength, harm_rh will be simply replaced by the new values of next grammar
}
 */


void G_parser::store_rules(string& nc){
    
    //new rule
    if (nc=="rule:"){
        //cout << "rules: entered and harn_rh==" << all_gr[gr_pop].harm_rh << " and all_gr[gr_pop].form_length==" << all_gr[gr_pop].form_length << endl;
        all_rules.push_back(rule());
        all_rules[rule_pop].is_optional = false;
        all_rules[rule_pop].timed_production = false;
        
        nc = get_nc();
        
        //left side
        while(nc!="->"){
            all_rules[rule_pop].left_str.push_back(nc);
            nc = get_nc();
        }
        
        //if (nc!="->") cout << "ERROR in line " << i << ": arrow required" << endl;
        int prod_pop=0;//production population
        
        while(nc!=":end_rule"){//store right_side
            all_rules[rule_pop].right_side.push_back(right_s());
            nc = get_nc();//production probability
            all_rules[rule_pop].right_side[prod_pop].prob = atof(nc.c_str());
            nc = get_nc();
            
            //nc = get_nc();////NECESSARY??
            
            //store right_str
            while(nc!="->" && nc!=":end_rule"){
                all_rules[rule_pop].right_side[prod_pop].right_str.push_back(nc);//={"STR1", "STR2"};//
                //nc = get_nc();
                
                store_opt_data(nc);
                store_prod_times(nc, all_rules[rule_pop]);
                
                nc = get_nc();
            }
            
            prod_pop++;
        }
        
        bool is_timed = false;
        vector<int> rule_time = {0, 0};//{beat, bar}
        
        //categorise (timed vs general)
        for (int i=0; i<all_rules[rule_pop].left_str.size(); i++) {
            
            for (int j=0; j<all_rules[rule_pop].left_str[i].length(); j++){
                
                if (all_rules[rule_pop].left_str[i][j]=='_'){
                    
                    is_timed = true;
                    int left_centre = i;
                    
                    //rule_time.clear();
                    
                    string bar_time_str, beat_time_str; // rule_semiq_str;//could be one variable only
                    j++;
                    
                    while(all_rules[rule_pop].left_str[i][j] != '_' && j<all_rules[rule_pop].left_str[i].length()){
                        bar_time_str.push_back(all_rules[rule_pop].left_str[i][j]);
                        j++;
                    }
                    
                    rule_time[1] = atoi(bar_time_str.c_str()) - 1;
                    
                    if (all_rules[rule_pop].left_str[i][j] == '_'){
                        
                        while (j<all_rules[rule_pop].left_str[i].length()){
                            j++;
                            beat_time_str.push_back(all_rules[rule_pop].left_str[i][j]);
                        }
                        
                        rule_time[0] = atoi(beat_time_str.c_str()) - 1;
                        
                    } else rule_time[0] = 0;
                    
                    all_rules[rule_pop].leftmost_time =  {rule_time[0], rule_time[1] - (i / all_gr[gr_pop].harm_rh)};
                    
                    //exclude "_*..", i.e. stores "I" in rule instead of "I_2" (plus time info of course)
                    all_rules[rule_pop].left_str[i] = exclude_times(all_rules[rule_pop].left_str[i]);
                    
                    //existing_rules.push_back();
                    break;
                    
                }
                else all_rules[rule_pop].leftmost_time = {0, 0};
                
            }
            
            if (is_timed) break;
            
        }
        
        all_rules[rule_pop].timed = is_timed;
        
        //storing timed_rules/general_rules separately
        if (is_timed) {
            //cout << "rule_time: " << rule_time[0] << " " << rule_time[1] << endl;
            existing_times.push_back(rule_time);//to test existence of rule_time, ELSE general_rules
            all_gr[gr_pop].timed_rules[rule_time].push_back(all_rules[rule_pop]);
            
        } else {
            
            all_gr[gr_pop].general_rules.push_back(all_rules[rule_pop]);
            //cout << "general rule" << endl;
        }
        
        rule_pop++;
    }
    
    //empty_gr_aux_elements();
}


void G_parser::store_opt_data(string& nc){
    
    //all_rules[rule_pop].options = {};//is this necessary? NOT! - {""} is first element..
    
    for(int i=0; i<nc.length(); i++){
        
        if (nc[i]=='|'){
            all_rules[rule_pop].is_optional = true;
            all_rules[rule_pop].opt_positions.push_back(all_rules[rule_pop].left_str.size() - 1);
            store_options(nc);
        }
    }
}


void G_parser::store_options(string& nc){
    
    string curr_opt;
    vector<string> curr_opt_v;
    
    for (int i=0; i<nc.length(); i++){
        
        while (nc[i]!='|' && i<nc.length()){
            
            curr_opt.push_back(nc[i]);
            i++;
        }
        
        //all_rules[rule_pop].options.push_back(curr_opt);
        curr_opt_v.push_back(curr_opt);
        curr_opt.erase();
    }
    
    all_rules[rule_pop].options.push_back(curr_opt_v);
    /*
     cout << "curr_opt_v added: ";
     for(int i=0; i<curr_opt_v.size(); i++){
     cout << curr_opt_v[i] << " ";
     }
     cout << endl;
     */
}


void G_parser::store_prod_times(string& nc, rule& r){
    
    for(int i=0; i<nc.length(); i++){
        
        if (nc[i]=='('){
            r.timed_production = true;
            
            //store production times
            string prod_t_str;
            i++;
            while (nc[i]!=')'){
                
                prod_t_str.push_back(nc[i]);
                i++;
            }
            int prod_t_int = atoi(prod_t_str.c_str()) - 1;
            r.prod_times.push_back(prod_t_int);
            
            //exclude '(*)'
            //exclude_times(nc)//need to store the ELEM (or smth) in r.right_str?
        }
    }
}


string G_parser::exclude_times(string& s_t_r){//excludes '(8)' or '_*'
    
    string new_str;
    for(int i=0; i<s_t_r.length(); i++){
        
        if (s_t_r[i] == '_' || s_t_r[i] == '(') break;
        new_str.push_back(s_t_r[i]);
    }
    
    return new_str;
}


void G_parser::log_curr_gr_elements(gr gram){

    //logging grammar Header
    logger.grammars_log.append("\n\n||||||||| Grammar * (rules overview) |||||||||\n");
    //logging
    //Sects
    logger.grammars_log.append("S rule: sects: [symbol : prod_time]\n");
    for (int i=0; i < gram.general_rules[0].right_side[0].right_str.size(); i++){
    
        logger.grammars_log.append("[" + gram.general_rules[0].right_side[0].right_str[i] + " : " + ofToString(gram.general_rules[0].prod_times[i]) + "]");
    }
    logger.grammars_log.append("]\n");
    
    //num of rules
    logger.grammars_log.append("======== RULE FAMILIES ========\n");
    logger.grammars_log.append("timed_rules pop size: " + ofToString(gram.timed_rules.size()) + "\n");
    logger.grammars_log.append("general_rules pop size: " + ofToString(gram.general_rules.size()) + "\n");
    
    
    //average length of rules etc..
    logger.grammars_log.append("======== NOT-T FAMILIES ========\n");
    
    //sect num + len
    logger.grammars_log.append("Sect number (i.e. Sects / form_length): " + ofToString(gram.general_rules[0].right_side[0].right_str.size()) + "\n");
    float avg_sect_len = (float)gram.form_length / (float)gram.general_rules[0].right_side[0].right_str.size();
    logger.grammars_log.append("-Average bars / sect: " + ofToString(avg_sect_len) + "\n");
    
    //dec num per sect
    //get num sect
    vector<int> dec_nums;
    
    for (int i=0; i < gram.general_rules[0].right_side[0].right_str.size(); i++){
        
        string _str = gram.general_rules[0].right_side[0].right_str[i];
        
        for (int j=0; j < gram.general_rules.size(); j++){
    
            for (int k=0; k < gram.general_rules[j].left_str.size(); k++){
            
                if (('S'==gram.general_rules[j].left_str[k][0]) && ('e'==gram.general_rules[j].left_str[k][1]) && ('c'==gram.general_rules[j].left_str[k][2]) && ('t'==gram.general_rules[j].left_str[k][3])){//(_str == gram.general_rules[j].left_str[k]){
                
                    dec_nums.push_back(gram.general_rules[j].right_side[0].right_str.size());
                    logger.grammars_log.append(_str + " dec number: " + ofToString(gram.general_rules[j].right_side[0].right_str.size()) + " || ");
                    //avg_decs.push_back(float());
                }
            }
        }
    }
    
    //get avg bars / dec
    int _sum = 0;
    for (int j=0; j < dec_nums.size(); j++) _sum += dec_nums[j];
    
    float avg_dec_nums;
    avg_dec_nums = (float)_sum / (float)dec_nums.size();
    
    float avg_dec_len;
    avg_dec_len = (float)avg_sect_len / (float)avg_dec_nums;
    
    logger.grammars_log.append("\n-Average bars / dec: " + ofToString(avg_dec_len) + "\n");
    
    
    /*
    level_name = "dec";
    rule_count = find_rule_pop(level_name);
    level_name = "dec";
    rule_count = find_rule_pop(level_name);
    level_name = "dec";
    rule_count = find_rule_pop(level_name);
     */
    

        //avrg length
            //pop / form length
    //function
        //number
    //pre_t
        //number
    
    
    logger.grammars_log.append("\nboring / repetitive??\n");
}


int G_parser::find_rule_pop(string& _name){
    /*
    for (int i=0; i < gram.general_rules.size(); i++){
        
        for (int j=0; j < gram.general_rules[i].left_str.size(); j++){
            
            if (gram.general_rules[i].left_str[j]=="dec") rule_count++;
        }
    }
    
    for (int i=0; i < gram.timed_rules.size(); i++){
        
        for (int j=0; j < gram.timed_rules[i].left_str.size(); j++){
            
            if (gram.timed_rules[i].left_str[j]=="dec") rule_count++;
        }
    }
     */
}


void G_parser::find_rule(vector<int>& seq_t){
    
    //cout << "reached 1" << endl;
    
    //cout << "seq_t: " << seq_t[2] << " & " << seq_t[3] << endl;
    // if (previous (in left_str) is rootpitch) { search for its terminal equivalents; }
    //if IV in previous context (instead of iv) then lookup corresponding chords (IV = iv*, iv6*, iv7*) OR (IV = iv*)
    //what about IV instead of iv later? (what if already translated as iv from IV?) --> e.g. "IV I_3" ->...
    //      if context not found, look for equivalent rootpitch
    //      perhaps categorise IV as iv|iv6|iv7
    
    bool rule_found = false;
    bool opt;
    rule r;
    
    int cont_size;
    vector<int> leftmost_t;
    
    //find (if) timed_rule
    //search existing_times
    for (int i=0; i<existing_times.size(); i++){//replace with while()
        
        //cout << "reached 2" << endl;
        if (seq_t[2]==existing_times[i][0] && seq_t[3]==existing_times[i][1]){//seq_t[2,3] for beat, bar..
            
            //cout << "reached 3" << endl;
            //check context (match rule with current context)
            for (int j=0; j<all_gr[gr_pop].timed_rules[{seq_t[2], seq_t[3]}].size(); j++){//in case more than 1 rule for seq_t?
                
                //cout << "reached 4" << endl;
                //check_context(seq_t);
                
                //i6 could map to I etc. if context not found ?
                cont_size = all_gr[gr_pop].timed_rules[{seq_t[2], seq_t[3]}][j].left_str.size();
                leftmost_t = all_gr[gr_pop].timed_rules[{seq_t[2], seq_t[3]}][j].leftmost_time;
                opt = all_gr[gr_pop].timed_rules[{seq_t[2], seq_t[3]}][j].is_optional;
                
                vector<string> curr_cont = get_context(leftmost_t, cont_size);
                
                if (all_gr[gr_pop].timed_rules[{seq_t[2], seq_t[3]}][j].is_optional){//timed and optional
                    
                    rule r = all_gr[gr_pop].timed_rules[{seq_t[2], seq_t[3]}][j];
                    rule_found = check_optional(r, seq_t);
                    
                    if (rule_found) break;
                    
                }
                else {//timed, but not optional
                    
                    //cout << "reached 11" << endl;
                    if (curr_cont == all_gr[gr_pop].timed_rules[{seq_t[2], seq_t[3]}][j].left_str){
                        
                        //cout << "reached 12" << endl;
                        rule_found = true;
                        
                        //expand to support optional too
                        r = all_gr[gr_pop].timed_rules[{seq_t[2], seq_t[3]}][j];
                        
                        the_rule = r;
                        //debug
                        cout << "the_rule: ";
                        for (int j=0; j<the_rule.left_str.size(); j++) cout << the_rule.left_str[j] << ", ";
                        cout << endl;
                        
                        rewrite(r, seq_t);
                        cout << "case 2" << endl;//timed, but not optional
                        
                        break;
                    }
                }
                
            }
            
            break;
        }
    }
    
    //else find general_rule
    if(!rule_found) {//replace with while()
        
        //search general_rules (context again)
        for (int i=0; i<all_gr[gr_pop].general_rules.size(); i++){
            
            //check if context exists before or after current chord (elem)
            int c_off = - all_gr[gr_pop].general_rules[i].left_str.size();//context offset (back n forth)
            cont_size = all_gr[gr_pop].general_rules[i].left_str.size();
            //leftmost_t = all_gr[gr_pop].general_rules[i].leftmost_time;//is {0, 0}
            opt = all_gr[gr_pop].general_rules[i].is_optional; //need to access specific general_rule
            
            for (int z = c_off; z < 0; z++){
                
                all_gr[gr_pop].general_rules[i].leftmost_time[1] = seq_t[3] + (z+1) / all_gr[gr_pop].harm_rh;
                
                vector<string> curr_cont = get_context(all_gr[gr_pop].general_rules[i].leftmost_time, cont_size);
                
                //check context if(general_rule[i].is_optional)
                if (opt){
                    
                    r = all_gr[gr_pop].general_rules[i];
                    rule_found = check_optional(r, seq_t);
                    if (rule_found) break;
                    
                }
                
                //check context if (!general_rule[i].is_optional)
                else if(get_context(all_gr[gr_pop].general_rules[i].leftmost_time, cont_size) == all_gr[gr_pop].general_rules[i].left_str){
                    
                    rule_found = true;
                    
                    r = all_gr[gr_pop].general_rules[i];
                    
                    the_rule = r;
                    //debug
                    cout << "the_rule: ";
                    for (int j=0; j<the_rule.left_str.size(); j++) cout << the_rule.left_str[j] << ", ";
                    cout << endl;
                    
                    //logging
                    //logger.File << "the_rule: ";
                    logger.rt_log.append("the_rule: ");
                    for (int j=0; j<the_rule.left_str.size(); j++) logger.rt_log.append(the_rule.left_str[j] + ", \n");
                    
                    rewrite(r, seq_t);
                    cout << "case 5" << endl;//general and not optional
                    
                    break;
                }
            }
            //j before find rule:
            if (rule_found) break;
        }
    }
    
    if (till_function){// && !updating_morph){//!comb_setup & transitioning){
        //stop if function
        if (!is_function(seq_t)) find_rule(seq_t);
    }
    else if (building_next_form){
        
        //find_rule ONLY once for S_rule - otherwise in beginning of find_rule..
        //find_rule(seq_t);
        //building_next_form = false;
    }
    else {
        //stop if terminal
        if (!is_terminal(seq_t)) find_rule(seq_t);
        //else return_terminal(curr_cycle[seq_t[3]].name);
        else translate_to_midi();//translate / spit out (return_terminal())
    }
}


bool G_parser::check_optional(rule& r, vector<int>& seq_t){
    
    bool r_found = false;
    
    //cout << "reached 5" << endl;
    size_t _index;
    int matches = 0;
    
    vector<int> l_t = r.leftmost_time;
    int cont_size = r.left_str.size();
    vector<string> g_c = get_context(l_t, cont_size);
    int k=0;
    //compare context with left_str
    for (int i=0; i < r.left_str.size(); i++){
        
        //cout << "reached 6" << endl;
        vector<int> v_int = r.opt_positions;
        vector<int>::iterator it_int = find(v_int.begin(), v_int.end(), i);
        _index = it_int - v_int.begin();
        
        if (it_int!=v_int.end()){
            
            //cout << "reached 7" << endl;
            vector<string> v_str = r.options[_index];
            vector<string>::iterator it_str = find(v_str.begin(), v_str.end(), g_c[i]);
            
            if (it_str!=v_str.end()) {
                
                //cout << "reached 8" << endl;
                matches++;
            }
        }
        else {
            
            //cout << "reached 9" << endl;
            if (g_c[i]==r.left_str[i]) matches++;
        }
        
    }
    
    if (matches==g_c.size()){
        
        //cout << "reached 10" << endl;
        r_found = true;
        
        the_rule = r;
        //debug
        cout << "the_rule: ";
        for (int j=0; j<the_rule.left_str.size(); j++) cout << the_rule.left_str[j] << ", ";
        cout << endl;
        
        rewrite(r, seq_t);
        cout << "case 1" << endl;//timed and optional
    }
    
    return r_found;
}


G_parser::rule G_parser::check_context(vector<int>& seq_t){
    //if optional check for both
    //else check as was
}


vector<string> G_parser::get_context(vector<int>& leftmost_t, int& size){
    
    //adapt to form_length, time_signature
    leftmost_t[1] = (leftmost_t[1] + all_gr[gr_pop].form_length) % all_gr[gr_pop].form_length;//it may be negative initially
    //cout << "leftmost_t[1]: " << leftmost_t[1] << endl;
    
    //leftmost[1] += leftmost[0] / t_sign;
    //leftmost[0] = leftmost[0] % t_sign;
    vector<elem_ID> context;
    vector<string> context_str;
    
    for (int i=0; i<size; i++){
        
        int mod = (leftmost_t[1] + i) % all_gr[gr_pop].form_length;//to cycle up is form_length is exceeded
        if (!transitioning) context.push_back(curr_cycle[mod]);
        else context.push_back(aux_cycle[mod]);
        context_str.push_back(context[i].name);//should be string because left_str is not elem_ID (yet..)
    }
    
    return context_str;
}


void G_parser::rewrite(rule& r, vector<int>& seq_t){
    
    cout << "rewriting_Rule:";
    //for (int i=0; i<r.left_str.size(); i++) cout << r.left_str[i] << ", ";
    cout << endl;
    
    //keep elem if FUNCTION - must happen here to not miss the pre-produced functions (else, every 4 bars..)
    if (!transitioning) keep_func_hist(seq_t);
    
    vector<elem_ID> production;
    vector<int> choices = rewrite_choices(r);

    for (int j=0; j < choices.size(); j++){
        //produce from rule
        
        //if (cc==gp) break;
        
        for (int i=0; i<r.right_side[choices[j]].right_str.size(); i++){
            
            //if (cc==gp) break;
            
            vector<int> t_aux(2);//beat, bar
            t_aux[0] = r.leftmost_time[0] % all_gr[gr_pop].t_sign;//time signature
            t_aux[1] = ((r.leftmost_time[1]+i / all_gr[gr_pop].harm_rh) + all_gr[gr_pop].form_length) % all_gr[gr_pop].form_length;
            
            production.push_back(elem_ID());
            
            if (r.timed_production){
                
                production[i].name = exclude_times(r.right_side[choices[j]].right_str[i]);
                production[i].time = {0, r.leftmost_time[1] + r.prod_times[i]};//{beat, bar} config. OK?
            }
            else {
                
                production[i].name = r.right_side[choices[j]].right_str[i];
                //else (generals.leftmost_time == {0 ,0})
                
                production[i].time = t_aux;
            }
        }
        
        if (!transitioning) update_cycle(production, r, seq_t);
        else {
            
            if (updating_morph) morph_update(production, r, seq_t);
            else if (!comb_setup) trans_update(production, r, seq_t);
             
            //isolate t (setup_t) for itarations for all right_side without changing aux_t in order to then move to next right_side
            //while den einai functions olo to r_side mhn pas sto epomeno r_side
            
            if (r.left_str[0]!="S" || rewriting_next){//without this filtering, it rewrites the whole cycle once (because right_str.size() of the S rule is 4, i.e. all the sects..) first and then stops at curr_bar==8 the 2nd time..
                //if rewriting_next do only once the whole S rule, i.e. the whole form and that's it..
                
                vector<int> setup_t = seq_t;
                for (int i=0; i < r.right_side[choices[j]].right_str.size(); i++){
                
                    //trans_update(production, r, setup_t);
                    //production.clear();//otherwise both right sides are added linearly (at least as prouction size)

                    //while (!is_function(setup_t)){
                    
                    setup_t[3] = production[i].time[1];
                    
                    //if (cc==gp) break;
                    
                    if(till_function){
                    
                        if(!is_function(setup_t)) find_rule(setup_t);
                    }
                    
                    //trans_update(production, r, seq_t);
                    
                    //}
                    //setup_t[3]++;
                     //*/
                }
            }
            production.clear();//otherwise both right sides are added linearly (at least as prouction size)
            //*/
        }
        
        //iterate rewrite?
        //itarate find_rule?
    }
    
    //production.clear();
    /*
     for (int i=0; i<production.size(); i++){
     cout << "name" << i << ": " << production[i].name << ", time: " << production[i].time[0] << " & " << production[i].time[1] << endl;
     }
     */
}

//choosing re-write based on rule probability
vector<int> G_parser::rewrite_choices(rule& r){
    
    vector<int> _choices;
    
    if (!transitioning){//choose one choice
    
        vector<float> p_vec;
        for (int i=0; i<r.right_side.size(); i++) p_vec.push_back(r.right_side[i].prob);
        
        float total_p = 0;
        srand(time(NULL));
        
        //cout << "probs: ";
        //for (int i=0; i<p_vec.size(); i++) cout << p_vec[i] << ", ";
        //cout << endl;
        
        float random = (rand()%100) / 100.0;
        //cout << "random: " << random << endl;
        
        for (int i=0; i<p_vec.size(); i++){
            
            if (random >= total_p && random <= total_p+p_vec[i]) {
                //cout << "p_vec " << i << ": " << p_vec[i] << endl;
                _choices.push_back(i);
                //cout << "re_choice: " << i << " // ";
                break;
            }
            
            total_p += p_vec[i];
        }
    }
    else {//keep population of all choises
    
        for (int i=0; i < r.right_side.size(); i++) _choices.push_back(i);
    }
    
    return _choices;
}


void G_parser::update_cycle(vector<elem_ID>& production, rule& r, vector<int>& seq_t){
    
    for(int i=0; i<production.size(); i++){
        
        //placing production in cycle
        //if optional element, don't update that (in curr_cycle)!!
        vector<int>::iterator it = find(r.opt_positions.begin(), r.opt_positions.end(), i);
        if (it==r.opt_positions.end()) curr_cycle[production[i].time[1]] = production[i];
        //i.e. if i does not exist in the r.opt_positions vector
    }
    
    //at end of cycle (after last rewrite) place "S" at start.
    //start_cycle(seq_t); //HERE or in blues.update()
    
    //update_ending(seq_t);//will overwrite start_cycle()
    
    //debug
    cout << "new cycle (update_cycle()): ";
    for (int i=0; i<all_gr[gr_pop].form_length; i++) cout << curr_cycle[i].name << " ";
    cout << endl << "=======" << endl;
    
    //logging
    logger.rt_log.append("new cycle (update_cycle()): ");
    for (int i=0; i<all_gr[gr_pop].form_length; i++) logger.rt_log.append(curr_cycle[i].name + " ");
    logger.rt_log.append("\n=======\n");
    
}


void G_parser::morph_update(vector<elem_ID> production, rule& r, vector<int>& seq_t){

    cout << endl << "Morph_update" << endl;
    //logging
    //logger.transition_data_log.append("Morph update: [symbol : time]\n");
    
    for(int i=0; i<production.size(); i++){
        
        morph_cycle[i] = production[i];
        aux_cycle[production[i].time[1]] = production[i];
        curr_cycle[production[i].time[1]] = production[i];
        //aux_cycle[seq_t[3]] = production[i];
        
        //logging
        logger.transition_data_log.append("[" + production[i].name + " : " + ofToString(production[i].time[1]) + "], ");
    }
    //logging
    //logger.transition_data_log.append("\n");
}


void G_parser::trans_update(vector<elem_ID>& production, rule& r, vector<int>& seq_t){
    //placement in aux_cycle & storage of func_chunks
    
    //depending on transition state, update what must be updated..
    
    if (!comb_setup){//for setting up, find best rule etc..
        
        //int function_count = 0;//if not =0 it got 24577 to 24580..
        bool are_functions = 0;
        
        for(int i=0; i<production.size(); i++){
            
            //placing production in cycle
            //if optional element, don't update that (in curr_cycle)!!
            vector<int>::iterator it = find(r.opt_positions.begin(), r.opt_positions.end(), i);
            if (it==r.opt_positions.end()){
            
                /*
                if (i >= aux_cycle.size()){
                    //in case moving from short to long cycle - what if long to short?
                    
                    cout << endl << "added to cycle at i: " << i << endl;
                    aux_cycle.push_back(elem_ID());
                }
                 */
                
                aux_cycle[production[i].time[1]] = production[i];
            }
            //i.e. if i does not exist in the r.opt_positions vector
            
        }
        
        //if production[i].name einai Function, tote ola einai functions..
        //check if all production.names are functions - if so, treat as ready chunk
        vector<int> aux_t = {0, 0, production[0].time[0], production[0].time[1]};//0 instead of i
        if (is_function(aux_t)){
            
            //function_count++;
            are_functions = 1;
            cout << endl << "are_functions: " << are_functions << endl;//function_count << endl;
        }
        
        if (are_functions){//function_count == production.size()){//if all production names are functions
            //NOT == production size because it may be 2 for 8 bars, e.g. decB1, decB2..
            //constitute funct chunk and add cleverly for combination in total possibilities of cycle up to goal..
            //manage the addition of pre-existing functions, e.g. D T on paper
            
            for (int i=0; i < production.size(); i++){
            
                func_chunk.push_back(production[i]);//chunk of functions..
            }
            
            func_chunks.push_back(func_chunk);
            
            func_chunk.clear();
            //function_count = 0;
        }
    }
    //at end of cycle (after last rewrite) place "S" at start.
    //start_cycle(seq_t); //HERE or in blues.update()
    
    //update_ending(seq_t);//will overwrite start_cycle()
    
    //debug
    cout << "aux cycle (trans_update()): ";
    for (int i=0; i<all_gr[gr_pop].form_length; i++) cout << aux_cycle[i].name << " ";
    cout << endl << "=======" << endl;
}


void G_parser::keep_func_hist(vector<int>& seq_t){
    
    //check if non-T is a function - if yes keep
    string n = curr_cycle[seq_t[3]].name;
    
    vector<string> func = all_gr[gr_pop].functions;
    vector<string>::iterator it_func = find(func.begin(), func.end(), n);
    
    if (it_func!=func.end()){
        
        //keep the elem - not sure if elem_ID.time is passed here - it may be managed only by the vector place in G_parser..
        function_cycle.push_back(curr_cycle[seq_t[3]]);
    }
    
    /*
    //test function cycle
    cout << endl << endl << endl << "FUNCTION_CYCLE: ";
    for (int i = 0; i < function_cycle.size(); i++){
    
        cout << function_cycle[i].name << " ";
    }
    cout << endl << endl << endl;
     */
}


void G_parser::start_cycle(vector<int>& seq_t){
    
    //if (seq_t[3] == all_gr[gr_pop].form_length / all_gr[gr_pop].harm_rh - 1 && !ending && is_terminal(seq_t)){
    if (seq_t[0] == 0 && seq_t[1] == 0 && seq_t[2] == 0 && seq_t[3] == 0 && !ending){// && is_terminal(seq_t)){
        
        curr_cycle[0].name = "S";
        //for (int i=0; i<dec_bars.size(); i++) curr_cycle[dec_bars[i]].name = "Sect";
        cout << "form length on start_cycle: " << all_gr[gr_pop].form_length << endl;
    }
    /*
    if (!gr_changed){
        
        //curr_cycle[seq_t[3]+1].name = "S";
        //curr_cycle[0].name = "S";
        
        //reset clock back to top (τιμερ 0 η -1?)
        //3. sigoura otan seq_t[2]==0?
        
        reset_t = 1;
        gr_changed = 1;
        initiate_cycle();
    }
     */
}


//cycle initiation must happen here (not in grammar builder) so that th actual gr_pop is used and not the last read..
//maybe this should be in another class (e.g. player manager class) but stays here for now..
void G_parser::initiate_cycle(){
    
    curr_cycle.clear();
    aux_cycle.clear();
    morph_cycle.clear();
    
    cout << "form length on initiation: " << all_gr[gr_pop].form_length << endl;
    
    for(int i=0; i<all_gr[gr_pop].harm_rh * all_gr[gr_pop].form_length; i++) {
        
        curr_cycle.push_back(elem_ID());
        curr_cycle[i].time = {0, i};// {beat, harm_rh next..}
        //curr_cycle[i].name = "i";
        //curr_cycle[0].name = "S";
    }
   
    curr_cycle[0].name = "S";
}


void G_parser::update_ending(vector<int>& seq_t){
    
    check_cadence(seq_t);
    
    cout << "at bar: " << seq_t[3] << " cadenced: " << cadenced << " t/non-t: " << curr_cycle[seq_t[3]].name << endl;
    
    if (!goal_reached){
        
        if (ending){
            
            if(!cad_updated) update_cad(seq_t);//with EVERY rewrite()??
            
            //upade_finals() one or zero bars before it comes (i.e. 2nd bar of cad) - if not recovering of course
            //int c_p = (cad_pos+1) % all_gr[gr_pop].form_length;
            
            update_finals(seq_t);
            
        }
        else if (cad_updated) recover(seq_t);//cad_updated??
    }
    else {
        //goal_reached is irreversible
        
        //int end_bar = (cad_pos + 2) % all_gr[gr_pop].form_length;//it's a mistake to get this from cad_pos. cad_pos becomes -1 if cadenced..
        string c_c = curr_cycle[seq_t[3]].name;
        
        if (!cad_updated) update_cad(seq_t);
        update_finals(seq_t);
        //fin_updated;
        
        //watch out: g_r might come too early
        //==0 doesn't happen
        //if (cad_pos==10 && cadenced) stop_sequencer();
        
        if ((c_c == "i" || c_c == "i6" || c_c == "i7") && cadenced) stop_sequencer();//(seq_t[3]==0 || seq_t[3]==4 || seq_t[3]==8) && seq_t[3]>=cad_pos
        
        /*
        if (end_bar!=0){
            if (end_bar <= seq_t[3] && (c_c == "i" || c_c == "i6" || c_c == "i7")) stop_sequencer();
        }
        else if ((c_c == "i" || c_c == "i6" || c_c == "i7") && seq_t[3]) stop_sequencer();
        */
    }
}

//print in console.txt
//TEST
//reach goal from cad
//reach goal from cad & fin
//reach goal from recovering
//reach goal !ending && !recovering //NOT WORKING for now..

void G_parser::update_cad(vector<int>& seq_t){
    
    cout << "update_cad" << endl;
    
    cad_updated = true;
    
    //place next "cad" point (only..)
    if (seq_t[3] >= cad_bars.back()) {
        
        curr_cycle[cad_bars[0]].name = "cad";
        cad_pos = cad_bars[0];
    }
    
    else {
        for (int i=0; i<cad_bars.size(); i++){
            
            if (seq_t[3] < cad_bars[i]) {
                
                curr_cycle[cad_bars[i]].name = "cad";
                cad_pos = cad_bars[i];
                break;
            }
        }
    }
}


void G_parser::recover(vector<int>& seq_t){
    
    //cout << "recover" << endl;
    
    fin_updated = false;
    cad_updated = false;
    cadenced = false;
    
    update_optimal(seq_t);
    reconcile(seq_t);//place "rec" in between!
}


void G_parser::update_optimal(vector<int>& seq_t){
    
    //cout << "update_optimal" << endl;
    
    //if end of form
    int optimal_pos = (seq_t[3] + 2) % all_gr[gr_pop].form_length;
    
    //TWO bars later?//depricated!!!!!
    for (int i=optimal_pos; i<curr_cycle.size(); i++) curr_cycle[i].name = basic_vector["optimal_form"][i];
}


void G_parser::reconcile(vector<int>& seq_t){
    
    //cout << "reconcile" << endl;
    
    int rec_pos = (seq_t[3] + 1) % all_gr[gr_pop].form_length;
    curr_cycle[rec_pos].name = "rec";
}


void G_parser::update_finals(vector<int>& seq_t){
    
    //cout << "update_finals" << endl;
    
    //fin meta to cad mexri prin apo kei pou eimai
    
    /*
    if (ending || goal_reached){
    
        //de vlepei kanona gia to cad..!!
        //CAD_POS?????????
        for(int i=cad_pos+2; i<all_gr[gr_pop].form_length; i++) curr_cycle[i].name = "fin";
        for(int i=0; i<seq_t[3]-1; i++) curr_cycle[i].name = "fin";
    }
     //*/
    
    ///*===============
    if (seq_t[3]==all_gr[gr_pop].form_length-1 && cadenced) {//if (!goal_reached && seq_t[3]==11 && cadenced)
        //cout << "finals_1" << endl;
        for (int i=cad_pos+1; i<curr_cycle.size() - 1; i++) curr_cycle[i].name = "fin";
        //curr_cycle[11].name = "V";
    }
    else if (!fin_updated){
        //cout << "finals_2" << endl;
        int fin_pos = (cad_pos + 2) % all_gr[gr_pop].form_length;
        
        //!=0 otherwise "fin" * 12 -> there is no general && contextless fin rule, so 12*fin -> infinite..
        if (fin_pos!=0) for (int i=fin_pos; i<curr_cycle.size(); i++) curr_cycle[i].name = "fin";
    }
    //=================*/
    
    fin_updated = true;
}


void G_parser::check_cadence(vector<int>& seq_t){
    
    if (seq_t[3]==cad_pos) {
        
        //cout << "cadenced ture???" << endl;
        cadenced = true;
        cad_pos = -1;
    }
}


void G_parser::stop_sequencer(){
    
    cout << "stop_sequencer" << endl;
    stop_seq = true;
}


bool G_parser::is_terminal(vector<int>& seq_t){
    
    //debug
    cout << "is_terminal?: " << curr_cycle[seq_t[3]].name << endl << endl;
    //logging
    logger.rt_log.append("is_terminal?: " + curr_cycle[seq_t[3]].name + "\n\n");
    
    bool is_t = false;
    vector<string>::iterator it = find(all_gr[gr_pop].terminals.begin(), all_gr[gr_pop].terminals.end(), curr_cycle[seq_t[3]].name);
    
    if (it!=all_gr[gr_pop].terminals.end()) is_t = true;
    return is_t;
}


bool G_parser::is_function(vector<int>& seq_t){
    
    bool is_f = false;
    vector<string>::iterator it = find(all_gr[gr_pop].functions.begin(), all_gr[gr_pop].functions.end(), aux_cycle[seq_t[3]].name);
    
    if (it!=all_gr[gr_pop].functions.end()) is_f = true;

    //cout << endl << "is_function: " << aux_cycle[seq_t[3]].name << ", is_f: " << is_f << endl;

    return is_f;
}


string G_parser::return_terminal(vector<int>& seq_t){//OVERLOAD FIND_RULE(..)
    
    return curr_cycle[seq_t[3]].name;
}


void G_parser::translate_to_midi(){
    
    trigger();
}


void G_parser::trigger(){}


void G_parser::get_rootpitch(int& bar){
    //for bar
}


void G_parser::translate(string& chord){
    //map terminals to what blues/seq wants
}


string G_parser::get_nc(){//get comment-filtered item

    string s;
    grammars >> s;
    nc = comment_filter(s);
    return nc;
}


string G_parser::comment_filter(string& c){
    
    string nc = c;
    vector<int> v = {0, 0};
    
    for (int i=0; i<c.length(); i++){
        if (c[i]=='/' && c[i+1]=='/'){
            v = {1, i};//{comment_found, point_in_string}
            i = c.length();
        } //else v = {0, 0};
    }
    
    if(v[0]){
        
        if (v[1]==0){
            grammars.ignore(numeric_limits<streamsize>::max(), '\n');
            grammars >> c;
            if (!grammars.eof()){
                nc = comment_filter(c);
            }
        }
        
        else {
            nc = c.substr(0, v[1]);//for (int x=0; x<i; i++) nc.push_back(c[x]);
            //com = c.substr(comment_check(c)[1], c.length());
            //http://www.cplusplus.com/reference/string/string/substr/
            grammars.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
    return nc;
}


void G_parser::print_rules(){
    
    for (int i=0; i<all_rules.size(); i++){
        cout << "rule " << i << ": ";
        
        for (int j=0; j<all_rules[i].left_str.size(); j++){
            cout << all_rules[i].left_str[j] << " ";
        }
        
        for (int j=0; j<all_rules[i].right_side.size(); j++){
            cout << "-> " << all_rules[i].right_side[j].prob << " ";
            
            for (int k=0; k<all_rules[i].right_side[j].right_str.size(); k++){
                cout << all_rules[i].right_side[j].right_str[k] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    
}
