POPCC=/opt/popc/bin/popcc
POPCRUN=/opt/popc/bin/popcrun

all:
	$(POPCC) -O3 -o mainpopc ../Matrix2D.cc ../Matrix2Dlc.cc ../Matrix2Dcl.cc MatWorker.ph MatWorker.cc mainpopc.cc

	$(POPCC) -object -O3 -o MatWorker.obj ../Matrix2D.cc ../Matrix2Dlc.cc ../Matrix2Dcl.cc MatWorker.ph MatWorker.cc
	./MatWorker.obj -listlong > obj.map

clean:
	rm -f *.obj
	rm -f *.o
	rm -f mainpopc
	rm -f ../*.o 
	rm -f obj.map

run: mainpopc  
	$(POPCRUN) obj.map ./mainpopc 1000 2 2
