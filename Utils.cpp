#include "Utils.h"
#include <iostream>

using namespace std;

void displayFrames(const vector<int>& frames, int capacity)
{
    cout << "[ ";

    for (int i = 0; i < capacity; i++)
    {
        if (i < frames.size() && frames[i] != -1)
            cout << frames[i] << " ";
        else
            cout << "_ ";
    }

    cout << "]";
}