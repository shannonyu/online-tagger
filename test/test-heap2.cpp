#include "k-heap2.h"
#include <ctime>
#include <queue>

void TEST_1() {
    KHeap<int>* heap = new KHeap<int>(5);
    int x[] = { 1, 3, 7,4, 2, 8, 5, 9, 6, 0 };
    for (int i = 0; i < 10; ++ i) {
        heap->insert( x[i] );
    }

    for (int i = 0; i < heap->size(); ++ i) {
        cout << (*heap->at(i)) << endl;
    }

    delete heap;
}

void TEST_2() {
    int N = 1024;
    int K = 128;
    int M = 10000;
    KHeap<int>* heap = new KHeap<int>(K);

    srand(time(NULL));
    int *x = new int[10000];
    for (int i = 0; i < N; ++ i) {
        x[i] = rand() % M;
        cout << x[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < N; ++ i) {
        heap->insert( x[i] );
    }

    for (int i = 0; i < heap->size(); ++ i) {
        cout << (*heap->at(i)) << endl;
    }

    delete heap;
    delete x;
}

struct node {
    node() {}
    node(int _x, int _y): x(_x), y(_y) {}
    int x;
    int y;

    bool operator < (const node& T) const {
        return x < T.x;
    }

    bool operator == (const node& T) const {
        return x == T.x;
    }

    bool operator > (const node& T) const {
        return x > T.x;
    }
};

void PERFORMANCE_1(int *x, int *y, int size) {
    KHeap< node > heap(128);
    for (int i = 0; i < size; ++ i) {
        heap.insert(node(x[i],y[i]));
    }
}

void PERFORMANCE_2(int *x, int *y, int size) {
    priority_queue< node > heap;

    for (int i = 0; i < size; ++ i) {
        if (heap.size() < 128) heap.push( node(x[i], y[i]) );
        else {
            if (heap.top() < node(x[i], y[i])) {
                heap.pop();
                heap.push(node(x[i], y[i]));
            }
        }
    }
}

int main() {
    int N = 1024;
    int M = 100000;
    int *x = new int[N];
    int *y = new int[N];

    srand(time(NULL));
    TEST_1();

    for (int i = 0; i < N; ++ i) {
        x[i] = rand() % M;
        y[i] = rand() % M;
    }

    cout << time(NULL) << endl;
    for (int i = 0; i < 100000; ++ i) {
        PERFORMANCE_1(x, y, N);
    }
    cout << time(NULL) << endl;
    for (int i = 0; i < 100000; ++ i) {
        PERFORMANCE_2(x, y, N);
    }
    cout << time(NULL) << endl;
    return 0;
}
