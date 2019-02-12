

#ifndef _BBOX_HPP_
#define _BBOX_HPP_

#include <vector>

using namespace std;

class BBox {

private:

  int _d;
  float* _min;
  float* _max;
  float  _step;
  
public:
  
  BBox(const int d, const vector<float>& v, const bool cube);
  BBox(const int d=3);
  ~BBox();

  int    getDimension() const;
  float* getMin() const;  
  float  getMin(const int i) const;  
  float* getMax() const;
  float  getMax(const int i) const;
  float  getCenter(const int i) const;  
  float  getSide() const;
  float  getSide(const int i) const;
  float  getMaxSide() const;
  float  getDiameter() const;

  void   setMin(const float* value /*[3]*/);
  void   setMax(const float* value /*[3]*/);
};

#endif // _BBOX_HPP_
