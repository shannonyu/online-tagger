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
#ifndef __C_DATA_H__
#define __C_DATA_H__

#include "ltp-data.h"
#include "stdio.h"
#include "stdlib.h"

using namespace ltp::framework;

#define __PAIR_HASH__ 0

class CItem : public Item {
public:
    CItem(int numLabels);
    CItem(int numLabels, int form);
    ~CItem();

    void append(int feat, int label);
    int at(int index, int label);
    int size(int label);
    int form();
private:

#if __PAIR_HASH__
    int **m_Features;
    int *m_NumFeats;
    int *m_CapFeats;
#else
    int *m_Features;
    int m_NumFeats;
    int m_CapFeats;
#endif

    int m_NumLabels;
    int m_Form;
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

#endif  // end for __TAGGER_DATA_H__
