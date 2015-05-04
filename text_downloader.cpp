
#include "text_downloader.h"


using namespace std;

TextDownloader:: TextDownloader() {}

size_t TextDownloader:: WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


void TextDownloader:: DeleteDelimeters(string *readBuffer) { 
    
    const string delimeters = ";:()[].,!?^$&*'`~·<>=+{}%@#-—\"\\/|_";
    //const string delimeters = ";:.,!?-—";
    
    readBuffer->push_back('t');

    for (string::reverse_iterator r_iter = readBuffer->rbegin(); r_iter != readBuffer->rend(); ++r_iter) {

        if (delimeters.find(*r_iter) != string::npos) {
            readBuffer->erase((r_iter + 1).base());
        }

    }

    //readBuffer.pop_back();
    readBuffer->erase(readBuffer->end() - 1);
    
    return;
}



void TextDownloader:: ConvertToLowerCase(string* readBuffer) { 
    
    const string upper_case = "ABCDEFGHIJKLMNOPQRSTUVWXYZАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    const string lower_case = "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя";

    for (string::iterator iter = readBuffer->begin(); iter != readBuffer->end(); ++iter) { 

        size_t pos_upper = upper_case.find(*iter);
        if (pos_upper != string:: npos) { 
            *iter = lower_case[pos_upper];
        }
        
    }
    return;
}


void TextDownloader:: InputOneText(const char* link_on_text, vector<string>* text) {
       
    CURL *curl;
    CURLcode res;
    
    string readBuffer;
 
    curl = curl_easy_init();
    
    if (curl) {
        
        curl_easy_setopt(curl, CURLOPT_URL, link_on_text);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);

        if (res == CURLE_OK) { 
            
            DeleteDelimeters(&readBuffer);
            ConvertToLowerCase(&readBuffer);
            
            istringstream ist(readBuffer);
            string new_string;
            
            while ( ist >> new_string ) {
                
                text->push_back(new_string);
                
            }
            
        } else {
            
            cout << "Error! " << "Can not get the data!" << endl; 
        
        }   
    } else {
        
        cout << "Error!" << "Can not initialize curl" << endl;
    }

    curl_easy_cleanup(curl);
       
    return;
}


void TextDownloader:: GetTextsFromLinks(const vector<string>& links) { 
       
    vector<string> current_text;
    
    for (int i = 0; i < static_cast<int>(links.size()); ++i) { 
        
        current_text.clear();
        InputOneText(links[i].c_str(), &current_text);
        this->all_texts.push_back(current_text);
    
    }
    
}