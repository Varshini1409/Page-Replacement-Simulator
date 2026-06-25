#include "Algorithms.h"
#include "Utils.h"

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <iomanip>

using namespace std;

Result FIFO(vector<int>& pages, int capacity, bool showSteps) {
    if(showSteps) cout << "\n===== FIFO PAGE REPLACEMENT =====\n";

    vector<int> frames(capacity, -1);
    queue<int> q;
    unordered_map<int, bool> inFrame;

    int faults = 0, hits = 0;

    for (int page : pages) {
        if(showSteps) cout << "\nPage " << page << " -> ";

        if (inFrame[page]) {
            hits++;
            if(showSteps){
                displayFrames(frames, capacity);
                cout << " HIT";
            }
        } else {
            faults++;

            if ((int)q.size() < capacity) {
                for (int i = 0; i < capacity; i++) {
                    if (frames[i] == -1) {
                        frames[i] = page;
                        break;
                    }
                }
            } else {
                int victim = q.front();
                q.pop();
                inFrame[victim] = false;

                for (int i = 0; i < capacity; i++) {
                    if (frames[i] == victim) {
                        frames[i] = page;
                        break;
                    }
                }
            }

            q.push(page);
            inFrame[page] = true;

            if(showSteps){
                displayFrames(frames, capacity);
                cout << " FAULT";
            }
        }
    }

    if(showSteps){
        cout << "\n\nTotal Hits   : " << hits;
        cout << "\nTotal Faults : " << faults;
        cout << "\nHit Ratio    : " << fixed << setprecision(2)
             << (double)hits / pages.size();
        cout << "\nFault Ratio  : "
             << (double)faults / pages.size() << "\n";
    }

    return {hits, faults};
}

Result LRU(vector<int>& pages, int capacity, bool showSteps) {
    if(showSteps) cout << "\n===== LRU PAGE REPLACEMENT =====\n";

    vector<int> frames;
    unordered_map<int,int> lastUsed;
    int hits=0, faults=0;

    for(int i=0;i<(int)pages.size();i++) {
        int page = pages[i];

        if(showSteps) cout << "\nPage " << page << " -> ";

        bool found = false;
        for(int p : frames)
            if(p == page) found = true;

        if(found){
            hits++;
            lastUsed[page] = i;
            if(showSteps){
                displayFrames(frames, capacity);
                cout << " HIT";
            }
        } else {
            faults++;

            if((int)frames.size() < capacity){
                frames.push_back(page);
            } else {
                int lruPage = frames[0];
                int minIdx = lastUsed[lruPage];

                for(int p : frames){
                    if(lastUsed[p] < minIdx){
                        minIdx = lastUsed[p];
                        lruPage = p;
                    }
                }

                for(int j=0;j<(int)frames.size();j++){
                    if(frames[j] == lruPage){
                        frames[j] = page;
                        break;
                    }
                }
            }

            lastUsed[page] = i;

            if(showSteps){
                displayFrames(frames, capacity);
                cout << " FAULT";
            }
        }
    }

    if(showSteps){
        cout << "\n\nTotal Hits   : " << hits;
        cout << "\nTotal Faults : " << faults;
        cout << "\nHit Ratio    : " << fixed << setprecision(2)
             << (double)hits / pages.size();
        cout << "\nFault Ratio  : "
             << (double)faults / pages.size() << "\n";
    }

    return {hits, faults};
}

Result Optimal(vector<int>& pages, int capacity, bool showSteps) {
    if(showSteps) cout << "\n===== OPTIMAL PAGE REPLACEMENT =====\n";

    vector<int> frames;
    int hits=0, faults=0;

    for(int i=0;i<(int)pages.size();i++) {
        int page = pages[i];

        if(showSteps) cout << "\nPage " << page << " -> ";

        bool found=false;
        for(int p : frames)
            if(p == page) found=true;

        if(found){
            hits++;
            if(showSteps){
                displayFrames(frames, capacity);
                cout << " HIT";
            }
        } else {
            faults++;

            if((int)frames.size() < capacity){
                frames.push_back(page);
            } else {
                int replaceIndex = 0;
                int farthest = -1;

                for(int j=0;j<(int)frames.size();j++){
                    int k;
                    for(k=i+1;k<(int)pages.size();k++){
                        if(frames[j] == pages[k]) break;
                    }

                    if(k == (int)pages.size()){
                        replaceIndex = j;
                        break;
                    }

                    if(k > farthest){
                        farthest = k;
                        replaceIndex = j;
                    }
                }

                frames[replaceIndex] = page;
            }

            if(showSteps){
                displayFrames(frames, capacity);
                cout << " FAULT";
            }
        }
    }

    if(showSteps){
        cout << "\n\nTotal Hits   : " << hits;
        cout << "\nTotal Faults : " << faults;
        cout << "\nHit Ratio    : " << fixed << setprecision(2)
             << (double)hits / pages.size();
        cout << "\nFault Ratio  : "
             << (double)faults / pages.size() << "\n";
    }

    return {hits, faults};
}

void compareAll(vector<int>& pages, int frames){
    Result fifo = FIFO(pages, frames, false);
    Result lru = LRU(pages, frames, false);
    Result opt = Optimal(pages, frames, false);

    cout << "\n========================================\n";
    cout << "      PERFORMANCE COMPARISON\n";
    cout << "========================================\n\n";
    cout << left << setw(12) << "Algorithm"
         << setw(10) << "Hits"
         << setw(10) << "Faults" << "\n";

    cout << "----------------------------------------\n";
    cout << setw(12) << "FIFO" << setw(10) << fifo.hits << setw(10) << fifo.faults << "\n";
    cout << setw(12) << "LRU" << setw(10) << lru.hits << setw(10) << lru.faults << "\n";
    cout << setw(12) << "Optimal" << setw(10) << opt.hits << setw(10) << opt.faults << "\n";

    string best = "FIFO";
    int minFaults = fifo.faults;

    if(lru.faults < minFaults){ best = "LRU"; minFaults = lru.faults; }
    if(opt.faults < minFaults){ best = "Optimal"; minFaults = opt.faults; }

    cout << "\nBest Algorithm: " << best << "\n";
}

void beladyDemo(){
    vector<int> pages = {1,2,3,4,1,2,5,1,2,3,4,5};

    Result f3 = FIFO(pages, 3, false);
    Result f4 = FIFO(pages, 4, false);

    cout << "\n===== BELADY'S ANOMALY DEMO =====\n";
    cout << "Frames = 3 -> Faults = " << f3.faults << "\n";
    cout << "Frames = 4 -> Faults = " << f4.faults << "\n";

    if(f4.faults > f3.faults)
        cout << "\nBELADY'S ANOMALY DETECTED!\n";
    else
        cout << "\nNo anomaly detected.\n";
}