#include <iostream>

#ifndef SUPERLONG
#define SUPERLONG
#define SUPERLONG_MAXLENGTH 30000
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
class superlong {
private:
    void getlength() {
        if (*value == 0) {
            length = 0;
            arraylength = 1;
            return;
        }
        uint16_t len = 0;
        uint64_t temp = *value;
        while (true) {
            temp = temp/10;
            ++len;
            if (temp == 0) break;
        }
        arraylength = (len/19)+(len%19>0 ? 1:0);
        length = len;
    }
    uint16_t getlength(char* str, bool internal = true) {
        if (str[0] == 0) return 0;
        bool _sign = (str[0] == '-'? false:true);
        uint16_t cursor = 0;
        if (!_sign) ++cursor;
        if (str[cursor] == '0') {
            _sign = true;
            cursor = 0;
        } else {
            while ((str[cursor] >= '0') && (str[cursor] <= '9')) cursor++;
            if (!_sign) --cursor;
        }
        if (internal) {
            sign = _sign;
            arraylength = (cursor/19)+(cursor%19>0 ? 1:0);
            length = cursor;
        }
        return cursor;
    }
    uint16_t getlength(uint64_t number, bool internal = true) {
        if (number == 0) return 0;
        uint16_t len = 0;
        while (true) {
            number = number/10;
            ++len;
            if (number == 0) break;
        }
        if (internal) {
            arraylength = (len/19)+(len%19>0 ? 1:0);
            length = len;
        }
        return len;
    }
    void replace(int64_t _signed_value) {
        // 포인터 청소
        if (arraylength > 1) delete [] value;
        else delete value;
        //delete[] str;
        // 음수 부호 분리
        if (_signed_value < 0) {
            sign = 0;
            _signed_value = -_signed_value;
        } else sign = 1;
        // 새로 입력 (길이 <= 19, 배열크기 = 1)
        value = new uint64_t((uint64_t)_signed_value);
        getlength();
        num2str(*value);
    }
    void replace(uint64_t _value) {
        // 포인터 청소
        sign = 1;
        if (arraylength > 1) delete [] value;
        else delete value;
        //delete [] str;
        getlength(_value);
        value = new uint64_t;
        if (arraylength > 1) { // 10^20이 넘어가면 추가 배열 사용
            uint64_t* newvalue = (uint64_t*)malloc(sizeof(uint64_t)*arraylength);
            newvalue[0] = _value - 10000000000000000000ULL;
            newvalue[1] = 1ULL;
            value = newvalue;
        } else {
            std::cout << "else 5\n";
            value = (uint64_t*)malloc(sizeof(uint64_t));
            std::cout << "else 6\n";
            value = &_value;
        }
        num2str(_value);
    }
    void replace(const char* _value) {
        // 포인터 청소
        if (arraylength > 1) delete [] value;
        else delete value;
        //delete [] str;
        str2num (_value);
        if (*value == 0) {
            char* _str = new char[2];
            _str[0] = '0';
            _str[1] = 0;
            str = _str;
        } else str = _value;
    }
    char* num2str(uint64_t _value, bool internal = true) {
        if (_value == 0) {
            char* result = (char*)malloc(sizeof(char)*2);
            result[0] = '0';
            result[1] = 0;
            if (internal) str = result;
            return result;
        }
        uint16_t size;
        if (internal) size = length+1;          // adding 1 for final '\0'
        else size = getlength(_value, false)+1; // adding 1 for final '\0'
        if (sign == 0) ++size;
        char* result = new char[size];
        if (sign == 0) result[0] = '-';
        result[--size] = 0;
        uint8_t buf;
        for (; size > !sign; --size) {
            buf = _value % 10;
            result[size-1] = buf + '0';
            _value /= 10;
        }
        if (internal) str = result;
        return result;
    }
    void setzero() {
        value = new uint64_t(0ULL);
        arraylength = 0;
        length = 0;
        sign = 1;
        char* zerostr = new char[2];
            zerostr[0] = '0';
            zerostr[1] = 0;
        str = zerostr;
    }
    const uint64_t* str2num (const char* _value, bool internal = true) {
        if (_value == nullptr) {
            if (internal) {
                setzero();
                return value;
            } else {
                uint64_t* result = new uint64_t(0ULL);
                return result;
            }
        } else if ((_value[0] == 0) || (_value[0] == '0')) {
            if (internal) {
                setzero();
                return value;
            } else {
                uint64_t* result = new uint64_t(0ULL);
                return result;
            }
        }
        bool _signshift = (_value[0] == '-');
        if ((_signshift) && (_value[1] == 0) || (_signshift) && (_value[1] == '0')) {
            if (internal) {
                setzero();
                return value;
            } else {
                uint64_t* result = new uint64_t(0ULL);
                return result;
            }
        }
        uint16_t len = getlength((char*)_value, internal);
        uint8_t firstarraylength = len%19;
        int _arraylength = (len/19)+(firstarraylength>0 ? 1:0);
        uint64_t* result = (uint64_t*)malloc(_arraylength);
        uint8_t arr = 0, cursor = 0 + _signshift;
        // first container (first rest of numbers)
        result[_arraylength-1] = 0; //initialize
        for (; cursor < firstarraylength + _signshift; ++cursor) {
            result[_arraylength-1] = (result[_arraylength-1] * 10) + (_value[cursor] - '0');
        }
        // second and more contaniers (rest of numbers, in each full group of 19 digits)
        if (_arraylength > 1) {
            for (arr = 0; arr < _arraylength-1; ++arr) {
                result[_arraylength-2-arr] = 0; //initialize
                for (cursor = firstarraylength + _signshift + (19*arr); cursor < firstarraylength + _signshift + (19*(arr+1)); cursor++) {
                    result[_arraylength-2-arr] = result[_arraylength-2-arr]*10 + (_value[cursor] - '0');
                }
            }
        }
        if (internal) {
            value = result;
            str = _value;
        }
        return result;
    }
public:
    int arraylength;
    uint16_t length;
    bool sign;
    const uint64_t* value;
    const char* str;
    superlong() {
        length = 1;
        arraylength = 1;
        sign = 1;
        value = new uint64_t(0);
        char* _str = new char[2];
        _str[0] = '0';
        _str[1] = 0;
        str = _str;
    }
    explicit superlong(int8_t _value) {
        superlong((int64_t)_value);
    }
    explicit superlong(int16_t _value) {
        superlong((int64_t)_value);
    }
    explicit superlong(int32_t _value) {
        superlong((int64_t)_value);
    }
    explicit superlong(int64_t _value) {
        value = new uint64_t(0);
        char* _str = new char;
        replace(_value);
    }
    explicit superlong(bool _value) {
        superlong((uint64_t)_value);
    }
    explicit superlong(uint8_t _value) {
        superlong((uint64_t)_value);
    }
    explicit superlong(uint16_t _value) {
        superlong((uint64_t)_value);
    }
    explicit superlong(uint32_t _value) {
        superlong((uint64_t)_value);
    }
    explicit superlong(uint64_t _value) {
        value = new uint64_t(0);
        char* _str = new char;
        replace(_value);
    }
    explicit superlong(const char* _value) {
        value = new uint64_t(0);
        char* _str = new char;
        replace(_value);
    }
    ~superlong() {
        // if (arraylength > 1) delete[] value;
        // else delete value;
        // delete[] str;
    }
    void operator=(int8_t newnumber) {
        replace((int64_t)newnumber);
    }
    void operator=(int16_t newnumber) {
        replace((int64_t)newnumber);
    }
    void operator=(int32_t newnumber) {
        replace((int64_t)newnumber);
    }
    void operator=(int64_t* newnumber) {
        replace(*newnumber);
    }
    void operator=(int64_t newnumber) {
        replace(newnumber);
    }
    void operator=(bool newnumber) {
        replace((uint64_t)newnumber);
    }
    void operator=(uint8_t newnumber) {
        replace((uint64_t)newnumber);
    }
    void operator=(uint16_t newnumber) {
        replace((uint64_t)newnumber);
    }
    void operator=(uint32_t newnumber) {
        replace((uint64_t)newnumber);
    }
    void operator=(uint64_t* newnumber) {
        replace(*newnumber);
    }
    void operator=(uint64_t newnumber) {
        replace(newnumber);
    }
    void operator=(const char* newstring) {
        replace(newstring);
    }
    #ifdef _LIBCPP_OSTREAM
        friend std::ostream& operator<<(std::ostream &ost, const superlong &number) {
            ost << number.str;
            return ost;
        }
    #endif
    #ifdef _LIBCPP_ISTREAM
        friend std::istream& operator>>(std::istream &ist, superlong &number) {
            char* _str = new char[SUPERLONG_MAXLENGTH];
            ist >> _str;
            number.operator=(_str);
            return ist;
        }
    #endif
};
#endif


superlong mynumber;
using namespace std;
void print() {
    cout << "Value: ";
    if (mynumber.arraylength > 1) {
        for (int i = mynumber.arraylength-1; i >= 0; i--) {
            cout << mynumber.value[i] << " ";
        }
    } else cout << *mynumber.value;
    cout << "\n";
    cout << "Sign: " << (mynumber.sign?'+':'-') << " / Length: " << mynumber.length << " / ArrayLength: " << mynumber.arraylength << " / String: \"" << mynumber << "\"\n";
    cout << "==============================" << endl;
}
int main() {
    uint64_t input1 = ULLONG_MAX;
    int64_t input2 = LLONG_MAX;
    int16_t input3 = -789;
    int input4 = 1;
    uint64_t input5 = 9999999999999999999ULL + 1;
    const char* input6 = "3141592653589793238462643383279502884197169399375105820";//"31415926535897932 3846264338327950288 4197169399375105820"
    cout << "Input your number > ";
    cin >> mynumber;
    print();
    mynumber = input1;
    print();
    mynumber = input2;
    print();
    mynumber = input3;
    print();
    mynumber = 0;
    print();
    mynumber = input4;
    print();
    mynumber = input5;
    print();
    mynumber = input6;
    print();
}
