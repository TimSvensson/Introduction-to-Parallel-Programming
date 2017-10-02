//
//  main.cpp
//  PosixThreads
//
//  Created by Adam Woods on 2017-10-01.
//  Copyright © 2017 Adam Woods. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <list>
#include <numeric>
#include <random>
#include <thread>



using namespace std;


#define NUM_THREADS 5

void *PrintHello(void *threadid) {
    long tid;
    tid = (long)threadid;
    cout << "Hello World! Thread ID, \n" << tid << endl;
    pthread_exit(NULL);
}

void printList(vector<int> list, string print) {
    
    for (int i = 0; i < list.size(); i++) {
        cout << print << list[i] << "\n";
    }
}

vector<int> filterList(vector<int> listUnfiltered, int max) {
    
    vector<int> listFiltered;
    
    for (int x = 0; x < max; x++) {
        if (listUnfiltered[x] != 0 && listUnfiltered[x] != 1) {
            listFiltered.push_back(listUnfiltered[x]);
        }
    }
    
    //printList(listFiltered, "Filtered list contents: ");
    return listFiltered;
}

int main (int argc, char *argv[]) {
    
    if (argc == 1) {
        cout << "Usage: ./main 'number'.\n";
    } else {
        
        int threadCount = thread::hardware_concurrency();
        int chunks = threadCount/2;
        int max = stoi(argv[1]);
        int squareOfMax = ceil(sqrt(max)) + 1;
        
        vector<int> listUnfiltered(max), listRemaining(max - squareOfMax);
        iota(listUnfiltered.begin(), listUnfiltered.end(), 1);
        
        // Loops through a list of integers and sets any non-prime to zero
        for (int k = 2; k*k <= squareOfMax; k++) {
            for (int i = k*k; i <= squareOfMax; i++) {
                if (listUnfiltered[i - 1] % k == 0  )  {
                    listUnfiltered[i - 1 ] = 0;
                }
            }
            cout << "Finished checking for multiples of " << k << "\n";
        }
        
        // Filter out zeroes from above list
        vector<int> listFiltered = filterList(listUnfiltered, squareOfMax);
        
        // Fill a list of remaining ints starting from +1 of the filtered list's last number
        iota(listRemaining.begin(), listRemaining.end(), (listFiltered.back() + 1));
        
        // Checks the remaining list for multiples of the filtered primes and sets these to zero
        for (int i = 0; i < listRemaining.size(); i++) {
            for (int y = 0; y < listFiltered.size(); y++) {
                if (listRemaining[i] % listFiltered[y] == 0) {
                    listRemaining[i] = 0;
                }
            }
        }
        cout << "listRemaining count: " << listRemaining.size() << "\n";
        // Final result of doing things sequentially
        vector<int> finalList = filterList(listRemaining, listRemaining.size());
        
        
        
        
        /*
         Principle: Calculate primes in sqrt(max) + 1,
         
         
         
         To paralellize:
         Array must be divided in P chunks
         Divide up the array in chunks with decreasing amount of numbers according to:
         For loop:
         ( (Number count of array) / cores ) * 1/x
         Store result in an array
         Go through the array after sqrt(max), iterate numbers from result array position and send to thread
         */
    }
    
    return 0;
    
    
    
    
    /*
     
     pthread_t threads[NUM_THREADS];
     int rc;
     int i;
     
     for( i = 0; i < NUM_THREADS; i++ ) {
     cout << "main() : creating thread, " << i << endl;
     rc = pthread_create(&threads[i], NULL, PrintHello, (void *)i);
     
     if (rc) {
     cout << "Error:unable to create thread," << rc << endl;
     exit(-1);
     }
     }
     pthread_exit(NULL);
     
     */
}

