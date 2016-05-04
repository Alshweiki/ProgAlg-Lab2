#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Matrix2Dlc.h"
#include "Matrix2Dcl.h"

//-- ---Constructors--------------------------------------------------------------
Matrix2Dlc::Matrix2Dlc(): Matrix2D()
{}

Matrix2Dlc::Matrix2Dlc(int line, int col): Matrix2D(line,col)
{}

Matrix2Dlc::Matrix2Dlc(const Matrix2Dlc& m):  Matrix2D(m)
{}

//-- -----------------------------------------------------------------------------
ValueType Matrix2Dlc::get(int line, int col) const
{ return value[line*nbCol+col];}

ValueType Matrix2Dlc::getInternal(int line, int col) const
{ return value[line*nbCol+col];}//get(line,col);}

//-- -----------------------------------------------------------------------------
inline void Matrix2Dlc::set(int line, int col, ValueType v)
{ value[line*nbCol+col]=v;}

//-- -----------------------------------------------------------------------------
//  Return a new Matrix (copy the data) which is the sum of *this and m
Matrix2Dlc Matrix2Dlc::operator+(const Matrix2Dlc& m) const
{    
  if ((nbLine==m.nbLine) && (nbCol==m.nbCol))
  {
    Matrix2Dlc tmp(nbLine, nbCol);
    int size = nbLine*nbCol;
    for (int i=0; i<size; i++) tmp.value[i] = value[i] + m.value[i]; 
    return tmp;
  } else {printf("\nAddition of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Return a new Matrix (copy the data) which is the sum of *this and m
Matrix2Dlc Matrix2Dlc::operator+(const Matrix2Dcl& m) const
{
  if ((nbLine==m.getLines()) && (nbCol==m.getCols()))
  {
    Matrix2Dlc tmp(nbLine, nbCol);
    ValueType* valueM = m.getMem();
    ValueType* valueTmp = tmp.getMem();
    int linesM = m.getLines();  

    for (int i=0; i<nbLine; i++)
    {
      int theLine = i*nbCol;
      ValueType* lineTmp = valueTmp + theLine; 
      ValueType* lineThis = value + theLine;
      for (int j=0; j<nbCol; j++) lineTmp[j] = lineThis[j] + valueM[j*linesM+i];
    }   
    return tmp;
  } else {printf("\nAddition of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Return a new Matrix (copy the data) which is the difference of *this and m
Matrix2Dlc Matrix2Dlc::operator-(const Matrix2Dlc& m) const
{    
  if ((nbLine==m.nbLine) && (nbCol==m.nbCol))
  {
    Matrix2Dlc tmp(nbLine, nbCol);
    int size = nbLine*nbCol;
    for (int i=0; i<size; i++) tmp.value[i] = value[i] - m.value[i];    
    return tmp;
  } else {printf("\nAddition of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Return a new Matrix (copy the data) which is the difference of *this and m
Matrix2Dlc Matrix2Dlc::operator-(const Matrix2Dcl& m) const
{
  if ((nbLine==m.getLines()) && (nbCol==m.getCols()))
  {
    Matrix2Dlc tmp(nbLine, nbCol);
    ValueType* valueM = m.getMem();
    ValueType* valueTmp = tmp.getMem();
    int linesM = m.getLines();  

    for (int i=0; i<nbLine; i++)
    {
      int theLine = i*nbCol;
      ValueType* lineTmp = valueTmp + theLine; 
      ValueType* lineThis = value + theLine;
      for (int j=0; j<nbCol; j++) lineTmp[j] = lineThis[j] - valueM[j*linesM+i];
    }   
    return tmp;
  } else {printf("\nAddition of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Return a new Matrix (copy the data) which is the multiplication of *this and m
//  Use the standard algorithm to multiply the matrices c = c + a*b
//  This function is optimised for memory accesses
Matrix2Dlc Matrix2Dlc::operator*(const Matrix2Dlc& m) const
{
  if (nbCol==m.getLines())
  {
    Matrix2Dlc tmp(nbLine, m.getCols());
    ValueType r=0;
    int c=tmp.nbCol;
    
    //efficient (optimized) implementation of: tmp[i,j] =  tmp[i,j] + this[i,k] * m[k,j]      
    for (int i=0; i<nbLine; i++)
       for (int j=0; j<c; j++)
       {
        int theLine = i*nbCol;
         for (int k=0; k<nbCol; k++) r += value[theLine+k] * m.value[k*nbLine+j];
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
//  This function is optimised for memory accesses
Matrix2Dlc Matrix2Dlc::operator*(const Matrix2Dcl& m) const
{
  if (nbCol==m.getLines())
  {
    Matrix2Dlc tmp(nbLine, m.getCols());  // Matrix to store the sum
    ValueType* dataM = m.getMem();        // Pointer on values of m
    ValueType* dataTmp = tmp.getMem();    // Pointer on values of tmp
    int c=tmp.nbCol;                      // Nb. col. of tmp
    int linesM= m.getLines();             // Nb. line of m
    ValueType* colM;        // Used to point on the correct col. of m
    ValueType* lineThis;    // Used to point on the correct line of this

    ValueType r=0;  // To accumulate the sum
    int theLine;    // The index of the correct line in this
    
    //efficient (optimized) implementation of: tmp[i,j] =  tmp[i,j] + this[i,k] * m[k,j]
    for (int i=0; i<nbLine; i++)
    {
       theLine = i*nbCol;
       lineThis = value + theLine;
       for (int j=0; j<c; j++)
       {
         colM = dataM + linesM*j;
         for (int k=0; k<nbCol; k++)  r += lineThis[k] * colM[k];  
         dataTmp[i*c+j] = r; //tmp.set(i,j,r);
         r=0;
       }
    }
    //tmp.shared=NULL;
    return tmp;
  }
  else {printf("\nMultiplication of incompatible Matrices\n"); exit(0);}
}

// -- ----------------------------------------------------------------------------
Matrix2Dlc Matrix2Dlc::strassen(const Matrix2Dcl& b, int sizeMin)
{
  if (nbCol!=b.getLines())
  {
    printf("Strassen multiplication of incompatible matrices\n");
    this->showState("a:\n", false); b.showState("a:\n", false);
    exit(0);
  }

  Matrix2Dlc c(nbLine, b.getCols());
  
  // When the one of the matrix sizes is less than minSize, uses standard algorithm
  if ((nbLine<=sizeMin) || (nbCol<=sizeMin) || (b.getCols()<=sizeMin))
    {c = *this * b;}
  // other cases use STRASSEN algorithm
  else
  { 
    int size, aSizeL, aSizeC, bSizeL, bSizeC;

    size = nbLine;
    if ((size % 2)==0) aSizeL = size/2; else aSizeL = size/2+1;
    size = nbCol;
    if ((size % 2)==0) aSizeC = size/2; else aSizeC = size/2+1;
    size = b.getLines();
    if ((size % 2)==0) bSizeL = size/2; else bSizeL = size/2+1;
    size = b.getCols();
    if ((size % 2)==0) bSizeC = size/2; else bSizeC = size/2+1;
 
    Matrix2Dlc a11(aSizeL,aSizeC), a12(aSizeL,aSizeC),
               a21(aSizeL,aSizeC), a22(aSizeL,aSizeC), ar(aSizeL,aSizeC);
    Matrix2Dcl b11(bSizeL,bSizeC), b12(bSizeL,bSizeC),
               b21(bSizeL,bSizeC), b22(bSizeL,bSizeC), br(bSizeL,bSizeC);
                            
    a11.clear(); a12.clear(); a21.clear(); a22.clear();                         
    b11.clear(); b12.clear(); b21.clear(); b22.clear();                         

    /*Matrix2Dlc p1(aSizeL, bSizeC), p2(aSizeL, bSizeC), p3(aSizeL, bSizeC),
               p4(aSizeL, bSizeC), p5(aSizeL, bSizeC), p6(aSizeL, bSizeC),
               p7(aSizeL, bSizeC);*/
               
    a11.setBloc(0,0,getBloc(0,0,aSizeL,aSizeC));
    a12.setBloc(0,0,getBloc(0,aSizeC,aSizeL,getCols()-aSizeC));
    a21.setBloc(0,0,getBloc(aSizeL,0, nbLine-aSizeL, aSizeC));
    a22.setBloc(0,0,getBloc(aSizeL,aSizeC,nbLine-aSizeL,getCols()-aSizeC));
    
    b11.setBloc(0,0,b.getBloc(0,0,bSizeL,bSizeC));
    b12.setBloc(0,0,b.getBloc(0,bSizeC,bSizeL,b.getCols()-bSizeC));
    b21.setBloc(0,0,b.getBloc(bSizeL,0, b.getLines()-bSizeL, bSizeC));
    b22.setBloc(0,0,b.getBloc(bSizeL,bSizeC,b.getLines()-bSizeL,b.getCols()-bSizeC));
 
    
    //ar.equ(a11+a22); br.equ(b11+b22);
    //p1.equ(ar.strassen(br, sizeMin));                // p1 = (a11+a22) * (b11+b22)
    //p1.equ((a11+a22).strassen((b11+b22), sizeMin));  // p1 = (a11+a22) * (b11+b22)
    Matrix2Dlc p1((a11+a22).strassen(b11+b22, sizeMin));
    //p1.showState("p1 :\n", true);
    
    //ar.equ(a21+a22);
    //p2.equ(ar.strassen(b11, sizeMin));         // p2 = (a21+a22) * (b11)
    Matrix2Dlc p2((a21+a22).strassen(b11,sizeMin));
    //p2.showState("p2 :\n", true);
   
    //br.equ(b12-b22);
    //p3.equ(a11.strassen(br, sizeMin));         // p3 = (a11) * (b12 - b22)   
    Matrix2Dlc p3(a11.strassen((b12-b22),sizeMin));
    //p3.showState("p3 :\n", true);
   
    //br.equ(b21-b11);
    //p4.equ(a22.strassen(br, sizeMin));         // p4 = (a22) * (b21 - b11)
    Matrix2Dlc p4(a22.strassen((b21-b11),sizeMin));
    //p4.showState("p4 :\n", true);

    //ar.equ(a11+a12);
    //p5=(ar.strassen(b22, sizeMin));            // p5 = (a11+a12) * (b22)	
    Matrix2Dlc p5((a11+a12).strassen(b22,sizeMin));
    //p5.showState("p5 :\n", true);
   
    //ar.equ(a21-a11); br.equ(b11+b12);
    //p6.equ(ar.strassen(br, sizeMin));          // p6 = (a21-a11) * (b11+b12)
    Matrix2Dlc p6((a21-a11).strassen((b11+b12),sizeMin));
    //p6.showState("p6 :\n", true);
   
    //ar.equ(a12-a22); br.equ(b21+b22);
    //p7.equ(ar.strassen(br, sizeMin));          // p7 = (a12-a22) * (b21+b22)
    Matrix2Dlc p7((a12-a22).strassen((b21+b22),sizeMin));
    //p7.showState("p7 :\n", true);

    // Calculating c11, c12, c211, c22
    // and grouping the results obtained in a single matrix c
    c.setBloc(0,0,(p7+p1+p4-p5).getBloc(0,0,aSizeL, bSizeC));              //c11
    c.setBloc(0,bSizeC,(p5+p3).getBloc(0,0,aSizeL,c.getCols()-bSizeC));    //c12 
    c.setBloc(aSizeL,0,(p4+p2).getBloc(0,0,c.getLines()-aSizeL,bSizeC));   //c21
    c.setBloc(aSizeL, bSizeC,
       (p1+p3+p6-p2).getBloc(0,0,c.getLines()-aSizeL,c.getCols()-bSizeC)); //c22 
       //c.showState("c:\n", true); 
  } // end of else
  return c;
} // end of Strassen function

//-- -----------------------------------------------------------------------------
//  Internal addition: *this becomes the sum of *this and m
void Matrix2Dlc::add(const Matrix2Dlc& m)
{
  if ((nbLine==m.nbLine) && (nbCol==m.nbCol))
    for (int i=0; i<dataSize; i++) value[i] = value[i] + m.value[i];
  else {printf("\nInternal addition of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Internal addition: *this becomes the sum of *this and m
void Matrix2Dlc::add(const Matrix2Dcl& m)
{
  if ((nbLine==m.getLines()) && (nbCol==m.getCols()))
  {
    for (int i=0; i<nbLine; i++)
      for (int j=0; j<nbCol; j++) set(i,j, get(i,j) + m.get(i,j));
  } else {printf("\nAddition of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Internal subtraction: *this become the difference of *this and m
void Matrix2Dlc::sub(const Matrix2Dlc& m)
{
  if ((nbLine==m.nbLine) && (nbCol==m.nbCol))
    for (int i=0; i<dataSize; i++) value[i] = value[i] - m.value[i];
  else {printf("\nInternal substraction of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Internal subtraction: *this becomes the difference of *this and m
void Matrix2Dlc::sub(const Matrix2Dcl& m)
{
  if ((nbLine==m.getLines()) && (nbCol==m.getCols()))
  {
    for (int i=0; i<nbLine; i++)
      for (int j=0; j<nbCol; j++) set(i,j, get(i,j) - m.get(i,j));
  } else {printf("\nInternal substraction of incompatible Matrices\n"); exit(0);}
}

//-- -----------------------------------------------------------------------------
//  Return a matrix which is a bloc of lines of *this (no copy, values are shared with *this) 
Matrix2Dlc Matrix2Dlc::getLinesBloc(int noLine, int nbLines) const
{
  if ( (value!=NULL) && (nbLine>=(noLine+nbLines)) )
  {
    Matrix2Dlc tmp;
    tmp.nbCol = nbCol;
    tmp.nbLine = nbLines;
    tmp.dataSize = dataSize;
    tmp.value = &(value[noLine*nbCol]);
	  if(shared==NULL) tmp.shared = value; else tmp.shared=shared;
    if (tmp.shared!=NULL) (tmp.shared[dataSize])= (tmp.shared[dataSize])+(ValueType)1;
    return tmp;
  }
  else
  {
    printf("\nMatrix ERROR: Non coherent Line block setting (%d+%d>%d)!!!\n",
                                                        noLine,nbLines,nbLine);
    //showState("Source matrix:", false);
    printf("PROGRAM ABORTED !!!\n");
    exit(0);
   }
}

//-- -----------------------------------------------------------------------------
//  Modify the values of a  bloc of lines of *this using values in 'v' 
void Matrix2Dlc::setLinesBloc(int noLine, Matrix2Dlc v)
{
  if ((nbCol==v.nbCol) && (nbLine>=(noLine+v.nbLine)) )
  {
    if (value==NULL)
    {
      dataSize = nbLine*nbCol;
      value = (ValueType*)malloc((dataSize+1)*ValueTypeSize);
	  if (value==NULL) {printf("\nMEMORY OVERFLOW !!!!\n"); exit(0);}   
      value[dataSize]=0;
      shared = NULL;
    }
    memcpy(&(value[noLine*nbCol]), v.value, v.nbCol*v.nbLine*ValueTypeSize);
     // memcpy replaces the following for loop
     // for (int i=0; i<v.nbCol*v.nbLine; i++) value[noLine*nbCol+i]=v.value[i];
  }
  else
  {
    printf("\nMatrix ERROR: Non coherent lines block setting at line %d!!!\n", noLine);
    v.showState("Source Matrix:\n", false);
    showState("Destination Matrix:\n", false);
    printf("PROGRAM ABORTED !!!\n");
    exit(0);
  }
}

//-- -----------------------------------------------------------------------------
//  Modify the values of a bloc *this at [noline,noCal] using values in 'v' 
void Matrix2Dlc::setBloc(int noLine, int noCol, Matrix2Dlc v)
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
    for (int i=0; i<v.nbLine; i++)
      memcpy(&(value[(noLine+i)*nbCol+noCol]),&(v.value[i*v.nbCol]), v.nbCol*ValueTypeSize);
     // memcpy replaces the following for loop
     //for (int j=0; j<v.nbCol; j++)
     //  value[(noLine+i)*nbCol+noCol+j]=v.value[i*v.nbCol+j];
  }
  else 
  {
    printf("\nMatrix ERROR: Non coherent bloc[%dx%d] setting at %d, %d !!!\n",
                 v.nbLine, v.nbCol, noLine, noCol );
    v.showState("Source Matrix:\n", false);
    showState("Destination Matrix:\n", false);
    printf("PROGRAM ABORTED !!!\n");
    exit(0);
  }
}

//-- -----------------------------------------------------------------------------
//  Return a new matrix (copy the data) which is the bloc of *this at [noLine,noCol]
Matrix2Dlc Matrix2Dlc::getBloc(int noLine, int noCol, int nbLines, int nbCols) const
{
  if ((value!=NULL) && (nbCol>=(noCol+nbCols)) && (nbLine>=(noLine+nbLines)))
  {
    Matrix2Dlc tmp(nbLines, nbCols);
    for (int i=0; i<nbLines; i++)
      memcpy(&(tmp.value[i*nbCols]), &(value[(noLine+i)*nbCol+noCol]),
             nbCols*ValueTypeSize); 
      // memcpy(dest, src, nb) replaces the following for loop
      //for (int j=0; j<nbLine; j++)	    
      //  tmp.value[i*nbLine+j] = value[(noCol+i)*nbLine+noLine+j]; 
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

