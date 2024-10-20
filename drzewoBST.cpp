#include<iostream>

using namespace std;

struct BST {
	BST* left;
	BST* right;
	BST* par;
	int val;
	
};
BST* root = NULL;

void insert(BST*& root, int x, BST* par=NULL) {
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
		cout << root->val << " ";  // Wypisz warto�� bie��cego w�z�a
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
    // Je�li w�ze� ma prawe poddrzewo, znajd� minimalny element w prawym poddrzewie
    if (n->right != NULL) {
        return findMin(n->right);
    }
    // Je�li nie ma prawego poddrzewa, idziemy w g�r�, szukaj�c pierwszego w�z�a, kt�ry jest lewym dzieckiem swojego rodzica
    BST* p = n->par; //tworzymy wska�nik p kt�ry odnosi si� do rodzica w�z�a n
    while (p != NULL && n == p->right) { //sprawdzamy czy w�ze� n jest prawym dzieckiem swojego rodzica p
        n = p; //aktualizacja p�tli, teraz n staje si� rodzicem
        p = p->par; //przechodzimy wy�ej i ustawiamy p na rodzica nowego n
    }
    // Zwracamy rodzica, kt�ry b�dzie nast�pnikiem
    return p;
}

BST* Poprzednik(BST* n) {
    // Je�li w�ze� ma lewe poddrzewo, znajd� maksymalny element w lewym poddrzewie
    if (n->left != NULL) {
        return findMax(n->left);
    }
    // Je�li nie ma lewego poddrzewa, idziemy w g�r�, szukaj�c pierwszego w�z�a, kt�ry jest prawym dzieckiem swojego rodzica
    BST* p = n->par;
    while (p != NULL && n == p->left) {
        n = p;
        p = p->par;
    }
    // Zwracamy rodzica, kt�ry b�dzie poprzednikiem, lub NULL, je�li nie ma poprzednika
    return p;
}

BST* deleteNode(BST* root, int x) {
    // Znajd� w�ze� do usuni�cia
    if (root == NULL) return root;

    if (x < root->val) {
        root->left = deleteNode(root->left, x);
    }
    else if (x > root->val) {
        root->right = deleteNode(root->right, x);
    }
    else {
        // W�ze� znaleziony
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
            // W�ze� ma dwoje dzieci, znajd� nast�pnika 
            BST* temp = findMin(root->right);
            root->val = temp->val;
            root->right = deleteNode(root->right, temp->val); // Usu� nast�pnika
        }
    }
    return root;
}

BST* rotateLeft(BST* root) {
    // Sprawdzamy, czy istnieje w�ze� do rotacji
    if (root == NULL || root->right == NULL) {
        return root; // Je�li nie ma w�z�a, lub nie ma prawego potomka, nic nie robimy
    }

    BST* newRoot = root->right; // Ustalamy nowy korze� (nowy w�ze�, kt�ry stanie si� korzeniem)

    // Przenosimy lewego potomka nowego korzenia (je�li istnieje) na praw� stron� starego korzenia
    root->right = newRoot->left;
    if (newRoot->left != NULL) {
        newRoot->left->par = root; // rodzic lewego dziecka nowego korzenia ustawiamy na stary root
    }

    // Przenosimy stary korze� (root) jako lewego potomka nowego korzenia
    newRoot->left = root;

    // Ustalamy rodzica dla nowego korzenia
    newRoot->par = root->par;
    root->par = newRoot; // Ustalamy rodzica dla starego korzenia (nowy korze�)

    return newRoot; // Zwracamy nowy korze�
}

BST* rotateRight(BST* root){
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
    // Dodajemy kilka w�z��w do drzewa
    insert(root, 15);
    insert(root, 20);
    insert(root, 25);
    insert(root, 10);
    insert(root, 18);
    insert(root, 17);
    insert(root, 22);
    insert(root, 33);

    cout << "Inorder traversal: ";
    inorder(root); // Wypisujemy w�z�y w porz�dku inorder
    cout << endl;

    // Sprawdzamy najmniejszy i najwi�kszy w�ze�
    cout << "Minimum value: " << findMin(root)->val << endl; // powinno wypisa� 2
    cout << "Maximum value: " << findMax(root)->val << endl; // powinno wypisa� 8

    // Sprawdzamy istnienie konkretnego w�z�a
    int searchValue = 15;
    BST* foundNode = findNode(root, searchValue);
    if (foundNode) {
        cout << "Found node with value: " << foundNode->val << endl; // powinno wypisa� 4
    }
    else {
        cout << "Node with value " << searchValue << " not found." << endl;
    }

    // Usuwamy w�ze� o warto�ci 3
    cout << "Deleting node with value 15..." << endl;
    root = deleteNode(root, 15);

    cout << "Inorder traversal after deletion: ";
    inorder(root); // Wypisujemy w�z�y w porz�dku inorder po usuni�ciu
    cout << endl;

    // Rotacja w lewo na w�le 15
    BST* nodeToRotate = findNode(root, 17);
    if (nodeToRotate) {
        root = rotateLeft(nodeToRotate); // Rotuj na 17
    }
   // root = rotateLeft(root->left);
    printf("Tree after rotation (in-order): ");
    inorder(root);
    printf("Tree after rotation (in-order): ");
    root = rotateRight(root);
    inorder(root);
	return 0;
}












