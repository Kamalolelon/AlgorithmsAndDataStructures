#include<iostream>

using namespace std;

struct BST {
    BST* left;
    BST* right;
    BST* par;
    int val;

};
BST* root = NULL;

void insert(BST*& root, int x, BST* par = nullptr) {
    if (root == NULL) {
        BST* newNode = new BST;
        newNode->val = x;
        newNode->right = NULL;
        newNode->left = NULL;
        root = newNode;
        newNode->par = par;
    }
    else {
        if (x >= root->val) {
            insert(root->right, x, root);
        }
        else {
            insert(root->left, x, root);
        }
    }
}

void inorder(BST* root) {
    if (root != NULL) {
        inorder(root->left);
        cout << root->val << " ";
        inorder(root->right);
    }
}

BST* findMin(BST* root) {
    while (root->left) {
        root = root->left;
    }
    return root;
}

BST* findMax(BST* root) {
    while (root->right) {
        root = root->right;
    }
    return root;
}

BST* findNode(BST* root, int x) {
    if (!root) {
        return NULL;
    }
    else if (root->val == x) {
        return root;
    }
    else if (x < root->val) {
        return findNode(root->left, x);
    }
    else {
        return findNode(root->right, x);
    }
}

BST* Nastepnik(BST* n) {
    // Jeśli węzeł ma prawe poddrzewo, znajdź minimalny element w prawym poddrzewie
    if (n->right != NULL) {
        return findMin(n->right);
    }
    // Jeśli nie ma prawego poddrzewa, idziemy w górę, szukając pierwszego węzła, który jest lewym dzieckiem swojego rodzica
    BST* p = n->par; //tworzymy wskaźnik p który odnosi się do rodzica węzła n
    while (p != NULL && n == p->right) { //sprawdzamy czy węzeł n jest prawym dzieckiem swojego rodzica p
        n = p; //aktualizacja pętli, teraz n staje się rodzicem
        p = p->par; //przechodzimy wyżej i ustawiamy p na rodzica nowego n
    }
    // Zwracamy rodzica, który będzie następnikiem
    return p;
}

BST* Poprzednik(BST* n) {
    // Jeśli węzeł ma lewe poddrzewo, znajdź maksymalny element w lewym poddrzewie
    if (n->left != NULL) {
        return findMax(n->left);
    }
    // Jeśli nie ma lewego poddrzewa, idziemy w górę, szukając pierwszego węzła, który jest prawym dzieckiem swojego rodzica
    BST* p = n->par;
    while (p != NULL && n == p->left) {
        n = p;
        p = p->par;
    }
    // Zwracamy rodzica, który będzie poprzednikiem, lub NULL, jeśli nie ma poprzednika
    return p;
}

BST* deleteNode(BST* root, int x) {
    // Znajdź węzeł do usunięcia
    if (root == NULL) return root;

    if (x < root->val) {
        root->left = deleteNode(root->left, x);
    }
    else if (x > root->val) {
        root->right = deleteNode(root->right, x);
    }
    else {
        // Węzeł znaleziony
        if (root->left == NULL) {
            BST* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == NULL) {
            BST* temp = root->left;
            delete root;
            return temp;
        }
        else {
            // Węzeł ma dwoje dzieci, znajdź następnika czy mogę skorzystać z funkcji znajdź następnika?
            BST* temp = findMin(root->right);
            root->val = temp->val;
            root->right = deleteNode(root->right, temp->val); // Usuń następnika
        }
    }
    return root;
}

BST* rotateLeft(BST* root) {
    // Sprawdzamy, czy istnieje węzeł do rotacji
    if (root == NULL || root->right == NULL) {
        return root; // Jeśli nie ma węzła, lub nie ma prawego potomka, nic nie robimy
    }

    BST* newRoot = root->right; // Ustalamy nowy korzeń (nowy węzeł, który stanie się korzeniem)

    // Przenosimy lewego potomka nowego korzenia (jeśli istnieje) na prawą stronę starego korzenia
    root->right = newRoot->left;
    if (newRoot->left != NULL) {
        newRoot->left->par = root; // rodzic lewego dziecka nowego korzenia ustawiamy na stary root
    }

    // Przenosimy stary korzeń (root) jako lewe dzieckogrzenia
    newRoot->left = root;

    // Ustalamy rodzica dla nowego korzenia
    newRoot->par = root->par;
    root->par = newRoot; // Ustalamy rodzica dla starego korzenia (nowy korzeń)

    return newRoot; // Zwracamy nowy korzeń
}

BST* rotateRight(BST* root) {
    if (root->left == NULL || root == NULL) {
        return root;
    }
    BST* newRoot = root->left;
    root->left = newRoot->right;
    if (newRoot->right != nullptr) {
        newRoot->right->par = root;
    }
    newRoot->right = root;
    newRoot->par = root->par;
    root->par = newRoot;
    return newRoot;
}


int main() {
    // Dodajemy kilka węzłów do drzewa
    insert(root, 15);
    insert(root, 20);
    insert(root, 25);
    insert(root, 10);
    insert(root, 18);
    insert(root, 17);
    insert(root, 22);
    insert(root, 33);

    cout << "Inorder: ";
    inorder(root);
    cout << endl;

    // Sprawdzamy najmniejszy i największy węzeł
    cout << "Min: " << findMin(root)->val << endl; // powinno wypisać 2
    cout << "Max: " << findMax(root)->val << endl; // powinno wypisać 8

    // Testowanie funkcji Nastepnik i Poprzednik
    BST * node = findNode(root, 15);
    BST* nastepnik = Nastepnik(node);
    BST* poprzednik = Poprzednik(node);

    
    cout << "Nastepnik 15: " << nastepnik->val << endl;
    cout << "Poprzednik 15: " << poprzednik->val << endl;
  
    // Usuwamy węzeł 
    root = deleteNode(root, 15);

    cout << "Inorder po usunięciu 15: ";
    inorder(root); // Wypisujemy węzły w porządku inorder po usunięciu
    cout << endl;

    root = rotateLeft(root);
    cout << "Inorder po rotacji w lewo: ";
    inorder(root);
    cout << endl;

    root = rotateRight(root);
    cout << "Inorder po rotacji w prawo: ";
    inorder(root);
    cout << endl;
    //return 0;
}













