#include <stdio.h>
#include <stdlib.h>

// Definice uzlu stromu
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

// Funkce pro vytvoření nového uzlu
Node* createNode(int value) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Funkce pro vložení hodnoty do stromu
Node* insert(Node* root, int value) {
    // Pokud je strom prázdný, vytvoříme kořen
    if (root == NULL)
        return createNode(value);

    // Rekurzivně najdeme správné místo
    if (value < root->data) {
        root->left = insert(root->left, value);
    } else if (value > root->data) {
        root->right = insert(root->right, value);
    }

    return root;
}

// Vyhledávání ve stromu
bool search(Node* root, int target) {
    if (root == NULL)
        return false;
    if (root->data == target)
        return true;

    if (target < root->data) {
        return search(root->left, target);
    } else {
        return search(root->right, target);
    }
}

// In-order průchod (vypíše data seřazená!)
void printInOrder(Node* root) {
    if (root != NULL) {
        printInOrder(root->left);  // 1. Jdi vlevo
        printf("%d ", root->data); // 2. Vypiš uzel
        printInOrder(root->right); // 3. Jdi vpravo
    }
}

int main() {
    Node* root = NULL;

    // Vložíme pár čísel
    root = insert(root, 50);
    insert(root, 30);
    insert(root, 70);
    insert(root, 20);
    insert(root, 40);
    insert(root, 60);
    insert(root, 80);

    printf("Vzestupne serazena data (In-order): ");
    printInOrder(root);
    printf("\n");

    // Zkusíme hledat
    int find = 40;
    if (search(root, find))
        printf("Cislo %d ve stromu je.\n", find);
    else
        printf("Cislo %d ve stromu neni.\n", find);

    return 0;
}