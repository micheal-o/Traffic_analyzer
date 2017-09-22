//About :This code generates a report that shows the hourly sensor statistics

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Function takes two file pointers for storing data from input files and writing to an output file
int reportGenerator(FILE *fp, FILE *fpW){
	int n=1, ch=0, r=0;
	if (fp == NULL){ //Checking if fp is valid
		printf("Cannot open the input file. Please check the File name or Path \n");
		return -1;
	}

	//Counting the no. of lines in the file
	while(!feof(fp)){
		ch = fgetc(fp);
		if(ch == '\n'){
			n++;
		}
	
	}

	n--; //Ignoring the trailing newline at the end of the file

	//Declaring the required variables
	int SNo[n], date[n], time[n], count=0;
	float scan_int[n];
	char macid[n][18];
	int i=0;
	char nullmac[] = "00:00:00:00:00:00";

	rewind(fp); //Move fp back to the start of the file

	//Loop scans the file line by line and stores the values in the corresponding arrays
	while(1){
		r = fscanf(fp, "%d %s %f %d %06d ", &SNo[i], &macid[i][18], &scan_int[i], &date[i], &time[i]);
		if (r == EOF || i== (n-1))
			break;
			i++;
	
	}


	int j=0;

	//Printing MAC ID statistics at the end of each hour to a file

	while (j < n){
		if (time[j]/10000 == time[j+1]/10000){ //First two digits correspond to Hr. So, check if the hr is the same with the next element in time array.
			if ((strcmp(&macid[j][18],nullmac))!=0){ //Checking for Null MAC address
				count++;
			}

		}
		else{
			fprintf(fpW, "\nNo. of valid MAC IDs detected from %02d:00:00 to %02d:59:59 = %d", (time[j]/10000),(time[j]/10000),count); //Print to file
			count=0; //Reset the value of count
		}
	
		j++;
	
	}
	return 1;
}
