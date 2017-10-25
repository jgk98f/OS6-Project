#ifndef RESOURCE_H
#define RESOURCE_H

#include <stdio.h>          
#include <stdlib.h>         
#include <getopt.h>         
#include <ctype.h>          
#include <time.h>           
#include <sys/time.h>       
#include <stdbool.h> 
#include <iostream> 
#include <unistd.h>      
#define frameSize 5

typedef struct page {
    int pageNumber;
    struct page *nextPage;
}page_t;

page_t *head;
int currentPointer;
int** createArray();
bool hasPageBeenRequested(int, int, int *);
void print(int **);
void cleanup(int **, int *);
void getOptimal(int *);
bool inFrame(int [], int );
page_t *createFrame();
bool hasFaultOccurred(int);
void replacePage(int);
#endif
