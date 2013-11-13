
#include "stdio.h"
#include "iostream.h"
#include "stdlib.h"
#include "math.h"

#define Size 64
#define ThreshUp 30
#define ThreshLow 20
#define THRESH 0.5
using namespace std;
unsigned char afterErode4[Size][Size];
unsigned char afterErode5[Size][Size];
unsigned char afterErode6[Size][Size];
unsigned char afterErode7[Size][Size];
unsigned char afterErode8[Size][Size];
int main( int argc, char *argv[])
{
	FILE *file;
	unsigned char Imagedata[Size][Size];
	unsigned char Image[16][Size][Size];
	double LawsDenum[9] = {36, 12, 12, 12, 4, 4, 12, 4, 4 };
	/**********************************************************************
	double Laws1[3][3] = { {1.0,2.0,1.0}, {2.0,4.0,2.0}, {1.0,2.0,1.0} };
	double Laws2[3][3] = { {1.0,0.0,-1.0},{2.0,0.0,-2.0},{1.0,0.0,-1.0}};
	double Laws3[3][3] = { {-1.0,2.0,-1.0}, {-2.0,4.0,-2.0}, {-1.0,2.0,-1.0} };					
	double Laws4[3][3] = { {-1.0,-2.0,-1.0},{0.0,0.0,0.0},{1.0,2.0,1.0} };
	double Laws5[3][3] = { {1.0,0.0,-1.0},{0.0,0.0,0.0},{1.0,-2.0,1.0} };
	double Laws6[3][3] = { {-1.0,2.0,-1.0}, {0.0,0.0,0.0}, {1.0,-2.0,1.0} };
	double Laws7[3][3] = { {-1.0,-2.0,-1.0},{2.0,4.0,2.0},{-1.0,-2.0,-1.0} };
	double Laws8[3][3] = { {-1,0,1}, {2,0,-2}, {-1,0,1} };
	double Laws9[3][3] = { {1,-2,1}, {-2,4,-2}, {1,-2,1} };
	/**********************************************************************/
	double Laws[9][3][3] = {   
    { {1.0,2.0,1.0}, {2.0,4.0,2.0}, {1.0,2.0,1.0} },							 
	 { {1.0,0.0,-1.0},{2.0,0.0,-2.0},{1.0,0.0,-1.0}},
	 { {-1.0,2.0,-1.0}, {-2.0,4.0,-2.0}, {-1.0,2.0,-1.0} },				
	 { {-1.0,-2.0,-1.0},{0.0,0.0,0.0},{1.0,2.0,1.0} },
	 { {1.0,0.0,-1.0},{0.0,0.0,0.0},{1.0,-2.0,1.0} },
	 { {-1.0,2.0,-1.0}, {0.0,0.0,0.0}, {1.0,-2.0,1.0} },
	 { {-1.0,-2.0,-1.0},{2.0,4.0,2.0},{-1.0,-2.0,-1.0} },
	 { {-1,0,1}, {2,0,-2}, {-1,0,1} },
	 { {1,-2,1}, {-2,4,-2}, {1,-2,1} }   };
						/************************************************/
						/******                                	 ******/
						/****** Read "input.raw" into Imagedata[][]******/
						/******                                	 ******/
						/************************************************/							
	/*if (!(file=fopen("circle.raw","rb"))){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size, file);
	fclose(file);
	*/
	int i = 1;
	char picFileName[16];
	for( i=1; i<=9; i++ ){
		sprintf( picFileName,"sample0%d.raw", i );
		file = fopen( picFileName, "rb" );
		fread( Image[i-1], sizeof(unsigned char), Size*Size, file );
		fclose(file);
	}
	for( i=0; i<=6; i++ ){
		sprintf( picFileName,"sample1%d.raw", i );
		//printf("\n%s ", picFileName );
		file = fopen( picFileName, "rb" );
		fread( Image[i+9], sizeof(unsigned char), Size*Size, file );
		fclose(file);
	}
	
	/**********************************	do some image processing task... ***/
						/********************************************/
						/******                                ******/
						/******     Constructing Element Type I 
						/******     8-Neighbor
						/******                                ******/
						/********************************************/	
	int j,imageLoop, maskLoop;
	int iOffset = 0, jOffset = 0; 	//	 the offset within the moving-window	
	double M[Size][Size]={0};
	double T[16][9];
	double centroid[4][9];
	
	float  maxNeighbor;
	double temp, featureSum, average, variance, sigma;
	/********************************/
	for(  i=0; i<9; i++ ){
		for( j = 0; j<3; j++ ){
			for( int k = 0; k<3; k++ ){
			printf("%f  ", Laws[i][j][k]);
		}
		printf("\n");
		}
		printf("\n");
		printf("%f.\n\n\n\n", LawsDenum[i]);
	}
	/********************/
	double windowSum, imageSum;
	for( imageLoop = 0; imageLoop < 16; imageLoop++ ){
		for( maskLoop = 0; maskLoop < 9; maskLoop ++ ){
			for( i = 2; i < Size-2; i++ ){
				for( j = 2; j < Size-2; j++){
					
					/* in the window */
					for( iOffset = -1; iOffset <= 1; iOffset++ ){
						for( jOffset = -1; jOffset <= 1; jOffset++ ){
							windowSum += Image[imageLoop][i][j] * Laws[maskLoop][iOffset+1][jOffset+1];
						}	//	End-4-for
					}	//	End-3-for	
					windowSum = windowSum / 	LawsDenum[maskLoop];	
					M[i][j] = windowSum;
					imageSum += windowSum;
				}	//	End-2-for
			}	//	End-1-for
			// in the same mask 
			/*******************************/
			average = imageSum / (Size*Size);
			for( i = 0; i < Size; i++ ){
				for( j = 0; j < Size; j++){
					variance += (M[i][j]-average) * (M[i][j]-average); 
				} // End-2-for
			} //End-1-for
			sigma = sqrt( variance / (Size*Size) );
			/***********************************/
			
			T[imageLoop][maskLoop] = sigma;
			printf("\nimage %d mask %d : %f" ,imageLoop, maskLoop, T[imageLoop][maskLoop] );
			
			} // End-0-for   <<   Laws Mask Loop 
			printf("\n");
	
		} // End-0-a-for   <<   Image Loop 
		
		
		//double centroid[4][9];

			for( j=0; j<9; j++ ){
				centroid[0][j] = T[0][j];
			}
			for( j=0; j<9; j++ ){
				centroid[1][j] = T[3][j];
			}
			for( j=0; j<9; j++ ){
				centroid[2][j] = T[4][j];
			}
			for( j=0; j<9; j++ ){
				centroid[3][j] = T[10][j];
			}
		double distMin, distSum[4], dist;
		int c, distMinIndex, textureGroup[16];
		for( i=0; i<16; i++ ){
			for( c=0; c<4; c++  ){
				for( j=0; j<9; j++ ){
					dist = ( T[i][j]-centroid[c][j]  )*( T[i][j]-centroid[c][j] );
					distSum[c] += dist;
				}
				//printf("\n%f", distSum[c] );
			}
			printf("\n");
			distMin = distSum[0];
			distMinIndex = 0;
			for( c=1; c<4; c++ ){
				if( distMin > distSum[c] ){
					//printf("%d:detect.   ", c);
					distMin = distSum[c];
					distMinIndex = c;
				}
			} // End min champion Loop 
			textureGroup[i] = distMinIndex;
			printf("image %d : group %d.\n\n\n", i, textureGroup[i] );
		}
		

	/******************************************************** b e g i n ***/

						/********************************************/
						/******                                ******/
						/******    write image to "boundary_I.raw"  ******/
						/******                                ******/
						/********************************************
	if (  !(file=fopen("afterErode.raw","wb"))  ){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	else{
		fwrite(afterErode, sizeof(unsigned char), Size*Size, file);
		fclose(file);
	}
						/********************************************/
						/******                                ******/
						/******    write image to "M.raw"  ******/
						/******                                ******/
						/********************************************	
	if (  !(file=fopen("M.raw","wb"))  ){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	else{
		fwrite(boundary1, sizeof(unsigned char), Size*Size, file);
		fclose(file);
	}
	
						/********************************************/		
	system("PAUSE");
	exit(0);
}

