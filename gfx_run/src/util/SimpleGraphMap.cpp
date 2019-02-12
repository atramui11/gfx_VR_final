#include "SimpleGraphMap.hpp"
#include <vector>

SimpleGraphMap::SimpleGraphMap(int N) {
    //initialize all to -1
    for (int i=0; i<N; i++) {
        _first.push_back(-1);
    }
    
}

SimpleGraphMap::~SimpleGraphMap() {}

// if (jV,kV) is found, return the value associated with (jV,kV)
// otherwise it returns -1
int SimpleGraphMap::get(int jV, int kV) const {
    int value = -1;
    if(jV>=0 && kV>=0 && jV!=kV) {
        int hV,i;
        if(jV>kV) { hV=jV; jV=kV; kV=hV; }
        if(jV<(int)_first.size()) {
            for(i=_first[jV];i>=0 && (hV=_data[i])>=0;i=_data[i+1]) {
                if(hV==kV) { value = _data[i+2]; break; }
            }
        }
    }
    // cerr << "get("<< jV <<","<< kV <<") = "<< value << endl;
    return value;
}
    

int SimpleGraphMap::insert(int jV, int kV, int value)  {
    int retValue = -1;
    if(jV>=0 && kV>=0 && jV!=kV) {
        if((retValue=get(jV,kV))<0) {
            int i,hV;
            // make sure that jV<kV
            if(jV>kV) { hV=jV; jV=kV; kV=hV; }
            while(jV>=(int)_first.size())
                _first.push_back(-1);
            i = (int)(_data.size());
            _data.push_back(kV); // vertex index
            _data.push_back(_first[jV]); // next
            _data.push_back(value);
            _first[jV] = i;
            retValue = value;
        }
    }
}

void SimpleGraphMap::clear()  {
    //initialize all to -1
    int tempN=_first.size();
    _first.clear();
    for (int i=0; i<tempN; i++) {_first.push_back(-1);}
}


