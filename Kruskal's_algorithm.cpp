#include <iostream>
#include <fstream>
using namespace std;

struct Edge {
    int u, v;    // Wierzcho�ki
    int weight;  // Waga
    Edge* next;  // Wska�nik do nast�pnej kraw�dzi (lista jednokierunkowa)
};

// Funkcja dodaj�ca kraw�d� do listy
void addEdge(Edge*& head, int u, int v, int weight) {
    Edge* newEdge = new Edge{ u, v, weight, nullptr }; //tworz� now� kraw�d�, dwa wierzcho�ki, waga i wska�nik na nast�pny na 0
    if (!head) {
        head = newEdge;
    }
    else { //dodaje na koniec listy
        Edge* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newEdge;
    }
}

// Funkcja sortuj�ca list� kraw�dzi wed�ug wag (sortowanie przez wstawianie)
Edge* sortEdges(Edge* head) {
    if (!head || !head->next) return head; //je�li lista jest pusta albo ma 1 el.

    Edge* sorted = nullptr; //wska�nik na posortowan� list� kraw�dzi
    Edge* current = head; //wska�nik na kraw�d� w nieposortowanej li�ci

    while (current) {
        Edge* next = current->next; //next zapamietuje wska�nik na nastempny el w nieposortowanej liscie
        if (!sorted || sorted->weight > current->weight) { //je�li sorted puste lub current mniejsz� wage ma
            current->next = sorted; 
            sorted = current; //ustawiam sorted na nowy pierwszy element posortowanej listy
        }
        else { //wstawianie w inne miejsce 
            Edge* temp = sorted;
            while (temp->next && temp->next->weight <= current->weight) {
                temp = temp->next;
            }
            current->next = temp->next; //ustawiam wska�nik next na kolejny element
            temp->next = current; //wstawiam current po temp czyli tak jakby pomi�dzy
        }
        current = next;
    }

    return sorted;
}

int main() {
    ifstream file("graf.txt"); // Plik wej�ciowy

    int n; // Liczba wierzcho�k�w
    file >> n;

    //  // Dynamiczna alokacja pami�ci na macierz s�siedztwa - przygotowanie na jej wczytanie
    int** macierzSasiedztwa = new int* [n];//[n] tablica n element�w gdzie ka�dy jej element wskazuje na tablic� liczb ca�kowitych czyli na wiersz macierzy, tak jakby lista pustych miejsc na wiersze
    for (int i = 0; i < n; i++) {
        macierzSasiedztwa[i] = new int[n];//w ka�dym wierszu dodaj� tablic� liczb o rozmiarze n
    } //no i mam macierz 2D

    // Lista kraw�dzi
    Edge* edges = nullptr;

    // Wczytanie macierzy s�siedztwa z pliku
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file >> macierzSasiedztwa[i][j];
            if (macierzSasiedztwa[i][j] != 0 && i < j) { //i-wiersz, j-kolumna, jak warto�� to 0 to nie ma kraw�dzi pomi�dzy wierzcho�akmi i zapewnienie ze macierz jest dodana raz
                addEdge(edges, i + 1, j + 1, macierzSasiedztwa[i][j]);
            }
        }
    }

    file.close();

    edges = sortEdges(edges);

    // Wy�wietlenie listy kraw�dzi po sortowaniu
    cout << "LE: " << endl;
    Edge* temp = edges;
    while (temp) {
        cout << "Krawedz: (" << temp->u << ", " << temp->v << ") - Waga: " << temp->weight << endl;
        temp = temp->next;
    }

    // Tablica kolor�w i las�w
    int* colors = new int[n];      
    int* forests = new int[n];     
    for (int i = 0; i < n; i++) {
        colors[i] = 0;             
        forests[i] = 0;            // Brak przypisanego lasu
    }

    Edge* MDT = nullptr;           
    int forestCount = 1;           

    // Przetwarzanie kraw�dzi
    temp = edges;
    while (temp) {
        int u = temp->u - 1; // Indeks wierzcho�ka u (0-based)
        int v = temp->v - 1; // Indeks wierzcho�ka v (0-based)

        if (colors[u] == 0 && colors[v] == 0) {
            // Dwa bia�e - tworzymy nowy las
            forests[u] = forests[v] = forestCount++;
            colors[u] = colors[v] = 1;
            addEdge(MDT, temp->u, temp->v, temp->weight);
        }
        else if (colors[u] == 1 && colors[v] == 0) {
            // Szary i bia�y - dopinamy bia�y do lasu szarego
            forests[v] = forests[u];
            colors[v] = 1;
            addEdge(MDT, temp->u, temp->v, temp->weight);
        }
        else if (colors[u] == 0 && colors[v] == 1) {
            // Bia�y i szary - dopinamy bia�y do lasu szarego
            forests[u] = forests[v];
            colors[u] = 1;
            addEdge(MDT, temp->u, temp->v, temp->weight);
        }
        else if (colors[u] == 1 && colors[v] == 1 && forests[u] != forests[v]) {
            // Dwa szare, ale z r�nych las�w - ��czymy lasy
            int oldForest = forests[v];
            int newForest = forests[u];
            for (int i = 0; i < n; i++) {
                if (forests[i] == oldForest) {
                    forests[i] = newForest;
                }
            }
            addEdge(MDT, temp->u, temp->v, temp->weight);
        }

        temp = temp->next;
    }

    // Wy�wietlenie wyniku
    cout << "MDR:" << endl;
    temp = MDT;
    while (temp) {
        cout << "Krawedz: (" << temp->u << ", " << temp->v << ") - Waga: " << temp->weight << endl;
        temp = temp->next;
    }

    // Zwolnienie pami�ci
    for (int i = 0; i < n; i++) {
        delete[] macierzSasiedztwa[i];
    }
    delete[] macierzSasiedztwa;

    while (edges) {
        Edge* next = edges->next;
        delete edges;
        edges = next;
    }

    while (MDT) {
        Edge* next = MDT->next;
        delete MDT;
        MDT = next;
    }

    delete[] colors;
    delete[] forests;

    return 0;
}
