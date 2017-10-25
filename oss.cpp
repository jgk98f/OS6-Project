#include "resource.h"
using namespace std;

/**
 * Author: Jason Klamert
 * Date: 12/2/2016
 * Description: This program uses a 2d array to represent paging algorithms used in operating systems
 * and their respective fault rates. In addition, these are compared to the most optimal replacement possible.
 **/
int main() {
    srand(time(NULL));
    int **processes = createArray(); 
    int *requests;
    int faults = 0, success = 0, pageCount = 0;
    int *pageRequest = (int *)malloc(sizeof(int) * 150);
    
    //Initialize head of linked list.
    head = createFrame();

    //Print the processes for the user so we can see the page distribution.
    print(processes);

    page *current = head;
    //Generate the pageRequest randomly for the request stream.
    for (int i = 0; i < 10; i++) 
    {
        for (int j = 0; j < 15; j++) 
        {
            int pageRequested = rand() % 8;
            pageRequest[pageCount] = processes[i][pageRequested];
            pageCount++;
        }
    }
    //Go through the page request one by one and determine if a fault occurs. Increment the proper counter.
    for (int i = 0; i < 150; i++) 
    {
        if (hasFaultOccurred(pageRequest[i])) 
        {
           faults++;
           replacePage(pageRequest[i]); 
        } 
        else
        {
              success++;
        }
    }
    //Report some statistics for the user.
    std::cout << endl;
    std::cout << "-----------------------------------------" << endl;
    std::cout << "REPORT: " << endl;
    std::cout << "-----------------------------------------" << endl;
    std::cout << "Using Frame Size: " << frameSize << std::endl;
    std::cout << "Using Paging Algorithm: FIFO" << std::endl;  
    std::cout << "Page Faults: " << faults << std::endl;
    std::cout << "Number Of Pages Found W/Out Fault: " << success << std::endl;
    std::cout << "-----------------------------------------" << endl;
    
    //Obtain the optimal page faults/successes.
    getOptimal(pageRequest); 
    
    //Clean up the memory used for the linked list.
    cleanup(processes, pageRequest);
    
    return 0;
}

/**
 * Author: Jason Klamert
 * Date: 12/2/2016
 * Description: Create the array that will be used to symbolically represent processes and their pages.
 **/
int** createArray() 
{
    //Create the array using malloc.
    int **parray; 
    int pageNumber = 0;
    parray = (int **)malloc(sizeof(int *) * 10);
    
    //Loop through our process array and malloc 8 integers per process for the pages.
    for (int n = 0; n < 10; n++) 
    {
        parray[n] = (int *) malloc(sizeof(int) * 8);
        for (int k = 0; k < 8; k++) 
        {
            do 
            {
                //This must run at least once so the do-while makes sure
                //that the page number is not duplicate in the current process.
                pageNumber = 1 + rand() % 50;
            } while (hasPageBeenRequested(pageNumber, k, parray[n]));
            parray[n][k] = pageNumber;
        }
    }
    
    //Return our newly generated array.
    return parray;
}

/**
 * Author: Jason Klamert
 * Date: 12/2/2016
 * Description: Function to make sure that there are no duplicate pages in a process.
 **/
bool hasPageBeenRequested(int page, int pageCount, int* process) {
     
    if (pageCount == 0)
    {
        return false;
    }
    else
    {
         for (int i = 0; i < pageCount; i++)
         {
             if (process[i] == page)
             {
                return true;
             }
         }
    }
    
    return false;
}

/**
 * Author: Jason Klamert
 * Date: 12/2/2016
 * Description: Function to print out array in a nice fashion.
 **/
void print(int **arr) 
{
    for (int n = 0; n < 10; n++) 
    {
        std::cout << "Process " << n + 1 << " Pages: ";
        
        for (int k = 0; k < 8; k++) 
        {
            std::cout << arr[n][k] << " ";
        }
        
        std::cout << std::endl;
    }
}

/**
 * Author: Jason Klamert
 * Date: 12/2/2016
 * Description: Function to obtain the optimal replacement and count the faults/successes.
 **/
void getOptimal(int *pageRequest) {
    
    std::cout << endl;
    std::cout << "-----------------------------------------" << endl;
    std::cout << "Optimal: " << std::endl;
    std::cout << "-----------------------------------------" << endl;
    
    int success = 0, faults = 0, pageCount = 0;
    int maxLoc = 0;
    int frame[frameSize]; 
    
    for (int n = 0; n < frameSize; n++)
    {
        frame[n] = 0;
    }
    
    for (int n = 0; n < 150; n++) 
    {
        //Determine if the page is in frame.
        //If so, increment success counter.
        if (inFrame(frame, pageRequest[n])) 
        {
            success++;
        } 
        else 
        {
            int max = 0;    
            
            //Calculate the distance of each page to its requestPage. 
            //This will allow us to make the best choice for replacement.
            for (int k = 0; k < frameSize; k++) 
            {
                pageCount = n;
                int distance = 0;

                while (pageRequest[pageCount] != frame[k] && pageCount < 150) 
                {
                    pageCount++;
                    distance++;
                }
                if (distance > max) 
                {
                    max = distance;
                    maxLoc = k;
                }
            }
            
            //The best page to replace will be the one with the greatest distance.
            //This will be located in max.
            frame[maxLoc] = pageRequest[n];
            faults++;
        }
    }
    
    std::cout << "Using Frame Size: " << frameSize << std::endl;
    std::cout << "Using Paging Algorithm: OPTIMAL" << std::endl; 
    std::cout << "Page Faults: " << faults << std::endl;
    std::cout << "Successful References: " << success << std::endl;
    std::cout << "-----------------------------------------" << endl;
    std::cout << endl;
   
}

/**
 * Author: Jason Klamert
 * Date: 12/2/2016
 * Description: Function for the optimal solution to determine if page is in frame.
 **/
bool inFrame(int frame[], int pageNumber) 
{
    for (int n = 0; n < frameSize; n++) 
    {
        if (pageNumber == frame[n]) 
        {
            return true;
        }
    }  
    
    return false;
}

/**
 * Author: Jason Klamert
 * Date: 12/2/2016
 * Description: Function to create linked list to simulate our frame to hold pages.
 **/
page_t* createFrame() {
        
    page_t *head = (page_t *)malloc(sizeof(page_t));
    page_t *current;
    head->pageNumber = 0;
    head->nextPage = NULL;
    current = head;
    
    for (int n = 0; n < frameSize - 1; n++) 
    {
        page_t *temp = (page_t *) malloc(sizeof(page_t));
        temp->nextPage = NULL;
        temp->pageNumber = 0;
        current->nextPage = temp;
        current = current->nextPage;
    }
    
    return head;
}

/**
 * Author: Jason Klamert
 * Date: 12/2/2016
 * Description: Function to determine if a fault has occured.
 **/
bool hasFaultOccurred(int pageNumber) {
   
    page_t *currentPtr = head;
    
    //While current is not null, traverse to find our page.
    //If page not found, fault has occured.
    while (currentPtr) 
    {
        if (currentPtr->pageNumber == pageNumber) 
        {
            return false;
        }
        
        currentPtr = currentPtr->nextPage;
    }
    
    return true;
}

/**
 * Author: Jason Klamert
 * Date: 12/2/2016
 * Description: Function to replace a page in a frame.
 **/
void replacePage(int pageNumber) {
     
    page_t *remove = head;
    page_t *current = head->nextPage;
    page_t *temp;
    
    //Maintain head.
    head = head->nextPage;
    
    //Deallocate the previous head of the list.
    free(remove);
    
    //Traverse the list.
    while (current->nextPage) 
    {
        current = current->nextPage;
    }
    
    temp = (page_t *) malloc(sizeof(page_t));
    temp->nextPage = NULL;
    temp->pageNumber = pageNumber;
    current->nextPage = temp;
}

/**
 * Author: Jason Klamert
 * Date: 12/2/2016
 * Description: Function to cleanup the environment's memory.
 **/
void cleanup(int **processes, int *pageRequest) {
    
    int n = 0;
    
    //Free our 2D array.
    for (int i = 0; i < 10; i++)
    {
        free(processes[i]);
    }
    
    free(processes);
    
    //Free the page request array.
    free(pageRequest);
    
    //Free the linked list.
    page_t *remove = head;
    
    while (n < frameSize) 
    {
        page_t *current = remove->nextPage;
        free(remove);
        remove = current;
        n++;
    }
    
    free(head);
    free(remove);
    
}
