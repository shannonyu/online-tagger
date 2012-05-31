#include "otws_dll.h"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    string text[] = {
        "美国内华达州３１匹野马遭枪杀",
        "自古英雄识英雄，在足球明星维阿里的眼里，只有神奇小子欧文可以成为超级巨星。"};
    vector<string> vec;

    otws_t handle = OTWS_Load("./wordseg.cfg");
    for (int t = 0; t < 2; ++ t) {
        int numofwords = OTWS_Wordseg_x(handle, text[t], vec);
        for (int i = 0; i < numofwords; ++ i)
            cout << vec[i] << " | ";
        cout << endl;
    }
    OTWS_Destroy(handle);
    return 0;
}
