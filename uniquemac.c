//About : This code computes the total number of unique mac ids for each sensor data file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bsort(char [][18]); //burble sort method declaration
int linescount(FILE *f); //to count number of lines in a file
int len;//length of the array of all macids

int genuniqcount(FILE *fp){ //fp is a pointer to a sensor data file
	if (fp == NULL){ //Checking if fp is valid
		printf("Cannot open the input file. Please check the File name or Path \n");
		return -1;
	}
	int n= 0; //just used for iterations
	char selectedmac[18];
	int temp1,temp3,temp4;
	float temp2;
	len= linescount(fp);//get number of lines in the file
	char mac[len][18];//array of all macids
	memset(mac,0,sizeof(mac));
	rewind(fp);
	while(!feof(fp)){
		fscanf(fp, "%d %s %f %d %d",&temp1,selectedmac,&temp2,&temp3,&temp4);
		if(strcmp(selectedmac,"00:00:00:00:00:00")!=0){//if the scanned mac address is not an invalid mac 00:00:00:00:00:00
			strcpy(mac[n],selectedmac); //add to the list of mac address
			n++;
		}
	}
	bsort(mac);//sort the list so duplicate mac address will be beside each other
	
	strcpy(selectedmac,"00:00:00:00:00:00");
	n=0;//increments for every unique mac address
	for(int i=0;i< len;i++){
		if(strcmp(selectedmac,"00:00:00:00:00:00")==0){ //happens only for the first element
			strcpy(selectedmac,mac[i]);
			n++;
			continue;

		}
		else{
			if(strcmp(mac[i],selectedmac)==0){//skip if the current mac address has been seen previously
				continue;
			}
			else{
				strcpy(selectedmac,mac[i]);
				n++;
			}
		}
	}
	return (n-1);//minus one because the last element is an empty string, because number of elements in mac[] < length of mac[]
}

void bsort(char arr[len][18]){//burble sort
	char temp[18];
	for(int i=0;i< len;i++){
		for(int j=0;j< len;j++){ //(int) sizeof(arr)/18
			if(strcmp(arr[i],arr[j])>0){
				strcpy(temp,arr[i]);
				strcpy(arr[i],arr[j]);
				strcpy(arr[j],temp);

			}
			else{continue;}
		}
	}
}

int linescount(FILE *f){ //returns number of lines in a file
	int ch=0;
	int lines=0;

	while(!feof(f)){
		ch = fgetc(f);
		if(ch == '\n'){
			lines++;
		}
	}
	return lines;
}
