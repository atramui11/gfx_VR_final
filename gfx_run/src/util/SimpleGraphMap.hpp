#ifndef SimpleGraphMap_hpp
#define SimpleGraphMap_hpp

#include <stdio.h>
#include <vector>

#endif /* SimpleGraphMap_hpp */

class SimpleGraphMap {

public:
    SimpleGraphMap(int N);
    ~SimpleGraphMap();
    void clear();
    // return (0<=jV && 0<=kV && jV!=kV && get(jV,kV)==-1)?value:-1
    int insert(int jV, int kV, int value);
    // if (jV,kV) is found, return the value associated with (jV,kV)
    // otherwise it returns -1
    int get(int jV, int kV) const;
    
 
private:
    std::vector<int> _data;
    std::vector<int> _first;
};
