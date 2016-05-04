// -------------------------------------------------------------------------
// Parallel Matrix multiplication by blocks with POP-C++ using standard algo.
// Authors: Pierre Kuonen
// Creation: 11.12.2009
// Modifications: PK: March 2011, for Labo 2b course UniFR 2011
//                PK: April 2011, to allow divLine and divCol NOT to be divisors
//                PK: Octobre 2012, Modify wrong time measures
//                PK: June 2013 add 'struct machine' to handle cores
//                PK: July 2013 Add multi cores, GPU version
//                PK: July 2013 add #define MatWorker for various versions
//                PK: October 2013 modify to only use the core version (UniNE)
//                PK: April 2014 modify to adapt to MSE Zurich
//                PK: May 2014 Cosmetics
//                PK: February 2016 Cosmetic and adaptation to usage of nopopc.h
//                PK: February 2016 make compatible with POP-V++ V3.0
//                PK: Change the file name "machines.txt"
// -------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../timer.h"
#include "../Matrix2Dlc.h"
#include "../Matrix2Dcl.h"
#include "MatWorker.ph"

#define nbMaxMachines 200
#define MachinesList "machines.txt"

typedef struct machine
{
   POPString* name;
   int cores;
} typeMachine;

// -------------------------------------------------------------
// Read  the file 'fileName' containing the list of machines,
// fill the array 'machines' and returns the number of machines
int getAvailableMachines(char* fileName, typeMachine machine[])
{
  int nbOfMachines = 0;
  FILE* f;
  if ( (f = fopen(fileName, "r"))!=NULL)
  {
    char* s;
    int c;
    while (!feof(f))
    {
      s=(char*)malloc(100);
      fscanf(f, "%s", s);
      fscanf(f, "%d\n", &c);
      if (strlen(s)>0)
      {
        machine[nbOfMachines].name = new POPString(s);
        machine[nbOfMachines].cores = c;
	    nbOfMachines++;
      }
    }
    fclose(f);
  } else
  {
    nbOfMachines=1;
    machine[0].name = new POPString("localhost");
    machine[0].cores = 1;
  }
  return nbOfMachines;
}

//------------------------------
int main(int argc, char** argv)
{
  if (argc<4)
  {
    printf("\nUsage: popcrun objmap %s size divLine divCol [resultFileName]\n\n",
            argv[0]);
    return 0;
  }
  
  int Alines ;          //argv[1];
  int Acols;            //argv[1];
  int Bcols;            //argv[1];
  int divLine;          //argv[2];
  int divCol;           //argv[3];
  char* resultFileName; //argv[4];
  
 // Get the execution parameters
  Alines   = Acols = Bcols = atoi(argv[1]);
  divLine  = atoi(argv[2]);
  divCol   = atoi(argv[3]);

  if (argc>4) resultFileName=argv[4]; else resultFileName=NULL; 
  
  int nbWorker = divLine*divCol;

 // Get the available machines
  typeMachine machine[nbMaxMachines];  
  int nbOfMachines = getAvailableMachines(MachinesList, machine);

  printf("\nPOP-C++: mainpopc has started with %d tasks.\n",nbWorker);
  printf("Initializing arrays...\n");
  printf("\nParameters are:\n");
  printf("Matrix Size=%d, Blocs (lines=%d, columns=%d), Workers=%d, Cores=%d\n",
                            Alines, divLine, divCol, nbWorker, machine[0].cores);

 // Matrix A and B declaration and initialisation
  Matrix2Dlc a(Alines,Acols);
  Matrix2Dcl b(Acols,Bcols);
    
 // Randomly initialize Matrix a and b and display them
  a.init();
  b.init();
  
  Timer timer;
  double initTime, compTime, sendTime;

  printf("\nPOP-C++: Starting standard Matrix multiplication program...\n\n");
  timer.Start(); //---------------------------------------------- Start Timer  

  try
  {
    //Declaration of pointers on workers 
    MatWorker* mw[divLine][divCol];

    // Create the workers
    for (int i=0; i<divLine; i++)
      for (int j=0; j<divCol; j++)
        // Create workers
        mw[i][j] = new MatWorker(j+i*divCol,
                                 *(machine[(j+i*divCol)%nbOfMachines].name),
                                 machine[(j+i*divCol)%nbOfMachines].cores);

   // Get time to create all workers
    initTime = timer.Elapsed();  // ---------------------- Initialisation Time


   //send the bloc of Matrix A and the Matrix B and launch the computation

    int lines, cols;               // Actual number of lin/col in a bloc
    int averline = Alines/divLine; // Average number of lines in a bloc
    int avercol = Bcols/divCol;    // Average number of col. in a bloc
    int extraL, extraC;            // Remaining lines or column
    int offsetL, offsetC;          // offset (for lines abd col.) of the bloc to send
    
    offsetL = 0;
    extraL = Alines%divLine;
    for (int i=0; i<divLine; i++)
    {
      offsetC = 0;
      extraC = Bcols%divCol;
      if (extraL>0) {lines=averline+1; extraL--;} else lines = averline;
      for (int j=0; j<divCol; j++)
      {
        if (extraC>0) {cols = avercol+1;  extraC--;} else cols  = avercol;
        // Send data and launch computation
        mw[i][j]->solve(a.getLinesBloc(offsetL,lines), b.getColsBloc(offsetC, cols));
        offsetC = offsetC + cols;
      }
      offsetL = offsetL + lines;
   }  
   // Get time to send all data to workers and launch computation
    sendTime = timer.Elapsed() - initTime;  // ------------------ Sending Time
   
   //Create a matrix for getting the results
    Matrix2Dlc res;
    Matrix2Dlc c(Alines,Acols);

   // Declare vector to store wait time of workers
    double workerTw[divLine][divCol];
    
   // Declare vector to store computing time of workers
    double workerTc[divLine][divCol];
    
   //Get the results and put inside matrix c
    offsetL=0;
    for (int i=0; i<divLine; i++)
    {
      offsetC=0;
      for (int j=0; j<divCol; j++)
      {
        res=mw[i][j]->getResult(workerTw[i][j], workerTc[i][j]);
        c.setBloc(offsetL, offsetC, res);
        offsetC = offsetC + res.getCols();
      }
      offsetL = offsetL + res.getLines();
    }

   // Get the elapsed time since all data have been sent (computing time)
    compTime = timer.Elapsed() - sendTime - initTime;  // --- Computing Time
    timer.Stop();

    printf("Times (init, send and computing) = %.3g, %.3g, %.3g sec\n\n",
            initTime, sendTime, compTime);
    printf("...End of matrix multiplication\n\n"); 
    printf("A[%dx%d]:\n",a.getLines(),a.getCols()); a.display(8); 
    printf("B[%dx%d]:\n",b.getLines(),b.getCols()); b.display(8);
    printf("C[%dx%d]=A*B:\n",c.getLines(),c.getCols()); c.display(8);

   // Storage of Results and Parametres in the file resultFileName
    if (resultFileName!=NULL)
    {
      FILE* f = fopen(resultFileName, "a");
      if (f!=NULL)
      {
        fprintf(f, "%d\t%d\t%d\t%g\t%g\t%g",
                Alines, divLine, divCol, initTime, sendTime, compTime); 
        for (int i=0; i<divLine; i++)
          for (int j=0; j<divCol; j++)
            fprintf(f, "\t%d\t%g\t%g",
                  machine[(j+i*divCol)%nbOfMachines].cores,workerTw[i][j],workerTc[i][j]);
        fprintf(f,"\n");
        fclose(f);
      } else
        printf("\nERROR OPENING result file - no results has been saved !!\n"); 
    }

   // Delete the workers
    for (int i=0; i<divLine; i++)
      for (int j=0; j<divCol; j++)
        if (mw[i][j]!=NULL) delete mw[i][j];
  } // end try
  catch (POPException *e)
  {
    printf("POPException %d raised !\n", e->Code());
    return 0;
  }
  return 0;
}
