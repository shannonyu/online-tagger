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

#ifndef __CPP_DATA_H__
#define __CPP_DATA_H__

#include "ltp-data.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace ltp::framework;


class CppItem : public Item {
public:
    CppItem(int numLabels);
    CppItem(int numLabels, int word);
    ~CppItem() {}

    void append(int feat, int label);
    int at(int index, int label);
    int size(int label);
    int item();
private:
    vector< vector<int> > m_Features;
    int m_NumLabels;
    int m_Word;
};

class CppItems : public Items {
public:
    CppItems() {}
    ~CppItems();

    void append(Item* item);
    Item* at(int index);
    int size();
private:
    vector<Item*> m_Items;
};

class CppLabels : public Labels {
public:
    CppLabels() {}
    CppLabels(int n) { m_Labels.resize(n, 0); }
    ~CppLabels() {}

    void set(int label, int index);
    void append(int label);
    void resize(int n);
    int at(int index);
    int size();
private:
    vector<int> m_Labels;
};

class CppInstance : public Instance {
public:
    CppInstance(Items* items, Labels* labels): m_Items(items), m_Labels(labels) {}
    ~CppInstance();

    Items*  items();
    Labels* labels();
private:
    Items*  m_Items;
    Labels* m_Labels;
};

class CppData : public Data {
public:
    CppData() {}
    ~CppData();

    void append(Instance* inst);
    Instance* at(int index);
    int size();
private:
    vector<Instance*> instances;
};

class CppDecodeResults : public DecodeResults {
public:
    CppDecodeResults() {}
    ~CppDecodeResults() {}

    void append(Labels* result);
    Labels* at(int index);
    Labels* best();
    int size();
private:
    vector<Labels*> m_Results;
};

#endif  // end for __CPP_DATA_H__
