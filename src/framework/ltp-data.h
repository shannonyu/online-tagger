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

/*
 * Define data storage interface of the framework.
 *
 * There are two fashions in feature extraction
 *
 *  - store the hash of (feature, tag) pair, which is used in
 *    hash kernel experiment.
 *  - store the feature string's hash value, which makes a more
 *    harmoney memory consume.
 *
 * All of the above two fashion can be represent in a single way.
 * When it comes to the second way, just bind the parameter with
 * -1 or some invalid value.
 *
 * Tree structure of Data is shown below.
 *
 *  Data
 *   `- Instance
 *       |- Items
 *       |   `- Item
 *       `- Labels
 */
class Item {
public:
    Item() {}
    virtual ~Item() {}

    /*
     * interface for append a feature/(feature,tag) hash value to
     * the item.
     *
     *  @param[in]  value   the hash value
     *  @parma[in]  label   the label, can be binded with -1
     */
    virtual void append(int value, int label) = 0;

    /*
     * interface for get a feature/(feature,tag) hash value at
     * certain index
     *
     *  @param[in]  index   the index
     *  @param[in]  label   the label, can be binded with -1
     *  @return     the hash value of a feature/(feature, tag)
     */
    virtual int at(int index, int label) = 0;

    /*
     * interface for get number of features in the item
     *
     *  @param[in]  label   the label, can be binded with -1 
     *                      NOTICE: some modification
     *  @return     number of features in the item
     */
    virtual int size(int label) = 0;

    /*
     * NOTICE
     * some modification should be made.
     *
     * interface for get hash value of a forms.
     */
    virtual int form() = 0;
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

    virtual void resize(int n) = 0;

    virtual void set(int label, int index) = 0;

    virtual void append(int label) = 0;

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
