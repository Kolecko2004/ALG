#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char color;
    struct Node *left;
    struct Node *right;
    int idx;
} Node;

typedef struct QueueNode {
    Node *node;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *front;
    QueueNode *rear;
} Queue;

int num_pearls;
char *pearls = NULL;
int max_pearl = 0;

Node* create_node(char color, int idx) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->color = color;
    node->left = NULL;
    node->right = NULL;
    node->idx = idx;
    return node;
}

Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, Node *node) {
    QueueNode *qn = (QueueNode*)malloc(sizeof(QueueNode));
    qn->node = node;
    qn->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = qn;
        return;
    }
    
    q->rear->next = qn;
    q->rear = qn;
}

Node* dequeue(Queue *q) {
    if (q->front == NULL) return NULL;
    
    QueueNode *temp = q->front;
    Node *node = temp->node;
    q->front = q->front->next;
    
    if (q->front == NULL) q->rear = NULL;
    
    free(temp);
    return node;
}

int is_empty(Queue *q) {
    return q->front == NULL;
}

void free_queue(Queue *q) {
    while (!is_empty(q)) dequeue(q);
    
    free(q);
}

Node* build_tree() {    
    if (num_pearls == 0) return NULL;
    
    Node *root = create_node(pearls[0], 0);
    
    Queue *q = create_queue();
    enqueue(q, root);
    
    int current_idx = 1;
    
    while (!is_empty(q) && current_idx < num_pearls) {
        Node *current = dequeue(q);
        
        if (current->color == '2') {
            continue;
        }

        current->left = create_node(pearls[current_idx], current_idx);
        enqueue(q, current->left);
        current_idx++;

        current->right = create_node(pearls[current_idx], current_idx);
        enqueue(q, current->right);
        current_idx++;
    }

    free_queue(q);
    return root;
}

int depth(Node *node, char parent) {
    if (node == NULL) return 0;

    int left_depth = 0, right_depth = 0;

    if (node->left != NULL) {
        if (parent == node->color && node->left->color == node->color) {
            depth(node->left, -1);
            left_depth = 0;
        } else {
            left_depth = depth(node->left, node->color);
        }
    }

    if (node->right != NULL) {
        if (parent == node->color && node->right->color == node->color) {
            depth(node->right, -1);
            right_depth = 0;
        } else {
            right_depth = depth(node->right, node->color);
        }
    }

    int path_length = left_depth + right_depth + 1;
    if (path_length > max_pearl) {
        max_pearl = path_length;
    }

    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

void free_tree(Node *node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

int main() {
    scanf("%d", &num_pearls);
    
    pearls = malloc(num_pearls * sizeof(char));
    if (pearls == NULL) {
        fprintf(stderr, "ERROR while allocating memory");
        return 101;
    }
    for (int i = 0; i < num_pearls; i++) scanf(" %c", &pearls[i]);

    Node *root = build_tree();
    
    if (pearls[0] == pearls[1] && pearls[0] == pearls[2]) {
        if (root->left) depth(root->left, -1);
        if (root->right) depth(root->right, -1);
    } else {
        depth(root, -1);
    }

    printf("%d\n", max_pearl);

    free_tree(root);
    free(pearls);
    return 0;
}