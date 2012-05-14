/*
 *
 *
 *
 *
 *
 *
 *
 *
 */
#ifndef __K_HEAP_2_H__
#define __K_HEAP_2_H__

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
using namespace std;

template <class T>
class KHeap {
public:
    KHeap() {
        _k = 0;
    }

    KHeap(int k) : _k(k) {
    }

    ~KHeap() {
    }

    int size() {
        return _data.size();
    }

    int setK(int k) {
        int _k = k;
    }

    int clear() {
        _data.clear();
        _k = 0;
    }

    void insert(const T& data) {
        if (_data.size() < _k) {
            _data.push_back(data);
            push_heap(_data.begin(), _data.end(), greater<T>());
        }
        else if (_data.front() < data) {
            pop_heap(_data.begin(), _data.end(), greater<T>());
            _data[_k - 1] = data;
            push_heap(_data.begin(), _data.end(), greater<T>());
        }
    }

    T* at(int index) {
        if (index < _data.size()) {
            return &_data[index];
        }
        return NULL;
    }

private:

    int _k;
    vector<T> _data;
};

#endif  // end for __K_HEAP_2_H__
