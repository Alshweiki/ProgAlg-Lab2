#!/bin/bash

# --- Indicate your parameters below ---

# The full PATH of your working directory
DIR=/etuhome/ggroupx/.......
cd $DIR
pwd

# Number of time of each execution (min 5)
TIMES=5

# The 5 matrix sizes
S1=....
S2=....
S3=...
S4=..
S5=..

# The values of L and C
L1=..
C1=..

L2=..
C2=..

L3=..
C3=..

L4=..
C4=..

L5=..
C5=..

# Names of the executable file and of the result file
EXEC=.........
RESULTS=.......

# --- One core or multi cores execution ---
# Choose one of the two possibilities below

# 1 - Parallel executions using machines with 4 cores 
cp machines_cores.txt machines.txt
echo "[`date`] Start the parallel executions on 4 cores"

#  2- Parallel executions using machines with 1 core
cp machines_nocore.txt machines.txt
echo "[`date`] Start the parallel executions on single core"

# --- End of parameters ---

# ----POP-C++ and POP-Java environment variables ----
#POP-C++ V2.5 -- Use V2.5
export POPC_LOCATION=/opt/popc
echo "Using POP-C++ version 2.5"

export POPJAVA_LOCATION=/usr/local/popj
export POPJAVA_JAVA=/usr/bin/java

export PATH=$PATH:$POPC_LOCATION/bin:$POPC_LOCATION/sbin
export PATH=$PATH:$POPJAVA_LOCATION/bin:/common/mpi/bin

echo "[`date`] Starting execution"

for m in $S1 $S2 $S3 $S4 $S5
do
  for ((a=1; a<=TIMES; a++))
  do
  ......
  done
....
done

echo "[`date`] End of executions"
