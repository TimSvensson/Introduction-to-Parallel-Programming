
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

int main (int argc, char *argv[]) {
    
    if (argc == 1) {
        cout << "Usage: ./main 'number'.\n Second flags: print=filtered, print=unfiltered \n";
    } else {
        
        int threadCount = std::thread::hardware_concurrency();
        int chunks = threadCount/2;
        int max = std::stoi(argv[1]);
        
        std::vector<int> listUnfiltered(max);
        std::iota(listUnfiltered.begin(), listUnfiltered.end(), 1);
        std::vector<int> listFiltered;
        
        cout << "Number of theads in system: " << threadCount << "\n";
        // Loops through a list of integers and sets any non-prime to zero
        for (int k = 2; k*k <= ceil(sqrt(max)); k++) {
            for (int i = k*k; i < max; i++) {
                if (listUnfiltered[i - 1] % k == 0  )  {
                    listUnfiltered[i - 1 ] = 0;
                }
            }
            cout << "Finished checking for multiples of " << k << "\n";
        }
        
        // Loops through a list and adds any non-zero to a new list
        for (int x = 0; x < max; x++) {
            if (listUnfiltered[x] != 0 && listUnfiltered[x] != 1) {
                listFiltered.push_back(listUnfiltered[x]);
            }
        }
        
        if (argv[2]) {
            if (!strcmp(argv[2], "print=unfiltered")) {
                for (int i = 0; i < max; i++) {
                    cout << "Unfiltered list: " << listUnfiltered[i] << "\n";
                }
            } else if (!strcmp(argv[2], "print=filtered")) {
                for (int i = 0; i < listFiltered.size(); i++) {
                    cout << "Filtered list: " << listFiltered[i] << "\n";
                }
            }
            
        }
        
        /*
         To paralellize:
         Divide up the array in chunks with decreasing amount of numbers according to:
         For loop:
         ( (Number count of array) / cores ) * 1/x
         Store result in an array
         Go through the array after sqrt(max), iterate numbers from result array position and send to thread
         */
    }
    
    
    
    
    
    
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
