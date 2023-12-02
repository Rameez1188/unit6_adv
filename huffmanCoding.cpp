#include <iostream>
#include <cstdlib>

using namespace std;

#define MAX_TREE_HT 100

struct HuffmanNode {
    char character;
    unsigned frequency;
    struct HuffmanNode *left, *right;
};

struct HuffmanMinHeap {
    unsigned size;
    unsigned capacity;
    struct HuffmanNode** array;
};

struct HuffmanNode* createNode(char character, unsigned frequency) {
    struct HuffmanNode* temp = (struct HuffmanNode*)malloc(sizeof(struct HuffmanNode));
    temp->left = temp->right = NULL;
    temp->character = character;
    temp->frequency = frequency;
    return temp;
}

struct HuffmanMinHeap* createMinHeap(unsigned capacity) {
    struct HuffmanMinHeap* minHeap = (struct HuffmanMinHeap*)malloc(sizeof(struct HuffmanMinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct HuffmanNode**)malloc(minHeap->capacity * sizeof(struct HuffmanNode*));
    return minHeap;
}

void swapNodes(struct HuffmanNode** a, struct HuffmanNode** b) {
    struct HuffmanNode* temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(struct HuffmanMinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
        smallest = right;

    if (smallest != idx) {
        swapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct HuffmanMinHeap* minHeap) {
    return (minHeap->size == 1);
}

struct HuffmanNode* extractMin(struct HuffmanMinHeap* minHeap) {
    struct HuffmanNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

void insertMinHeap(struct HuffmanMinHeap* minHeap, struct HuffmanNode* node) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && node->frequency < minHeap->array[(i - 1) / 2]->frequency) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = node;
}

void buildMinHeap(struct HuffmanMinHeap* minHeap) {
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

void printCodes(struct HuffmanNode* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    if (!root->left && !root->right) {
        cout << root->character << ": ";
        for (int i = 0; i < top; ++i)
            cout << arr[i];
        cout << "\n";
    }
}

struct HuffmanMinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct HuffmanMinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = createNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

struct HuffmanNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct HuffmanNode *left, *right, *top;
    struct HuffmanMinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = createNode('$', left->frequency + right->frequency);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

void HuffmanCodes(char data[], int freq[], int size) {
    struct HuffmanNode* root = buildHuffmanTree(data, freq, size);

    int arr[MAX_TREE_HT], top = 0;
    printCodes(root, arr, top);
}

int main() {
    int size;

    cout << "Enter the size of the character array: ";
    cin >> size;

    char arr[size];
    int freq[size];

    cout << "Enter the characters and their frequencies:" << endl;
    for (int i = 0; i < size; ++i) {
        cout << "Character " << i + 1 << ": ";
        cin >> arr[i];
        cout << "Frequency " << i + 1 << ": ";
        cin >> freq[i];
    }

    HuffmanCodes(arr, freq, size);

    return 0;
}
