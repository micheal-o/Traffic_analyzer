//About :This code is the main program and executes every other module

#include <stdio.h>
#include <stdlib.h>
#include "hourlystats.c"
#include "uniquemac.c"
#include "manufacturerstats.c"
#include "sensorsmatch.c"


int main(){	
	int errcatch;//for error catching purpose
	//file pointer error handling is done within each module called with a file pointer
	FILE *fpQ, *fpR, *fpreport;
	fpQ = fopen("Sensor Q-data.txt","r");
	fpR = fopen("Sensor R-data.txt","r");
	
	//Execution the hourly valid mac statistics module
	fpreport = fopen("Hourly_Statistics_Sensor_Q.txt","w");
	if(fpreport != NULL){
		fprintf(fpreport,"Hourly valid mac detection Statistics for Sensor Q: \n");
		errcatch=0;
		errcatch= reportGenerator(fpQ, fpreport); //Calling reportGenerator function for Sensor Q file
		if(errcatch == 1){
			printf(">>> Hourly valid mac statistics for Sensor Q:: Hourly_Statistics_Sensor_Q.txt  \n ");
		}
		fclose(fpreport);
	}
	else {
		printf("Cannot create/open the required Hourly_Statistics_Sensor_Q.txt file. Please check the File name or Path \n");
	}
	
	fpreport = fopen("Hourly_Statistics_Sensor_R.txt","w");
	//
	if(fpreport != NULL){
		fprintf(fpreport,"Hourly valid mac detection Statistics for Sensor R: \n"); //Calling reportGenerator function for Sensor R file
		errcatch=0;
		errcatch= reportGenerator(fpR, fpreport);
		if(errcatch == 1){
			printf(">>> Hourly valid mac statistics for Sensor R :: Hourly_Statistics_Sensor_R.txt \n ");
		}
		fclose(fpreport);
	}
	else {
		printf("Cannot create/open the required Hourly_Statistics_Sensor_R.txt file. Please check the File name or Path \n");
	}
	//end of hourly valid mac statistics module execution
	rewind(fpQ);
	rewind(fpR);
		
	//Execution of count of unique mac detection per sensor module
	fpreport = fopen("unique_macid_count_report.txt","w");
	fprintf(fpreport,"Total number of Unique Mac ID detected by Sensor Q: %d \n",genuniqcount(fpQ));
	fclose(fpreport);
	fpreport = fopen("unique_macid_count_report.txt","a");
	fprintf(fpreport,"\nTotal number of Unique Mac ID detected by Sensor R: %d \n",genuniqcount(fpR));
	fclose(fpreport);
	printf(">>> Total unique macids detected per Sensor :: unique_macid_count_report.txt \n ");
	//end of Execution of count of unique mac detection per sensor module
	
	rewind(fpQ);
	rewind(fpR);
	
	//Execution of number of detected macids for each manufacturer per sensor
	FILE *fpreg= fopen("IEEE_MACID_REGISTRY.csv","r");
	fpreport = fopen("manufacturerreport.txt","w");
	if(fpreport != NULL && fpreg!= NULL && fpQ != NULL){
		fprintf(fpreport,"Number of detected macids for each manufacturer in Sensor Q : \n");
		errcatch=0;
		errcatch= genstats(fpreg,fpQ,fpreport);
		if(errcatch==1){
			printf(">>> Manufacturer statistics for Sensor Q :: manufacturerreport.txt \n ");
		}
	}
	else {
		printf("Cannot create/open the required manufacturerreport.txt/IEEE_MACID_REGISTRY.csv/Sensor Q-data.txt files. Please check the File name or Path for these files \n");
	}
	rewind(fpreg);
	//fclose(fpreg);
	fclose(fpreport);
	
	//fpreg= fopen("IEEE_MACID_REGISTRY.csv","r");
	fpreport = fopen("manufacturerreport.txt","a");
	if(fpreport != NULL && fpreg!= NULL && fpR != NULL){
		fprintf(fpreport,"\nNumber of detected macids for each manufacturer in Sensor R : \n");
		errcatch=0;
		errcatch= genstats(fpreg,fpR,fpreport);
		if(errcatch==1){
			printf(">>> Manufacturer statistics for Sensor R :: manufacturerreport.txt \n ");
		}
	}
	else {
		printf("Cannot create/open the required manufacturerreport.txt/IEEE_MACID_REGISTRY.csv/Sensor R-data.txt files. Please check the File name or Path for these files \n");
	}
	fclose(fpreg);
	fclose(fpreport);
	
	//End of execution of number of detected macids for each manufacturer per sensor
	
	rewind(fpQ);
	rewind(fpR);	

	//Execution of sensors reading match and outlier report
	fpreport= fopen("sensorsmatchreport.txt","w");
	if(fpreport != NULL){
		errcatch=0;
		errcatch= matchdata(fpQ,fpR,fpreport);
		fclose(fpreport);
		if(errcatch == 1){
			printf(">>> Sensors data match and outliers :: sensorsmatchreport.txt, outliersreport.txt \n");
		}
	}
	else{
		printf("Cannot create/open the required sensorsmatchreport.txt file. Please check the File name or Path \n");
	}
	//end of Execution of sensors reading match and outlier report
	
	printf("All output reports are available in the current program directory \n ");
	fclose(fpQ);
	fclose(fpR);
	return 0;
}
