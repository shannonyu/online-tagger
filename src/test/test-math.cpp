#include <iostream>
#include "ltp-math.h"

using namespace std;
using namespace ltp::math;

void TEST_1() {
    SparseVector vec;

    vec.add( 1, 1.0 );
    vec.add( 1, -1.0 );
    vec.add( 2, 1.0 );

    for (map<int, double>::iterator itx = vec.begin();
            itx != vec.end(); ++ itx) {
        cout << itx->first << " " << itx->second << endl;
    }
} 

int main() {
    TEST_1();
    return 0;
}
