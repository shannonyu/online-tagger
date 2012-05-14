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

#include "data.h"

// ==============================================
// CItem::Operations
// ==============================================

CItem :: CItem(int numLabels) : m_NumLabels(numLabels), m_Word(-1) {
    m_NumFeats = new int[numLabels];
    m_CapFeats = new int[numLabels];
    m_Features = new int *[numLabels];

    for (int i = 0; i < numLabels; ++ i) {
        m_NumFeats[i] = 0;
        m_CapFeats[i] = 0;
        m_Features[i] = 0;
    }
}

CItem :: CItem(int numLabels, 
        int word) : m_NumLabels(numLabels), m_Word(word) {
    m_NumFeats = new int[numLabels];
    m_CapFeats = new int[numLabels];
    m_Features = new int *[numLabels];

    for (int i = 0; i < numLabels; ++ i) {
        m_NumFeats[i] = 0;
        m_CapFeats[i] = 0;
        m_Features[i] = 0;
    }
}

CItem :: ~CItem() {
    delete [] m_NumFeats;
    delete [] m_CapFeats;
    for (int i = 0; i < m_NumLabels; ++ i) {
        delete [] m_Features[i];
    }
    delete [] m_Features;
}

void
CItem :: append(int feat, int label) {
    if (label < 0 || label >= m_NumLabels) {
        return;
    }

    if (m_CapFeats[label] <= m_NumFeats[label]) {
        m_CapFeats[label] = ((m_CapFeats[label] << 1) + 1);
        m_Features[label] = (int *)realloc(
                m_Features[label], m_CapFeats[label] * sizeof(int));
    }
    m_Features[label][m_NumFeats[label]] = feat;
    ++ m_NumFeats[label];
}

inline int
CItem :: at(int index, int label) {
    if (label < 0 || label >= m_NumLabels) {
        return -1;
    }

    if (index >= 0 && index < m_NumFeats[label]) {
        return m_Features[label][index];
    }
    return -1;
}

inline int
CItem :: size(int label) {
    if (label < 0 || label >= m_NumLabels)
        return -1;
    return m_NumFeats[label];
}

inline int
CItem :: item() {
    return m_Word;
}

// ==============================================
// CItems::Operations
// ==============================================

CItems :: CItems() {
    m_NumItems = 0;
    m_CapItems = 0;
    m_Items = NULL;
}

CItems :: ~CItems() {
    for (int i = 0; i < m_NumItems; ++ i) {
        delete m_Items[i];
    }

    delete[] m_Items;
}

void
CItems :: append(Item *item) {
    if (m_CapItems <= m_NumItems) {
        m_CapItems = ((m_CapItems << 1) + 1);
        m_Items = (Item **)realloc(m_Items, m_CapItems * sizeof(Item *));
    }
    m_Items[m_NumItems] = item;
    ++ m_NumItems;
}

inline Item*
CItems :: at(int index) {
    if (index >= 0 && index < m_NumItems) {
        return m_Items[index];
    }
    return NULL;
}

int
CItems :: size() {
    return m_NumItems;
}

// ==============================================
// CLabels::Operations
// ==============================================

CLabels :: CLabels() {
    m_NumLabels = 0;
    m_CapLabels = 0;
    m_Labels = NULL;
}

CLabels :: CLabels(int n) {
    m_NumLabels = n;
    m_CapLabels = n;
    m_Labels = new int[n];
}

CLabels :: ~CLabels() {
    delete[] m_Labels;
}

void
CLabels :: set(int label, int index) {
    if (index >= 0 && index < m_NumLabels) {
        m_Labels[index] = label;
    }
}

void
CLabels :: append(int label) {
    if (m_CapLabels <= m_NumLabels) {
        m_CapLabels = (m_CapLabels << 1) + 1;
        m_Labels = (int *)realloc(m_Labels, m_CapLabels * sizeof(int));
    }
    m_Labels[m_NumLabels] = label;
    ++ m_NumLabels;
}

void
CLabels :: resize(int n) {
//    cerr << "Operation resize is not provided." << endl;
}

int
CLabels :: at(int index) {
    if (index >= 0 && index < m_NumLabels) {
        return m_Labels[index];
    }
    return -1;
}

int
CLabels :: size() {
    return m_NumLabels;
}

// ==============================================
// CInstance::Operations
// ==============================================

CInstance :: CInstance(Items *items, Labels *labels) : m_Items(items), m_Labels(labels) {
}

CInstance :: ~CInstance() {
    delete m_Items;
    delete m_Labels;
}

Labels *
CInstance :: labels() {
    return m_Labels;
}

Items *
CInstance :: items() {
    return m_Items;
}

// ==============================================
// CData::Operations
// ==============================================

CData :: CData() {
    m_NumInstances = 0;
    m_CapInstances = 0;
    m_Instances = NULL;
}

CData :: ~CData() {
    for (int i = 0; i < m_NumInstances; ++ i) {
        delete m_Instances[i];
    }
    delete[] m_Instances;
}

void
CData :: append(Instance *inst) {
    if ( m_CapInstances <= m_NumInstances) {
        m_CapInstances = ((m_CapInstances << 1) + 1);
        m_Instances = (Instance **)realloc(m_Instances, m_CapInstances * sizeof(Instance *));
    }
    m_Instances[m_NumInstances] = inst;
    ++ m_NumInstances;
}

Instance *
CData :: at(int index) {
    if (index >= 0 && index < m_NumInstances) {
        return m_Instances[index];
    }
    return NULL;
}

inline int
CData :: size() {
    return m_NumInstances;
}

CDecodeResults :: CDecodeResults() {
    m_NumResults = 0;
    m_CapResults = 0;
    m_Results = NULL;
}

CDecodeResults :: ~CDecodeResults() {
    for (int i = 0; i < m_NumResults; ++ i) {
        delete m_Results[i];
    }

    delete[] m_Results;
}

void
CDecodeResults :: append(Labels *result) {
    if (m_CapResults <= m_NumResults) {
        m_CapResults = ((m_CapResults << 1) + 1);
        m_Results = (Labels **)realloc(m_Results, m_CapResults * sizeof(Labels *));
    }

    m_Results[m_NumResults] = result;
    ++ m_NumResults;
}

Labels *
CDecodeResults :: at(int index) {
    if (index >= 0 && index < m_NumResults) {
        return m_Results[index];
    }
    return NULL;
}

Labels *
CDecodeResults :: best() {
    if (m_NumResults > 0)
        return m_Results[0];
    return NULL;
}

int
CDecodeResults :: size() {
    return m_NumResults;
}

// ==============================================
// CppItem :: Operations
// ==============================================

CppItem :: CppItem(int numLabels) : m_NumLabels(numLabels), 
    m_Word(-1) {
    m_Features.resize(numLabels);
}

CppItem :: CppItem(int numLabels, int word) : m_NumLabels(numLabels),
    m_Word(word) {
    m_Features.resize(numLabels);
}


inline void 
CppItem :: append(int feat, int label) {
    if (label < 0 || label >= m_NumLabels) {
        return;
    }
    m_Features[label].push_back(feat);
}

inline int
CppItem :: at(int index, int label) {
    if (label < 0 || label > m_NumLabels) {
        return -1;
    }
    if (index < 0 || index > m_Features[label].size()) {
        return -1;
    }
    return m_Features[label][index];
}

inline int
CppItem :: item() {
    return m_Word;
}

inline int
CppItem :: size(int label) {
    if (label < 0 || label > m_NumLabels)
        return -1;

    return m_Features[label].size();
}

// ==============================================
// CppItems :: Operations
// ==============================================

CppItems :: ~CppItems() {
    for (int i = 0; i < m_Items.size(); ++ i) {
        delete m_Items[i];
    }
}

inline void
CppItems :: append(Item* item) {
    m_Items.push_back( item );
}

inline Item* 
CppItems :: at(int index) {
    if (index >= 0 || index < m_Items.size()) {
        return m_Items[index];
    }
    return NULL;
}

inline int
CppItems :: size() {
    return m_Items.size();
}

// ==============================================
// CppLabels :: Operations
// ==============================================

inline void
CppLabels :: resize(int n) {
    m_Labels.resize(n);
}

inline void
CppLabels :: set(int label, int index) {
    if (index >= 0 && index < m_Labels.size()) {
        m_Labels[index] = label;
    }
}

inline void
CppLabels :: append(int label) {
    m_Labels.push_back( label );
}

inline int
CppLabels :: at(int index) {
    if (index >= 0 && index < m_Labels.size()) {
        return m_Labels[index];
    }
    return -1;
}

inline int
CppLabels :: size() {
    return m_Labels.size();
}

// ==============================================
// CppInstance :: Operations
// ==============================================

CppInstance :: ~CppInstance() {
    delete m_Items;
    delete m_Labels;
}

inline Items*
CppInstance :: items() {
    return m_Items;
}

inline Labels*
CppInstance :: labels() {
    return m_Labels;
}

// ==============================================
// CppData :: Operations
// ==============================================

CppData :: ~CppData() {
    for (int i = 0; i < instances.size(); ++ i) {
        delete instances[i];
    }
}

inline void
CppData :: append(Instance *inst) {
    instances.push_back( inst );
}

inline Instance *
CppData :: at(int index) {
    if (index < 0 || index > instances.size()) {
        return NULL;
    } else {
        return instances[index];
    }
}

inline int
CppData :: size() {
    return instances.size();
}

// ==============================================
// CppDecodeResults :: Operations
// ==============================================

inline void
CppDecodeResults :: append(Labels *result) {
    m_Results.push_back( result );
}

inline Labels *
CppDecodeResults :: at(int index) {
    if (index < 0 || index > m_Results.size()) {
        return NULL;
    }
    return m_Results[index];
}

inline Labels*
CppDecodeResults :: best() {
    return m_Results[0];
}

inline int
CppDecodeResults :: size() {
    return m_Results.size();
}

