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
#ifndef __TAGGER_DATA_H__
#define __TAGGER_DATA_H__

#include "ltp-data.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace ltp::framework;

class CItem : public Item {
public:
    CItem(int numLabels);
    CItem(int numLabels, int word);
    ~CItem();

    void append(int feat, int label);
    int at(int index, int label);
    int size(int label);
    int item();
private:
    int **m_Features;
    int *m_NumFeats;
    int *m_CapFeats;
    int m_NumLabels;
    int m_Word;
};

class CItems : public Items {
public:
    CItems();
    ~CItems();

    void append(Item *item);
    Item* at(int index);
    int size();
private:
    Item **m_Items;
    int m_NumItems;
    int m_CapItems;
};

class CLabels : public Labels {
public:
    CLabels();
    CLabels(int n);
    ~CLabels();

    void set(int label, int index);
    void append(int label);
    void resize(int n);
    int at(int index);
    int size();
private:
    int *m_Labels;
    int m_NumLabels;
    int m_CapLabels;
};

class CInstance : public Instance {
public:
    CInstance(Items *items, Labels *labels);
    ~CInstance();

    Items*  items();
    Labels* labels();
private:
    Items*  m_Items;
    Labels* m_Labels;
};

class CData : public Data {
public:
    CData();
    ~CData();

    void append(Instance *inst);
    Instance *at(int index);
    int size();
private:
    Instance **m_Instances;
    int m_NumInstances;
    int m_CapInstances;
};

class CDecodeResults : public DecodeResults {
public:
    CDecodeResults();
    ~CDecodeResults();

    void append(Labels *result);
    Labels *at(int index);
    Labels *best();
    int size();
private:
    Labels **m_Results;
    int m_NumResults;
    int m_CapResults;
};

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


#endif  // end for __TAGGER_DATA_H__
