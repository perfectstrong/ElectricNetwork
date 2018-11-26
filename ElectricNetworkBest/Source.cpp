#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>

#define BATCH 5000 //max entries to write each time
#define NMAX 10000 //max number of nodes

using namespace std;

struct Point {
    double x;
    double y;
};

struct Network {
    int n = 0; //number of points
    int m = 0; //number of connections
    int vertex[NMAX + 1] = { -1 }; //points
    int edge[NMAX][2]; //connections
    double totalLength = 0.0;
};

Network network;
int N; // total number of points on map
double maxX;
double maxY;
Point root = { 0, 0 };
Point inputPoints[NMAX + 1];
bool inTree[NMAX + 1] = { false };
double dist2[NMAX + 1][NMAX + 1] = { {0} };
int nearestNetworkNeighbor[NMAX + 1] = { 0 };
int newestNode = -1;

void addVertex(int p) {
    network.vertex[network.n] = p;
    inTree[p] = true;
    newestNode = p;
    network.n++;
}

void addEdge(int p, int q) {
    network.edge[network.m][0] = p;
    network.edge[network.m][1] = q;
    network.m++;
    network.totalLength += sqrt(dist2[p][q]);
}

void readInputPoints(string filepath) {
    inputPoints[0] = root;
    inTree[0] = true;
    ifstream f;
    f.open(filepath, std::ofstream::in);
    if (f.is_open()) {
        // First 2 lines are number of points, max abscissa, max coordinate
        f >> N >> maxX >> maxY;
        // Then coordinates of points
        for (int i = 1; i <= N; i++) {
            f >> inputPoints[i].x >> inputPoints[i].y;
        }
        f.close();
    } else
        throw invalid_argument("Error on reading");
}

void writeOut(string filepath) {
    ofstream f;
    f.open(filepath, std::ofstream::out);
    if (f.is_open()) {
        f << network.totalLength << "\n";
        for (int i = 0; i < network.m; ++i) {
            f << inputPoints[network.edge[i][0]].x
                << " "
                << inputPoints[network.edge[i][0]].y
                << " "
                << inputPoints[network.edge[i][1]].x 
                << " "
                << inputPoints[network.edge[i][1]].y
                << "\n";
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

// Best algorithm O(n^2)
void growNetwork() {
    for (int i = 1; i <= N; ++i) {
        // Each time we add a new edge
        int newEdgeStart = 0; // should be in network
        int newEdgeEnd = -1; // should be outside network
        double dist2Min = maxX * maxX + maxY * maxY;
        for (int j = 0; j <= N; ++j) {
            if (!inTree[j]) {
                // Check a node outside network
                // Update nearest neighbor
                if (dist2[newestNode][j] < dist2[nearestNetworkNeighbor[j]][j]) {
                    nearestNetworkNeighbor[j] = newestNode;
                }
                // Find shortest line to add
                if (dist2[nearestNetworkNeighbor[j]][j] < dist2Min) {
                    dist2Min = dist2[nearestNetworkNeighbor[j]][j];
                    newEdgeStart = nearestNetworkNeighbor[j];
                    newEdgeEnd = j;
                }
            }
        }
        addVertex(newEdgeEnd);
        addEdge(newEdgeStart, newEdgeEnd);
    }
}

int main() {
    string filename = "";
    cout << "input = ";
    getline(cin, filename);
    addVertex(0);
    readInputPoints(filename);
    calculateDist2();
    growNetwork();
    writeOut("output_" + filename);
    return 0;
}