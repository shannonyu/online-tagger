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

