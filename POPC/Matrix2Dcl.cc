#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Matrix2Dcl.h"
#include "Matrix2Dlc.h"

//-- -----------------------------------------------------------------------------
Matrix2Dcl::Matrix2Dcl(): Matrix2D()
{}

Matrix2Dcl::Matrix2Dcl(int line, int col): Matrix2D(line,col)
{}

Matrix2Dcl::Matrix2Dcl(const Matrix2Dcl& m): Matrix2D(m)
{}

//-- -----------------------------------------------------------------------------
ValueType Matrix2Dcl::get(int line, int col) const
{ return value[col*nbLine+line];}

ValueType Matrix2Dcl::getInternal(int line, int col) const
{return value[col*nbLine+line];}//get(line,col);}

//-- -----------------------------------------------------------------------------
inline void Matrix2Dcl::set(int line, int col, ValueType v)
{ value[col*nbLine+line]=v;}

//-- -----------------------------------------------------------------------------
//  Return a new Matrix (copy the data) which is the sum of *this and m
Matrix2Dcl Matrix2Dcl::operator+(const Matrix2Dcl& m) const
{
  if ((nbLine==m.nbLine) && (nbCol==m.nbCol))
  {
    Matrix2Dcl tmp(nbLine, nbCol);
    int size = nbLine*nbCol;
    for (int i=0; i<size; i++) tmp.value[i] = value[i] + m.value[i];    
    return tmp;
  } else {printf("\nAddition of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Return a new Matrix (copy the data) which is the sum of *this and m
Matrix2Dcl Matrix2Dcl::operator+(const Matrix2Dlc& m) const
{
  if ((nbLine==m.getLines()) && (nbCol==m.getCols()))
  {
    Matrix2Dcl tmp(nbLine, nbCol);
    ValueType* valueM = m.getMem();
    ValueType* valueTmp = tmp.getMem();
    int colsM = m.getCols();  

    for (int j=0; j<nbCol; j++)
    {
      int theCol = j*nbLine;
      ValueType* colTmp = valueTmp + theCol; 
      ValueType* colThis = value + theCol;
      for (int i=0; i<nbLine; i++) colTmp[theCol+i] = colThis[theCol+i] + valueM[i*colsM+j];
    }   
    
   /* for (int i=0; i<nbLine; i++)
      for (int j=0; j<nbCol; j++)
        tmp.value[j*nbLine+i]=value[j*nbLine+i] + m.get(i,j);
        //tmp.set(i,j,value[j*nbLine+i] + m.get(i,j));*/
    return tmp;
  } else {printf("\nAddition of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Return a new Matrix (copy the data) which is the difference of *this and m
Matrix2Dcl Matrix2Dcl::operator-(const Matrix2Dcl& m) const
{
  if ((nbLine==m.nbLine) && (nbCol==m.nbCol))
  {
    Matrix2Dcl tmp(nbLine, nbCol);
    int size = nbLine*nbCol;
    for (int i=0; i<size; i++) tmp.value[i] = value [i] - m.value[i];
    return tmp;
  }
  else {printf("\nAddition of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Return a new Matrix (copy the data) which is the difference of *this and m
Matrix2Dcl Matrix2Dcl::operator-(const Matrix2Dlc& m) const
{
  if ((nbLine==m.getLines()) && (nbCol==m.getCols()))
  {
    Matrix2Dcl tmp(nbLine, nbCol);
    for (int i=0; i<nbLine; i++)
      for (int j=0; j<nbCol; j++)
        tmp.value[j*nbLine+i]=value[j*nbLine+i] - m.get(i,j);
        //tmp.set(i,j, get(i,j) - m.get(i,j));
    return tmp;
  } else {printf("\nAddition of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Return a new Matrix (copy the data) which is the multiplication of *this and m
//  Use the standard algorithm to multiply the matrices c = c + a*b
Matrix2Dcl Matrix2Dcl::operator*(const Matrix2Dlc& m) const
{
  if (nbCol==m.getLines())
  {
    Matrix2Dcl tmp(nbLine, m.getCols());    
    ValueType r=0; int c = tmp.nbCol;  
    for (int i=0; i<nbLine; i++)
       for (int j=0; j<c; j++)
       {
         for (int k=0; k<nbCol; k++) r += value[k*nbLine+i] * m.get(k,j);
          // tmp[i,j] =  tmp[i,j] + this[i,k] * m[k,j]
         tmp.set(i,j,r);
         r=0;
       } 
    tmp.shared=NULL;
    return tmp;
  }
  else {printf("\nMultiplication of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Return a new Matrix (copy the data) which is the multiplication of *this and m
//  Use the standard algorithm to multiply the matrices c = c + a*b
Matrix2Dcl Matrix2Dcl::operator*(const Matrix2Dcl& m) const
{
  if (nbCol==m.getLines())
  {
    Matrix2Dcl tmp(nbLine, m.getCols());    
    ValueType r=0; int c = tmp.nbCol;  
    for (int i=0; i<nbLine; i++)
       for (int j=0; j<c; j++)
       {
        int theLine = j*nbLine;
         for (int k=0; k<nbCol; k++) r += value[k*nbLine+i] * m.value[j*nbLine+k];
          // tmp[i,j] =  tmp[i,j] + this[i,k] * m[k,j]
         tmp.set(i,j,r);
         r=0;
       } 
    tmp.shared=NULL;
    return tmp;
  }
  else {printf("\nMultiplication of incompatible Matrices\n"); exit(0);}
}

// -- ----------------------------------------------------------------------------
Matrix2Dcl Matrix2Dcl::strassen(const Matrix2Dlc& b, int sizeMin)
{
  Matrix2Dcl c(nbLine, b.getCols());
  
  // When the one of the matrix sizes is less than minSize, uses standard algorithm
  if ((nbLine<=sizeMin) || (nbCol<=sizeMin) || (b.getCols()<=sizeMin))
    {c = *this * b;}
    
  // other cases use STRASSEN algorithm
  else
  {
    int size, aSizeL, aSizeC, bSizeL, bSizeC;  // Sizes of the small matrices
  
    size = nbLine;
    if ((size % 2)==0) aSizeL = size/2; else aSizeL = size/2+1;
    size = nbCol;
    if ((size % 2)==0) aSizeC = size/2; else aSizeC = size/2+1;
    size = b.getLines();
    if ((size % 2)==0) bSizeL = size/2; else bSizeL = size/2+1;
    size = b.getCols();
    if ((size % 2)==0) bSizeC = size/2; else bSizeC = size/2+1;
 
    Matrix2Dcl a11(aSizeL,aSizeC), a12(aSizeL,aSizeC),
               a21(aSizeL,aSizeC), a22(aSizeL,aSizeC), ar(aSizeL,aSizeC);
    Matrix2Dlc b11(bSizeL,bSizeC), b12(bSizeL,bSizeC),
               b21(bSizeL,bSizeC), b22(bSizeL,bSizeC), br(bSizeL,bSizeC);

                           
    a11.clear(); a12.clear(); a21.clear(); a22.clear();                         
    b11.clear(); b12.clear(); b21.clear(); b22.clear();                         

    Matrix2Dcl p1(aSizeL, bSizeC), p2(aSizeL, bSizeC), p3(aSizeL, bSizeC),
               p4(aSizeL, bSizeC), p5(aSizeL, bSizeC), p6(aSizeL, bSizeC),
               p7(aSizeL, bSizeC);
               
    a11.setBloc(0,0,getBloc(0,0,aSizeL,aSizeC));
    a12.setBloc(0,0,getBloc(0,aSizeC,aSizeL,getCols()-aSizeC));
    a21.setBloc(0,0,getBloc(aSizeL,0, nbLine-aSizeL, aSizeC));
    a22.setBloc(0,0,getBloc(aSizeL,aSizeC,nbLine-aSizeL,getCols()-aSizeC));
    
    b11.setBloc(0,0,b.getBloc(0,0,bSizeL,bSizeC));
    b12.setBloc(0,0,b.getBloc(0,bSizeC,bSizeL,b.getCols()-bSizeC));
    b21.setBloc(0,0,b.getBloc(bSizeL,0, b.getLines()-bSizeL, bSizeC));
    b22.setBloc(0,0,b.getBloc(bSizeL,bSizeC,b.getLines()-bSizeL,b.getCols()-bSizeC));
 
    
    ar.equ(a11+a22); br.equ(b11+b22);
    p1.equ(ar.strassen(br, sizeMin));                // p1 = (a11+a22) * (b11+b22)
    //p1.equ((a11+a22).strassen((b11+b22), sizeMin));  // p1 = (a11+a22) * (b11+b22)
    //Matrix2Dcl p1((a11+a22).strassen((b11+b22),sizeMin));
    
    ar.equ(a21+a22);
    p2.equ(ar.strassen(b11, sizeMin));         // p2 = (a21+a22) * (b11)
    //Matrix2Dcl p2((a21+a22).strassen(b11,sizeMin));
   
    br.equ(b12-b22);
    p3.equ(a11.strassen(br, sizeMin));         // p3 = (a11) * (b12 - b22)   
    //Matrix2Dcl p3(a11.strassen((b12-b22),sizeMin));
   
    br.equ(b21-b11);
    p4.equ(a22.strassen(br, sizeMin));         // p4 = (a22) * (b21 - b11)
    //Matrix2Dcl p4(a22.strassen((b21-b11),sizeMin));

    ar.equ(a11+a12);
    p5=(ar.strassen(b22, sizeMin));            // p5 = (a11+a12) * (b22)	
    //Matrix2Dcl p5((a11+a12).strassen(b22,sizeMin));
   
    ar.equ(a21-a11); br.equ(b11+b12);
    p6.equ(ar.strassen(br, sizeMin));          // p6 = (a21-a11) * (b11+b12)
    //Matrix2Dcl p6((a21-a11).strassen((b11+b12),sizeMin));
   
    ar.equ(a12-a22); br.equ(b21+b22);
    p7.equ(ar.strassen(br, sizeMin));          // p7 = (a12-a22) * (b21+b22)
    //Matrix2Dcl p7((a12-a22).strassen((b21+b22),sizeMin));
 
    // Calculating c11, c12, c211, c22
    // and grouping the results obtained in a single matrix c
    c.setBloc(0,0,(p7+p1+p4-p5).getBloc(0,0,aSizeL, bSizeC));              //c11
    c.setBloc(0,bSizeC,(p5+p3).getBloc(0,0,aSizeL,c.getCols()-bSizeC));    //c12 
    c.setBloc(aSizeL,0,(p4+p2).getBloc(0,0,c.getLines()-aSizeL,bSizeC));   //c21
    c.setBloc(aSizeL, bSizeC,
        (p1+p3+p6-p2).getBloc(0,0,c.getLines()-aSizeL,c.getCols()-bSizeC));//c22  
  } // end of else
  return c;
} // end of Strassen function
/*

//-- -----------------------------------------------------------------------------
void Matrix2Dcl::strassenP1
   (const Matrix2Dlc& b, int sizeL, int sizeC, int sizeMin, long int p)
{
    Matrix2Dcl a11(aSizeL,aSizeC), a22(aSizeL,aSizeC), ar(aSizeL,aSizeC);
    Matrix2Dlc b11(bSizeL,bSizeC), b22(bSizeL,bSizeC), br(bSizeL,bSizeC);
                         
    a11.clear(); a22.clear();                         
    b11.clear(); b12.clear(); b21.clear(); b22.clear();   
                          
    a11.setBloc(0,0,getBloc(0,0,aSizeL,aSizeC));
    a22.setBloc(0,0,getBloc(aSizeL,aSizeC,nbLine-aSizeL,getCols()-aSizeC));
    
    b11.setBloc(0,0,b.getBloc(0,0,bSizeL,bSizeC));
    b22.setBloc(0,0,b.getBloc(bSizeL,bSizeC,b.getLines()-bSizeL,b.getCols()-bSizeC));
 
    ar.equ(a11+a22); br.equ(b11+b22);
    *((Matrix2Dcl*)p)=ar.strassen(br, sizeMin);          // p1 = (a11+a22) * (b11+b22)
}

//-- -----------------------------------------------------------------------------
Matrix2Dcl  Matrix2Dcl::strassenP2(const Matrix2Dlc& b, int sizeL, int sizeC, int sizeMin)
{
  Matrix2Dcl a21(aSizeL,aSizeC), a22(aSizeL,aSizeC), ar(aSizeL,aSizeC);
  Matrix2Dlc b11(bSizeL,bSizeC);
  a21.clear(); a22.clear(); b11.clear();                     
 
  a21.setBloc(0,0,getBloc(aSizeL,0, nbLine-aSizeL, aSizeC));
  a22.setBloc(0,0,getBloc(aSizeL,aSizeC,nbLine-aSizeL,getCols()-aSizeC));
  b11.setBloc(0,0,b.getBloc(0,0,bSizeL,bSizeC));
    
  ar.equ(a21+a22);
  return ar.strassen(b11, sizeMin);         // p2 = (a21+a22) * (b11)
}

//-- -----------------------------------------------------------------------------
Matrix2Dcl  Matrix2Dcl::strassenP3(const Matrix2Dlc& b, int sizeL, int sizeC, int sizeMin)
{
  Matrix2Dcl a11(aSizeL,aSizeC);
  Matrix2Dlc b12(bSizeL,bSizeC), b22(bSizeL,bSizeC), br(bSizeL,bSizeC);
  a11.clear(); b12.clear(); b22.clear();                         

  a11.setBloc(0,0,getBloc(0,0,aSizeL,aSizeC));
    
  b12.setBloc(0,0,b.getBloc(0,bSizeC,bSizeL,b.getCols()-bSizeC));
  b22.setBloc(0,0,b.getBloc(bSizeL,bSizeC,b.getLines()-bSizeL,b.getCols()-bSizeC));
    
  br.equ(b12-b22);
  return a11.strassen(br, sizeMin);         // p3 = (a11) * (b12 - b22)   
}

//-- -----------------------------------------------------------------------------
Matrix2Dcl  Matrix2Dcl::strassenP4(const Matrix2Dlc& b, int sizeL, int sizeC, int sizeMin)
{
    Matrix2Dcl a22(aSizeL,aSizeC);
    Matrix2Dlc b11(bSizeL,bSizeC), b21(bSizeL,bSizeC), br(bSizeL,bSizeC);                           
    a22.clear(); b11.clear(); b21.clear();                         
               
    a22.setBloc(0,0,getBloc(aSizeL,aSizeC,nbLine-aSizeL,getCols()-aSizeC));
    
    b11.setBloc(0,0,b.getBloc(0,0,bSizeL,bSizeC));
    b21.setBloc(0,0,b.getBloc(bSizeL,0, b.getLines()-bSizeL, bSizeC));
 
  br.equ(b21-b11);
  return a22.strassen(br, sizeMin);         // p4 = (a22) * (b21 - b11)
}

//-- -----------------------------------------------------------------------------
Matrix2Dcl  Matrix2Dcl::strassenP5(const Matrix2Dlc& b, int sizeL, int sizeC, int sizeMin)
{
  Matrix2Dcl a11(aSizeL,aSizeC), a12(aSizeL,aSizeC), ar(aSizeL,aSizeC);
  Matrix2Dlc b22(bSizeL,bSizeC);                          
  a11.clear(); a12.clear(); b22.clear();                         
               
  a11.setBloc(0,0,getBloc(0,0,aSizeL,aSizeC));
  a12.setBloc(0,0,getBloc(0,aSizeC,aSizeL,getCols()-aSizeC));
    
  b22.setBloc(0,0,b.getBloc(bSizeL,bSizeC,b.getLines()-bSizeL,b.getCols()-bSizeC));

  ar.equ(a11+a12);
  return ar.strassen(b22, sizeMin);            // p5 = (a11+a12) * (b22)	
}    

Matrix2Dcl  Matrix2Dcl::strassenP6(const Matrix2Dlc& b, int sizeL, int sizeC, int sizeMin)
{
  Matrix2Dcl a11(aSizeL,aSizeC), a21(aSizeL,aSizeC), ar(aSizeL,aSizeC);
  Matrix2Dlc b11(bSizeL,bSizeC), b12(bSizeL,bSizeC), br(bSizeL,bSizeC);
  a11.clear(); a21.clear(); b11.clear(); b12.clear();                         
               
  a11.setBloc(0,0,getBloc(0,0,aSizeL,aSizeC));
  a21.setBloc(0,0,getBloc(aSizeL,0, nbLine-aSizeL, aSizeC));
    
  b11.setBloc(0,0,b.getBloc(0,0,bSizeL,bSizeC));
  b12.setBloc(0,0,b.getBloc(0,bSizeC,bSizeL,b.getCols()-bSizeC));

  ar.equ(a21-a11); br.equ(b11+b12);
  return ar.strassen(br, sizeMin);          // p6 = (a21-a11) * (b11+b12)
}

//-- -----------------------------------------------------------------------------
Matrix2Dcl  Matrix2Dcl::strassenP7(const Matrix2Dlc& b, int sizeL, int size C, int sizeMin)
{
  Matrix2Dcl a12(aSizeL,aSizeC), a22(aSizeL,aSizeC), ar(aSizeL,aSizeC);
  Matrix2Dlc b21(bSizeL,bSizeC), b22(bSizeL,bSizeC), br(bSizeL,bSizeC);                           
  a12.clear(); a22.clear(); b21.clear(); b22.clear();                         
               
  a12.setBloc(0,0,getBloc(0,aSizeC,aSizeL,getCols()-aSizeC));
  a22.setBloc(0,0,getBloc(aSizeL,aSizeC,nbLine-aSizeL,getCols()-aSizeC));
    
  b21.setBloc(0,0,b.getBloc(bSizeL,0, b.getLines()-bSizeL, bSizeC));
  b22.setBloc(0,0,b.getBloc(bSizeL,bSizeC,b.getLines()-bSizeL,b.getCols()-bSizeC));
 
  ar.equ(a12-a22); br.equ(b21+b22);
  return ar.strassen(br, sizeMin);          // p7 = (a12-a22) * (b21+b22)
}


//-- -----------------------------------------------------------------------------
Matrix2Dcl Matrix2Dcl::strassen(const Matrix2Dlc& b, int sizeMin)
{
  Matrix2Dcl c(nbLine, b.getCols());
  
  // When the one of the matrix sizes is less than minSize, uses standard algorithm
  if ((nbLine<=sizeMin) || (nbCol<=sizeMin) || (b.getCols()<=sizeMin))
    {c = *this * b;}
    
  // other cases use STRASSEN algorithm
  else
  {
    int size, aSizeL, aSizeC, bSizeL, bSizeC;  // Sizes of the small matrices
  
    size = nbLine;
    if ((size % 2)==0) aSizeL = size/2; else aSizeL = size/2+1;
    size = nbCol;
    if ((size % 2)==0) aSizeC = size/2; else aSizeC = size/2+1;
    size = b.getLines();
    if ((size % 2)==0) bSizeL = size/2; else bSizeL = size/2+1;
    size = b.getCols();
    if ((size % 2)==0) bSizeC = size/2; else bSizeC = size/2+1;
 
    Matrix2Dcl p1(aSizeL, bSizeC), p2(aSizeL, bSizeC), p3(aSizeL, bSizeC),
               p4(aSizeL, bSizeC), p5(aSizeL, bSizeC), p6(aSizeL, bSizeC),
               p7(aSizeL, bSizeC);               
 
    if (nbCores>1);
    {
      this->strassenP1(b, sizeL, sizeC, sizeMin, ((long int)(&p1)); // p1 = (a11+a22) * (b11+b22)   
//      p2=this->strassenP2(b, sizeL, sizeC, sizeMin);         // p2 = (a21+a22) * (b11)   
//      p3=this->strassenP3(b, sizeL, sizeC, sizeMin);         // p3 = (a11) * (b12 - b22)       
//      p4=this->strassenP4(b, sizeL, sizeC, sizeMin);         // p4 = (a22) * (b21 - b11) 
//      p5=this->strassenP5(b, sizeL, sizeC, sizeMin);         // p5 = (a11+a12) * (b22)	    
//      p6=this->strassenP6(b, sizeL, sizeC, sizeMin);         // p6 = (a21-a11) * (b11+b12)    
//      p7=this->strassenP7(b, sizeL, sizeC, sizeMin);         // p7 = (a12-a22) * (b21+b22)
    }
    else
    {
      Matrix2Dcl a11(aSizeL,aSizeC), a12(aSizeL,aSizeC),
                 a21(aSizeL,aSizeC), a22(aSizeL,aSizeC), ar(aSizeL,aSizeC);
      Matrix2Dlc b11(bSizeL,bSizeC), b12(bSizeL,bSizeC),
                 b21(bSizeL,bSizeC), b22(bSizeL,bSizeC), br(bSizeL,bSizeC);

                           
      a11.clear(); a12.clear(); a21.clear(); a22.clear();                         
      b11.clear(); b12.clear(); b21.clear(); b22.clear();                         
               
      a11.setBloc(0,0,getBloc(0,0,aSizeL,aSizeC));
      a12.setBloc(0,0,getBloc(0,aSizeC,aSizeL,getCols()-aSizeC));
      a21.setBloc(0,0,getBloc(aSizeL,0, nbLine-aSizeL, aSizeC));
      a22.setBloc(0,0,getBloc(aSizeL,aSizeC,nbLine-aSizeL,getCols()-aSizeC));
    
      b11.setBloc(0,0,b.getBloc(0,0,bSizeL,bSizeC));
      b12.setBloc(0,0,b.getBloc(0,bSizeC,bSizeL,b.getCols()-bSizeC));
      b21.setBloc(0,0,b.getBloc(bSizeL,0, b.getLines()-bSizeL, bSizeC));
      b22.setBloc(0,0,b.getBloc(bSizeL,bSizeC,b.getLines()-bSizeL,b.getCols()-bSizeC));
 
    
      ar.equ(a11+a22); br.equ(b11+b22);
      p1.equ(ar.strassen(br, sizeMin));          // p1 = (a11+a22) * (b11+b22)
    
      ar.equ(a21+a22);
      p2.equ(ar.strassen(b11, sizeMin));         // p2 = (a21+a22) * (b11)
   
      br.equ(b12-b22);
      p3.equ(a11.strassen(br, sizeMin));         // p3 = (a11) * (b12 - b22)   
    
      br.equ(b21-b11);
      p4.equ(a22.strassen(br, sizeMin));         // p4 = (a22) * (b21 - b11)
 
      ar.equ(a11+a12);
      p5=(ar.strassen(b22, sizeMin));            // p5 = (a11+a12) * (b22)	
    
      ar.equ(a21-a11); br.equ(b11+b12);
      p6.equ(ar.strassen(br, sizeMin));          // p6 = (a21-a11) * (b11+b12)
    
      ar.equ(a12-a22); br.equ(b21+b22);
      p7.equ(ar.strassen(br, sizeMin));          // p7 = (a12-a22) * (b21+b22)
    }
 
    // Calculating c11, c12, c211, c22
    // and grouping the results obtained in a single matrix c
    c.setBloc(0,0,(p7+p1+p4-p5).getBloc(0,0,aSizeL, bSizeC));              //c11
    c.setBloc(0,bSizeC,(p5+p3).getBloc(0,0,aSizeL,c.getCols()-bSizeC));    //c12 
    c.setBloc(aSizeL,0,(p4+p2).getBloc(0,0,c.getLines()-aSizeL,bSizeC));   //c21
    c.setBloc(aSizeL, bSizeC,
        (p1+p3+p6-p2).getBloc(0,0,c.getLines()-aSizeL,c.getCols()-bSizeC));//c22  
  } // end of else
  return c;
} // end of Strassen function
*/
//-- -----------------------------------------------------------------------------
//  Internal addition: *this becomes the sum of *this and m
void Matrix2Dcl::add(const Matrix2Dcl& m)
{
  if ((nbLine==m.nbLine) && (nbCol==m.nbCol))
    for (int i=0; i<dataSize; i++) value[i] = value[i] + m.value[i];
  else {printf("\nInternal addition of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Internal addition: *this becomes the sum of *this and m
void Matrix2Dcl::add(const Matrix2Dlc& m)
{
  if ((nbLine==m.getLines()) && (nbCol==m.getCols()))
  {
    for (int i=0; i<nbLine; i++)
      for (int j=0; j<nbCol; j++) set(i,j, get(i,j) + m.get(i,j));
  } else {printf("\nAddition of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Internal subtraction: *this becomes the difference of *this and m
void Matrix2Dcl::sub(const Matrix2Dcl& m)
{
  if ((nbLine==m.nbLine) && (nbCol==m.nbCol))
    for (int i=0; i<dataSize; i++) value[i] = value[i] - m.value[i];
  else {printf("\nInternal substraction of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Internal subtraction: *this becomes the difference of *this and m
void Matrix2Dcl::sub(const Matrix2Dlc& m)
{
  if ((nbLine==m.getLines()) && (nbCol==m.getCols()))
  {
    for (int i=0; i<nbLine; i++)
      for (int j=0; j<nbCol; j++) set(i,j, get(i,j) - m.get(i,j));
  } else {printf("\nInternal substraction of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Return a matrix which is a bloc of col. of *this (no copy, values are shared with *this) 
Matrix2Dcl Matrix2Dcl::getColsBloc(int noCol, int nbCols) const
{
  if ((value!=NULL) || (nbCol>=(noCol+nbCols)))
  {
    Matrix2Dcl tmp;
    tmp.nbCol = nbCols;
    tmp.nbLine = nbLine;
    tmp.dataSize = dataSize;
    tmp.value=&(value[noCol*nbLine]);
    if(shared==NULL) tmp.shared = value; else tmp.shared=shared;
    if (tmp.shared!=NULL) (tmp.shared[dataSize])=(tmp.shared[dataSize])+(ValueType)1;
    return tmp;
  }
  else
  {
    printf("\nMatrix ERROR: Non coherent columns block getting (%d+%d>%d)!!!\n",
                                                        noCol,nbCols,nbCol);
    showState("Source matrix:", false);
    printf("PROGRAM ABORTED !!!\n");
    exit(0);
  }
}

//-- -----------------------------------------------------------------------------
//  Modify the values of a  bloc of col. of *this using values in 'v' 
void Matrix2Dcl::setColsBloc(int noCol, Matrix2Dcl v)
{
  if ((nbLine==v.nbLine) && (nbCol>=(noCol+v.nbCol)) )
  {  
    if (value==NULL)
    {
      dataSize = nbLine*nbCol;
      value = (ValueType*)malloc((dataSize+1)*ValueTypeSize);
      if (value==NULL) {printf("\nMEMORY OVERFLOW !!!!\n"); exit(0);}
      value[dataSize]=0;
      shared = NULL;
    }
    memcpy (&(value[noCol*nbLine]),v.value, v.nbCol*v.nbLine*ValueTypeSize);
     // memcpy replaces the following for loop
     // for (int i=0; i<v.nbCol*v.nbLine; i++) value[noCol*nbLine+i]=v.value[i];
  }
  else
  {
    printf("\nMatrix ERROR: Non coherent columns block setting at column %d!!!\n", noCol);
    v.showState("Source Matrix:\n", false);
    showState("Destination Matrix:\n", false);
    printf("PROGRAM ABORTED !!!\n");
    exit(0);
  }
}

//-- -----------------------------------------------------------------------------
//  Modify the values of a bloc *this at [noline,noCal] using values in 'v' 
void Matrix2Dcl::setBloc(int noLine, int noCol, Matrix2Dcl v)
{
  if ((nbCol>=noCol+v.nbCol) && (nbLine>=(noLine+v.nbLine)) )
  {
    if (value==NULL)
    {
      dataSize = nbLine*nbCol;
      value = (ValueType*)malloc((dataSize+1)*ValueTypeSize);
      if (value==NULL) {printf("\nMEMORY OVERFLOW !!!!\n"); exit(0);}
      value[dataSize]=0;
      shared = NULL;
    }
    for (int i=0; i<v.nbCol; i++)
    memcpy(&(value[(noCol+i)*nbLine+noLine]),&(v.value[i*v.nbLine]), v.nbLine*ValueTypeSize); 
     // memcpy replaces the following for loop
     //for (int j=0; j<nbLine; j++)
     //  value[(noCol+i)*nbLine+noLine+j]=v.value[i*v.nbLine+j];
  }
  else
  {
    printf("Matrix ERROR: Non coherent bloc[%dx%d] setting at %d, %d !!!\n",
                 v.nbLine, v.nbCol, noLine, noCol );
    v.showState("Source Matrix:\n", false);
    showState("Destination Matrix:\n", false);
    printf("PROGRAM ABORTED !!!\n");
    exit(0);
  }
}

//-- -----------------------------------------------------------------------------
//  Return a new matrix (copy the data) which is the bloc of *this at [noLine,noCol]
Matrix2Dcl Matrix2Dcl::getBloc(int noLine, int noCol, int nbLines, int nbCols) const
{
  if ((value!=NULL) && (nbCol>=(noCol+nbCols)) && (nbLine>=(noLine+nbLines)))
  {
    Matrix2Dcl tmp(nbLines,nbCols);
    for (int i=0; i<nbCols; i++)
       memcpy(&(tmp.value[i*nbLines]),
              &(value[(noCol+i)*nbLine+noLine]), nbLines*ValueTypeSize); 
      // memcpy(dest, scr, nb) replaces the following for loop
      //for (int j=0; j<nbLine; j++)	    
      //  tmp.value[i*v.nbLine+j] = value[(noCol+i)*nbLine+noLine+j];
      return tmp;
  }
  else
  {
    printf("\nMatrix ERROR: Non coherent bloc[%dx%d] getting at %dx%d !!!\n",
                             nbLines, nbCols, noLine, noCol);
    showState("Source Matrix:\n", false);
    printf("PROGRAM ABORTED !!!\n");
    exit(0);
  }
}



