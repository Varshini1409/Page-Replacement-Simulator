#include "Fileio.h"
#include <fstream>

using namespace std;

bool loadFromFile(vector<int>& pages, int& frames)
{
    ifstream fin("input.txt");

    if (!fin)
        return false;

    int n;
    fin >> n;

    pages.resize(n);

    for (int i = 0; i < n; i++)
        fin >> pages[i];

    fin >> frames;

    fin.close();

    return true;
}