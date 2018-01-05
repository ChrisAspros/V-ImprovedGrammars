//
//  Rule_comparer.hpp
//  Egypt_pub_2grammars
//
//  Created by Christodoulos Aspromallis on 22/08/2017.
//
//

#ifndef Rule_comparer_hpp
#define Rule_comparer_hpp

#include <stdio.h>

//class G_parser;

#include "Grammar_parser.h"

class Rule_comparer{
    
public:
    
    G_parser parser;
    
    //bool transitioning;
    bool trans_incomplete;
    bool un_dist_found;
    int un_dist, undist_bar;
    
    int curr_gr;
    //int prev_gr;
    int next_gr;
    
    int n_s_pos;//next sect position
    
    void combine_rules(vector<int>& seq_t);//the main/central function of the class
    bool rules_combined;
    
    vector<int> get_sect_lengths(G_parser::rule _S_rule, int _gr_pop);
    vector<int> sect_lengths_curr, sect_lengths_next;
    
    struct aug_sect_rule{//augmented sect rules
    
        string sect_name;
        G_parser::rule s_rule;
        int r_len;
        int score;
        string next_sect;//also indirect identifier between same "Sects" in different places..
        int position;//its position in the form..
    };
    
    vector<aug_sect_rule> aug_sect_rules;
    
    //vector<G_parser::elem_ID>
    void find_best_rule(vector<int>& seq_t);
    G_parser::rule get_S_rule(int _gr_pop);
    G_parser::rule S_rule_curr, S_rule_next;
    vector<G_parser::elem_ID> best_r;//needs to be global?
    
    //vector<G_parser::elem_ID> aux_cycle;
    
    //bool comb_set_up = 0;
    void setup_combination(vector<int>& seq_t);
    int get_distance_to_goal(vector<int>& seq_t);
    void rewrite_funcs();
    void rewrite_curr_gr_t_g();//till funcs
    void rewrite_next_all();//till funcs
    
    vector<vector<G_parser::elem_ID>> curr_func_chunks;
    vector<vector<G_parser::elem_ID>> next_func_chunks;
    
    void compare_t_g();
    void compare_include_history(int _form_pc);
    vector<vector<int>> hist_scores;
    vector<vector<int>> sorted_hist_scores;
    vector<vector<int>> best_hist_scores;
    vector<vector<int>> top_curr_func_line_scores;
    vector<vector<int>> top_next_func_line_scores;
    vector<vector<int>> earliest_next_form_scores;
    vector<int> final_best_score;
    
    void compare_include_future(int _form_pc);
    
    void mix_in_rt();
    void implement_recovery();
    
    vector<vector<G_parser::elem_ID>> construct_lines(vector<vector<G_parser::elem_ID>> func_chunks, int gr_num, int length, int init_curr_bar);
    vector<vector<G_parser::elem_ID>> curr_func_lines;
    //vector<G_parser::elem_ID> curr_func_lines;
    vector<vector<G_parser::elem_ID>> next_func_lines;
    void filter_func_lines_pop(vector<vector<G_parser::elem_ID>>& func_lines, int max_pop);
    vector<vector<vector<int>>> un_dist_scores;
    vector<vector<int>> formed_local_scores;
    vector<vector<vector<int>>> get_un_dist_scores();
    vector<vector<int>> bubble_sort_scores(vector<vector<int>> _unsorted_scores);
    vector<vector<int>> merge_sort_scores(vector<vector<int>> _unsorted_scores);
    vector<vector<int>> insertion_sort_scores(vector<vector<int>> _unsorted_scores);
    vector<vector<int>> TimSort_scores(vector<vector<int>> _unsorted_scores);
    vector<vector<int>> TimMerge_scores(vector<vector<int>> _unsorted_scores);//NON-ITERATIVE
    vector<vector<int>> sorted_local_scores;
    vector<vector<int>> get_best_hist_scores(vector<vector<int>> _scores);
    vector<vector<int>> get_top_curr_func_line_scores(vector<vector<int>> _scores);
    vector<vector<int>> get_sort_n_best_scores(vector<vector<int>> _scores);
    vector<vector<int>> get_earliest_next_form_scores(vector<vector<int>> _scores);
    vector<vector<int>> best_local_scores;
    vector<vector<int>> form_scores();
    vector<vector<int>> get_best_local_scores_percentage();
    int score_pc = 20;//percentage of best scores
    int b_s_pop;
    vector<vector<int>> get_best_local_scores_singleBest();
    
    vector<G_parser::elem_ID> intermediate_functions;
    vector<G_parser::elem_ID> history;
    
    void weight_choose_morph(vector<G_parser::elem_ID> _curr_best, vector<G_parser::elem_ID> _next_best);
    
    int form_pc = 2;//for now i.e. number of previous sects to include as history
    //form percentage to check for history & future..
    
    int dist;
    int thread;
    void get_goal_point();
    int g_p;
    int length_to_goal;//number of bars until goal
    
    void update_combination(vector<int>& seq_t);
    void build_next_form();
    void build_next_sects();
    
    void initiate_aux_cycle();
    
    void clearance();
    
    void project_curr_gr();//projects current (ending) grammar up to goal point
    void project_next_gr();//projects next (upcoming) grammar up to goal point
    
};

#endif /* Rule_comparer_hpp */
