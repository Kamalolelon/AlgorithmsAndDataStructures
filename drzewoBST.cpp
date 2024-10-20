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
		cout << root->val << " ";  // Wypisz wartoœæ bie¿¹cego wêz³a
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
    // Jeœli wêze³ ma prawe poddrzewo, znajdŸ minimalny element w prawym poddrzewie
    if (n->right != NULL) {
        return findMin(n->right);
    }
    // Jeœli nie ma prawego poddrzewa, idziemy w górê, szukaj¹c pierwszego wêz³a, który jest lewym dzieckiem swojego rodzica
    BST* p = n->par; //tworzymy wskaŸnik p który odnosi siê do rodzica wêz³a n
    while (p != NULL && n == p->right) { //sprawdzamy czy wêze³ n jest prawym dzieckiem swojego rodzica p
        n = p; //aktualizacja pêtli, teraz n staje siê rodzicem
        p = p->par; //przechodzimy wy¿ej i ustawiamy p na rodzica nowego n
    }
    // Zwracamy rodzica, który bêdzie nastêpnikiem
    return p;
}

BST* Poprzednik(BST* n) {
    // Jeœli wêze³ ma lewe poddrzewo, znajdŸ maksymalny element w lewym poddrzewie
    if (n->left != NULL) {
        return findMax(n->left);
    }
    // Jeœli nie ma lewego poddrzewa, idziemy w górê, szukaj¹c pierwszego wêz³a, który jest prawym dzieckiem swojego rodzica
    BST* p = n->par;
    while (p != NULL && n == p->left) {
        n = p;
        p = p->par;
    }
    // Zwracamy rodzica, który bêdzie poprzednikiem, lub NULL, jeœli nie ma poprzednika
    return p;
}

BST* deleteNode(BST* root, int x) {
    // ZnajdŸ wêze³ do usuniêcia
    if (root == NULL) return root;

    if (x < root->val) {
        root->left = deleteNode(root->left, x);
    }
    else if (x > root->val) {
        root->right = deleteNode(root->right, x);
    }
    else {
        // Wêze³ znaleziony
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
            // Wêze³ ma dwoje dzieci, znajdŸ nastêpnika 
            BST* temp = findMin(root->right);
            root->val = temp->val;
            root->right = deleteNode(root->right, temp->val); // Usuñ nastêpnika
        }
    }
    return root;
}

BST* rotateLeft(BST* root) {
    // Sprawdzamy, czy istnieje wêze³ do rotacji
    if (root == NULL || root->right == NULL) {
        return root; // Jeœli nie ma wêz³a, lub nie ma prawego potomka, nic nie robimy
    }

    BST* newRoot = root->right; // Ustalamy nowy korzeñ (nowy wêze³, który stanie siê korzeniem)

    // Przenosimy lewego potomka nowego korzenia (jeœli istnieje) na praw¹ stronê starego korzenia
    root->right = newRoot->left;
    if (newRoot->left != NULL) {
        newRoot->left->par = root; // rodzic lewego dziecka nowego korzenia ustawiamy na stary root
    }

    // Przenosimy stary korzeñ (root) jako lewego potomka nowego korzenia
    newRoot->left = root;

    // Ustalamy rodzica dla nowego korzenia
    newRoot->par = root->par;
    root->par = newRoot; // Ustalamy rodzica dla starego korzenia (nowy korzeñ)

    return newRoot; // Zwracamy nowy korzeñ
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
    // Dodajemy kilka wêz³ów do drzewa
    insert(root, 15);
    insert(root, 20);
    insert(root, 25);
    insert(root, 10);
    insert(root, 18);
    insert(root, 17);
    insert(root, 22);
    insert(root, 33);

    cout << "Inorder traversal: ";
    inorder(root); // Wypisujemy wêz³y w porz¹dku inorder
    cout << endl;

    // Sprawdzamy najmniejszy i najwiêkszy wêze³
    cout << "Minimum value: " << findMin(root)->val << endl; // powinno wypisaæ 2
    cout << "Maximum value: " << findMax(root)->val << endl; // powinno wypisaæ 8

    // Sprawdzamy istnienie konkretnego wêz³a
    int searchValue = 15;
    BST* foundNode = findNode(root, searchValue);
    if (foundNode) {
        cout << "Found node with value: " << foundNode->val << endl; // powinno wypisaæ 4
    }
    else {
        cout << "Node with value " << searchValue << " not found." << endl;
    }

    // Usuwamy wêze³ o wartoœci 3
    cout << "Deleting node with value 15..." << endl;
    root = deleteNode(root, 15);

    cout << "Inorder traversal after deletion: ";
    inorder(root); // Wypisujemy wêz³y w porz¹dku inorder po usuniêciu
    cout << endl;

    // Rotacja w lewo na wêŸle 15
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












