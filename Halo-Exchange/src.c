#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

double sTime, eTime, time, maxTime;
int max, p, x, n, tl, tr, bl, br;
double copy2D[1026][1026];


// Stencil Computation
void compute (double **arr2D){
    for(int i = 0; i < max; i++){
        for(int j = 0; j < max; j++){
            copy2D[i][j] = arr2D[i][j];
        }
    }
   
    for(int i = 1; i < max-1; i++){
	for(int j = 1; j < max-1; j++){
		if(copy2D[i - 1][j] == -1 && copy2D[i][j - 1] == -1){
         	    //top left
         	    arr2D[i][j] = (copy2D[i + 1][j] + copy2D[i][j + 1]) / 2;
	        }
		else if(copy2D[i + 1][j] == -1 && copy2D[i][j - 1] == -1){
         		//bottom left
         		arr2D[i][j] = (copy2D[i - 1][j] + copy2D[i][j + 1]) / 2;
         	}
               else if(copy2D[i - 1][j] == -1 && copy2D[i][j + 1] == -1){
         		//top right
         		arr2D[i][j] = (copy2D[i][j - 1] + copy2D[i + 1][j]) / 2;
         	}
         	else if(copy2D[i + 1][j] == -1 && copy2D[i][j + 1] == -1){
         		//bottom right
         		arr2D[i][j] = (copy2D[i - 1][j] + copy2D[i][j - 1]) / 2;
         	}
         	else if(copy2D[i - 1][j] == -1){
         		//top
         		arr2D[i][j] = (copy2D[i + 1][j] + copy2D[i][j - 1] + copy2D[i][j + 1]) / 3;
         	}
         	else if(copy2D[i + 1][j] == -1){
         		//bottom
         		arr2D[i][j] = (copy2D[i - 1][j] + copy2D[i][j - 1] + copy2D[i][j + 1]) / 3;
         	}
               else if(copy2D[i][j - 1] == -1){
         		//left
         		arr2D[i][j] = (copy2D[i - 1][j] + copy2D[i + 1][j] + copy2D[i][j + 1]) / 3;
         	}
         	else if(copy2D[i][j + 1] == -1){
         	    //right
         	    arr2D[i][j] = (copy2D[i - 1][j] + copy2D[i + 1][j] + copy2D[i][j - 1]) / 3;
         	}
         	else{
            	arr2D[i][j] = (copy2D[i - 1][j] + copy2D[i + 1][j] + copy2D[i][j - 1] + copy2D[i][j + 1]) / 4;
            }
        }
    }
}


// 1st problem
void part1 (double **arr2D, int myrank){
    MPI_Status status;
    sTime = MPI_Wtime();
    for(int t = 0; t < p; t++){
    	
    	// SEND
        if(myrank == tl){

        for(int i = 1; i < max - 1; i++)
		    MPI_Send (&arr2D[i][max - 2], 1, MPI_DOUBLE, tl + 1, myrank, MPI_COMM_WORLD);
        for(int j = 1; j < max - 1; j++)
            MPI_Send (&arr2D[max - 2][j], 1, MPI_DOUBLE, tl + x, myrank, MPI_COMM_WORLD);  
        }

        else if(myrank == tr){

        for(int i = 1; i < max - 1; i++)
            MPI_Send (&arr2D[i][1], 1, MPI_DOUBLE, tr - 1, myrank, MPI_COMM_WORLD);
        for(int j = 1; j < max - 1; j++)
            MPI_Send (&arr2D[max - 2][j], 1, MPI_DOUBLE, tr + x, myrank, MPI_COMM_WORLD);
        }

        else if(myrank == bl){

        for(int i = 1; i < max - 1; i++)
            MPI_Send (&arr2D[i][max - 2], 1, MPI_DOUBLE, bl + 1, myrank, MPI_COMM_WORLD);
        for(int j = 1; j < max - 1; j++)
            MPI_Send (&arr2D[1][j], 1, MPI_DOUBLE, bl - x, myrank, MPI_COMM_WORLD);
        }

        else if(myrank == br){

        for(int i = 1; i < max - 1; i++)
            MPI_Send (&arr2D[i][1], 1, MPI_DOUBLE, br - 1, myrank, MPI_COMM_WORLD);
        for(int j = 1; j < max - 1; j++)
            MPI_Send (&arr2D[1][j], 1, MPI_DOUBLE, br - x, myrank, MPI_COMM_WORLD);
        }

        else if(myrank > tl && myrank < tr){

        for(int i = 1; i < max - 1; i++)
            MPI_Send (&arr2D[i][1], 1, MPI_DOUBLE, myrank - 1, myrank, MPI_COMM_WORLD);
        for(int i = 1; i < max - 1; i++)
            MPI_Send (&arr2D[i][max - 2], 1, MPI_DOUBLE, myrank + 1, myrank, MPI_COMM_WORLD);
        for(int j = 1; j < max - 1; j++)
            MPI_Send (&arr2D[max - 2][j], 1, MPI_DOUBLE, myrank + x, myrank, MPI_COMM_WORLD);
		}

        else if(myrank > bl && myrank < br){

        for(int i = 1; i < max - 1; i++)
            MPI_Send (&arr2D[i][1], 1, MPI_DOUBLE, myrank - 1, myrank, MPI_COMM_WORLD);
        for(int i = 1; i < max - 1; i++)
            MPI_Send (&arr2D[i][max - 2], 1, MPI_DOUBLE, myrank + 1, myrank, MPI_COMM_WORLD);
        for(int j = 1; j < max - 1; j++)
            MPI_Send (&arr2D[1][j], 1, MPI_DOUBLE, myrank - x, myrank, MPI_COMM_WORLD);
        }

        else if(myrank % x == 0){

        for(int j = 1; j < max - 1; j++)
            MPI_Send (&arr2D[1][j], 1, MPI_DOUBLE, myrank - x, myrank, MPI_COMM_WORLD);
        for(int j = 1; j < max - 1; j++)
            MPI_Send (&arr2D[max - 2][j], 1, MPI_DOUBLE, myrank + x, myrank, MPI_COMM_WORLD);
        for(int i = 1; i < max - 1; i++)
            MPI_Send (&arr2D[i][max - 2], 1, MPI_DOUBLE, myrank + 1, myrank, MPI_COMM_WORLD);
		}

        else if((myrank + 1) % x == 0){

        for(int j = 1; j < max - 1; j++)
            MPI_Send (&arr2D[1][j], 1, MPI_DOUBLE, myrank - x, myrank, MPI_COMM_WORLD);
        for(int j = 1; j < max - 1; j++)
            MPI_Send (&arr2D[max - 2][j], 1, MPI_DOUBLE, myrank + x, myrank, MPI_COMM_WORLD);
        for(int i = 1; i < max - 1; i++)
            MPI_Send (&arr2D[i][1], 1, MPI_DOUBLE, myrank - 1, myrank, MPI_COMM_WORLD);
		}

        else{

        for(int j = 1; j < max - 1; j++)
            MPI_Send (&arr2D[1][j], 1, MPI_DOUBLE, myrank - x, myrank, MPI_COMM_WORLD);
        for(int j = 1; j < max - 1; j++)
            MPI_Send (&arr2D[max - 2][j], 1, MPI_DOUBLE, myrank + x, myrank, MPI_COMM_WORLD);
        for(int i = 1; i < max - 1; i++)
            MPI_Send (&arr2D[i][1], 1, MPI_DOUBLE, myrank - 1, myrank, MPI_COMM_WORLD);
        for(int i = 1; i < max - 1; i++)
            MPI_Send (&arr2D[i][max - 2], 1, MPI_DOUBLE, myrank + 1, myrank, MPI_COMM_WORLD);
	    }
     
        // RECEIVE
        if(myrank == tl){

        for(int i = 1; i < max - 1; i++)
            MPI_Recv (&arr2D[i][max - 1], 1, MPI_DOUBLE, tl + 1 , MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for(int j = 1; j < max - 1; j++)
            MPI_Recv (&arr2D[max - 1][j], 1, MPI_DOUBLE, tl + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        else if(myrank == tr){

        for(int i = 1; i < max - 1; i++)
            MPI_Recv (&arr2D[i][0], 1, MPI_DOUBLE, tr - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for(int j = 1; j < max - 1; j++)
            MPI_Recv (&arr2D[max - 1][j], 1, MPI_DOUBLE, tr + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        else if(myrank == bl){

        for(int i = 1; i < max - 1; i++)
            MPI_Recv (&arr2D[i][max - 1], 1, MPI_DOUBLE, bl + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for(int j = 1; j < max - 1; j++)
            MPI_Recv (&arr2D[0][j], 1, MPI_DOUBLE, bl - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        else if(myrank == br){

        for(int i = 1; i < max - 1; i++)
            MPI_Recv (&arr2D[i][0], 1, MPI_DOUBLE, br - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for(int j = 1; j < max - 1; j++)
            MPI_Recv (&arr2D[0][j], 1, MPI_DOUBLE, br - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        else if(myrank > tl && myrank < tr){

        for(int i = 1; i < max - 1; i++)
            MPI_Recv (&arr2D[i][0], 1, MPI_DOUBLE, myrank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for(int i = 1; i < max - 1; i++)
            MPI_Recv (&arr2D[i][max - 1], 1, MPI_DOUBLE, myrank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for(int j = 1; j < max - 1; j++)
            MPI_Recv (&arr2D[max - 1][j], 1, MPI_DOUBLE, myrank + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        else if(myrank > bl && myrank < br){

        for(int i = 1; i < max - 1; i++)
            MPI_Recv (&arr2D[i][0], 1, MPI_DOUBLE, myrank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for(int i = 1; i < max - 1; i++)
            MPI_Recv (&arr2D[i][max - 1], 1, MPI_DOUBLE, myrank + 1 , MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for(int j = 1; j < max - 1; j++)
            MPI_Recv (&arr2D[0][j], 1, MPI_DOUBLE, myrank - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        else if(myrank % x == 0){

        for(int j = 1; j < max - 1; j++)
            MPI_Recv (&arr2D[0][j], 1, MPI_DOUBLE, myrank - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for(int j = 1; j < max - 1; j++)
            MPI_Recv (&arr2D[max - 1][j], 1, MPI_DOUBLE, myrank + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for(int i = 1; i < max - 1; i++)
            MPI_Recv (&arr2D[i][max - 1], 1, MPI_DOUBLE, myrank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        else if((myrank + 1) % x == 0){

        for(int j = 1; j < max - 1; j++)
            MPI_Recv (&arr2D[0][j], 1, MPI_DOUBLE, myrank - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for(int j = 1; j < max - 1; j++)
            MPI_Recv (&arr2D[max - 1][j], 1, MPI_DOUBLE, myrank + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for(int i = 1; i < max - 1; i++)
            MPI_Recv (&arr2D[i][0], 1, MPI_DOUBLE, myrank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        else{

        for(int j = 1; j < max - 1; j++)
            MPI_Recv (&arr2D[0][j], 1, MPI_DOUBLE, myrank - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for(int j = 1; j < max - 1; j++)
            MPI_Recv (&arr2D[max - 1][j], 1, MPI_DOUBLE, myrank + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for(int i = 1; i < max - 1; i++)
            MPI_Recv (&arr2D[i][0], 1, MPI_DOUBLE, myrank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for(int i = 1; i < max - 1; i++)
            MPI_Recv (&arr2D[i][max - 1], 1, MPI_DOUBLE, myrank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }
    
        compute(arr2D);
    }
	      	   
    eTime = MPI_Wtime();
    time = eTime - sTime;

    // obtain max time
    MPI_Reduce (&time, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (!myrank) printf ("t1 = %lf\n", maxTime); 
}



// 2nd problem
void part2(double **arr2D, int myrank){
    MPI_Status status1;
    int size2 = n, count = n * 8;
    double srbuffer[size2], slbuffer[size2], subuffer[size2], sdbuffer[size2];
    int position = 0;
   
    sTime = MPI_Wtime();
   
    for(int t = 0; t < p; t++){
    	
    	// SEND
        if(myrank == tl){
     
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Pack (&arr2D[i][max - 2], 1, MPI_DOUBLE, srbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (srbuffer, position, MPI_PACKED, tl + 1, myrank, MPI_COMM_WORLD);
        
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Pack (&arr2D[max - 2][j], 1, MPI_DOUBLE, sdbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (sdbuffer, position, MPI_PACKED, tl + x, myrank, MPI_COMM_WORLD);
        }
       
        else if(myrank == tr){
     
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Pack (&arr2D[i][1], 1, MPI_DOUBLE, slbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (slbuffer, position, MPI_PACKED, tr - 1, myrank, MPI_COMM_WORLD);
        
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Pack (&arr2D[max - 2][j], 1, MPI_DOUBLE, sdbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (sdbuffer, position, MPI_PACKED, tr + x, myrank, MPI_COMM_WORLD);
	}
     
        else if(myrank == bl){
     
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Pack (&arr2D[i][max - 2], 1, MPI_DOUBLE, srbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (srbuffer, position, MPI_PACKED, bl + 1, myrank, MPI_COMM_WORLD);
        
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Pack (&arr2D[1][j], 1, MPI_DOUBLE, subuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (subuffer, position, MPI_PACKED, bl - x, myrank, MPI_COMM_WORLD); 
        }
     
        else if(myrank == br){
     
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Pack (&arr2D[i][1], 1, MPI_DOUBLE, slbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (slbuffer, position, MPI_PACKED, br - 1, myrank, MPI_COMM_WORLD);
        
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Pack (&arr2D[1][j], 1, MPI_DOUBLE, subuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (subuffer, position, MPI_PACKED, br - x, myrank, MPI_COMM_WORLD);
        }
     
        else if(myrank > tl && myrank < tr){
        	
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Pack (&arr2D[i][1], 1, MPI_DOUBLE, slbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (slbuffer, position, MPI_PACKED, myrank - 1, myrank, MPI_COMM_WORLD);
        
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Pack (&arr2D[i][max - 2], 1, MPI_DOUBLE, srbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (srbuffer, position, MPI_PACKED, myrank + 1, myrank, MPI_COMM_WORLD);
        
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Pack (&arr2D[max - 2][j], 1, MPI_DOUBLE, sdbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (sdbuffer, position, MPI_PACKED, myrank + x, myrank, MPI_COMM_WORLD);  
        }
     
        else if(myrank > bl && myrank < br){
        
	position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Pack (&arr2D[i][1], 1, MPI_DOUBLE, slbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (slbuffer, position, MPI_PACKED, myrank - 1, myrank, MPI_COMM_WORLD);
        
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Pack (&arr2D[i][max - 2], 1, MPI_DOUBLE, srbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (srbuffer, position, MPI_PACKED, myrank + 1, myrank, MPI_COMM_WORLD);
        
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Pack (&arr2D[1][j], 1, MPI_DOUBLE, subuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (subuffer, position, MPI_PACKED, myrank - x, myrank, MPI_COMM_WORLD);
	}
     
        else if(myrank % x == 0){
        
	position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Pack (&arr2D[1][j], 1, MPI_DOUBLE, subuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (subuffer, position, MPI_PACKED, myrank - x, myrank, MPI_COMM_WORLD);
        
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Pack (&arr2D[max - 2][j], 1, MPI_DOUBLE, sdbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (sdbuffer, position, MPI_PACKED, myrank + x, myrank, MPI_COMM_WORLD);
        
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Pack (&arr2D[i][max - 2], 1, MPI_DOUBLE, srbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (srbuffer, position, MPI_PACKED, myrank + 1, myrank, MPI_COMM_WORLD); 
	}
     
        else if((myrank + 1) % x == 0){
        
	position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Pack (&arr2D[1][j], 1, MPI_DOUBLE, subuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (subuffer, position, MPI_PACKED, myrank - x, myrank, MPI_COMM_WORLD);
        
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Pack (&arr2D[max - 2][j], 1, MPI_DOUBLE, sdbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (sdbuffer, position, MPI_PACKED, myrank + x, myrank, MPI_COMM_WORLD);
        
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Pack (&arr2D[i][1], 1, MPI_DOUBLE, slbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (slbuffer, position, MPI_PACKED, myrank - 1, myrank, MPI_COMM_WORLD); 
        }
     
        else{
        
	position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Pack (&arr2D[1][j], 1, MPI_DOUBLE, subuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (subuffer, position, MPI_PACKED, myrank - x, myrank, MPI_COMM_WORLD);
        
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Pack (&arr2D[max - 2][j], 1, MPI_DOUBLE, sdbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (sdbuffer, position, MPI_PACKED, myrank + x, myrank, MPI_COMM_WORLD);
        
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Pack (&arr2D[i][1], 1, MPI_DOUBLE, slbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (slbuffer, position, MPI_PACKED, myrank - 1, myrank, MPI_COMM_WORLD);
        
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Pack (&arr2D[i][max - 2], 1, MPI_DOUBLE, srbuffer, size2 * 8, &position, MPI_COMM_WORLD);
        MPI_Send (srbuffer, position, MPI_PACKED, myrank + 1, myrank, MPI_COMM_WORLD);  
        }
     
        // RECEIVE
     
        if(myrank == tl){
     
        MPI_Recv (srbuffer, count, MPI_PACKED, tl + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Unpack (srbuffer, size2 * 8, &position, &arr2D[i][max - 1], 1, MPI_DOUBLE, MPI_COMM_WORLD );
           
        MPI_Recv (sdbuffer, count, MPI_PACKED, tl + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Unpack (sdbuffer, size2 * 8, &position, &arr2D[max - 1][j], 1, MPI_DOUBLE, MPI_COMM_WORLD );
	}
     
        else if(myrank == tr){
     
        MPI_Recv (slbuffer, count, MPI_PACKED, tr - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Unpack (slbuffer, size2 * 8, &position, &arr2D[i][0], 1, MPI_DOUBLE, MPI_COMM_WORLD );
           
        MPI_Recv (sdbuffer, count, MPI_PACKED, tr + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Unpack (sdbuffer, size2 * 8, &position, &arr2D[max - 1][j], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        }
     
        else if(myrank == bl){
        
        MPI_Recv (srbuffer, count, MPI_PACKED, bl + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Unpack (srbuffer, size2 * 8, &position, &arr2D[i][max - 1], 1, MPI_DOUBLE, MPI_COMM_WORLD );
       
       
        MPI_Recv (subuffer, count, MPI_PACKED, bl - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Unpack (subuffer, size2 * 8, &position, &arr2D[0][j], 1, MPI_DOUBLE, MPI_COMM_WORLD );
	}
     
        else if(myrank == br){
        
        MPI_Recv (slbuffer, count, MPI_PACKED, br - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Unpack (slbuffer, size2 * 8, &position, &arr2D[i][0], 1, MPI_DOUBLE, MPI_COMM_WORLD );
      
        
        
        MPI_Recv (subuffer, count, MPI_PACKED, br - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Unpack (subuffer, size2 * 8, &position, &arr2D[0][j], 1, MPI_DOUBLE, MPI_COMM_WORLD );   
        }
     
        else if(myrank > tl && myrank < tr){
     
        MPI_Recv (slbuffer, count, MPI_PACKED, myrank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Unpack (slbuffer, size2 * 8, &position, &arr2D[i][0], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        
       
        MPI_Recv (srbuffer, count, MPI_PACKED, myrank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Unpack (srbuffer, size2 * 8, &position, &arr2D[i][max - 1], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        
        
        MPI_Recv (sdbuffer, count, MPI_PACKED, myrank + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Unpack (sdbuffer, size2 * 8, &position, &arr2D[max - 1][j], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        }
     
        else if(myrank > bl && myrank < br){
        	
        MPI_Recv (slbuffer, count, MPI_PACKED, myrank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Unpack (slbuffer, size2 * 8, &position, &arr2D[i][0], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        
        
        MPI_Recv (srbuffer, count, MPI_PACKED, myrank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Unpack (srbuffer, size2 * 8, &position, &arr2D[i][max - 1], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        
        
        MPI_Recv (subuffer, count, MPI_PACKED, myrank - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int j = 1; j < max - 1; j++)
           MPI_Unpack (subuffer, size2 * 8, &position, &arr2D[0][j], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        }
     
        else if(myrank % x == 0){
        	
        MPI_Recv (subuffer, count, MPI_PACKED, myrank - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Unpack (subuffer, size2 * 8, &position, &arr2D[0][j], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        
        
        MPI_Recv (sdbuffer, count, MPI_PACKED, myrank + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Unpack (sdbuffer, size2 * 8, &position, &arr2D[max - 1][j], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        
       
        MPI_Recv (srbuffer, count, MPI_PACKED, myrank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Unpack (srbuffer, size2 * 8, &position, &arr2D[i][max - 1], 1, MPI_DOUBLE, MPI_COMM_WORLD );
	}
     
        else if((myrank + 1) % x == 0){
        	
        MPI_Recv (subuffer, count, MPI_PACKED, myrank - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Unpack (subuffer, size2 * 8, &position, &arr2D[0][j], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        
        
        MPI_Recv (sdbuffer, count, MPI_PACKED, myrank + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Unpack (sdbuffer, size2 * 8, &position, &arr2D[max - 1][j], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        
        
        MPI_Recv (slbuffer, count, MPI_PACKED, myrank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Unpack (slbuffer, size2 * 8, &position, &arr2D[i][0], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        }
     
        else{
        	
        MPI_Recv (subuffer, count, MPI_PACKED, myrank - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Unpack (subuffer, size2 * 8, &position, &arr2D[0][j], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        
       
        MPI_Recv (sdbuffer, count, MPI_PACKED, myrank + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int j = 1; j < max - 1; j++)
            MPI_Unpack (sdbuffer, size2 * 8, &position, &arr2D[max - 1][j], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        
        
        MPI_Recv (slbuffer, count, MPI_PACKED, myrank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Unpack (slbuffer, size2 * 8, &position, &arr2D[i][0], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        
       
        MPI_Recv (srbuffer, count, MPI_PACKED, myrank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status1);
        position = 0;
        for(int i = 1; i < max - 1; i++)
            MPI_Unpack (srbuffer, size2 * 8, &position, &arr2D[i][max - 1], 1, MPI_DOUBLE, MPI_COMM_WORLD );
        }
        
        compute(arr2D);
    }
    
    eTime = MPI_Wtime();
    time = eTime - sTime;

    // obtain max time
    MPI_Reduce (&time, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (!myrank) printf ("t2 = %lf\n", maxTime);
}



// 3rd problem
void part3(double **arr2D, int myrank){
    MPI_Status status3;
    MPI_Datatype newvtype1;  // For sending rows
    MPI_Datatype newvtype2;  // For sending columns
   
    int count1 = 1;
    int count2 = n;
    int blocklen1 = n;
    int blocklen2 = 1;
    int stride1 = 1;
    int stride2 = max;
   
    double rreceived[n], lreceived[n], ureceived[n], dreceived[n];
      
    MPI_Type_vector (count1, blocklen1, stride1, MPI_DOUBLE, &newvtype1);
    MPI_Type_commit (&newvtype1);
   
    MPI_Type_vector (count2, blocklen2, stride2, MPI_DOUBLE, &newvtype2);
    MPI_Type_commit (&newvtype2);
   
    sTime = MPI_Wtime();
    int k = 0;
    for(int t = 0; t < p; t++){
    	
    	// SEND
		if(myrank == tl){
		
		    MPI_Send(&arr2D[1][n], 1, newvtype2, tl + 1, myrank, MPI_COMM_WORLD);
            MPI_Send(&arr2D[n][1], 1, newvtype1, tl + x, myrank, MPI_COMM_WORLD); 
        }
     
        else if(myrank == tr){
     
            MPI_Send(&arr2D[1][1], 1, newvtype2, tr - 1, myrank, MPI_COMM_WORLD);
            MPI_Send(&arr2D[n][1], 1, newvtype1, tr + x, myrank, MPI_COMM_WORLD);
		}
     
        else if(myrank == bl){
     
            MPI_Send(&arr2D[1][n], 1, newvtype2, bl + 1, myrank, MPI_COMM_WORLD);
            MPI_Send(&arr2D[1][1], 1, newvtype1, bl - x, myrank, MPI_COMM_WORLD);
        }
     
        else if(myrank == br){
     
            MPI_Send(&arr2D[1][1], 1, newvtype2, br - 1, myrank, MPI_COMM_WORLD);
            MPI_Send(&arr2D[1][1], 1, newvtype1, br - x, myrank, MPI_COMM_WORLD);
        }
     
        else if(myrank > tl && myrank < tr){
     
            MPI_Send(&arr2D[1][1], 1, newvtype2, myrank - 1, myrank, MPI_COMM_WORLD);
            MPI_Send(&arr2D[1][n], 1, newvtype2, myrank + 1, myrank, MPI_COMM_WORLD);
            MPI_Send(&arr2D[n][1], 1, newvtype1, myrank + x, myrank, MPI_COMM_WORLD);
        }
     
        else if(myrank > bl && myrank < br){
     
            MPI_Send(&arr2D[1][1], 1, newvtype2, myrank - 1, myrank, MPI_COMM_WORLD);
            MPI_Send(&arr2D[1][n], 1, newvtype2, myrank + 1, myrank, MPI_COMM_WORLD);
            MPI_Send(&arr2D[1][1], 1, newvtype1, myrank - x, myrank, MPI_COMM_WORLD);
        }
     
        else if(myrank % x == 0){
     
            MPI_Send(&arr2D[1][1], 1, newvtype1, myrank - x, myrank, MPI_COMM_WORLD);
            MPI_Send(&arr2D[n][1], 1, newvtype1, myrank + x, myrank, MPI_COMM_WORLD);
            MPI_Send(&arr2D[1][n], 1, newvtype2, myrank + 1, myrank, MPI_COMM_WORLD);
        }
     
        else if((myrank + 1) % x == 0){
        
		    MPI_Send(&arr2D[1][1], 1, newvtype1, myrank - x, myrank, MPI_COMM_WORLD);
            MPI_Send(&arr2D[n][1], 1, newvtype1, myrank + x, myrank, MPI_COMM_WORLD);
            MPI_Send(&arr2D[1][1], 1, newvtype2, myrank - 1, myrank, MPI_COMM_WORLD);  
        }
     
        else{
     
            MPI_Send(&arr2D[1][1], 1, newvtype1, myrank - x, myrank, MPI_COMM_WORLD);
            MPI_Send(&arr2D[n][1], 1, newvtype1, myrank + x, myrank, MPI_COMM_WORLD);
            MPI_Send(&arr2D[1][1], 1, newvtype2, myrank - 1, myrank, MPI_COMM_WORLD);
            MPI_Send(&arr2D[1][n], 1, newvtype2, myrank + 1, myrank, MPI_COMM_WORLD);
        }
        
        
        // RECEIVE
        
        if(myrank == tl){
     
        MPI_Recv(rreceived, n, MPI_DOUBLE, tl + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[i][max - 1] = rreceived[k++];
            
        MPI_Recv(dreceived, n, MPI_DOUBLE, tl + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[max - 1][i] = dreceived[k++];
        }
     
        else if(myrank == tr){
     
        MPI_Recv(lreceived, n, MPI_DOUBLE, tr - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[i][0] = lreceived[k++];
            
        MPI_Recv(dreceived, n, MPI_DOUBLE, tr + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[max - 1][i] = dreceived[k++];
		}
     
        else if(myrank == bl){
        	
        MPI_Recv(rreceived, n, MPI_DOUBLE, bl + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[i][max - 1] = rreceived[k++];
            
        MPI_Recv(ureceived, n, MPI_DOUBLE, bl - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[0][i] = ureceived[k++];
        }
     
        else if(myrank == br){
     
        MPI_Recv(lreceived, n, MPI_DOUBLE, br - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[i][0] = lreceived[k++];
            
        MPI_Recv(ureceived, n, MPI_DOUBLE, br - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[0][i] = ureceived[k++];
        }
     
        else if(myrank > tl && myrank < tr){
        	
        MPI_Recv(lreceived, n, MPI_DOUBLE, myrank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[i][0] = lreceived[k++];
            
        MPI_Recv(rreceived, n, MPI_DOUBLE, myrank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[i][max - 1] = rreceived[k++];
            
        MPI_Recv(dreceived, n, MPI_DOUBLE, myrank + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[max - 1][i] = dreceived[k++];
        }
     
        else if(myrank > bl && myrank < br){
     
        MPI_Recv(lreceived, n, MPI_DOUBLE, myrank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[i][0] = lreceived[k++];
        
	MPI_Recv(rreceived,n, MPI_DOUBLE, myrank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[i][max - 1] = rreceived[k++];
        
	MPI_Recv(ureceived, n, MPI_DOUBLE, myrank - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[0][i] = ureceived[k++];
        }
     
        else if(myrank % x == 0){
     
        MPI_Recv(ureceived, n, MPI_DOUBLE, myrank - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[0][i] = ureceived[k++];
        
	MPI_Recv(dreceived, n, MPI_DOUBLE, myrank + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[max - 1][i] = dreceived[k++];
        
	MPI_Recv(rreceived, n, MPI_DOUBLE, myrank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[i][max - 1] = rreceived[k++];
        }
     
     else if((myrank + 1) % x == 0){
        
        MPI_Recv(ureceived, n, MPI_DOUBLE, myrank - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[0][i] = ureceived[k++];
        
	MPI_Recv(dreceived, n, MPI_DOUBLE, myrank + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[max - 1][i] = dreceived[k++];
        
	MPI_Recv(lreceived, n, MPI_DOUBLE, myrank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[i][0] = lreceived[k++];
        }
     
     else{
     
        MPI_Recv(ureceived, n, MPI_DOUBLE, myrank - x, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[0][i] = ureceived[k++];
        
	MPI_Recv(dreceived, n, MPI_DOUBLE, myrank + x, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[max - 1][i] = dreceived[k++];
        
	MPI_Recv(lreceived, n, MPI_DOUBLE, myrank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[i][0] = lreceived[k++];
        
	MPI_Recv(rreceived, n, MPI_DOUBLE, myrank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status3);
        k = 0;
        for(int i = 1; i < max - 1; i++)
            arr2D[i][max - 1] = rreceived[k++];
        }
        
        compute(arr2D);
    }
    
    MPI_Type_free (&newvtype1);
    MPI_Type_free (&newvtype2);
    
    eTime = MPI_Wtime();
    time = eTime - sTime;

    // obtain max time
    MPI_Reduce (&time, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (!myrank) printf ("t3 = %lf\n", maxTime);  
}


int main(int argc, char *argv[]){
    max = sqrt(atoi(argv[1]));
    n = max;
    p = atoi (argv[2]);
    int size, myrank;
    MPI_Init ( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD , &myrank );
    MPI_Comm_size( MPI_COMM_WORLD , &size );
    
    max = max + 2;                                     //  We are padding buffers into the matrix itself
    
    x = sqrt(size);
   
    // Top left process
    tl = 0;
   
    // Top right process
    tr = x - 1;
   
    // Bottom left process
    bl = size - x;
   
    // Bottom right process
    br = size - 1;
    
    
    // Initialisation
    double **arr2D = malloc(max * sizeof(double*));          // allocating pointers
    arr2D[0] = malloc(max * max * sizeof(double));           // allocating data
    for(int i=1;i<max;i++)
        arr2D[i]=&(arr2D[0][i*max]);                         // for contigous allocation   
    for(int i = 0; i < max ; i++){
        for(int j = 0; j < max ; j++)
            arr2D[i][j] = (double) (rand() % (n * n));
    }
    
    // Storing -1 in padded buffer to discriminate
    for(int j = 0; j < max; j++){
        arr2D[0][j] = -1;                                  
       	arr2D[max - 1][j] = -1;
    }
    for(int i = 0; i < max; i++){
       	arr2D[i][0] = -1;
       	arr2D[i][max - 1] = -1;
    }
	
    // Calling 1st Function
    part1(arr2D, myrank);
    
    
    for(int i = 0; i < max ; i++){
       for(int j = 0; j < max ; j++)
          arr2D[i][j] = (double) (rand() % (n * n));
    }  
    
    // Storing -1 in padded buffer to discriminate
    for(int j = 0; j < max; j++){
       	arr2D[0][j] = -1;
       	arr2D[max - 1][j] = -1;
    }
    for(int i = 0; i < max; i++){
       	arr2D[i][0] = -1;
       	arr2D[i][max - 1] = -1;
    }
    
    // Calling 2nd Function
    part2(arr2D, myrank);
    
    
    for(int i = 0; i < max ; i++){
        for(int j = 0; j < max ; j++)
          arr2D[i][j] = (double) (rand() % (n * n));
    } 
	
    // Storing -1 in padded buffer to discriminate 
    for(int j = 0; j < max; j++){
       	arr2D[0][j] = -1;
       	arr2D[max - 1][j] = -1;
    }
    for(int i = 0; i < max; i++){
       	arr2D[i][0] = -1;
       	arr2D[i][max - 1] = -1;
    }
    
    // Calling 3rd function
    part3(arr2D, myrank);
  
    MPI_Finalize();
    return 0;
}
