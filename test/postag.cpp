#include "otpos_dll.h"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    vector<string> sent;
    vector<string> pos;
    string raw_sent[] = {"新华社", "上海", "二月", 
        "十日", "电", "（", "记者",
        "谢金虎", "、", "张持坚", "）"};

    sent.resize(11);
    for (int i = 0; i < 11; ++ i)
        sent[i] = raw_sent[i];

    otpos_t handle = OTPOS_Load("./postag.cfg");

    int numofwords = OTPOS_Postag_x(handle, sent, pos);
    for (int i = 0; i < numofwords; ++ i)
        cout << sent[i] << 
            "(" << pos[i] << ") | ";
    cout << endl;

    OTPOS_Destroy(handle);
    return 0;
}
