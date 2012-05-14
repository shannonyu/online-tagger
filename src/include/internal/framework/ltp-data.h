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
#ifndef __LTP_DATA_H__
#define __LTP_DATA_H__

namespace ltp {

namespace framework {

class Item {
public:
    Item() {}
    virtual ~Item() {}

    virtual void append(int index, int label) = 0;
    virtual int at(int index, int label) = 0;
    virtual int size(int label) = 0;
    virtual int item() = 0;
};

class Items {
public:
    Items() {}
    virtual ~Items() {}

    virtual void append(Item *item) = 0;
    virtual Item *at(int index) = 0;
    virtual int size() = 0;
};

class Labels {
public:
    Labels() {}
    virtual ~Labels() {}

    virtual void set(int label, int index) = 0;
    virtual void append(int label) = 0;
    virtual void resize(int n) = 0;
    virtual int at(int index) = 0;
    virtual int size() = 0;
};

class Instance {
public:
    Instance() {}
    virtual ~Instance() {}

    virtual Items*  items() = 0;
    virtual Labels* labels() = 0;
};

class Data {
public:
    Data() {}
    virtual ~Data() {}

    virtual void append(Instance *inst) = 0;
    virtual Instance * at(int index) = 0;
    virtual int size() = 0;
};

class DecodeResults {
public:
    DecodeResults() {}
    virtual ~DecodeResults() {}

    virtual void append(Labels *result) = 0;
    virtual Labels* at(int index) = 0;
    virtual Labels* best() = 0;
    virtual int size() = 0;
};

}  // namespace framework

}  // namespace ltp

#endif  // end for __LTP_DATA_H__
