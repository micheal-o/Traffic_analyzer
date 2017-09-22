#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct node {
   int data;
   char key[100];
   struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

//display the list
void printList(FILE *fp) {
   struct node *ptr = head;
   
   //start from the beginning
   while(ptr != NULL) {
      fprintf(fp,"%s: %d \n",ptr->key,ptr->data);
      ptr = ptr->next;
   }
	
}

//insert link at the first location
void insertFirst(char key[100], int data) {
   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   strcpy(link->key,key);
   link->data = data;
	
   //point it to old first node
   link->next = head;
	
   //point first to new first node
   head = link;
}

//is list empty
bool isEmpty() {
   return head == NULL;
}

int length() {
   int length = 0;
   struct node *current;
	
   for(current = head; current != NULL; current = current->next) {
      length++;
   }
	
   return length;
}

//find a link with given key
struct node* find(char key[100]) {

   //start from the first link
   struct node* current = head;

   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(strcmp(current->key,key)!=0) {
	
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
	
   //if data found, return the current Link
   return current;
}


void sort() {

   int i, j, k, tempData;
   char tempKey[100];
   struct node *current;
   struct node *next;
	
   int size = length();
   k = size ;
	
   for ( i = 0 ; i < size - 1 ; i++, k-- ) {
      current = head;
      next = head->next;
		
      for ( j = 1 ; j < k ; j++ ) {   
		
         if ( strcmp(current->key,next->key) > 0 ) {
            tempData = current->data;
            current->data = next->data;
            next->data = tempData;

            strcpy(tempKey,current->key);
            strcpy(current->key,next->key);
            strcpy(next->key,tempKey);
         }
			
         current = current->next;
         next = next->next;
      }
   }   
}
