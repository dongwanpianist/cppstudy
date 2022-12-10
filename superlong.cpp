#ifndef SUPERLONG
#define SUPERLONG
#include <stdlib.h>
typedef unsigned int uint32_t;
class superlong {
public:
    uint32_t length;
    uint32_t* value;
    superlong() {}
    superlong(uint32_t _length, int _value) {
        length = _length;
        value = (unsigned int*)malloc(length*sizeof(uint32_t));
        /////value = _value;
    }
    superlong(uint32_t _length, uint32_t* _value) {
        length = _length;
        value = (unsigned int*)malloc(length*sizeof(uint32_t));
        value = _value;
    }

};
#endif

#include <iostream>
using namespace std;
int main() {
    superlong* mynumber;
    mynumber = new superlong(39, 0); //314159265358979323846264338327950288419
    cout << mynumber->value;
}
