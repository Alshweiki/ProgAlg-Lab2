#ifndef _MATWORKER_PH
#define _MATWORKER_PH

#include "../Matrix2Dlc.h"
#include "../Matrix2Dcl.h"

parclass MatWorker
{
  classuid(1500);

  public:
    MatWorker();
    MatWorker(int i, POPString machine, int NbC=4)@{od.url(machine);};
    ~MatWorker();
    
    //  Compute a*b using NbCores (NbCores is an attribut of *this)
    async seq void solve(Matrix2Dlc a, Matrix2Dcl b);
    
     // Do the computation for the core st
    async conc void solveCore(int st);                            
 
    // Return results
    sync  seq Matrix2Dlc getResult(double &tw, double &tc);
    
  private:
  

    Matrix2Dlc* c;                    // the result matrix
    int nbLinesA, nbColsA, nbColsB;   // size of the matrices to multiply
    int id;                           // my ID.
    Matrix2Dlc* aMat;                 // the a matrix
    Matrix2Dcl* bMat;                 // the b matrix
    POPSynchronizer barrier, cntLock; // for synchronisation between cores
    int NbCores, counter;             // Number of cores, internal counter
    POPString myMachine;              // the name of my machine

    Timer timer;                       // to measure times
    double computeTime, waitTime;
};
#endif
