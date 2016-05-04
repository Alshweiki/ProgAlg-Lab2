#ifndef _MATRIX2D_H
#define _MATRIX2D_H
#include "timer.h"

#define ValueType double          // Type of values of the Matrix values

class Matrix2D : public POPBase   //mxn 2D Matrix with values of type "ValueType" 
{
  public:
    Matrix2D();                   // Default constructor, create an empty matrix 0x0
    Matrix2D(int line, int col);  // Constructor, create a 'line'x'col' empty matrix
    Matrix2D(const Matrix2D& m);  // Copy constructor  !! do not copy data but shares data
                                  // with m
    ~Matrix2D();                  // Destroy the matrix but only destroy data if not shared
    
    void init();                  // Fill matrix random values
    void init(char* filename);    // Fill matrix from a file
    void init(ValueType v);       // Fill matrix with v
    void clear();                 // Fill matrix with 0
    
    void null();  // Make the matrix a 0x0 matrix
    void Free();  // Free the memory used by data but keep all other meta info (size,...)
    
    virtual void  set(int line, int col, ValueType v)=0;  // set the value at line,col
    
    virtual int getLines() const;  // get number of lines
    virtual int getCols() const;   // get number of columns
    
    virtual Matrix2D& operator=(const Matrix2D& m); // assignment operator for Matrix2D
                                                    // create a copy (including data) of m
    virtual bool operator==(const Matrix2D& m) const; // equality test operator for Matrix2D
                                                   
    virtual void equ(const Matrix2D& m);  // no-copy assignment for Matrix2D
                                          // shares the data with m
    void display() const;                 // Display the matrix content
    void display(int n) const;            // Display only partially the matrix content
                                                  
    void showState(const char* s, bool all) const;// Display information on the Matrix status

    ValueType testMat();  // Feel de matrix with col[0]=1, col[1]=2,.. and
                          // return the value for test (for testing purpose)
                          
     ValueType* getMem() const; // Return the pointer to the matrix data (the values)
                                    
    // Serialize Matrix 2D for POP-C++ usage  
    virtual void Serialize(POPBuffer &buf, bool pack);

  protected:
    int nbLine;             // Nb. lines of the matrix
    int nbCol;              // Nb. columns of the matrix
    int dataSize;           // Current size (in byte) of the attached matrix
    ValueType* value;       // The matrix data (the values)
    ValueType* shared;      // If data are shared point the the beginning of shared values
    static const int ValueTypeSize = sizeof(ValueType); // Size of one value in bytes
    virtual ValueType getInternal(int line, int col) const;
};

#endif
