#include <iostream>
#include <fstream>
#include <limits> // Do INT_MAX
using namespace std;

struct Node {
    int vertex;
    int weight;
    Node* next;
};

// Funkcja dodaj¹ca s¹siada do listy s¹siedztwa
void addNeighbor(Node*& head, int vertex, int weight) {
    Node* newNode = new Node{ vertex, weight, nullptr };
    if (!head) {
        head = newNode;
    }
    else {
        Node* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

//zwolnienie pamieci
void deleteAdjList(Node** adjList, int n) {
    for (int i = 0; i < n; i++) {
        Node* current = adjList[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] adjList;
}

int main() {
    ifstream file("graf.txt");

    int n; // Liczba wierzcho³ków
    file >> n;

    // Dynamiczna alokacja pamiêci na listê s¹siedztwa
    Node** adjList = new Node * [n];
    for (int i = 0; i < n; i++) {
        adjList[i] = nullptr;
    }

    // Wczytanie macierzy s¹siedztwa i przekszta³cenie jej na listê s¹siedztwa
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int weight;
            file >> weight;
            if (weight != 0) {
                addNeighbor(adjList[i], j, weight);
            }
        }
    }
    file.close();

    int* colors = new int[n];
    for (int i = 0; i < n; i++) {
        colors[i] = 0; // Wszystkie wierzcho³ki pocz¹tkowo bia³e
    }

    int s = 0; // Wierzcho³ek startowy
    colors[s] = 1; 

    cout << "MDR:" << endl;

    while (true) {
        int minWeight = INT_MAX;
        int chosenU = -1, chosenV = -1;

        // Przeszukiwanie wszystkich szarych wierzcho³ków
        for (int u = 0; u < n; u++) {
            if (colors[u] == 1) {
                Node* neighbor = adjList[u];
                while (neighbor) {
                    int v = neighbor->vertex;
                    int weight = neighbor->weight;
                    if (colors[v] == 0 && weight < minWeight) {
                        minWeight = weight;
                        chosenU = u;
                        chosenV = v;
                    }
                    neighbor = neighbor->next;
                }
            }
        }

        // Jeœli nie znaleziono ¿adnej krawêdzi, koñczymy
        if (chosenU == -1 || chosenV == -1) {
            break;
        }

        // Wyœwietlenie wybranej krawêdzi
        cout << "Krawedz: (" << chosenU + 1 << ", " << chosenV + 1 << ") - Waga: " << minWeight << endl;
        colors[chosenV] = 1; // Zamalowujemy nowy wierzcho³ek na szaro
    }

    // Zwolnienie pamiêci
    deleteAdjList(adjList, n);
    delete[] colors;

    return 0;
}
