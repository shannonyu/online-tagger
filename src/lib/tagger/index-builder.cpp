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
#include "index-builder.h"
#include <cassert>

int
IndexBuilder :: index(int feature, int label, bool bigram) {
	int base = 0;
	if (bigram == true) {
		base = m_NumFeatures * m_NumLabels;
	}
    assert( base + feature * m_NumLabels + label >= 0 );
	return base + feature * m_NumLabels + label;
}

int
IndexBuilder :: numLabels() {
	return m_NumLabels;
}

int
IndexBuilder :: numFeatures() {
	return m_NumFeatures;
}

int
IndexBuilder :: numParameters() {
	return m_NumFeatures * m_NumLabels + (m_NumLabels + 1) * m_NumLabels;
}
