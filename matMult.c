#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#define cn an	//number of rows of c matrix = number of rows in a matrix
#define cm bm	//number of columns of c matrix = number of columns in b matrix
#define ce am	//number of columns of a matrix and number of rows in b matrix too

int aMatrix[100][100] ;
int bMatrix[100][100] ;
int cMatrix[100][100] ;

int an,am,bn,bm;	//number of colmuns ad rows for a and b matrices (globa to be used by all functions)

//this function is called by threads in the row threaded version. It calculates each row of the output matrix
void *rowMult(void* ii)
{
	int i,j,k;
	i=(int) ii;	//casting from void to integer
	for(j=0; j<cm; j++){ 	//loop over columns of row i 
		cMatrix[i][j]=0;	//reseting c matrix elemnts as next instruction accumulates over these elements
		for(k=0; k<ce; k++)	//loop over each elemnt in a matrix row and in matrix b column too
			cMatrix[i][j] += aMatrix[i][k] * bMatrix[k][j];	//multiply a matric row elemnt by b matrix columns
	}
}


//this function is called by threads in the element threaded version. It calculates each element of the output matrix
void *elementMult(void* ll)
{
	int l,i,j,k;
	l=(int) ll;	//casting from void to integer
	i=l/cm;	//extracting i from l (I used this method instead of passing structs)
	j=l%cm; //extracting j from l
	cMatrix[i][j]=0;	//reseting c matrix elemnts as next instruction accumulates over these elements
	for(k=0; k<ce; k++)	//loop over each elemnt in a matrix row and in matrix b column too
		cMatrix[i][j] += aMatrix[i][k] * bMatrix[k][j];	//multiply a matric row elemnt by b matrix columns
	
}



//this is a version of matrix multiplication that does not use threads
void nonThreadedMatMult (){
   
    struct timeval stop, start;
    gettimeofday(&start, NULL);

   /*******************Code goes here **************************/

	int i,j,k;
	for (i=0; i<cn;i++)	//loop over c matrix rows
		for(j=0; j<cm; j++){	//loop over c matrix columns
			cMatrix[i][j]=0;	//reseting c matrix elemnts as next instruction accumulates over these elements
			for(k=0; k<ce; k++) 	//loop over each elemnt in a matrix row and in matrix b column too
				cMatrix[i][j] += aMatrix[i][k] * bMatrix[k][j];	//multiply a matric row elemnt by b matrix columns
			}	
	
    /*******************Code ends here **************************/
    
    gettimeofday(&stop, NULL); //end checking time
    
    printf("For unthreaded solution:\nSeconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);

    
}



//this is a version of matrix multiplication that creates a thread to calculate the value of one element of the output matrix
void threadedMatMultPerElement (){


    struct timeval stop, start;
    gettimeofday(&start, NULL);

      /*******************Code goes here **************************/
	int i,j,l;
	pthread_t elementThreads[cn*cm];	//defining cn*cm threads as the number of elements in c matrix
	for(i = 0; i < cn; i++)	//loop over c matrix rows
		for(j = 0; j < cm; j++){	//loop over c matrix cloumns
			l=i*cm+j;	//calculating 1D elemnt index
			pthread_create(&elementThreads[l], NULL, rowMult, (void *)l);	//creat threads passing to each thread the element 1D index
		}
	for(l = 0; l < cn*cm; l++)	//checking thread has terminated .. it increaes excution time actually, but needed.
		pthread_join(elementThreads[i],NULL);


      /*******************Code ends here **************************/

    gettimeofday(&stop, NULL);

    printf("For element threaded solution:\nSeconds taken: %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);

    
}

//this is a version of matrix multiplication that creates a thread to calculate the values of elements in one row of the output matrix
void threadedMatMultPerRow (){


    struct timeval stop, start;
    gettimeofday(&start, NULL);

      /*******************Code goes here **************************/
	int i;
	pthread_t rowThreads[cn];	//defining cn threads as the number of rows in c matrix
	for(i = 0; i < cn; i++)	//loop over c matrix rows
		pthread_create(&rowThreads[i], NULL, rowMult, (void *)i);	//creat threads passing to each thread the row index
	for(i = 0; i < cn; i++)	//checking thread has terminated .. it increaes excution time actually, but needed. 	
		pthread_join(rowThreads[i],NULL);


      /*******************Code ends here **************************/

    gettimeofday(&stop, NULL);

    printf("For row threaded solution:\nSeconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);

    
}

/******** This function reads a file into a matrix ***********/
// n: will contain number of rows of the read matrix
// m: will contain number of columns of the read matrix
//fileName: name of file to read
// arr: will contain the read matrix
// call the function like:
//int r,c;
//readFile(&r,&c,mat1File,aMatrix);

void readFile(int *n , int *m , char *fileName , int arr[][100] ){
    FILE *in= fopen(fileName, "r") ;
    int nn , mm ;
    char line [1000];
    fgets(line ,1000 , in) ;
    int error = sscanf (line , "row=%i col=%i" ,&nn ,&mm) ;

    if (error==0)
        return ;
    
    *n=nn ;
    *m=mm;
    int i , j ;
    for (i=0;i<nn;i++){
        for (j=0;j<mm;j++){
            int tmp ;
            fscanf(in , "%d", &tmp) ;
            arr[i][j] = tmp ;
        }
    }
    fclose(in);
}

/******* This function writes the result matrix 'cMatrix' to a file ****/
// rowsNum: number of rows in cMatrix
// colsNum: number of columns in cMatrix
// fileName: name of output file

void writeToFile(int rowsNum, int colsNum, char *fileName){
	FILE *out=fopen(fileName, "w") ;
	   int i,j=0;
	    for (i=0;i<rowsNum;i++){
		for (j=0;j<colsNum;j++){
		    fprintf (out ,"%d\t" ,cMatrix[i][j]);
		}
		fprintf(out ,"\n");
	    }
	    fclose (out);
}


int main(int argc , char *argv[])
{

      char * mat1File="a.txt" ;
      char * mat2File="b.txt" ;
      char * matOutFile1= "c_1.out" ;
      char * matOutFile2= "c_2.out" ;
      char * matOutFile3= "c_3.out" ;
      
	
	readFile(&an, &am , mat1File ,aMatrix );	//read a matrix and put it in aMatrix array
	readFile(&bn, &bm , mat2File ,bMatrix ); 	//read b matrix and put it in bMatrix array
        	
	if(am==bn){	//check if inner dimensions matches

		//calling eacg function and write its output in a seperate file		
	        nonThreadedMatMult();
		writeToFile(cn, cm, matOutFile1);
	
	        threadedMatMultPerElement();
		writeToFile(cn, cm, matOutFile2);

	        threadedMatMultPerRow();
		writeToFile(cn, cm, matOutFile3);
	}
	else
		printf("ERROR! Matrices Dimensions Do Not Match\n");
	
}


