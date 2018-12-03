#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <ctime>
#include <chrono>

#define NMAX 10000 //max number of nodes

using namespace std;
using namespace std::chrono;

struct Point {
    double x;
    double y;
};

struct Network {
    int n = 0; //number of points
    int m = 0; //number of connections
    int vertex[NMAX + 1]; // index of points
    int edge[NMAX][2]; // connections
};

Network network;
int N; // total number of points on map
double maxX;
double maxY;
Point root = { 0, 0 };
Point inputPoints[NMAX + 1];
bool inTree[NMAX + 1] = { false };
int orderedPoints[NMAX + 1]; // 0 to network.n-1 are points in tree, others are outside
double dist2[NMAX + 1][NMAX + 1] = { {0} };
double totalLength = 0;

void registerNode(int p) {
    if (p == network.n - 1) return;
    int temp = orderedPoints[network.n - 1];
    orderedPoints[network.n - 1] = orderedPoints[p];
    orderedPoints[p] = temp;
}

void addVertex(int p) {
    network.vertex[network.n] = orderedPoints[p];
    network.n++;
    registerNode(p);
}

void addEdge(int p, int q) {
    network.edge[network.m][0] = orderedPoints[p];
    network.edge[network.m][1] = orderedPoints[q];
    network.m++;
    totalLength += sqrt(dist2[p][q]);
}

void readInputPoints(string filepath) {
    inputPoints[0] = root;
    ifstream f;
    f.open(filepath);
    if (f.is_open()) {
        // First 2 lines are number of points, max abscissa, max coordinate
        f >> N >> maxX >> maxY;
        // Then coordinates of points
        for (int i = 1; i <= N; i++) {
            f >> inputPoints[i].x >> inputPoints[i].y;
            orderedPoints[i] = i;
        }
        f.close();
    } else
        throw invalid_argument("Error on reading");
}

void writeOut(string filepath) {
    ofstream f;
    f.open(filepath);
    if (f.is_open()) {
        f << totalLength << endl;
        for (int i = 0; i < network.m; ++i) {
            f << inputPoints[network.edge[i][0]].x << " "
                << inputPoints[network.edge[i][0]].y << " "
                << inputPoints[network.edge[i][1]].x << " "
                << inputPoints[network.edge[i][1]].y
                << endl;
        }
        f.close();
    } else
        throw invalid_argument("Error on writing");
}

void calculateDist2() {
    for (int i = 0; i <= N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            dist2[i][j] = (inputPoints[i].x - inputPoints[j].x)*(inputPoints[i].x - inputPoints[j].x)
                + (inputPoints[i].y - inputPoints[j].y)*(inputPoints[i].y - inputPoints[j].y);
            dist2[j][i] = dist2[i][j];
        }
    }
}

// Improved algorithm O(n^3)
void growNetwork() {
    for (int i = 1; i <= N; ++i) {
        // Each time we add a new edge
        int newEdgeStart = 0; // should be in network
        int newEdgeEnd = -1; // should be outside network
        double dist2Min = maxX * maxX + maxY * maxY;
        // Search for new shortest edge
        // O(n^2)
        for (int j = 0; j < network.n; ++j) {
            // j indicates index of a node in network
            int inNode = orderedPoints[j];
            for (int k = network.n; k <= N; ++k) {
                // k indicates index of a node outside of network
                int outNode = orderedPoints[k];
                if (dist2[inNode][outNode] < dist2Min) {
                    newEdgeStart = j;
                    newEdgeEnd = k;
                    dist2Min = dist2[inNode][outNode];
                }
            }
        }
        addVertex(newEdgeEnd);
        addEdge(newEdgeStart, newEdgeEnd);
    }
}

high_resolution_clock::time_point now() {
    return high_resolution_clock::now();
}

int main() {
    high_resolution_clock::time_point start, finish;
    duration<double, std::milli> timespan;

    string filename = "";
    cout << "input = ";
    getline(cin, filename);
    addVertex(0);
    readInputPoints(filename);
    calculateDist2();

    start = now();
    growNetwork();
    finish = now();
    timespan = finish - start;
    cout << "Time = " << timespan.count() << " ms" << endl;
    writeOut("output_" + filename);
    getchar();
    return 0;
}