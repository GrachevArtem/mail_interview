


#include<fstream>
#include<iostream>

#include <string>
#include <vector>
#include <map>
#include <deque>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

class MarkovChain { 

    map<deque<string>, map<string, double> > transitions_matrix;

public:
    
    MarkovChain(map<deque<string>, map<string, double> > transitions_matrix_);
   
    MarkovChain();
    
    void FinalizationChain();
    void InsertInChain(const deque<string>& current_condition, string next_word);
    void AddTextToChain(const vector<string>& new_text, int power_of_chain);
    void LearnChain(const vector< vector <string> >& all_texts, int power_of_chain);
    
    friend istream& operator >> (istream& is, MarkovChain& markov_chain);
    friend ostream& operator << (ostream& os, MarkovChain& markov_chain);
    
    string GenerateNewWord(const deque<string>& current_condition);
    
    
    
};

