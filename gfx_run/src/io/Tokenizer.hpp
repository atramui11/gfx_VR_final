// Tokenizer.h
#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_
#include <fstream>
#include <string>
using namespace std;
class Tokenizer : public string {
    protected:
        ifstream& _ifstr;
    public:
        Tokenizer(ifstream& ifstr):_ifstr(ifstr) {}
        bool get();
        bool expecting(const string& s);
};
#endif /* _TOKENIZER_H_ */
