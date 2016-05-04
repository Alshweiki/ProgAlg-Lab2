#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "Matrix2D.h"
ValueType Matrix2D::getInternal(int line, int col) const {return ValueType(0);}
//-- -----------------------------------------------------------------------------
Matrix2D::Matrix2D()
{
  nbLine=0; nbCol=0;
  dataSize =0; value = NULL;
  shared=NULL;
}

//-- -----------------------------------------------------------------------------
Matrix2D::Matrix2D(int line, int col)
{
  nbLine=line; nbCol=col;
  dataSize =nbLine*nbCol;
  value=(ValueType*)malloc((dataSize+1)*ValueTypeSize);
  if (value==NULL) {printf("\nMEMORY OVERFLOW !!!!\n"); exit(0);}
  value[dataSize]=0;
  shared = NULL;
}

//-- -----------------------------------------------------------------------------
Matrix2D::Matrix2D(const Matrix2D &m)
{
  nbLine = m.nbLine;
  nbCol = m.nbCol;
  dataSize = m.dataSize;
  value = m.value;
  if(m.shared==NULL) shared = value; else shared = m.shared;
  if (shared!=NULL) shared[dataSize] = shared[dataSize] + (ValueType)1;
}

//-- -----------------------------------------------------------------------------
Matrix2D::~Matrix2D()
{
 Free();
 nbLine=0; nbCol=0;

}

//-- -----------------------------------------------------------------------------
//  Release space used by data only when it is not anymore used by any matrix
void Matrix2D::Free()
{
 if (value!=NULL)  // if there are data
  {
    if (shared!=NULL) // if the data have been shared
    {
     if  (shared[dataSize]<0.9) free(shared);                // counter < 1 release data
     else shared[dataSize] = shared[dataSize]-(ValueType)1;  // ELSE dec. counter
    }
    else
    {
     if (value[dataSize]<0.9) free(value);                   // counter < 1 release data
     else value[dataSize] = value[dataSize]-(ValueType)1;    // ELSE dec. counter
    }
    value=NULL; shared=NULL; dataSize=0; 
  } 
}

//-- -----------------------------------------------------------------------------
//  Initialise the matrix with random values [0 -> 199/7]
void Matrix2D::init()
{
  srand(time(NULL));
  if (value!=NULL)
    for (int i=0; i<nbCol*nbLine; i++)
      value[i] = ((ValueType)(rand()%200))/7;
}

//-- -----------------------------------------------------------------------------
//  Initialise the matrix with from file values in a file (not yet implemented)
void Matrix2D::init(char* filename) 
{ // !!! Still need to be written, currently fills with 0
  if (value!=NULL)
    for (int i=0; i<nbCol*nbLine; i++)
      value[i]=(ValueType)0;
}

//-- -----------------------------------------------------------------------------
//  Initialise the matrix with random values v
void Matrix2D::init(ValueType v)
{
  if (value!=NULL)
    for (int i=0; i<nbCol*nbLine; i++)
      value[i]=(ValueType)v;
}

//-- -----------------------------------------------------------------------------
//  Initialise the matrix with random values v
void Matrix2D::clear()
{
  memset(value,0,nbCol*nbLine*ValueTypeSize);
}

//-- -----------------------------------------------------------------------------
//  Make the matrix an empty matrix with no data
//  and, if possible (not shared), Free memory space used by data
inline void Matrix2D::null() 
{
  Free();
  nbLine=0; nbCol=0;
}
 
//-- -----------------------------------------------------------------------------
inline int Matrix2D::getLines() const
{ return nbLine;}

//-- -----------------------------------------------------------------------------
inline int Matrix2D::getCols() const
{ return nbCol;}


//-- -----------------------------------------------------------------------------
bool Matrix2D::operator==(const Matrix2D& m) const
{
  return ( (nbLine==m.nbLine) &&
           (nbCol==m.nbCol) && 
           (dataSize==m.dataSize) &&
           (shared==m.shared) &&
           (value==m.value) );
}

//-- -----------------------------------------------------------------------------
Matrix2D& Matrix2D::operator=(const Matrix2D& m)
{
  if (m.value != value)
  {
    Free();     // free the currents matrix values
    nbLine = m.nbLine;
    nbCol = m.nbCol;
    dataSize = nbLine*nbCol;//m.dataSize;
    value = (ValueType*)malloc((dataSize+1)*ValueTypeSize);
    if (value==NULL) {printf("\nMEMORY OVERFLOW !!!!\n"); exit(0);}
    value[dataSize]=0;
    memcpy (value, m.value, ValueTypeSize*dataSize);
    // memcpy replaces the following for loop
    // for (int i=0; i<dataSize; i++) value[i]=m.value[i];
  } 
  return *this;
}

//-- -----------------------------------------------------------------------------
void Matrix2D::equ(const Matrix2D& m)
{
  if (!(*this==m))
  {
    Free();     // free the currents matrix values
    nbLine = m.getLines();
    nbCol = m.nbCol;
    dataSize = m.dataSize;
    value = m.value;
    if (m.shared==NULL) shared = value; else shared = m.shared;
    /*if (shared!=NULL)*/ shared[dataSize] = shared[dataSize] + (ValueType)1;
  }
}

//-- -----------------------------------------------------------------------------
// Display only the nxn first values of the matrix
void Matrix2D::display(int n) const
{
 int nl=n, nc=n;
 if (n>nbLine) nl=nbLine; if (n>nbCol) nc=nbCol;
 for (int i=0; i<nl; i++)
  {
    for (int j=0; j<nc; j++) printf("%6.1lf ", (double)getInternal(i,j));
    if (nc<nbCol-1) printf(".. %6.1lf",(double)getInternal(i, nbCol-1));
    printf("\n");
  }
  if (nl<nbLine-1)
  {
    printf("    ...  ...  ...\n");
    for (int i=0; i<n; i++)  printf("%6.1lf ", (double)getInternal(nbLine-1,i));
    if (n<nbCol-1)  printf(".. %6.1lf",(double)getInternal(nbLine-1,nbCol-1));
  }
  printf("\n-----------------------\n");
}

//-- -----------------------------------------------------------------------------
// Display all the matrix (all values)
void Matrix2D::display() const
{ display(dataSize); }

//-- -----------------------------------------------------------------------------
void Matrix2D::showState(const char* s, bool all) const
{
  printf("%s M%dx%d\tat: %p, data size=%d, ", s, nbLine, nbCol, this, dataSize);
  if (shared!=NULL) printf("counter=%d, ", (int)shared[dataSize]);
  else if (value!=NULL) printf("counter=%d, ", (int)value[dataSize]);
  printf("value=%p, shared=%p", value, shared);
  printf("\n");
  if (all) if (value!=NULL) display();
}

//-- -----------------------------------------------------------------------------
ValueType* Matrix2D::getMem() const
{ return value;}

//-- -----------------------------------------------------------------------------
ValueType Matrix2D::testMat()
{
  if (value!=NULL)
  {
    for (int i=0; i<nbLine; i++)
      for (int j=0; j<nbCol; j++)
        set(i,j,(ValueType)(i+1));
    return (ValueType)((nbCol*(nbCol+1)*(2*nbCol+1))/6);
  } else
  return (ValueType)-1;
}

//-- -----------------------------------------------------------------------------
void Matrix2D::Serialize(POPBuffer &buf, bool pack)
{
  if (pack)
  {
    int s;
    buf.Pack(&nbCol,1);
    buf.Pack(&nbLine,1);
    if (value==NULL) s = 0; else s = nbLine*nbCol;  
    buf.Pack(&s,1);
    if (s>0) buf.Pack(value, s);
  }
  else
  {
    Free();
    buf.UnPack(&nbCol,1);
    buf.UnPack(&nbLine,1);
    buf.UnPack(&dataSize,1);
    shared=NULL;
    if (dataSize>0)
    {
      value=(ValueType*)malloc((dataSize+1)*ValueTypeSize);
      if (value==NULL) {printf("\nMEMORY OVERFLOW !!!!\n"); exit(0);}
      buf.UnPack(value, dataSize);
      value[dataSize]=0;
    }
    else value = NULL;
  }
}
