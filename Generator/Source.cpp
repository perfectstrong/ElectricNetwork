#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include "MersenneTwister.h"

using namespace std;

MTRand G;

void gen(int n, double maxX, double maxY) {
    ofstream f;
    string s = "input_" + to_string(n) + "_" + to_string(maxX) + "_" + to_string(maxY) + ".txt";
    f.open(s);
    if (!f.is_open()) return ;
    f << n << endl;
    f << maxX << " " << maxY << endl;
    double x, y;
    for (int i = 0; i < n; i++) {
        x = maxX * (1 - G.randExc());
        y = maxY * (1 - G.randExc());
        f << x << " " << y << endl;
    }
    f.close();
}

int main() {
    const int num = 7;
    G.seed(2018);
    double tests[num][3] = {
        {5, 10, 10},
        {10, 10, 10},
    {100, 10, 10},
    {200, 10, 10},
    {500, 10, 10},
    {1000, 10, 10},
    {10000, 10, 10}
    };
    string testslist[num];
    for (int i = 0; i < num; i++) {
        gen(tests[i][0], tests[i][1], tests[i][2]);
    }
    return 0;
}