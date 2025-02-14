#include <iostream>
#include <string>

using namespace std;

struct Node {
    string value;  
    Node* next;    

    Node(const string& val) : value(val), next() {}
};

void AddList(Node*& head, const string& value) {
    Node* newNode = new Node(value);

    if (!head) {
        head = newNode;
    }
    else {
        Node* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

int FunHash(const string& name) {
    int sum = 0;
    for (int i = 0; i < name.length(); i++) {
        char c = toupper(name[i]); //biblioteka cctype, ale tutaj iostream te¿ jakoœ obejmuje
        sum += (c - 'A' + 1);
    }
    return sum % 13;
}

void Insert(const string& x, Node* head[]) {
    int key = FunHash(x); 
    AddList(head[key], x);      
}

bool Search(const string& x, Node* head[]) {
    int key = FunHash(x);       
    Node* current = head[key];  
    while (current) {
        if (current->value == x) {
            return true;        
        }
        current = current->next; 
    }
    return false; 
}

bool Remove(const string& x, Node* head[]) {
    int key = FunHash(x);
    Node* current = head[key];  
    Node* prev = nullptr;      
    while (current) {
        if (current->value == x) {
            if (prev) {
                prev->next = current->next; //czyli ze poprzedni wskazuje na ten co by³ nastêpnym po obecnym
                ///ania->kasia->ola->null to teraz jeœli current=kasia: ania->ola->null
            }
            else {
                head[key] = current->next; 
            }
            delete current; 
            return true;    
        }
        prev = current;       
        current = current->next;
    }
    return false; 
}

void DisplayHashTable(Node* head[], int size) {
    for (int i = 0; i < size; i++) {
        cout << i << ": ";
        Node* current = head[i];
        while (current) {
            cout << current->value << " -> ";
            current = current->next;
        }
        cout << "nullptr\n";
    }
}

int main() {
    Node* head[13] = {};
    Insert("Antek", head);
    Insert("Piotr", head);
    Insert("Ola", head);
    Insert("Kasia", head);

    DisplayHashTable(head, 13);

    cout << "\nWyszukiwanie elementow:\n";
    string toSearch[] = { "Antek", "Piotr", "Asia", "Kasia" };
    for (const string& name : toSearch) { //pêtla zasiêgowa, iteruje bez jawnych indeksów
        cout << "Element '" << name << "' "
            << (Search(name, head) ? "istnieje\n" : "nie istnieje\n"); //warunek ? wartoœæ_jeœli_prawda : wartoœæ_jeœli_fa³sz;
    }
    //for (int i = 0; i < 4; i++) { 
    //    cout << "Element '" << toSearch[i] << "' ";
    //    if (Search(toSearch[i], head)) {
    //        cout << "istnieje";
    //    }
    //    else {
    //        cout << "nie istnieje";
    //    }
    //    cout << "\n";
    //}

      cout << "\nUsuwanie elementow:\n";
    string toRemove[] = { "Antek", "Asia", "Kasia" };
    for (const string& name : toRemove) {
        cout << "Element '" << name << "' "
            << (Remove(name, head) ? "usunieto\n" : "nie usunieto\n");
    }

    DisplayHashTable(head, 13);

    for (int i = 0; i < 13; i++) {
        Node* current = head[i];
        while (current) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }

    return 0;
}