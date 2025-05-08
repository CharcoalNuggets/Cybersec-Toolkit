#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h> //needed for directory access
#include <time.h>

// This is the dimension of a playing card with a 3:4 Aspect Ratio (standing upright)
#define DIMX (690)
#define DIMY (920)

#define max(X, Y) ((X) > (Y) ? (X) : (Y))
#define SQDIM (max(DIMX, DIMY))

unsigned char P[SQDIM][SQDIM];     // Pixel array of gray values
unsigned char TP[SQDIM][SQDIM];    // Transpose of Pixel array
unsigned char RRP[SQDIM][SQDIM];   // Rotation Right of Pixel array
unsigned char RLP[SQDIM][SQDIM];   // Rotation Left of Pixel array

void zeroPixMat(unsigned char Mat[][SQDIM]);
void transposePixMat(unsigned char Mat[][SQDIM], unsigned char TMat[][SQDIM]);
void swapColPixMat(unsigned char Mat[][SQDIM], unsigned char TMat[][SQDIM], int square_size);
void swapRowPixMat(unsigned char Mat[][SQDIM], unsigned char TMat[][SQDIM], int square_size);


// PGM file utilities with simple byte by byte I/O
void readPGMHeaderSimple(int fdin, char *header);
void printPGMHeader(char *header);
void readPGMDataSimple(int fdin, unsigned char Mat[][SQDIM]);
void writePGMSimple(int fdout, char *header, unsigned char Mat[][SQDIM]);

// PGM file utilities with fast large read and write I/O
void readPGMHeaderFast(int fdin, char *header);
void readPGMDataFast(int fdin, unsigned char Mat[][SQDIM]);
void writePGMFast(int fdout, char *header, unsigned char Mat[][SQDIM]);
void writePGMFastSquare(int fdout, char *header, unsigned char Mat[][SQDIM]);


int main(){
    struct dirent *entry;
    int file_count = 0;
    DIR *dir = opendir("cards_3x4_pgm");
    if(!dir){
        printf("Directory isn't there.\n");
        exit(-1);
    }
    
    mkdir("cards_rotated", 0777);   //0777 sets permissions

    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start); //start timer before actual file accessing
    
    while((entry = readdir(dir)) != NULL){
        if(entry->d_type == DT_REG && strstr(entry->d_name, ".pgm")){

            int fdin, fdout;
            char header[80], input_path[256], output_path[256];

            snprintf(input_path, sizeof(input_path), "cards_3x4_pgm/%.240s", entry->d_name);
            snprintf(output_path, sizeof(output_path), "cards_rotated/%.240s", entry->d_name);
            
            // open binary file to read in data instead of using test pattern data
            if((fdin = open(input_path, O_RDONLY)) < 0){
                printf("Error opening input file.\n"); exit(-1);
            }
            
            // open binary file to write out data
            if((fdout = open(output_path, O_WRONLY | O_CREAT, 0644)) < 0){
                printf("Error opening output file.\n"); exit(-1);
            }
            
            zeroPixMat(P);

            // read in the PGM data here
            readPGMHeaderSimple(fdin, header);
            readPGMDataFast(fdin, P);
            close(fdin);
            
            // Update header to be square 920x920
            header[26] = '9'; header[27] = '2'; header[28] = '0';
            
            int len = strlen(entry->d_name);
            char suit = entry->d_name[len - 5]; //search for suit character
            
            transposePixMat(P, TP);
            if(suit == 'S' || suit == 'C'){         //rotate right if spades or clubs
                swapColPixMat(TP, RRP, SQDIM);
                writePGMFastSquare(fdout, header, RRP);
                file_count++;
            } 
            else if(suit == 'H' || suit == 'D'){    //rotate left if hearts or diamonds
                swapRowPixMat(TP, RLP, SQDIM);
                writePGMFastSquare(fdout, header, RLP);
                file_count++;
            }
            close(fdout);
        }
    }
    
    closedir(dir);  // close the handle (pointer)

    clock_gettime(CLOCK_MONOTONIC_RAW, &finish);   //end timer
    double elapsed = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1e9;

    printf("Images rotated: %d\n", file_count);
    printf("Execution time: %f seconds\n", elapsed);

    return 0;
}


void swapRowPixMat(unsigned char Mat[][SQDIM], unsigned char TMat[][SQDIM], int square_size)
{
    int idx, jdx;

    for(idx=0; idx<square_size; idx++)       
        for(jdx=0; jdx<square_size; jdx++)  
        {
            // copy into TMat and swap row values         
            TMat[idx][jdx]=Mat[square_size-1-idx][jdx];
        }
}


void swapColPixMat(unsigned char Mat[][SQDIM], unsigned char TMat[][SQDIM], int square_size)
{
    int idx, jdx;

    for(idx=0; idx<square_size; idx++)       
        for(jdx=0; jdx<square_size; jdx++)  
        {
            // copy into TMat and swap column values         
            TMat[idx][jdx]=Mat[idx][square_size-1-jdx];
        }
}


void zeroPixMat(unsigned char Mat[][SQDIM])//resets PixMat to zero
{
    int idx, jdx;

    for(idx=0; idx<SQDIM; idx++)    
        for(jdx=0; jdx<SQDIM; jdx++)
        {
            Mat[idx][jdx]=0;
        }
}


void transposePixMat(unsigned char Mat[][SQDIM], unsigned char TMat[][SQDIM])
{
    int idx, jdx;

    for(idx=0; idx<SQDIM; idx++)
        for(jdx=0; jdx<SQDIM; jdx++)
        {
            // transpose row as column
            TMat[jdx][idx]=Mat[idx][jdx];
        }
}


void readPGMHeaderSimple(int fdin, char *header)
{
    int bytesRead, bytesLeft, bytesWritten;

    //printf("Reading PGM header here\n");

    // header on each card is 38 bytes
    bytesLeft=38;
    bytesRead=read(fdin, (void *)header, bytesLeft);

    if(bytesRead < bytesLeft)
        exit(-1);
    //else
        //printf("header=%s\n", header);

}

void printPGMHeader(char *header)
{
    printf("%s", header);
}

void readPGMDataSimple(int fdin, unsigned char Mat[][SQDIM])
{
    int bytesRead, bytesLeft, bytesWritten;
    int rowIdx, colIdx;

    //printf("Reading PGM data here\n");

    // now read in all of the data
    bytesRead=0;

    // read in the actual size and aspect ratio of original card
    for(rowIdx = 0; rowIdx < DIMY; rowIdx++)
    {
        for(colIdx = 0; colIdx < DIMX; colIdx++)
            bytesRead=read(fdin, (void *)&P[rowIdx][colIdx], 1);
    }

}


void readPGMDataFast(int fdin, unsigned char Mat[][SQDIM])
{
    int bytesRead, bytesLeft, bytesWritten;
    int rowIdx, colIdx;

    //printf("Reading PGM data here\n");

    // now read in all of the data
    bytesRead=0;

    // read in whole rows at a time to speed up
    for(rowIdx = 0; rowIdx < DIMY; rowIdx++)
    {
        bytesRead=read(fdin, (void *)&P[rowIdx][0], DIMX);
    }

}


void writePGMSimple(int fdout, char *header, unsigned char Mat[][SQDIM])
{
    int bytesRead, bytesLeft, bytesWritten;
    int rowIdx, colIdx;

    //printf("Would write out a header and data here\n");
    bytesLeft=38;

    bytesWritten=write(fdout, (void *)header, bytesLeft);
    
    //printf("wrote %d bytes for header\n", bytesWritten);

    // now write out all of the data
    bytesWritten=0;

    for(rowIdx = 0; rowIdx < DIMY; rowIdx++)
    {
        for(colIdx = 0; colIdx < DIMX; colIdx++)
            bytesWritten=write(fdout, (void *)&P[rowIdx][colIdx], 1);
    }
}

void writePGMFast(int fdout, char *header, unsigned char Mat[][SQDIM])
{
    int bytesRead, bytesLeft, bytesWritten;
    int rowIdx, colIdx;

    //printf("Would write out a header and data here\n");
    bytesLeft=38;

    bytesWritten=write(fdout, (void *)header, bytesLeft);
    
    //printf("wrote %d bytes for header\n", bytesWritten);

    // now write out all of the data
    bytesWritten=0;

    for(rowIdx = 0; rowIdx < DIMY; rowIdx++)
    {
        bytesWritten=write(fdout, (void *)&P[rowIdx][0], DIMX);
    }
}

void writePGMFastSquare(int fdout, char *header, unsigned char Mat[][SQDIM])
{
    int bytesRead, bytesLeft, bytesWritten;
    int rowIdx, colIdx;

    //printf("Would write out a header and data here\n");
    bytesLeft=38;

    bytesWritten=write(fdout, (void *)header, bytesLeft);
    
    //printf("wrote %d bytes for header\n", bytesWritten);

    // now write out all of the data
    bytesWritten=0;

    for(rowIdx = 0; rowIdx < SQDIM; rowIdx++)
    {
        bytesWritten=write(fdout, (void *)&Mat[rowIdx][0], SQDIM);
        if(bytesWritten < SQDIM)
        {
            printf("ERROR in write\n"); exit(-1);
        }
    }
}

