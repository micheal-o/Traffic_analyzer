//About : This code matches the data in one sensor file with another sensor file, produces the report
//				& also produces a report of outliers using some statistics formulas.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "quicksort.c"

int temp1;
float temp3;
char selectedmac[18],selecteddate[9],selectedtime[7];
struct readings{ //to store the matched record in both sensor files
	char mac[18];
	char ddate1[9];
	char dtime1[7];
	char ddate2[9];
	char dtime2[7];
	double travelt;
	double speed;
}rd;
struct tm date_1;
struct tm date_2;
time_t time_1,time_2;
char (*maclist)[18];//list of already matched mac address, if the mac add occurs again in the file, ignore
double *speedlist; //pointer to a list of all speeds calculated, for statistical analysis
int spdlistcount=0;
int checkmac(char mac[18]);//checks if the mac has been matched previously,if not, add it to the maclist and match
//int linescount(FILE *fp);
int listcount=0;
struct normalspeed{//stores the thresholds, result of analyzing the speedlist
	double minval;
	double maxval;
}normspeed;
void getoutliers(FILE *fprep); //generates report of outliers
double firstquartile(double spdlist[],int spdcount);//calculates the first quartile of the speedlist
double thirdquartile(double spdlist[],int spdcount);//calculates the third quartile of the speedlist
void normspeedcalc(double firstquartile, double thirdquartile);//calculates the normal speed range

int matchdata(FILE *fp1,FILE *fp2,FILE *fpreport){
	//FILE *fp1= fopen("Sensor Q-data.txt","r");//Sensor Q-data
	//FILE *fp2= fopen("Sensor R-data.txt","r");//Sensor R-data
	//FILE *fprep= fopen("sensorsmatchreport.txt","w");
	if (fp1 == NULL || fp2 == NULL || fpreport == NULL){ //Checking if the file pointers are invalid
		printf("Cannot open the input file. Please check the File name or Path \n");
		return -1;
	}
	maclist= malloc(sizeof(char)*1000*18);
	speedlist= malloc(sizeof(double)*1000);
	//printf("mem add: %p \n", maclist);
	while(fscanf(fp1, "%d %s %f %s %s",&temp1,selectedmac,&temp3,selecteddate,selectedtime)==5){ //read first sensor file
			memset(&rd,0,sizeof(rd));
			
			if(strcmp(selectedmac,"00:00:00:00:00:00")!=0){//skip invalid mac 00:00:00:00:00:00
				if(checkmac(selectedmac)==0){//checks if the mac add hasn't been matched previously, returns 0 if not matched before
					strcpy(rd.mac,selectedmac);
					strcpy(rd.ddate1,selecteddate);
					strcpy(rd.dtime1,selectedtime);
					while(fscanf(fp2, "%d %s %f %s %s",&temp1,selectedmac,&temp3,selecteddate,selectedtime)==5){//read second file
						if(strcmp(selectedmac,rd.mac)==0){ //checks if the mac address matches
							strcpy(rd.ddate2,selecteddate); //picks the date and time in the second sensor file
							strcpy(rd.dtime2,selectedtime);
							rewind(fp2);
							break;
						}
					}
				}
				else{continue;}
			}
			else{
				continue;
			}
			/*memset(&temp1,0,sizeof(temp1));
			memset(temp2,0,sizeof(temp2));
			memset(&temp3,0,sizeof(temp3));
			memset(temp4,0,sizeof(temp4));
			memset(temp5,0,sizeof(temp5));*/
			//converting the dates and times into appropriate datetime format according to time.h
			char str[2];
			sprintf(str,"%c%c",rd.dtime1[0],rd.dtime1[1]); date_1.tm_hour= atoi(str);//printf("hour 1: %d \n", date_1.tm_hour);
			sprintf(str,"%c%c",rd.dtime1[2],rd.dtime1[3]); date_1.tm_min=atoi(str); 
			sprintf(str,"%c%c",rd.dtime1[4],rd.dtime1[5]); date_1.tm_sec=atoi(str); 
			sprintf(str,"%c%c",rd.ddate1[4],rd.ddate1[5]); date_1.tm_mon=atoi(str); 
			sprintf(str,"%c%c",rd.ddate1[6],rd.ddate1[7]); date_1.tm_mday=atoi(str); 
			sprintf(str,"%c%c",rd.ddate1[2],rd.ddate1[3]); date_1.tm_year=atoi(str);
			
			sprintf(str,"%c%c",rd.dtime2[0],rd.dtime2[1]); date_2.tm_hour= atoi(str); 
			sprintf(str,"%c%c",rd.dtime2[2],rd.dtime2[3]); date_2.tm_min=atoi(str); 
			sprintf(str,"%c%c",rd.dtime2[4],rd.dtime2[5]); date_2.tm_sec=atoi(str); 
			sprintf(str,"%c%c",rd.ddate2[4],rd.ddate2[5]); date_2.tm_mon=atoi(str); 
			sprintf(str,"%c%c",rd.ddate2[6],rd.ddate2[7]); date_2.tm_mday=atoi(str); 
			sprintf(str,"%c%c",rd.ddate2[2],rd.ddate2[3]); date_2.tm_year=atoi(str);
			
			time_1= mktime(&date_1);
			time_2= mktime(&date_2);
			rd.travelt= difftime(time_2,time_1);//time difference in seconds
			rd.speed= (double) (1.86/(double)(rd.travelt/3600)); //speed in miles per hour
			if(strcmp(rd.ddate2,"")==0 && strcmp(rd.dtime2,"")==0){//if no match in the entire second sensor file
				rd.travelt =0.0;
				rd.speed =0.0;
			}

			if(spdlistcount== ((int) sizeof(speedlist)/sizeof(double))){//resizing speedlist if full
				speedlist= realloc(speedlist,sizeof(speedlist)+(1000*sizeof(double)));
			}
			speedlist[spdlistcount]=rd.speed;//add speed to speedlist
			spdlistcount++;
			fprintf(fpreport,"%s %s/%s %s/%s %lf %lf \n",rd.mac,rd.ddate1,rd.dtime1,rd.ddate2,rd.dtime2,rd.travelt,rd.speed);
	}
	free(maclist);
	//speedlist= malloc(sizeof(double)*1000);
	quickSort(speedlist,0,spdlistcount-1);//sort the list of speeds in ascending order
	normspeedcalc(firstquartile(speedlist,spdlistcount),thirdquartile(speedlist,spdlistcount));
	free(speedlist);
	getoutliers(fpreport);
	fclose(fpreport);
	//fclose(fp1);
	//fclose(fp2);
	return 1;
}
//another method getoutliers
void getoutliers(FILE *fprep){//generate outliers report
	FILE *fprep2= fopen("outliersreport.txt","w");
	if (fprep2 == NULL){ //Checking if fp is valid
		printf("Cannot open/create the required outliersreport.txt file. Please check the File name or Path \n");
		return;
	}
	else if(fprep==NULL){
		printf("Cannot open/find the required sensorsmatchreport.txt file. Please check the File name or Path \n");
		return;
	}
	rewind(fprep);
	int outlcount=0; char temp1[18], temp2[18], temp3[18]; double temp4, selectedspeed;
	fprintf(fprep2,"Outlier criteria....above maxspeed: %lf MPH....below minspeed: %lf MPH \n", normspeed.maxval, normspeed.minval);
	while(fscanf(fprep, "%s %s %s %lf %lf",temp1,temp2,temp3,&temp4,&selectedspeed)==5){
		if(selectedspeed > normspeed.maxval || selectedspeed < normspeed.minval){ //outlier condition
			fprintf(fprep2,"%s %s %s %lf %lf \n",temp1,temp2,temp3,temp4,selectedspeed);
			outlcount++;
		}
	}
	if(outlcount==0){fprintf(fprep2,"No outliers found in the sensors data match report \n");}
	fclose(fprep2);
	return;
}

int checkmac(char mac[18]){ //check if a mac address is in the maclist
	
	for(int i=0;i<= listcount;i++){
		if(strcmp(maclist[i],mac)==0){
			return 1;//if found
		}else{continue;}
	}
	if(listcount== ((int) sizeof(maclist)/18)){//resizing the maclist if full
		maclist= realloc(maclist,sizeof(maclist)+(1000*18*sizeof(char)));
	}
	strcpy(maclist[listcount],mac);//if not found
	//printf("maclist[%d]: %s \n", listcount,mac);
	listcount++;
	return 0;
}

double firstquartile(double spdlist[],int spdcount){//calculating the first quartile of the speedlist
	int mid; //position of the median in the list
	int lowercount;//count of items less than median
	if((spdcount % 2)==1){ //if the number of items in the list is odd
		mid= (int)ceil((double) spdcount/2)-1;
		lowercount= mid;
		if((lowercount % 2)==1){ //if the number of items in the lower list is odd
			return spdlist[(int)ceil((double) lowercount/2)-1];
		}else{ //if the number of items in the lower list is even
			return (double)(spdlist[lowercount/2]+spdlist[(lowercount/2)-1])/2;
		}
	}
	else{ //if the number of items in the list is even
		lowercount= spdcount/2;
		if((lowercount % 2)==1){ //if the number of items in the lower list is odd
			return spdlist[(int)ceil((double) lowercount/2)-1];
		}else{ //if the number of items in the lower list is even
			return (double)(spdlist[lowercount/2]+spdlist[(lowercount/2)-1])/2;
		}
	}
}

double thirdquartile(double spdlist[],int spdcount){//calculating the third quartile of the speedlist
	int mid; //position of the median in the list
	int uppercount;//count of items greater than median
	if(spdcount%2==1){ //if the number of items in the list is odd
		mid= (int)ceil((double) spdcount/2)-1;
		uppercount= mid;
		if(uppercount%2==1){ //if the number of items in the upper list is odd
			return spdlist[mid + (int)ceil((double) uppercount/2)];
		}else{ //if the number of items in the upper list is even
			return (double)(spdlist[mid + (uppercount/2)]+spdlist[mid + (uppercount/2) + 1])/2;
		}
	}
	else{ //if the number of items in the list is even
		mid= (spdcount/2)-1;
		uppercount= spdcount/2;
		if(uppercount%2==1){ //if the number of items in the upper list is odd
			return spdlist[mid + (int)ceil((double) uppercount/2)];
		}else{ //if the number of items in the upper list is even
			return (double)(spdlist[mid + (uppercount/2)]+spdlist[mid + (uppercount/2) + 1])/2;
		}
	}
}

void normspeedcalc(double firstquartile, double thirdquartile){//calculates the normal speed range
	double iqr= thirdquartile - firstquartile; //interquartile range
	normspeed.minval = firstquartile - (1.5 * iqr);
	normspeed.maxval= thirdquartile + (1.5 * iqr);
	//printf("max & min: %lf %lf \n",normspeed.maxval,normspeed.minval );
	return;
}
