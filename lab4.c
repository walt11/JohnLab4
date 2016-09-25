/*
##################################
Name:		John Walter
Date:		9/24/16
Class:		ECE 3220
Assignment:	Lab 4
##################################
*/

#include <stdio.h>
#include <stdlib.h>

//Global variables 
int length = 0;
double offset,scale;
int max_value;

// This function reads in the data from a file to an integer array and returns a pointer to the array
int *Readfile(int file){
 	int cell, i;
 	int array[2000];
 	int *arrayPtr = array;
	char file_name[20];

	//Formats the file name based on the number inputted by the user
	if(file<9){ // includes a zero in the name
		sprintf(file_name,"Raw_data_0%d.txt",file);
	}else{ //doesn't include a zero in the name
		sprintf(file_name,"Raw_data_%d.txt",file);
	}
	FILE *fp = fopen(file_name,"r");
	if (fp == NULL){
		printf("Error opening file.\n");
		return NULL;
	}
	fscanf(fp,"%d %d",&length,&max_value);
	for(i=0;i<length;i++){
		fscanf(fp,"%d",&cell);
		array[i] = cell;
	}

	fclose(fp);
	return arrayPtr;
}

// This function writes the values in a double array to a file 
void WriteFile(char file_name[20], double *array, double x){ // the x represents the offset or scale factor used in the transformations 
	int i;
	
	FILE *fp = fopen(file_name,"w");
	if(fp == NULL){
		printf("Error opening file\n");
	}
	fprintf(fp,"%d %.4f\n",length,x);
	for(i=0;i<length;i++){
		fprintf(fp,"%.4f\n",array[i]);
	}
	fclose(fp);
}

// This function calculates the mean (average) of an integer array
double mean(int *array){
	double total = 0.0;
	int i=0;
	for(i=0;i<length;i++){
		total = total+array[i];
	}
	return total/(double)length; //length is type int
}

// This function returns the maximum possible value of the signal
double max(int *array){
	// Originally thought that this function needed to actually find the maximum value of the signal
	// but then realized it just needs to return the max value given in the file, which is declared as
	// a global variable above and found the the Readfile() function
	/*int i=0;
	double max = (double)array[i];
	for(i=0;i<length;i++){
		if(array[i] > max){max = array[i];}
	}
	return max;
	*/
	return (double)max_value;
}

// This function generates a file containing the mean and max of each raw file
void createStatisticsFile(int *array, int file){
	char file_name[20];
	if(file<10){
		sprintf(file_name,"Statistics_data_0%d.txt",file);
	}else{
		sprintf(file_name,"Statistics_data_%d.txt",file);
	}
	
	FILE * fp = fopen(file_name,"w");
	fprintf(fp,"%.4f %.0f",mean(array),max(array));
	fclose(fp);
}

// This function offsets the signal based on an offset value inputted
void offsetSignal(int *array, double offset_value, char file_name[20]){
	int i;
	double arrayD[2000];
	for(i=0;i<length;i++){
		arrayD[i] = (double)array[i] + offset_value;
	}
	//Writes the new array to a file
	WriteFile(file_name, arrayD, offset_value);
}

//This function scales the signal based on a scale factor inputted
void scaleSignal(int *array, double scale_factor, char file_name[20]){
	int i;
	double arrayD[2000];
	for(i=0;i<length;i++){
		arrayD[i] = (double)array[i] * scale_factor;
	}
	//Writes the new array to a file
	WriteFile(file_name, arrayD, scale_factor);
}

//This function centers the signal 
void centerSignal(int *array, char file_name[20]){
	// Since centering is the same as offsetting the signal by a factor
	// of -mean, the offsetSignal can be used with an offset value of -mean
	offsetSignal(array, (-mean(array)), file_name);
}

// This function normalizes the singal
void normalizeSignal(int *array, char file_name[20]){
	// Since normalizing is the same as scaling the signal by a factor
	// of 1/max, the scaleSignal() function can be used with a scale value of 1/max
	scaleSignal(array, (1/max(array)), file_name);
}

int main(void){
	char file_name[20];
	int file, choice;
	printf("Enter file number: ");
	scanf("%d",&file);
	while((file < 1) | (file > 10)){
		printf("Invalid file number: ");
		scanf("%d",&file);
	}
	//Read file and create int array of data in file
	int *array = Readfile(file);
	printf("Select\n\t(1) Offset\n\t(2) Scale\n\t(3) Center\n\t(4) Normalize\n> ");
	scanf("%d",&choice);
	while((choice < 1) | (choice > 4)){
		printf("Invalid choice. Try again: ");
		scanf("%d",&choice);
	}
	switch(choice){
		case 1:
				if(file<10){
					sprintf(file_name,"Offset_data_0%d.txt",file);
				}else{
					sprintf(file_name,"Offset_data_%d.txt",file);
				}
				printf("Enter offset value: ");
				scanf("%lf",&offset);
				offsetSignal(array, offset, file_name);
				break;
		case 2:
				if(file<10){
					sprintf(file_name,"Scaled_data_0%d.txt",file);
				}else{
					sprintf(file_name,"Scaled_data_%d.txt",file);
				}
				printf("Enter scale factor: ");
				scanf("%lf",&scale);
				scaleSignal(array, scale, file_name);
				break;
		case 3:
				if(file<10){
					sprintf(file_name,"Centered_data_0%d.txt",file);
				}else{
					sprintf(file_name,"Centered_data_%d.txt",file);
				}
				centerSignal(array,file_name);
				break;
		case 4:
				if(file<10){
					sprintf(file_name,"Normalized_data_0%d.txt",file);
				}else{
					sprintf(file_name,"Normalized_data_%d.txt",file);
				}
				normalizeSignal(array,file_name);
				break;
	}
	createStatisticsFile(array,file); //generate statistics file
	return 0;
	
}