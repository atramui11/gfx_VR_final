// AUTHOR : Nikhil Trehan <nikhil_trehan@brown.edu>
// Tokenizer.cpp

#include "Tokenizer.hpp"
#include <fstream>
#include <iostream>
#include <cctype>
using namespace std;

bool Tokenizer::expecting(const string& str) {
    bool ans = Tokenizer::get() && (str == *this);
    return ans;
}


bool Tokenizer::get() {
    this->erase(); //clear current token
    char nextChar;
    
    nextChar=_ifstr.get();
    
    //pass all white space for any given call of get()
    bool temp = isspace(nextChar) ||(nextChar==',');
    while (temp) {
        nextChar=_ifstr.get();
        temp=isspace(nextChar);
    }
    
    
    // if tokenizer can parse new token, return true
    if (nextChar=='{' || nextChar=='}' || nextChar=='[' || nextChar==']') {
        this->push_back(nextChar);
        return true;
    }
    
    //*this (string) gets the full string of digits or letters, forming a word or number as a string
    else if (isalnum(nextChar) || nextChar=='.'|| nextChar=='-') {
        while (isalnum(nextChar) || nextChar=='.' || nextChar=='-') {
            this->push_back(nextChar);
            nextChar=_ifstr.get();
        }
        
        //end of array i.e. [... 4]
        if (nextChar==']') {
            this->erase();
            this->push_back(nextChar);
        }
        return true;
    }
    
    else {return false;}
    
}
