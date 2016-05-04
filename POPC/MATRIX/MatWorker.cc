#include "MatWorker.ph"
#include <stdio.h>
#include <string.h>

//-- -----------------------------------------------------------------------------
MatWorker::MatWorker()
{
   nbLinesA = 0;
   nbColsA = 0;
   nbColsB = 0;
   c = NULL;
   NbCores=1;
}

//-- -----------------------------------------------------------------------------
MatWorker::MatWorker(int i, POPString machine, int NbC)
{
   NbCores=NbC;
   myMachine=machine;
   //printf("MatWorker created on host: %s\n", POPGetHost());
   timer.Start();
}

//-- -----------------------------------------------------------------------------
MatWorker::~MatWorker()
{ if (c != NULL) delete c;}

//-- -----------------------------------------------------------------------------
// Do the computation for one core
void MatWorker::solveCore(int st)
{
  ValueType r = 0; 
  for (int j=st; j<nbLinesA; j=j+NbCores)
    for (int k=0; k<nbColsB; k++)
    {
      for (int l=0; l<nbColsA; l++)
        r += aMat->get(j,l) * bMat->get(l,k);
      c->set(j,k,r);
      r = 0; 
    }
  cntLock.lock();
    counter--; if (counter==0) barrier.raise();
  cntLock.unlock(); 
}

//-- -----------------------------------------------------------------------------
//  Compute a*b using NbCores (NbCores is an attribut of *this)
void MatWorker::solve(Matrix2Dlc a, Matrix2Dcl b)
{
  waitTime = timer.Elapsed();
  nbLinesA = a.getLines();
  nbColsA  = a.getCols();
  nbColsB  = b.getCols();
  c = new Matrix2Dlc(nbLinesA, nbColsB);
  aMat=&a;
  bMat=&b;
  counter=NbCores;
  
  barrier.lock();
  //Launch NbCores "SolveCore" methods (Threads)
  for (int i=0; i<NbCores; i++) this->solveCore(i); 
  barrier.wait(); // Wait until all "SolveCore" methods are terminated
  barrier.unlock();  
  computeTime=timer.Elapsed() - waitTime; timer.Stop();
}

//-- -----------------------------------------------------------------------------
Matrix2Dlc MatWorker::getResult(double &tw, double &tc)
{
  tw = waitTime;
  tc = computeTime;
  return *c;
}

@pack(MatWorker);
