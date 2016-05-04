#ifndef _MATRIX2DCL_H
#define _MATRIX2DCL_H
#include "Matrix2D.h"

class Matrix2Dlc;

class Matrix2Dcl : public Matrix2D
{
  public:
    Matrix2Dcl();
    Matrix2Dcl(int line, int col);  // Constructor, create a line x col empty matrix
    Matrix2Dcl(const Matrix2Dcl& m); // Copy constructor  !! do not copy data but shares data

    ValueType get(int line, int col) const;
    void  set(int line, int col, ValueType v);
            
    Matrix2Dcl operator+(const Matrix2Dcl& m) const;  // addition operator for Matrix2Dcl
    Matrix2Dcl operator+(const Matrix2Dlc& m) const;   // return a new sum matrix

    void add(const Matrix2Dcl& m);  // internal addition for Matrix2Dcl
    void add(const Matrix2Dlc& m);
    
    Matrix2Dcl operator-(const Matrix2Dcl& m) const; // substraction operator for Matrix2Dcl                                            
    Matrix2Dcl operator-(const Matrix2Dlc& m) const;  // return a new diff. matrix
    
    void sub(const Matrix2Dcl& m);  // internal substraction for Matrix2Dcl                                            
    void sub(const Matrix2Dlc& m);
    
    Matrix2Dcl operator*(const Matrix2Dlc& m) const;  // multiply operator for Matrix2Dcl
                                                    // return a new mult. matrix  
    Matrix2Dcl operator*(const Matrix2Dcl& m) const;  // multiply operator for Matrix2Dcl
                                                    // return a new mult. matrix  
    Matrix2Dcl strassen(const Matrix2Dlc& b, int sizeMin);
    
    Matrix2Dcl getColsBloc(int noLine, int nbLines) const;
    Matrix2Dcl getBloc(int noLine, int noCol, int nbLines, int nbCols) const;
    
    void setColsBloc(int noLine, Matrix2Dcl v);
    void setBloc(int noLine, int noCol, Matrix2Dcl v);
    
   protected:
     ValueType getInternal(int line, int col) const;
     
         //void Matrix2Dcl::strassenP1
   //(const Matrix2Dlc& b, int sizeL, int sizeC, int sizeMin, long int p)


};

#endif
