#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Node {
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int value;
};

typedef struct Node Node;

Node* new_Node() {
    return (Node*)malloc(sizeof(Node));
}

void serialize_Node(char* serialized, Node* node) {
    sprintf(serialized, "(Node){parent: %p, left: %p, right %p, value: %d} at %p",
            node->parent, node->left, node->right, node->value, node);
}

void print_Node(Node* node, char* name) {
    char to_print[100] = {};
    serialize_Node(to_print, node);
    printf("%s %s\n", name, to_print);
}

struct NodeStack {
    Node* nodes[100];
    int length;
};

typedef struct NodeStack NodeStack;

NodeStack* new_NodeStack() {
    NodeStack* stack = (NodeStack*)malloc(sizeof(NodeStack));
    stack->length = 0;
    return stack;
}

void push(NodeStack* stack, Node* node) {
    stack->nodes[stack->length] = node;
    stack->length++;
}

Node* pop(NodeStack* stack) {
    stack->length--;
    Node* popped = stack->nodes[stack->length];
    stack->nodes[stack->length] = NULL;
    return popped;
}

void print_tree(Node* root) {
    Node* traversed_nodes[100] = {};
    int num_nodes_traversed = 0;

    NodeStack* nodes_to_check = new_NodeStack();
    push(nodes_to_check, root);

    Node* checking;

    while (nodes_to_check->length > 0) {
        checking = pop(nodes_to_check);
        if (checking->left) {
            push(nodes_to_check, checking->left);
        }
        if (checking->right) {
            push(nodes_to_check, checking->right);
        }
        traversed_nodes[num_nodes_traversed] = checking;
        num_nodes_traversed++;
    }

    for (int i=0; i<num_nodes_traversed; i++) {
        print_Node(traversed_nodes[i], ">>");
    }
}

void insertLeft(Node* node, int value) {
    Node* left = new_Node();
    *left = (Node){node, NULL, NULL, value};
    node->left = left;
}

void insertRight(Node* node, int value) {
    Node* right = new_Node();
    *right = (Node){node, NULL, NULL, value};
    node->right = right;
}

void insert(Node* root, int value) {
    if (value <= root->value) {
        if (root->left) {
            insert(root->left, value);
        } else {
            insertLeft(root, value);
            return;
        }
    } else {
        if (root->right) {
            insert(root->right, value);
        } else {
            insertRight(root, value);
            return;
        }
    }
}

void pop_Node() {
}

void delete_tree(Node* root) {
    if (root->left) {
        delete_tree(root->left);
    }
    if (root->right) {
        delete_tree(root->right);
    }
    free(root);
}

Node* get_root(Node* node) {
    Node* root_result = node;
    while (root_result->parent) {
        root_result = root_result->parent;
    }
    return root_result;
}

Node* min_Node(Node* a, Node* b) {
    if (a->value < b->value) { return a; }
    return b;
}

int cmp(int a, int b) {
    if (a<b) {
        return -1;
    }
    if (a>b) {
        return 1;
    }
    return 0;
}

Node* search(Node* root, int value) {
    char serialized[100] = {};
    serialize_Node(serialized, root);
    switch (cmp(value, root->value)) {
        case 0:
            return root;
            break;
        case -1:
            if (root->left) {
                return search(root->left, value);
            }
            break;
        case 1:
            if (root->right) {
                return search(root->right, value);
            }
            break;
    }
    return NULL;
}

Node* search_min(Node* root) {
    // Finds the minimum value in the tree, making no assumptions about how values have been
    // inserted.
    Node* minSoFar = root;
    if (root->left) {
        minSoFar = min_Node(minSoFar, search_min(root->left));
    }
    if (root->right) {
        minSoFar = min_Node(minSoFar, search_min(root->right));
    }
    return minSoFar;
}

int main() {
    Node* root = new_Node();
    *root = (Node){NULL, NULL, NULL, 10};
    insert(root, 5);
    insert(root, 6);
    insert(root, 10);
    insert(root, 11);
    insert(root, 3);
    assert(root->left->value == 5);
    print_Node(root, "root");
    print_Node(root->left, "root->left");

    Node* maybeRoot = get_root(root->left);

    print_Node(root->left, "root->left");
    print_Node(maybeRoot, "maybeRoot");

    assert(maybeRoot == root);
    assert(root->left->value == 5);
    assert(maybeRoot->left->value == 5);

    Node* search_min_result = search_min(root);
    print_Node(search_min_result, "search_min_result");

    Node* search_result = search(root, 5);
    if (search_result) {
        print_Node(search_result, "search_result");
    } else {
        printf("Value not found!\n");
    }

    //print_tree(root);

    delete_tree(root);
    return 0;
}
