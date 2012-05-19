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
#ifndef _K_HEAP_H_
#define _K_HEAP_H_

#include <iostream>
using namespace std;

template <class T>
class KHeap {
public:
    KHeap();
    KHeap(int k);
    ~KHeap();

    int  size();
    void setK(int k);
    void clear();
    void insert(const T& data);
    T* at(int index);

private:
    int _k;
    int _last;
    T*   _pool;
    T*   _p;
    T**  _wrapper;
};

template <class T>
KHeap<T> :: KHeap() {
    _k       = 0;
    _last    = 0;
    _p       = NULL;
    _pool    = NULL;
    _wrapper = NULL;
}

template <class T>
KHeap<T> :: KHeap(int k) {
    _k       = k;
    _last    = 0;
    _p       = NULL;
    _pool    = NULL;
    _wrapper = NULL;

    _pool    = new T[k + 2];
    _wrapper = new T*[k + 2];
    _p       = _pool;
}

template <class T>
KHeap<T> :: ~KHeap() {
    if (_pool != NULL)    delete [] _pool;
    if (_wrapper != NULL) delete [] _wrapper;
}

template <class T>
inline int KHeap<T> :: size() {
    return _last;
}

template <class T>
void KHeap<T> :: clear() {
    _last = 0;
    _p = _pool;
}

template <class T>
void KHeap<T> :: setK(int k) {
    _k = k;
    _pool    = new T[k + 2];
    _wrapper = new T*[k + 2];
    _p = _pool;
}

template <class T>
inline T* KHeap<T> :: at(int index) {
    if(index >= 0 && index < _last) {
        return _wrapper[index + 1];
    }

    return NULL;
}

template <class T>
inline void KHeap<T> :: insert(const T& _data) {
    if (_last < _k) {
        // push data into pool
        // @TODO
        *_p = _data;
        // link pointer to wrapper
        _wrapper[0]        = _p;
        _wrapper[++ _last] = _p;

        // pool point add
        _p ++;
        // sift up
        int i = (_last >> 1), j = _last;
        T* tmp = _wrapper[_last];

        while ((*tmp) < (*_wrapper[i])) {
            _wrapper[j] = _wrapper[i];
            j = i;
            i = (j >> 1);
        }
        _wrapper[j] = tmp;

    } else {
        // data is smaller than the smallest
        if (_data < *_wrapper[1]) {
            return;
        } else {
            // _data is larger than the smallest and the smallest is delete.
            *_wrapper[1] = _data;
            // sift down
            // siftDown(1);
            int i = (1 << 1), j = 1;
            T* tmp = _wrapper[1];

            while (i <= _last) {
                if (i < _last && (*_wrapper[i] > *_wrapper[i + 1])) ++ i;
                if (*tmp < *_wrapper[i]) break;
                _wrapper[j] = _wrapper[i];
                j = i;
                i = (j << 1);
            }

            _wrapper[j] = tmp;
        }
    }
}

#endif  // end _K_HEAP_H_
