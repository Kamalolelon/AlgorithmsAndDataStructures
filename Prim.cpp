#include <iostream>
#include <fstream>
#include <limits> // Do INT_MAX
using namespace std;

struct Node {
    int vertex;
    int weight;
    Node* next;
};

// Funkcja dodaj�ca s�siada do listy s�siedztwa
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

    int n; // Liczba wierzcho�k�w
    file >> n;

    // Dynamiczna alokacja pami�ci na list� s�siedztwa
    Node** adjList = new Node * [n];
    for (int i = 0; i < n; i++) {
        adjList[i] = nullptr;
    }

    // Wczytanie macierzy s�siedztwa i przekszta�cenie jej na list� s�siedztwa
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
        colors[i] = 0; // Wszystkie wierzcho�ki pocz�tkowo bia�e
    }

    int s = 0; // Wierzcho�ek startowy
    colors[s] = 1; 

    cout << "MDR:" << endl;

    while (true) {
        int minWeight = INT_MAX;
        int chosenU = -1, chosenV = -1;

        // Przeszukiwanie wszystkich szarych wierzcho�k�w
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

        // Je�li nie znaleziono �adnej kraw�dzi, ko�czymy
        if (chosenU == -1 || chosenV == -1) {
            break;
        }

        // Wy�wietlenie wybranej kraw�dzi
        cout << "Krawedz: (" << chosenU + 1 << ", " << chosenV + 1 << ") - Waga: " << minWeight << endl;
        colors[chosenV] = 1; // Zamalowujemy nowy wierzcho�ek na szaro
    }

    // Zwolnienie pami�ci
    deleteAdjList(adjList, n);
    delete[] colors;

    return 0;
}
