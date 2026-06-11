
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <iomanip>
using namespace std;

struct Result {
    int hits;
    int faults;
};

void displayFrames(const vector<int>& frames, int capacity) {
    cout << "[ ";
    for (int i = 0; i < capacity; i++) {
        if (i < (int)frames.size() && frames[i] != -1)
            cout << frames[i] << " ";
        else
            cout << "_ ";
    }
    cout << "]";
}

Result FIFO(vector<int>& pages, int capacity, bool showSteps=true) {
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

Result LRU(vector<int>& pages, int capacity, bool showSteps=true) {
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

Result Optimal(vector<int>& pages, int capacity, bool showSteps=true) {
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

bool loadFromFile(vector<int>& pages, int& frames){
    ifstream fin("input.txt");
    if(!fin) return false;

    int n;
    fin >> n;

    pages.resize(n);
    for(int i=0;i<n;i++) fin >> pages[i];

    fin >> frames;
    return true;
}

int main(){
    vector<int> pages;
    int frames;

    cout << "Load data from input.txt? (1=Yes, 0=No): ";
    int choiceInput;
    cin >> choiceInput;

    if(choiceInput == 1){
        if(!loadFromFile(pages, frames)){
            cout << "input.txt not found!\n";
            return 0;
        }
    } else {
        int n;
        cout << "Enter number of page references: ";
        cin >> n;

        pages.resize(n);
        cout << "Enter reference string:\n";
        for(int i=0;i<n;i++) cin >> pages[i];

        cout << "Enter number of frames: ";
        cin >> frames;
    }

    int choice;

    do{
        cout << "\n\n===== PAGE REPLACEMENT SIMULATOR =====\n";
        cout << "1. FIFO\n";
        cout << "2. LRU\n";
        cout << "3. Optimal\n";
        cout << "4. Compare All\n";
        cout << "5. Belady's Anomaly Demo\n";
        cout << "6. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch(choice){
            case 1: FIFO(pages, frames); break;
            case 2: LRU(pages, frames); break;
            case 3: Optimal(pages, frames); break;
            case 4: compareAll(pages, frames); break;
            case 5: beladyDemo(); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid Choice!\n";
        }
    }while(choice != 6);

    return 0;
}
