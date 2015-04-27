

#include <cstdio>
#include <cstdlib>
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <vector>
using std::vector;
#include <map>
using std::map;
using std::multimap;
#include <deque>
using std::deque;
using std::pair;

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
 
#include "curl/curl.h"
#pragma comment(lib,"curllib.lib")


class MarkovChain { 

    map<deque<string>, map<string, double>> transitions_matrix;

public:

    MarkovChain(map<deque<string>, map<string, double>> transitions_matrix_) {
        transitions_matrix = transitions_matrix_;
    }

    MarkovChain() {
        map<deque<string>, map<string, double>> transitions_matrix;
    }



    void FinalizationChain() {

        for (map<deque<string>, map<string, double>>::iterator it_chain = this->transitions_matrix.begin(); 
            it_chain != this->transitions_matrix.end(); ++it_chain) {

            double sum_next_states = 0;

            for (map<string, double>:: iterator it_state = it_chain->second.begin(); it_state != it_chain->second.end(); ++it_state) {
                sum_next_states += it_state->second;
            }

            double upper_bound_next_state = 0;

            for (map<string, double>:: iterator it_state = it_chain->second.begin(); it_state != it_chain->second.end(); ++it_state) {
                upper_bound_next_state += it_state->second/sum_next_states;
                it_state->second = upper_bound_next_state;
            }

        }

    }


    void InsertInChain(const deque<string>& current_condition, string next_word) { 

        map<deque<string>, map<string, double>>::iterator it_current_condition;
        it_current_condition = this->transitions_matrix.find(current_condition);

        if ( it_current_condition == this->transitions_matrix.end() )   { 

            map<string, double> new_state;
            new_state.insert(pair<string, double>(next_word, 1));
            this->transitions_matrix.insert(pair< deque<string>, map<string, double> >(current_condition, new_state));

        } else if ( it_current_condition->second.find(next_word) == it_current_condition->second.end() )   { 

            it_current_condition->second.insert(pair<string, double>(next_word, 1));

        } else {
            ++it_current_condition->second[next_word];
        }

    }

    
    void AddTextToChain(const vector<string>& new_text, int power_of_chain) { 

        deque<string> current_condition;

        for(int i = 0; i < static_cast<int>(new_text.size()); ++i) {

            if (i == 0) { 
                continue;
            }

            if (i <  power_of_chain) { 
                current_condition.push_front(new_text[i-1]);
                continue;
            }
        
            if (i == power_of_chain) { 
                current_condition.push_front(new_text[i-1]);
                this->InsertInChain(current_condition, new_text[i]);
                continue;
            }


            current_condition.pop_back();
            current_condition.push_front(new_text[i-1]);
            this->InsertInChain(current_condition, new_text[i]);
        }

        this->FinalizationChain();

    }

    string GenerateNewWord(const deque<string>& current_condition) { 

        srand (time(NULL));

        string result_word;
        result_word.clear();

        map<deque<string>, map<string, double>>::iterator it_current_condition;
        it_current_condition = this->transitions_matrix.find(current_condition);

        if ( it_current_condition != this->transitions_matrix.end() )   { 

            //double prob = double (rand())/double(RAND_MAX);
            double prob = (rand()%100)/(100 * 1.0);

            map<string, double>:: iterator it_state = it_current_condition->second.begin(); 

            while (prob > it_state->second) { 
                ++it_state;
            }

            result_word = it_state->first;
        }

        return result_word;
    }

};

 
int InputChainData(vector<string>* full_text) { 

    string current_string;

    int power_of_chain = 0;

    cin >> power_of_chain;
    cin >> current_string;

    while (current_string[0] != '.') { 
        full_text->push_back(current_string);
        cin >> current_string;
    }

    return power_of_chain;
}


int main() { 

    vector<string> full_text;
    int power_of_chain = 0;

    power_of_chain = InputChainData(&full_text);

    for (int i = 0; i < full_text.size(); ++i) { 
        cout << full_text[i] << endl;
    }

    MarkovChain markov_chain; 
    //deque<string> distinct_words;

    markov_chain.AddTextToChain(full_text, power_of_chain);

    deque<string> current_condition;
    current_condition.push_back("bb");
    string new_word = markov_chain.GenerateNewWord(current_condition);
    
    cout << endl << "new_word:" << endl << new_word;

    int a;
    cin >> a;

    return 0;
}






