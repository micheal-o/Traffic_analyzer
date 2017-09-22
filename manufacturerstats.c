//About :This code generates a report that shows the number of macids detected per manufacturer per sensor

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.c"

int temp1;
float temp3;
char selectedmac[18],temp4[9],temp5[7];
char threebytes[7];//holds the first three bytes of a macaddress e.g 00EFBC
char* getfield(char* line, int num);//used with fgets() to get a specific field in the line/string read

int genstats(FILE *fpreg,FILE *fpsen,FILE *fprep){
	char oui[7],company[100];
	strcpy(company,"Unknown");
	insertFirst(company, 0);//Add first node to the linkedlist of companies, 0 means 0 mac ids detected for the company
	while(fscanf(fpsen, "%d %s %f %s %s",&temp1,selectedmac,&temp3,temp4,temp5)==5){//reads a line in sensor file
		if(strcmp(selectedmac,"00:00:00:00:00:00")!=0){//ignore if invalid mac
			memset(threebytes,0,sizeof(threebytes));
			sprintf(threebytes,"%c%c%c%c%c%c",selectedmac[0],selectedmac[1],selectedmac[3],selectedmac[4],selectedmac[6],selectedmac[7]);
			char line[1024];
			while (fgets(line, 1024, fpreg)){//read the IEEE MACID registry file
				char* tmp = strdup(line);
				char field[100];
				strcpy(field,getfield(tmp, 2));//get the OUI field in the line read
				if(strcmp(threebytes,field)==0){
					char* tmp = strdup(line);
					strcpy(field,getfield(tmp, 3));//get the organisation name field in the line read
					strcpy(company,field);
					break;
				}else{memset(line,0,sizeof(line));continue;}
				memset(line,0,sizeof(line));
			}
			struct node *searchnode= find(company);
			if(searchnode != NULL){//if the company is already on the list
				searchnode->data += 1;//increment its count data
			}
			else {//if not
				insertFirst(company, 1);//add the company to the linkedlist and set count data as 1
			}
		}else{continue;}
		memset(&temp1,0,sizeof(temp1));
		memset(selectedmac,0,sizeof(selectedmac));
		memset(&temp3,0,sizeof(temp3));
		memset(temp4,0,sizeof(temp4));
		memset(temp5,0,sizeof(temp5));	
	}
	printList(fprep);//print the linkedlist of companies to the report,the linkedlist has companies and their count
	fclose(fpreg);
	fclose(fpsen);
	fclose(fprep);
	return 1;
}

char* getfield(char* line, int num){//to get a specific field in the line read by fgets()
    char* tok;
    for (tok = strtok(line, ",");tok && *tok;tok = strtok(NULL, ",\n")){
        if (!--num)
            return tok;
    }
    return NULL;
}
