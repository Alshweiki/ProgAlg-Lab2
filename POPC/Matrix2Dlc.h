#ifndef _MATRIX2DLC_H
#define _MATRIX2DLC_H
#include "Matrix2D.h"

class Matrix2Dcl;

class Matrix2Dlc : public Matrix2D
{
  public:
    Matrix2Dlc();
    Matrix2Dlc(int line, int col);
    Matrix2Dlc(const Matrix2Dlc& m);
 
    ValueType get(int line, int col) const;
    void  set(int line, int col, ValueType v);
    
    Matrix2Dlc operator+(const Matrix2Dlc& m) const;  // addition operator for Matrix2Dlc
    Matrix2Dlc operator+(const Matrix2Dcl& m) const;  // return a new sum matrix
    
    void add(const Matrix2Dlc& m);  // internal addition for Matrix2Dlc
    void add(const Matrix2Dcl& m);  // 

    Matrix2Dlc operator-(const Matrix2Dlc& m) const; // substraction operator for Matrix2Dlc                                            
    Matrix2Dlc operator-(const Matrix2Dcl& m) const; // return a new diff matrix

    void sub(const Matrix2Dlc& m);  // internal substraction for Matrix2Dlc                                            
    void sub(const Matrix2Dcl& m);
    
    Matrix2Dlc operator*(const Matrix2Dlc& m) const;  // multiply operator for Matrix2Dlc 
    Matrix2Dlc operator*(const Matrix2Dcl& m) const;  // return a new mult. matrix   
    Matrix2Dlc strassen(const Matrix2Dcl& b, int sizeMin);  // Strassen algorithm                                                                                       
  
    Matrix2Dlc getLinesBloc(int noLine, int nbLines) const;
    Matrix2Dlc getBloc(int noLine, int noCol, int nbLines, int nbCols) const;
    
    void setLinesBloc(int noLine, Matrix2Dlc v);
    void setBloc(int noLine, int noCol, Matrix2Dlc v);
    
   protected:
     ValueType getInternal(int line, int col) const;

};
#endif
