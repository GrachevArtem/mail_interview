
#include <curl/curl.h>
#include <vector>
#include <string> 
#include <iostream>
#include <sstream>

using namespace std;







class TextDownloader { 
    
public:
    
    vector< vector <string> > all_texts;
    
    TextDownloader();
    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void *userp);
    void DeleteDelimeters(string* readBuffer);
    void ConvertToLowerCase(string* readBuffer);
    
    void InputOneText(const char* link_on_text, vector<string>* text);
    void GetTextsFromLinks(const vector<string>& links);
    
};