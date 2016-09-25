#include <stdio.h>
#include <stdlib.h>

int length = 0;
double offset,scale;
int *Readfile(int file){
 	int cell, max, i;
 	int array[2000];
 	int *arrayPtr = array;
	char file_name[20];
	if(file<9){
		sprintf(file_name,"Raw_data_0%d.txt",file);
	}else{
		sprintf(file_name,"Raw_data_%d.txt",file);
	}
	FILE *fp = fopen(file_name,"r");
	if (fp == NULL){
		printf("Error opening file.\n");
		return NULL;
	}
	fscanf(fp,"%d %d",&length,&max);
	for(i=0;i<length;i++){
		fscanf(fp,"%d",&cell);
		array[i] = cell;
	}

	fclose(fp);
	return arrayPtr;
}

void WriteFile(int file, int OorS, double *array){
	char file_name[20];
	int i;
	double x;
	if(OorS == 1){
		if(file<10){
			sprintf(file_name,"Offset_data_0%d.txt",file);
		}else{
			sprintf(file_name,"Offset_data_%d.txt",file);
		}
		x = offset;
	}
	else{
		if(file<10){
			sprintf(file_name,"Scaled_data_0%d.txt",file);
		}else{
			sprintf(file_name,"Scaled_data_%d.txt",file);
		}
		x = scale;

	}
	FILE *fp = fopen(file_name,"w");
	if(fp == NULL){
		printf("Error opening file\n");
	}
	fprintf(fp,"%d %f\n",length,x);
	for(i=0;i<length;i++){
		fprintf(fp,"%.4f\n",array[i]);
	}
	fclose(fp);
}

double mean(int *array){
	double total = 0.0;
	int i=0;
	for(i=0;i<length;i++){
		total = total+array[i];
	}
	return total/(double)length;
}

double max(int *array){
	int i=0;
	double max = (double)array[i];
	for(i=0;i<length;i++){
		if(array[i] > max){max = array[i];}
	}
	return max;
}

void writeMeanMaxToFile(int *array, int file){
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

void centerSignal(int *array, int file){
	char file_name[20];
	if(file<10){
		sprintf(file_name,"Centered_data_0%d.txt",file);
	}else{
		sprintf(file_name,"Centered_data_%d.txt",file);
	}
	
	FILE * fp = fopen(file_name,"w");
	if(fp == NULL){
		printf("File could not be opened\n");
	}
	int i;
	fprintf(fp,"%d %.4f\n",length,mean(array));
	for(i=0;i<length;i++){
		fprintf(fp,"%.4f\n",(double)array[i]-mean(array));
	}
	fclose(fp);
	
}

void normalizeSignal(int *array, int file){
	char file_name[20];
	if(file<10){
		sprintf(file_name,"Normalized_data_0%d.txt",file);
	}else{
		sprintf(file_name,"Normalized_data_%d.txt",file);
	}
	
	FILE * fp = fopen(file_name,"w");
	
	int i;
	fprintf(fp,"%d %.4f",length,mean(array));
	for(i=0;i<length;i++){
		fprintf(fp,"%.4f\n",(double)array[i]/max(array));
	}
	fclose(fp);
	
}



int main(void){
	double arrayD[2000];
	int file, choice, i=0;
	printf("Enter file number: ");
	scanf("%d",&file);
	int *array = Readfile(file);
	printf("Select\n\t(1) Offset\n\t(2) Scale\n\t(3) Center\n\t(4) Normalize\n> ");
	scanf("%d",&choice);
	switch(choice){
		case 1:
				printf("Enter offset value: ");
				scanf("%lf",&offset);
				for(i=0;i<length;i++){
					arrayD[i] = (double)array[i] + offset;
				}
				break;
		case 2:
				printf("Enter scaling value: ");
				scanf("%lf",&scale);
				for(i=0;i<length;i++){
					arrayD[i] = (double)array[i] * scale;
				}
				break;
		case 3:
				centerSignal(array,file);
				break;
		case 4:
				normalizeSignal(array,file);
				break;
	}
	WriteFile(file,choice,arrayD);
	writeMeanMaxToFile(array,file);

	return 0;
}