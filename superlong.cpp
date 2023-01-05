#ifndef _LIBCPP_IOSTREAM
#include <iostream>
#endif
#ifndef _LIBCPP_CSTDLIB
#include <cstdlib>
#endif
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

bool isminus(char* _str) { return
        isminus((const char*)_str); }
bool isminus(const char* _str) {
    if (_str == NULL) return false;
    if (_str[0] == '-') return true;
    return false;
}
int32_t _strlen(char* _str) { return
        _strlen((const char*)_str); }
int32_t _strlen(const char* _str) {
    if (_str == NULL) return -1;
    if (_str[0] == 0) return 0;
    int l = 0;
    while (_str[l] == 0) ++l;
    return l;
}
int32_t _strnumlen(char* _str) { return
        _strnumlen((const char*)_str); }
int32_t _strnumlen(const char* _str) { // 음수기호는 통과, 숫자만 세기
    if (_str == NULL) return -1;
    int l = 0;
    if (_str[0] == '-') ++l;
    if (_str[l] == 0) return 0;
    while ((_str[l] >= '0') && (_str[l] <= '9')) ++l;
    if (_str[0] == '-') --l;
    return l; // _strnumlen("-1111a1") = 4
}
int32_t _numlen(uint64_t _value) {
    int l = 0;
    while (_value) { // not zero
        _value /= 10;
        ++l;
    }
    return l; // _numlen(0) = 0
}
char* _strappend(char* str1, char* str2) { return
        _strappend((const char*)str1, (const char*)str2); }
char* _strappend(const char* str1, const char* str2) {
    int len1 = _strlen(str1);
    int len2 = _strlen(str2);
    int i;
    char* newstr = (char*)calloc(len1 + len2 + 1, 1);
    for (i = 0; i < len1; i++) newstr[i] = str1[i];
    for (i = 0; i < len2; i++) newstr[i+len1] = str2[i];
    newstr[len1 + len2 + 1] = 0;
    return newstr;
}
char* _strappendchar(char ch1, int times, char* str2) { return
        _strappendchar(ch1, times, (const char*)str2); }
char* _strappendchar(char ch1, int times, const char* str2) {
    int len2 = _strlen(str2);
    int i;
    char* newstr = (char*)calloc(times + len2 + 1, 1);
    for (i = 0; i < times; i++) newstr[i] = ch1;
    for (i = 0; i < len2; i++) newstr[times+i] = str2[i];
    newstr[times + len2 + 1] = 0;
    return newstr;
}
char* _strappendchar(char* str1, char ch2, int times) { return
        _strappendchar((const char*)str1, ch2, times); }
char* _strappendchar(const char* str1, char ch2, int times) {
    int len1 = _strlen(str1);
    int i;
    char* newstr = (char*)calloc(len1 + times + 1, 1);
    for (i = 0; i < len1; i++) newstr[i] = str1[i];
    for (i = 0; i < times; i++) newstr[len1+i] = ch2;
    newstr[len1 + times + 1] = 0;
    return newstr;
}
char* _substr(const char* str, int &position, int length = 0) {
    int newlen = _strlen(str);
    char* result;
    if (length > 0) if (position+length < newlen) newlen = position+length;
    for (int i = position; i < newlen; i++) {
        result[i-position] = str[i];
    }
    return result;
}
void addmatchzeros(const char* str1, const char* str2) {
    int sizediff = _strlen(str1) - _strlen(str2);
    if (sizediff > 0) {
        str2 = _strappendchar('0', sizediff, str2);
    } else if (sizediff < 0) {
        sizediff = -sizediff;
        str1 = _strappendchar('0', sizediff, str1);
    }
}
void trimzeros(const char* _str) {
    if (_str == NULL) return;
    if (_str[0] == 0) return;
    bool minus = (_str[0] == '-');
    int zerocount = 0;
    int len = _strlen(_str);
    if (minus) zerocount++;
    for (; zerocount < len; zerocount++) { if (_str[zerocount] != '0') break; }
    _str = _substr(_str, zerocount);
    if (minus) _str = _strappendchar('-', 1, _str);
}
void _debug_printstrchar(const char* _str) {
    if (_str == NULL) {
        std::cout << "NULL" << std::endl;
        return;
    }
    int len = _strlen(_str);
    std::cout << "{ ";
    for (int i = 0; i < len; i++) {
        std::cout << (int16_t)_str[i];
        if (i < len-1) std::cout << ', ';
    }
    std::cout << "} [" << len << "]" << std::endl;
}
void _debug_printstr(const char* _str) {
    if (_str == NULL) {
        std::cout << "NULL" << std::endl;
        return;
    }
    int len = _strlen(_str);
    std::cout << "\"" << _str << "\" [" << len << "]" << std::endl;
}

class superlong {
private:
    bool alloc_array = false;
    bool alloc_str = false;
    void calclength64(uint64_t* _value) {
        if (_value == NULL) {
            length = 0;
            arraylength = 1;
        }
        if (*_value == 0) {
            length = 0;
            arraylength = 1;
            return;
        }
        length = _numlen(*_value);
        arraylength = (length/19)+(length%19>0);
    }
    void calclengthstr(char* _str) {

        int32_t l = _strlen(_str);
        if (_str == NULL) {
            return;
        }
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
    void replace(int64_t _signed_value, bool init = false) {
        // 포인터 청소
        if (!init) {
            if (arraylength > 1) delete [] value;
            else delete value;
        }
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
    void replace(uint64_t _value, bool init = false) {
        // 포인터 청소
        sign = 1;
        if (!init) {
            if (arraylength > 1) delete [] value;
            else delete value;
        }
        //delete [] str;
        getlength(_value);
        value = new uint64_t;
        if (arraylength > 1) { // 10^20이 넘어가면 추가 배열 사용
            uint64_t* newvalue = (uint64_t*)malloc(sizeof(uint64_t)*arraylength);
            newvalue[0] = _value - 10000000000000000000ULL;
            newvalue[1] = 1ULL;
            value = newvalue;
        } else {
            value = (uint64_t*)malloc(sizeof(uint64_t));
            value = &_value;
        }
        num2str(_value);
    }
    void replace(const char* _value, bool init = false) {
        // 포인터 청소
        if (!init) {
            if (arraylength > 1) delete [] value;
            else delete value;
        }
        //delete [] str;
        str2num (_value);
        if ((arraylength <= 1) && (*value == 0)) {
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
    void freearray() {
        if (alloc_array) {
            if (arraylength > 1) delete[] value;
            else delete value;
            alloc_array = false;
        }
    }
    void freestr() {
        if (alloc_str) {
            if (length > 0) delete[] str;
            else delete str;
            alloc_str = false;
        }
    }
    void allocarray(uint16_t newarraylength = 1, bool freed = true) {
        if (!alloc_array) {
            if (newarraylength > 1) value = new uint64_t[newarraylength];
            else value = new uint64_t(0ULL);
        } else if (arraylength != newarraylength) {
            if ((arraylength > 1) || (newarraylength > 1)) {
                if (!freed) {
                    if (arraylength > 1) delete[] value;
                    else delete value;
                }
                if (newarraylength > 1) value = new uint64_t[newarraylength];
                else value = new uint64_t(0ULL);
            }
        }
        alloc_array = true;
    }
    void allocstr(uint16_t newstrlength = 2) {
        if (!alloc_str) {
            if (newstrlength > 2) str = new char[newstrlength];
            else {
                char* zerostr = new char[2];
                zerostr[0] = '0';
                zerostr[1] = 0;
                str = zerostr;
            }
            alloc_str = true;
        }
    }
    void setzero() {
        freearray();
        allocarray(0);
        freestr();
        allocstr(0);
        arraylength = 1;
        length = 1;
        sign = 1;
    }
    const uint64_t* str2num (const char* _value, bool internal = true) {
        if (_value == nullptr) {
            if (internal) {
                setzero();
                return value;
            } else {
                uint64_t* tempvalue = new uint64_t(0ULL);
                return tempvalue;
            }
        }
        bool _signshift = (_value[0] == '-');
        if ((_value[_signshift] == 0) || (_value[_signshift] <= '0') || (_value[_signshift] > '9')) {
            if (internal) {
                setzero();
                return value;
            } else {
                uint64_t* tempvalue = new uint64_t(0ULL);
                return tempvalue;
            }
        }
        uint16_t len = getlength((char*)_value, internal);
        uint8_t firstarraylength = len%19;
        int _arraylength = (len/19)+(firstarraylength>0 ? 1:0);
        uint64_t* result = (uint64_t*)malloc(_arraylength);
        uint8_t arr = 0;
        uint16_t cursor = 0 + _signshift;
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
    char* addnumstr(const char* str1, const char* str2) {
        bool carry10 = false;
        uint8_t calc;
        addmatchzeros(str1, str2);
        int totallen = sl_strlen(str1);
        char* result = (char*)calloc(totallen, 1);
        for (int i = totallen-1; i >= 0; i--) {
            calc = (str1[i]-'0') + (str2[i]-'0') + carry10;
            if (calc >= 10) {
                calc -= 10;
                carry10 = true;
            } else carry10 = false;
            result[i] = (calc+'0');
        }
        if (carry10 == true) return (char*)sl_strappend("1", result);
        else return (char*) result;
    }
    /*
    string subtractnumstr(string str1, string str2, bool absolute = false) {
        string result;
        bool carry10 = false;
        int8_t calc;
        addmatchzeros(&str1, &str2);
        result.resize(str1.size());
        for (int i = str1.size()-1; i >= 0; i--) {
            calc = (str1[i]-'0') - (str2[i]-'0') - carry10;
            if (calc < 0) {
                calc += 10;
                carry10 = true;
            } else carry10 = false;
            result[i] = (calc+'0');
        }
        if (carry10 == true) {
            string newstr1 = string(result.size(), '0');
            newstr1[0] = carry10 + '0';
            string newstr2 = result.substr(1);
            return (absolute? "":"-") + subtractnumstr(newstr1, newstr2);
        }
        trimzeros(&result);
        return result;
    }
    */
public:
    uint16_t arraylength;
    uint16_t length;
    bool sign;
    const uint64_t* value;
    const char* str;
    superlong() {
        setzero();
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
        allocarray(0);
        allocstr(0);
        replace(_value, true);
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
        allocarray(0);
        allocstr(0);
        replace(_value, true);
    }
    explicit superlong(char* _value) {
        superlong((const char*)_value);
    }
    explicit superlong(const char* _value) {
        allocarray(0);
        allocstr(0);
        replace(_value, true);
    }
    ~superlong() {
        freearray();
        freestr();
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
    void operator=(int64_t newnumber) {
        replace(newnumber);
    }
    void operator=(int64_t* newnumber) {
        replace(*newnumber);
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
    void operator=(uint64_t newnumber) {
        replace(newnumber);
    }
    void operator=(uint64_t* newnumber) {
        replace(*newnumber);
    }
    void operator=(const char* newstring) {
        replace(newstring);
    }
    friend superlong operator+ (const superlong& longnum1, const superlong& longnum2) {
        superlong superlongsum;
        superlongsum = superlongsum.addnumstr(longnum1.str, longnum2.str);
        return superlongsum;
    }
    /*
    friend superlong operator- (const superlong& longnum1, const superlong& longnum2) {
        return superlong(subtractnumstr(longnum1.str, longnum2.str));
    }
    */
    //#ifdef _LIBCPP_ISTREAM
        friend std::istream& operator>>(std::istream &ist, superlong &number) {
            char* _str = new char[SUPERLONG_MAXLENGTH];
            ist >> _str;
            number.replace((const char*)_str);
            return ist;
        }
    //#endif
    //#ifdef _LIBCPP_OSTREAM
        friend std::ostream& operator<<(std::ostream &ost, const superlong &number) {
            ost << number.str;
            return ost;
        }
        void print() {
            std::cout << "Sign: " << (sign?'+':'-') << " / Decimal length: " << length << " / Array length: " << arraylength << "\nValue";
            if (arraylength > 1) {
                std::cout << "[" << arraylength << "] = { " << value[arraylength-1];
                for (int i = arraylength-2; i >= 0; i--) {
                    std::cout << ", " << value[i];
                }
                std::cout << " }";
            } else std::cout << ": " << *value;
            std::cout << "\nString: \"" << str << "\"\n==============================" << std::endl;
        }
    //#endif
};
#endif

superlong mynumber;
using namespace std;

int main() {
    superlong longnum1, longnum2;
    longnum1 = "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    longnum2 = "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    longnum1.print();
    longnum2.print();
    mynumber = longnum1 + longnum2;
    mynumber.print();

    //superlong longnum1, longnum2;
    char instruction = 0;
    while (instruction != '0') {
        cout << "Input two big numbers...\n> ";
        cin >> longnum1;
        cout << "> ";
        cin >> longnum2;
        cout << "What do you want with these two long numbers? [+, -, 0]\n> ";
        cin >> instruction;
        if (instruction == '+') {
            mynumber = longnum1 + longnum2;
            cout << "  " << longnum1 << "\n";
            cout << "+ " << longnum2 << "\n";
            string mybar(longnum1.length+2, '-');
            cout << mybar << "\n";
            cout << "=" << (mynumber.length > longnum1.length ? "":" ") << mynumber << endl;
        } else if (instruction == '-') {
            //mynumber = longnum1 - longnum2;
            cout << "To be made soon! " << mynumber << endl;
        }
    }
}
/*
int main() {
    uint64_t input1 = 0b1111111111111111111111111111111111111111111111111111111111111111ULL;
    int64_t input2 = 0b0111111111111111111111111111111111111111111111111111111111111111LL;
    int16_t input3 = -789;
    int input4 = 1;
    uint64_t input5 = 9999999999999999999ULL + 1;
    const char* input6 = "3141592653589793238462643383279502884197169399375105820";//"31415926535897932 3846264338327950288 4197169399375105820"
    std::cout << "Input your number > ";
    std::cin >> mynumber;
    mynumber.print();
    mynumber = input1;
    mynumber.print();
    mynumber = input2;
    mynumber.print();
    mynumber = input3;
    mynumber.print();
    mynumber = 0;
    mynumber.print();
    mynumber = input4;
    mynumber.print();
    mynumber = input5;
    mynumber.print();
    mynumber = input6;
    mynumber.print();
    char* str = new char[3000];
    for (int i = 0; i < 101; i++) {
        for (int j = 9; j >= 0; j--) str[(i*10)+(9-j)] = j+'0';
    }
    mynumber = str;
    mynumber.print();
    delete [] str;
}

int main() {
    char* str = new char[3000];
    mynumber = 1;
    while (mynumber.str[0] != '0') {
        std::cout << "Input your number > ";
        std::cin >> mynumber;
        mynumber.print();
    }
}*/
