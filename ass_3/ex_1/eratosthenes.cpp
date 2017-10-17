//
//  main.cpp
//  cplustest
//
//  Created by Adam Woods on 2017-10-05.
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
#include <chrono>

using namespace std;

#define NUM_THREADS 4

struct thread_args {
    vector<int> indexPositions;
    vector<int> primeMultiples;
    vector<bool> listToCheck;
};

void printList(vector<int> list, string print) {
    
    for (int i = 0; i < list.size(); i++) {
        cout << print << list[i] << "\n";
    }
    cout << "\n";
}

vector<int> filterList(vector<int> listUnfiltered, int max) {
    
    vector<int> listFiltered;
    
#pragma omp for
    for (int x = 0; x < max; x++) {
        if (listUnfiltered[x] != 0 && listUnfiltered[x] != 1) {
            listFiltered.push_back(listUnfiltered[x]);
        }
    }
    
    return listFiltered;
}

vector<bool> filterListBool(vector<bool> listUnfiltered, unsigned long max) {
    
    vector<bool> listFiltered;
    
    for (int x = 0; x < max; x++) {
        if (listUnfiltered[x] == false) {
            listFiltered.push_back(listUnfiltered[x]);
        }
    }
    
    return listFiltered;
}

vector<bool> fillVector(int startIndex, int length) {
    vector<bool> a(length, false);
    
    return a;
}


int main(int argc, const char * argv[]) {
    // insert code here...
    if (argc == 1) {
        cout << "Usage: ./main 'number'.\n";
    }else {
        
        int threadCount = thread::hardware_concurrency();
        //int chunks = threadCount/2;
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
            //cout << "Finished checking for multiples of " << k << "\n";
        }
        
        // Filter out zeroes from above list, use this list to compare the other lists
        vector<int> primeMultiples = filterList(listUnfiltered, squareOfMax);
        
        
        // Fill a list of remaining ints starting from +1 of the filtered list's last number
        iota(listRemaining.begin(), listRemaining.end(), (primeMultiples.back() + 1));
        //printList(listRemaining, "Unfiltered :");
        
        // Checks the remaining list for multiples of the filtered primes and sets these to zero
        auto start = std::chrono::high_resolution_clock::now();
        int tid;
        for (int i = 0; i < listRemaining.size(); i++) {
            for (int y = 0; y < primeMultiples.size(); y++) {
                if (listRemaining[i] % primeMultiples[y] == 0) {
                    //tid = __builtin_omp_get_thread_num();
                    //printf("Thread id: %d \n", tid);
                    listRemaining[i] = 0;
                }
            }
        }
        
        
        vector<int> result = filterList(listRemaining, listRemaining.size());
        
        //printList(result, "Result: ");
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "Elapsed time: " << elapsed.count() << " s\n";
        
        
        
        
        
    }
    
    return 0;
}
