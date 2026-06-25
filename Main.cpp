#include <iostream>
#include <vector>

#include "algorithms.h"
#include "fileio.h"

using namespace std;

int main()
{
    vector<int> pages;
    int frames;

    cout << "Load data from input.txt? (1=Yes, 0=No): ";
    int choiceInput;
    cin >> choiceInput;

    if (choiceInput == 1)
    {
        if (!loadFromFile(pages, frames))
        {
            cout << "input.txt not found!\n";
            return 0;
        }
    }
    else
    {
        int n;

        cout << "Enter number of page references: ";
        cin >> n;

        pages.resize(n);

        cout << "Enter reference string:\n";

        for (int i = 0; i < n; i++)
            cin >> pages[i];

        cout << "Enter number of frames: ";
        cin >> frames;
    }

    int choice;

    do
    {
        cout << "\n\n===== PAGE REPLACEMENT SIMULATOR =====\n";
        cout << "1. FIFO\n";
        cout << "2. LRU\n";
        cout << "3. Optimal\n";
        cout << "4. Compare All\n";
        cout << "5. Belady's Anomaly Demo\n";
        cout << "6. Exit\n";
        cout << "Enter Choice: ";

        cin >> choice;

        switch (choice)
        {
            case 1:
                FIFO(pages, frames);
                break;

            case 2:
                LRU(pages, frames);
                break;

            case 3:
                Optimal(pages, frames);
                break;

            case 4:
                compareAll(pages, frames);
                break;

            case 5:
                beladyDemo();
                break;

            case 6:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid Choice!\n";
        }

    } while (choice != 6);

    return 0;
}