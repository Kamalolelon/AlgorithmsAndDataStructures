#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
using namespace std;

// Struktura dla pojedynczej krawêdzi
struct Edge {
    int from;
    int to;
    int weight;
};

// Funkcja do wczytywania macierzy s¹siedztwa z jednego pliku
int** readMatrix(const string& filename, int& size) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Nie mozna otworzyc pliku: " + filename);
    }

    file >> size;
    if (size <= 0) {
        ostringstream oss;
        oss << "Nieprawidlowy rozmiar macierzy: " << size;
        throw runtime_error(oss.str());
    }

    int** matrix = new int* [size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new int[size];
        for (int j = 0; j < size; ++j) {
            if (!(file >> matrix[i][j])) {
                throw runtime_error("Blad odczytu danych macierzy w pliku: " + filename);
            }
        }
    }
    return matrix;
}

// Generowanie listy krawêdzi z macierzy s¹siedztwa
Edge* generateEdgeListFromMatrix(int** matrix, int size, int& edgeCount) {
    edgeCount = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (matrix[i][j] != 0) {
                ++edgeCount;
            }
        }
    }

    Edge* edges = new Edge[edgeCount];
    int index = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (matrix[i][j] != 0) {
                edges[index++] = { i, j, matrix[i][j] };
            }
        }
    }
    return edges;
}

// Generowanie listy s¹siedztwa z listy krawêdzi
int** generateAdjListFromEdgeList(Edge* edges, int edgeCount, int size) {
    int** adjList = new int* [size];
    int* neighborCounts = new int[size]();

    for (int i = 0; i < edgeCount; ++i) {
        neighborCounts[edges[i].from]++;
    }

    for (int i = 0; i < size; ++i) {
        adjList[i] = new int[2 * neighborCounts[i] + 1];
        adjList[i][0] = neighborCounts[i];
    }

    int* currentIndex = new int[size]();
    for (int i = 0; i < edgeCount; ++i) {
        int from = edges[i].from;
        int index = currentIndex[from];
        adjList[from][2 * index + 1] = edges[i].to;
        adjList[from][2 * index + 2] = edges[i].weight;
        currentIndex[from]++;
    }

    delete[] neighborCounts;
    delete[] currentIndex;
    return adjList;
}

// Generowanie listy krawêdzi z listy s¹siedztwa
Edge* generateEdgeListFromAdjList(int** adjList, int size, int& edgeCount) {
    edgeCount = 0;
    for (int i = 0; i < size; ++i) {
        edgeCount += adjList[i][0];
    }

    Edge* edges = new Edge[edgeCount];
    int index = 0;
    for (int i = 0; i < size; ++i) {
        int neighbors = adjList[i][0];
        for (int j = 0; j < neighbors; ++j) {
            edges[index++] = { i, adjList[i][2 * j + 1], adjList[i][2 * j + 2] };
        }
    }
    return edges;
}

// Wypisanie listy krawêdzi
void printEdgeList(const Edge* edges, int edgeCount) {
    cout << "Lista krawedzi (LE):\n";
    for (int i = 0; i < edgeCount; ++i) {
        cout << edges[i].from << " -> " << edges[i].to << " [waga=" << edges[i].weight << "]" << endl;
    }
}

// Wypisanie listy s¹siedztwa
void printAdjList(int** adjList, int size) {
    cout << "Lista sasiedztwa (LN):\n";
    for (int i = 0; i < size; ++i) {
        cout << "Wierzcholek " << i << ": ";
        for (int j = 0; j < adjList[i][0]; ++j) {
            cout << adjList[i][2 * j + 1] << "(waga=" << adjList[i][2 * j + 2] << ") ";
        }
        cout << endl;
    }
}

// Funkcja do zwalniania pamiêci dla macierzy s¹siedztwa
void freeMatrix(int** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// Funkcja do zwalniania pamiêci dla listy s¹siedztwa
void freeAdjList(int** adjList, int size) {
    for (int i = 0; i < size; ++i) {
        delete[] adjList[i];
    }
    delete[] adjList;
}

int main() {
    try {
        int size, edgeCount;

        // Wczytanie macierzy s¹siedztwa z pliku
        int** matrix = readMatrix("graf.txt", size);

        // Generowanie listy krawêdzi z macierzy s¹siedztwa
        Edge* edgeListFromMatrix = generateEdgeListFromMatrix(matrix, size, edgeCount);
        printEdgeList(edgeListFromMatrix, edgeCount);

        // Generowanie listy s¹siedztwa z listy krawêdzi
        int** adjListFromEdgeList = generateAdjListFromEdgeList(edgeListFromMatrix, edgeCount, size);
        printAdjList(adjListFromEdgeList, size);

        // Generowanie listy krawêdzi z listy s¹siedztwa
        Edge* edgeListFromAdjList = generateEdgeListFromAdjList(adjListFromEdgeList, size, edgeCount);
        printEdgeList(edgeListFromAdjList, edgeCount);

        // Zwolnienie pamiêci
        delete[] edgeListFromMatrix;
        delete[] edgeListFromAdjList;
        freeMatrix(matrix, size);
        freeAdjList(adjListFromEdgeList, size);

    }
    catch (const exception& e) {
        cerr << "Blad: " << e.what() << endl;
    }
    return 0;
}
