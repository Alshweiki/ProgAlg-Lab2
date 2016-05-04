#! /bin/bash

export POPC_LOCATION=/opt/popc/
export PATH=${PATH}:$POPC_LOCATION/bin:$POPC_LOCATION/sbin

$DATE=date

echo $DATE
echo $PATH

cd /etuhome/ggroup20/project/POPC/MATRIX/
pwd


# TODO
#|w  |= L x C| size=N|
#|:--|-------|------:|
#|2  |= 1 x 2|  100 |
#|4  |= 2 x 2|  200 |
#|6  |= 2 x 3|  3240 |
#|9  |= 3 x 3|  4620 |
#|10 |= 5 x 2|  6240 |

# Cleanup an rebuild everything
#make clean && make all;

# Copy the machine name in machines.txt
cat ./machines_cores.txt > ./machines.txt

# Lets gets started
touch ./output.log
OUT=./output.log

echo $DATE >> $OUT;

for size  in  200 100 
do 
	for i in {1..2}
	do
		case  $size in
			100 )
				popcrun ./obj.map ./mainpopc $size 1 2 $OUT;;
			200 )
				popcrun ./obj.map ./mainpopc $size 2 2 $OUT;;
		esac			
	done
done 

# Cleanup
#rm -f machines.txt
