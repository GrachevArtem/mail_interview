

#include <cstdio>
#include <cstdlib>
#include "markov_chain.h"
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <sstream>
#include <vector>


//using std::cout;
//using std::cin;
//using std::endl;
//#include "curl/curl.h"
//#pragma comment(lib,"curllib.lib")
//using std::string;
//using std::vector;




using namespace std;






static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//"https://dl.dropboxusercontent.com/u/37499722/1.txt"


void InputOneText(const char* link_on_text, vector<string>* text) {
       
    CURL *curl;
    CURLcode res;
    
    string readBuffer;
 
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, link_on_text);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

    }
    
    istringstream ist(readBuffer);
    
    string new_string;
    while ( ist >> new_string ) {
        text->push_back(new_string);
    }

    
}


void LearningPart() { 
   
    cout << "Enter power of chain: " << endl;
    int power_of_chain;
    cin >> power_of_chain;
    
    cout << "Enter text's links:" << endl;
    
    vector<string> links;
    string current_link;
    
    while (cin >> current_link) { 
        links.push_back(current_link);
    }
    
    vector< vector< string> > all_texts;
    vector<string> current_text;
    
    for (int i = 0; i < static_cast<int>(links.size()); ++i) { 
        current_text.clear();
        InputOneText(links[i].c_str(), &current_text);
        all_texts.push_back(current_text);
    }
    
    MarkovChain markov_chain;
    markov_chain.LearnChain(all_texts, power_of_chain);
    cout << markov_chain;

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
        current_condition.push_front(current_word);
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
        
        current_condition.pop_back();
        current_condition.push_front(new_word);
        
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
        //error;
    }

    
    return 0;
    
}
















