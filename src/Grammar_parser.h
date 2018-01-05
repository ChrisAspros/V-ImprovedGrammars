//
//  Grammar_parser.h
//  
//
//  Created by Christodoulos Aspromallis on 25/04/2016.
//
//

#ifndef ____Grammar_parser__
#define ____Grammar_parser__

#include <stdio.h>
#include "ofxMidiOut.h"
//#include "ofxMidi.h" //??
#include "Logger.hpp"



class G_parser{

public:
    
    G_parser();// : grammar(grammar_path, std::ifstream::in){}
    //~G_parser();
    
    string grammars_path;// = "/Users/christodoulosaspromallis/Documents/UCL/PhD_Y_3/OF/of_v0.9.8_osx_release/apps/myApps/ICMC_test/from_SMC/bin/data/blues_grammar.txt";
    ifstream grammars;
    
    string nc, c;
    
    string get_nc();
    string comment_filter(string& c);
    
    void get_new_grammar(string& nc);
    void get_grammar_elements();
    void get_time_signature(string& nc);
    void get_form_length(string& nc);
    void get_harm_rh(string& nc);
    void get_optimal_form_functions(string& nc);
    void get_end_times(string& nc);
    void get_decs(string& nc);
    void get_functions(string& nc);
    void get_terminals(string& nc);
    //void get_basic_vectors(string& nc);
    
    int gr_pop;
    int t_sign;
    int form_length;
    int harm_rh;
    vector<int> dec_bars;
    vector<int> cad_bars;
    map<string, vector<string>> basic_vector;
    
    void store_rules(string& nc);
    vector<vector<int>> existing_times;
    int rule_pop = 0;
    //int leftmost_time;
    
    struct right_s{
        vector<string> right_str;
        float prob;
    };
    
    struct rule{
        vector<string> left_str;
        vector<right_s> right_side;
        vector<int> leftmost_time;//{beat, bar}
        
        bool timed;//re: left side of rule "dec_1"
        bool timed_production;//re: right side of rule "SectA(9)"
        vector<int> prod_times;//production times of right elements (if timed)
        
        //optional elements
        bool is_optional;
        vector<int> opt_positions; //positions of optional elements in left_str
        vector<vector<string>> options;
    };
    
    struct gr{
        int gr_pop;
        int t_sign;
        int form_length;
        int harm_rh;
        vector<string> optimal_form_functions;
        vector<int> end_times;
        vector<string> decs;
        vector<string> functions;
        vector<string> terminals;
        
        //vector<rule> all_rules;//all_rules only necessary for store_rules process.. not for actually keeping / finding them..
        map<vector<int>, vector<rule>> timed_rules;
        //map<vector<int>, rule> timed_rules;
        vector<rule> general_rules;
    };
    
    void log_curr_gr_elements(gr g1);
    int find_rule_pop(string& _name);//e.g. how many dec rules ther are
    
    vector<gr> all_gr;
    void empty_gr_aux_elements();
    
    vector<rule> all_rules;//for now it stores all rules of the last-read grammar..//optimise by sending straight to timed or general rules..
    //vector<rule> timed_rules, general_rules;//subtotal of all_rules
    //map<vector<int>, vector<rule>> timed_rules;//specified rule: labels timed rules (t vector)
    //vector<rule> general_rules;//store non-timed rules
    
    void store_opt_data(string& nc);
    void store_options(string& nc);
    void store_prod_times(string& nc, rule& r);
    
    string exclude_times(string& s_t_r);
    
    void print_rules();
    
    struct elem_ID{
        string name;
        vector<int> time;//beat, bar (harm_rh next)
    };
    
    void find_rule(vector<int>& seq_t);
    void keep_func_hist(vector<int>& seq_t);//keeping function history for lenght comparison
    vector<elem_ID> function_cycle;
    bool check_optional(rule& r, vector<int>& seq_t);
    bool is_terminal(vector<int>& seq_t);
    bool is_function(vector<int>& seq_t);
    string return_terminal(vector<int>& seq_t); //better to overload find_rule??
    void translate_to_midi();
    void trigger();
    
    void get_rootpitch(int& bar);
    
    rule check_context(vector<int>& seq_t);
    void rewrite(rule& r, vector<int>& seq_t);
    vector<int> rewrite_choices(rule& r);
    vector<string> get_context(vector<int>& leftmost_t, int& size);
    
    vector<elem_ID> curr_cycle;
    vector<elem_ID> aux_cycle;
    vector<elem_ID> morph_cycle;
    int gp;
    int cc;
    //vector<int> setup_t;
    bool right_s_finished;
    vector<elem_ID> func_chunk;
    vector<vector<elem_ID>> func_chunks;
    void update_cycle(vector<elem_ID>& production, rule& r, vector<int>& seq_t);//size() = [form_length * harmonic_rhythm]
    void trans_update(vector<elem_ID>& production, rule& r, vector<int>& seq_t);
    void morph_update(vector<elem_ID> production,rule& r, vector<int>& seq_t);
    void start_cycle(vector<int>& seq_t);
    void update_ending(vector<int>& seq_t);
    void update_cad(vector<int>& seq_t);
    void recover(vector<int>& seq_t);
    void update_optimal(vector<int>& seq_t);
    void reconcile(vector<int>& seq_t);
    void update_finals(vector<int>& seq_t);
    void stop_sequencer();
    void check_cadence(vector<int>& seq_t);
    
    void initiate_cycle();
    
    void translate(string& chord);//translate to MIDI//what blues / sequencer wanna see
    
    rule the_rule;
    bool transitioning;
    bool building_next_form;
    bool comb_setup;
    bool till_function;
    bool rewriting_curr = 0;
    bool rewriting_next = 0;
    bool updating_morph = 0;
    //bool setting_up;
    bool soon = false;// <= four bars' time (+ smth maybe..)
    bool approaching = true;
    bool ending = (soon && approaching);//continuously updated
    bool cadenced = false;
    bool goal_reached = false;
    bool cad_updated = false;
    int cad_pos = -10;
    bool recovering = !ending;//before or after cad?
    bool fin_updated = false;
    bool stop_seq = false;
    
    bool gr_changed;
    bool reset_t;
  
    Logger logger;
};

#endif /* defined(____Grammar_parser__) */
