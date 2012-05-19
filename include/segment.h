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

#ifndef __LTP_SEGMENT_H__
#define __LTP_SEGMENT_H__

class Segmentor {
public:
    Segmentor();
    ~Segmentor();

    void loadModel(const char *file);
    void segment(const char *line);
};

#endif  // end for __LTP_SEGMENT_H__
