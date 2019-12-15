#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
typedef struct Matrix
{
    int rowsNumber;
    int colsNumber;
    int **matrix;


}Matrix;

/** This struct will be send to the thread function **/
typedef struct args
{
    int row;
    int col;
    Matrix *m; /// this is a pointer to Matrix
}args;
Matrix *readMatrix();
Matrix byElement(Matrix *m);
void *calcByElemnts(void *data);
void *calcByRows(void *data);
Matrix byRows(Matrix *m);
FILE *ffptr ; /// OUTPUT file
int main()
{
Matrix *m = malloc(sizeof(Matrix)*4);
m = readMatrix();
ffptr= fopen("output.txt","w");
byElement(m); /// first method
byRows(m); /// second method
fclose(ffptr);
return 0;
}
Matrix* readMatrix()
{
    Matrix *m =  malloc(sizeof(Matrix) *4); /** allocate memory for 4 mat's
                                                Here m[0] and m[1] are the input's
                                                , m[2] and m[3] are the output's**/
    int temp;


    FILE *fptr = fopen("100.txt","r");
    fscanf(fptr,"%d%d",&m[0].rowsNumber,&m[0].colsNumber);
    m[2].matrix =(int **) malloc(m[0].rowsNumber*sizeof(int*)); ///allocate rows number for the 1'st output mat
    m[2].rowsNumber = m[0].rowsNumber;
    m[3].matrix =(int **) malloc(m[0].rowsNumber*sizeof(int*)); ///allocate rows number for the 2'nd output mat
    m[3].rowsNumber = m[0].rowsNumber;
    m[0].matrix = (int **) malloc(m[0].rowsNumber * sizeof(int*));
    for(int i = 0 ; i < m[0].rowsNumber ; i++ )
    {
        m[0].matrix[i] = (int *) malloc(m[0].colsNumber *sizeof(int)); ///allocate a col for each row
        for(int j=0 ; j<m[0].colsNumber ; j++)
        {
            fscanf(fptr,"%d",&temp);
            m[0].matrix[i][j] = temp; /// set the matrix
        }
    }


    fscanf(fptr,"%d%d",&m[1].rowsNumber,&m[1].colsNumber);

    m[1].matrix = (int **) malloc(m[1].rowsNumber *sizeof(int*));
    for(int i = 0 ; i < m[1].rowsNumber ; i++ )
    {


        m[1].matrix[i] = (int *)malloc(m[1].colsNumber *sizeof(int));

        for(int j=0 ; j<m[1].colsNumber ; j++)
        {
            fscanf(fptr,"%d",&m[1].matrix[i][j]);
        }
    }


    m[2].colsNumber = m[1].colsNumber;

    for(int k =0 ; k < m[2].rowsNumber; k++)
    {
        m[2].matrix[k] =(int*) malloc(m[2].colsNumber * sizeof(int)); ///allocate cols number for the 1'st , 2'nd output mat's
        m[3].matrix[k] =(int*) malloc(m[3].colsNumber * sizeof(int));

    }
    fclose(fptr);

          // fflush(stdout);

    return m;



}

/** 1'st method : each element in a thread **/
Matrix byElement(Matrix *m)
{

    args *arg;
    arg = malloc(sizeof(args));
    arg->m[0] = m[0]; /// pointer to mat
    //printf("%d",arg->m[0].colsNumber);
    arg->m[1] = m[1];
    arg->m[2] = m[2];
    clock_t start,end;
    start = clock();
    float totalTime;
    pthread_t thread_id[m[0].rowsNumber * m[1].colsNumber];
    int numberOfThreads= 0 ;
    // output.matrix = malloc(arg->m[0].rowsNumber*sizeof(int*));
   for(int i=0 ; i< m[0].rowsNumber ; i++)
    {
        for(int j = 0 ; j < m[1].colsNumber ; j++)
        {
            arg->row = i;
            arg->col = j;
            pthread_create(&thread_id[numberOfThreads],NULL,calcByElemnts,arg);
            pthread_join(thread_id[numberOfThreads],NULL);
            numberOfThreads++;



        }
                   fprintf(ffptr,"\n");

    }
    end = clock();
    totalTime = ((float)end-start)/CLOCKS_PER_SEC;
    fprintf(ffptr,"END1\t[%f]\n",totalTime);
    //printOP(arg->m[2]);
    return arg->m[2];





}

void *calcByElemnts(void *data)
{
    //    printf("\n");

    args *arg = malloc(sizeof(args));
    arg = data;

    for(int l = 0 ; l < arg->m[0].colsNumber ; l++)
    {
        arg->m[2].matrix[arg->row][arg->col] += (arg->m[0].matrix[arg->row][l]*arg->m[1].matrix[l][arg->col]);

    }
            fprintf(ffptr,"%d\t",arg->m[2].matrix[arg->row][arg->col]);


    return NULL;
}


/** 1'st method : each row in a thread **/
Matrix byRows(Matrix *m){
    args *arg;
    arg = malloc(sizeof(args));
    arg->m[0] = m[0];
    //printf("%d",arg->m[0].colsNumber);
    arg->m[1] = m[1];
    arg->m[3] = m[3];
    clock_t start,end;
    start = clock();
    float totalTime;
    pthread_t thread_id[m[0].rowsNumber];
    int numberOfThreads= 0;
    for(int i=0 ; i < arg->m[0].rowsNumber ; i++){
        arg->row = i;
        arg->col = 0;
        pthread_create(&thread_id[numberOfThreads],NULL,calcByRows,arg);
        pthread_join(thread_id[numberOfThreads],NULL);
            numberOfThreads++;
                               fprintf(ffptr,"\n");


    }


    end = clock();
    totalTime = ((float)end-start)/CLOCKS_PER_SEC;
    fprintf(ffptr,"END2\t[%f]",totalTime);
    //printOP(arg->m[3]);
    return arg->m[3];

}
void *calcByRows(void *data){

args *arg = malloc(sizeof(args));
    arg = data;

for(int j=0; j<arg->m[1].colsNumber; j++){

        arg->m[3].matrix[arg->row][j] = 0;

        for(int k=0; k< arg->m[0].colsNumber; k++)
        {

            arg->m[3].matrix[arg->row][j] += arg->m[0].matrix[arg->row][k] * arg->m[1].matrix[k][j];

        }
        fprintf(ffptr,"%d\t",arg->m[3].matrix[arg->row][j]);

    }

}
