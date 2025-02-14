#include <iostream>
#include <fstream>
using namespace std;

struct Edge {
    int u, v;    // Wierzcho³ki
    int weight;  // Waga
    Edge* next;  // WskaŸnik do nastêpnej krawêdzi (lista jednokierunkowa)
};

// Funkcja dodaj¹ca krawêdŸ do listy
void addEdge(Edge*& head, int u, int v, int weight) {
    Edge* newEdge = new Edge{ u, v, weight, nullptr }; //tworzê now¹ krawêdŸ, dwa wierzcho³ki, waga i wskaŸnik na nastêpny na 0
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

// Funkcja sortuj¹ca listê krawêdzi wed³ug wag (sortowanie przez wstawianie)
Edge* sortEdges(Edge* head) {
    if (!head || !head->next) return head; //jeœli lista jest pusta albo ma 1 el.

    Edge* sorted = nullptr; //wskaŸnik na posortowan¹ listê krawêdzi
    Edge* current = head; //wskaŸnik na krawêdŸ w nieposortowanej liœci

    while (current) {
        Edge* next = current->next; //next zapamietuje wskaŸnik na nastempny el w nieposortowanej liscie
        if (!sorted || sorted->weight > current->weight) { //jeœli sorted puste lub current mniejsz¹ wage ma
            current->next = sorted; 
            sorted = current; //ustawiam sorted na nowy pierwszy element posortowanej listy
        }
        else { //wstawianie w inne miejsce 
            Edge* temp = sorted;
            while (temp->next && temp->next->weight <= current->weight) {
                temp = temp->next;
            }
            current->next = temp->next; //ustawiam wskaŸnik next na kolejny element
            temp->next = current; //wstawiam current po temp czyli tak jakby pomiêdzy
        }
        current = next;
    }

    return sorted;
}

int main() {
    ifstream file("graf.txt"); // Plik wejœciowy

    int n; // Liczba wierzcho³ków
    file >> n;

    //  // Dynamiczna alokacja pamiêci na macierz s¹siedztwa - przygotowanie na jej wczytanie
    int** macierzSasiedztwa = new int* [n];//[n] tablica n elementów gdzie ka¿dy jej element wskazuje na tablicê liczb ca³kowitych czyli na wiersz macierzy, tak jakby lista pustych miejsc na wiersze
    for (int i = 0; i < n; i++) {
        macierzSasiedztwa[i] = new int[n];//w ka¿dym wierszu dodajê tablicê liczb o rozmiarze n
    } //no i mam macierz 2D

    // Lista krawêdzi
    Edge* edges = nullptr;

    // Wczytanie macierzy s¹siedztwa z pliku
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file >> macierzSasiedztwa[i][j];
            if (macierzSasiedztwa[i][j] != 0 && i < j) { //i-wiersz, j-kolumna, jak wartoœæ to 0 to nie ma krawêdzi pomiêdzy wierzcho³akmi i zapewnienie ze macierz jest dodana raz
                addEdge(edges, i + 1, j + 1, macierzSasiedztwa[i][j]);
            }
        }
    }

    file.close();

    edges = sortEdges(edges);

    // Wyœwietlenie listy krawêdzi po sortowaniu
    cout << "LE: " << endl;
    Edge* temp = edges;
    while (temp) {
        cout << "Krawedz: (" << temp->u << ", " << temp->v << ") - Waga: " << temp->weight << endl;
        temp = temp->next;
    }

    // Tablica kolorów i lasów
    int* colors = new int[n];      
    int* forests = new int[n];     
    for (int i = 0; i < n; i++) {
        colors[i] = 0;             
        forests[i] = 0;            // Brak przypisanego lasu
    }

    Edge* MDT = nullptr;           
    int forestCount = 1;           

    // Przetwarzanie krawêdzi
    temp = edges;
    while (temp) {
        int u = temp->u - 1; // Indeks wierzcho³ka u (0-based)
        int v = temp->v - 1; // Indeks wierzcho³ka v (0-based)

        if (colors[u] == 0 && colors[v] == 0) {
            // Dwa bia³e - tworzymy nowy las
            forests[u] = forests[v] = forestCount++;
            colors[u] = colors[v] = 1;
            addEdge(MDT, temp->u, temp->v, temp->weight);
        }
        else if (colors[u] == 1 && colors[v] == 0) {
            // Szary i bia³y - dopinamy bia³y do lasu szarego
            forests[v] = forests[u];
            colors[v] = 1;
            addEdge(MDT, temp->u, temp->v, temp->weight);
        }
        else if (colors[u] == 0 && colors[v] == 1) {
            // Bia³y i szary - dopinamy bia³y do lasu szarego
            forests[u] = forests[v];
            colors[u] = 1;
            addEdge(MDT, temp->u, temp->v, temp->weight);
        }
        else if (colors[u] == 1 && colors[v] == 1 && forests[u] != forests[v]) {
            // Dwa szare, ale z ró¿nych lasów - ³¹czymy lasy
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

    // Wyœwietlenie wyniku
    cout << "MDR:" << endl;
    temp = MDT;
    while (temp) {
        cout << "Krawedz: (" << temp->u << ", " << temp->v << ") - Waga: " << temp->weight << endl;
        temp = temp->next;
    }

    // Zwolnienie pamiêci
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
