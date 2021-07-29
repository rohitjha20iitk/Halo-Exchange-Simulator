echo "*****Running Assignment 1*****"          

#Generate the hostfile

make

echo "Generating Hostfile"
chmod +x hostfile.sh
./hostfile.sh

echo "hostfile generated"

for execution in 1 2 3 4 5
do
    echo $execution
    for P in 16 36 49 64
    do 
	echo $P    
        for N in 256 1024 4096 16384 65536 262144 1048576 
        do
	   echo $N
           mpirun -np $P -f hostfile ./halo $N 50 >> data_$P 
        done
    done
done

python3 plot.py

echo "*****Completed Assignment 1*****"
