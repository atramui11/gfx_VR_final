

#ifndef _ROTATION_HPP_
#define _ROTATION_HPP_

class StaticRotation {

public:

  static void rotate
  (float* r /*[4]*/, float* x /*[3]*/, float* y /*[3]*/);

  static void vectorToMatrix
  (float ang_deg, float u0, float u1, float u2, float* R /*[16]*/);

  static void vectorToMatrix
  (float* r /*[4]*/, float* R /*[16]*/);

  static void matrixToVector
  (float* R /*[16]*/, float* r /*[4]*/);

  static void multiplyMatrices
  (float* A /*[16]*/, float* B /*[16]*/, float* C /* C[16]=A*B */);

  static void multiplyMatricesLeft
  (float* A /*[16]*/, float* B /*[16]*/);

  static void vectorMultiplyLeft
  (float ang_deg, float u0, float u1, float u2, float* r /*[4]*/);

  static void crossProduct
  (double* x /*[3]*/, double* y /*[3]*/, double* z /*[3]*/);

};

#endif // _ROTATION_HPP_
