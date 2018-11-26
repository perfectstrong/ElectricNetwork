#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

string gen(int n, double maxX, double maxY) {
    srand(time(NULL));
    ofstream f;
    string s = "input_" + to_string(n) + "_" + to_string(maxX) + "_" + to_string(maxY) + ".txt";
    f.open(s);
    if (!f.is_open()) return "error";
    f << n << endl;
    f << maxX << " " << maxY << endl;
    double x, y;
    for (int i = 0; i < n; i++) {
        x = maxX * (double(rand()) / RAND_MAX);
        y = maxY * (double(rand()) / RAND_MAX);
        f << x << " " << y << endl;
    }
    f.close();
    return s;
}

int main() {
    const int num = 7;
    double tests[num][3] = {
        {5, 10, 10},
        {10, 10, 10},
    {100, 10, 10},
    {200, 10, 10},
    {500, 10, 10},
    {1000, 10, 10},
    {10000, 100, 100}
    };
    string testslist[num];
    for (int i = 0; i < num; i++) {
        testslist[i] = gen(tests[i][0], tests[i][1], tests[i][2]);
    }
    ofstream f;
    f.open("testslist.txt");
    if (!f.is_open()) return 0;
    for (int i = 0; i < num; i++) {
        f << testslist[i] << endl;
    }
    f.close();
    return 0;
}