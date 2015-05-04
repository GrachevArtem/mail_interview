

#include <cstdio>
#include <cstdlib>
#include "markov_chain.h"
#include "text_downloader.h"
#include <iostream>
#include <string>
#include <vector>


using namespace std;








void LearningPart() { 
   
    cout << "Enter power of chain: " << endl;
    int power_of_chain;
    cin >> power_of_chain;
    
        

    string chain_filename;
    cout << "Enter the name of file for chain: " << endl;
    cin >> chain_filename;
    
    
    cout << "Enter text's links:" << endl;
    
    vector<string> links;
    string current_link;
    
    while (cin >> current_link) { 
        links.push_back(current_link);
    }
    
    
    TextDownloader text_downloader;
    text_downloader.GetTextsFromLinks(links);
    
    MarkovChain markov_chain;
    markov_chain.LearnChain(text_downloader.all_texts, power_of_chain);
    
    ofstream out_f;
    out_f.open(chain_filename.c_str());
    out_f << markov_chain;
    out_f.close();
    //cout << markov_chain;

}
 

 
 void ExploitationPart() { 
   
    
    cout << "Input Markov Chain file:" << endl;
    string filename;
    cin >> filename;
    
    cout << "Enter power of Chain:" << endl;
    int power_of_chain;
    cin >> power_of_chain;
    
    cout << "Enter first " << power_of_chain << " words: " << endl;
    deque< string > current_condition; 
    string current_word;
    
    for (int i = 0; i < power_of_chain; ++i) { 
        cin >> current_word;
        current_condition.push_back(current_word);
    }
   
    cout << "Enter the number of words you want to generate: " << endl;
    int number_of_words;
    cin >> number_of_words;
    
    fstream fin;
    fin.open(filename.c_str());
    
    MarkovChain markov_chain;
    fin >> markov_chain;
    
    //cout << markov_chain;
    
    cout << "Sequence: " << endl;
    string new_word;
    for( int i = 0; i < number_of_words; ++i) { 
        
        new_word = markov_chain.GenerateNewWord(current_condition);
        
        if (new_word.empty()) { 
            cout << endl;
            cout << "Unknown sequence" << endl;
            break;
        }
        
        cout << new_word << endl;
        
        current_condition.pop_front();
        current_condition.push_back(new_word);
        
    }

}



 
int main(void) {
    
    srand (time(NULL));
    cout << "Learning or Exploitation? (l/e)" << endl;
    
    char user_choose;
    cin >> user_choose;
    
    if (user_choose == 'l' || user_choose == 'L') {
        LearningPart();
    } else if (user_choose == 'e' || user_choose == 'E') { 
        ExploitationPart();
    } else {
        cout << "Undefined character" << endl; 
    }

    
    return 0;
    
}
















