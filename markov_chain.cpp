
    
#include "markov_chain.h"
    
using namespace std;
    
MarkovChain::MarkovChain(map<deque<string>, map<string, double> > transitions_matrix_) {
    transitions_matrix = transitions_matrix_;
}
    
MarkovChain::MarkovChain() {
    map<deque<string>, map<string, double> > transitions_matrix;
}


void MarkovChain::FinalizationChain() {

    for (map<deque<string>, map<string, double> >::iterator it_chain = this->transitions_matrix.begin(); 
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


void MarkovChain::InsertInChain(const deque<string>& current_condition, string next_word) { 

    map<deque<string>, map<string, double> >::iterator it_current_condition;
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

    
void MarkovChain::AddTextToChain(const vector<string>& new_text, int power_of_chain) { 

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

    

}


void MarkovChain:: LearnChain(const vector< vector <string> >& all_texts, int power_of_chain) { 
    
    for (int i = 0; i < static_cast<int>(all_texts.size()); ++i) { 
        this->AddTextToChain(all_texts[i], power_of_chain); 
    }
    
    this->FinalizationChain();
}
 


 
istream& operator >> (istream& is, MarkovChain& markov_chain) { 
    
    string cur_string;
    deque<string> current_condition;
    is >> cur_string;
    
    while (cur_string[0] != '.') {
        
        current_condition.clear();
        current_condition.push_front(cur_string);
        
        is >> cur_string;
    
        while(cur_string[0] != ':') { 
            is >> cur_string;
            current_condition.push_front(cur_string);
        }
                
        is >> cur_string;
        
        map<string, double> set_of_next_states;
        set_of_next_states.clear();
            
        while(cur_string[0] != ';') {
            
            string new_state = cur_string;
            is >> cur_string;
            double new_prob = atof(cur_string.c_str());
            is >> cur_string;
            
            set_of_next_states.insert(pair<string, double>(new_state, new_prob));
        }
        
        markov_chain.transitions_matrix.insert(pair< deque<string>, map<string, double> >(current_condition, set_of_next_states));
        
        is >> cur_string;
    }
    
    return is;
    
}


ostream& operator << (ostream& os, MarkovChain& markov_chain) { 
    
    map< deque<string>, map<string, double> >:: iterator it_chain;
    
    for (map<deque<string>, map<string, double> >::iterator it_chain = markov_chain.transitions_matrix.begin(); 
        it_chain != markov_chain.transitions_matrix.end(); ++it_chain) {

        for (int i = 0; i < static_cast<int>(it_chain->first.size() ); ++i) { 
            
            cout << it_chain->first[i] << " ";
            
        }
            
        cout << ": ";

        for (map<string, double>:: iterator it_state = it_chain->second.begin(); it_state != it_chain->second.end(); ++it_state) {
            
            cout << it_state->first << " ";
            cout << it_state->second << " ";
            
        }
        
        cout << "; " << endl;
    }
    
    cout << ". " << endl;
 
    return os;
}





string MarkovChain::GenerateNewWord(const deque<string>& current_condition) { 

    string result_word;
    result_word.clear();

    map<deque<string>, map<string, double> >::iterator it_current_condition;
    it_current_condition = this->transitions_matrix.find(current_condition);

    if ( it_current_condition != this->transitions_matrix.end() )   { 

        //double prob = double (rand())/double(RAND_MAX);
        double prob = (rand()%100)/(100.0);
        //cout << prob;

        map<string, double>:: iterator it_state = it_current_condition->second.begin(); 
        
        while (prob > it_state->second) { 
            ++it_state;
        }

        result_word = it_state->first;
        
    }
    return result_word;
}
