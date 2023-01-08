// Custom _str implemented without including (purpose of study):
// _strlen, _strappend, _strappendchar, _charrepeat, _substr, ...
//
// number decoding and encoding:
// isnumeric, isminus, iszero, iseven, _strnumlen, _numlen,
// zerostr, trimzeros, matchzeros, numberize(including trimzeros), ...
// flipsign, absstrnum,
//
// number operator:
// mod2, mod4, mod5, mod10
// comparenumstr(returning char '<', '=', or '>'),
// addnumstr, subtractnumstr
// superlong_operatoradd -> operator+
// superlong_operatorsubtract -> operator-
// etc...
//
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

int32_t _strlen(const char*);
int32_t _strlen(char*);
int32_t _strnumlen(const char*);
int32_t _strnumlen(char*);
size_t _numlen(uint64_t);
char* _numtostr(int32_t);
char* _numtostr(int64_t);
char* _numtostr(uint32_t);
char* _numtostr(uint64_t, bool positive = true);
bool _strcmp(const char*, const char*);
bool _strcmp(char*, char*);
char* _strduplicate(const char*);
char* _strduplicate(char*);
char* _strappend(const char*, const char*);
char* _strappend(char*, char*);
char* _strappendchar(char leftchar, uint16_t charrepetition, const char* rightstring);
char* _strappendchar(char leftchar, uint16_t charrepetition, char* rightstring);
char* _strappendchar(const char* leftstring, char rightchar, uint16_t charrepetition);
char* _strappendchar(char* leftstring, char rightchar, uint16_t charrepetition);
char* _charrepeat(char thechar, uint16_t charrepetition);
char* _substr(const char* string, int start_position, int length = 0);
char* _substr(char* string, int start_position, int length = 0);
void _debug_printstrchar(const char*);
void _debug_printstrchar(char*);
void _debug_printstr(const char*);
void _debug_printstr(char*);

int32_t _strlen(const char* _str) { return
        _strlen((char*)_str); }
int32_t _strlen(char* _str) {
    if (_str == NULL) return -1;
    if (_str[0] == 0) return 0;
    int l = 0;
    while (_str[l] != 0) ++l;
    return l;
}
int32_t _strnumlen(const char* _str) { return
        _strnumlen((char*)_str); }
int32_t _strnumlen(char* _str) { // 음수기호는 통과, 숫자만 세기
    if (_str == NULL) return -1;
    if (_str[0] == 0) return 0;
    int l = 0;
    if (_str[0] == '-') ++l;
    while ((_str[l] >= '0') && (_str[l] <= '9')) ++l;
    if (_str[0] == '-') --l;
    return l; // _strnumlen("-1111a1") = 4
}
size_t _numlen(uint64_t _value) {
    size_t l = 0;
    while (_value) { // not zero
        _value /= 10;
        ++l;
    }
    return l; // _numlen(0) = 0
}
char* _numtostr(int32_t _value) {
    uint64_t _unsigned_value = 0;
    if (_value >= 0) {
        _unsigned_value += _value;
        return _numtostr(_unsigned_value, true);
    } else {
        _unsigned_value += -(_value);
        return _numtostr(_unsigned_value, false);
    }
}
char* _numtostr(int64_t _value) {
    uint64_t _unsigned_value = 0;
    if (_value >= 0) {
        _unsigned_value += _value;
        return _numtostr(_unsigned_value, true);
    } else {
        _unsigned_value += -(_value);
        return _numtostr(_unsigned_value, false);
    }
}
char* _numtostr(uint32_t _value) {
    uint64_t _expanded_value = 0 + _value;
    return _numtostr(_expanded_value, true);
}
char* _numtostr(uint64_t _value, bool positive) {
    if (_value == 0) {
        char* _zero = (char*)calloc(2,1);
        _zero[0] = '0';
        _zero[1] = 0;
        return _zero;
    }
    size_t l = _numlen(_value);\
    char* result = (char*)calloc(l+!positive+1, 1);
    result[l+!positive] = 0;
    if (!positive) result[0] = '-';
    for (size_t i = l+!positive-1; i >= !positive; i--) {
        result[i] = (_value%10) + '0';
        _value /= 10;
        if (i == !positive) break;
    }
    return result;
}
bool _strcmp(const char* str1, const char* str2) { return
     _strcmp((char*)str1, (char*)str2); }
bool _strcmp(char* str1, char* str2) {
    if (str1 == NULL) return false;
    if (str2 == NULL) return false;
    if ((str1[0] == 0) && (str2[0] == 0)) return true; // 둘다 ""
    for (size_t l = 0; str1[l] != 0; l++) if (str1[l] != str2[l]) return false;
    return true;
}
char* _strduplicate(const char* str) { return
      _strduplicate((char*)str); }
char* _strduplicate(char* str) {
    size_t l = _strlen(str);
    char* newstr = (char*)calloc(l+1, 1);
    for (size_t i = 0; i < l+1; i++) newstr[i] = str[i];
    return newstr;
}
char* _strappend(const char* str1, const char* str2) { return
      _strappend((char*)str1, (char*)str2); }
char* _strappend(char* str1, char* str2) {
    int32_t len1 = _strlen(str1);
    int32_t len2 = _strlen(str2);
    size_t i;
    char* newstr = (char*)calloc(len1 + len2 + 1, 1);
    for (i = 0; i < len1; i++) newstr[i] = str1[i];
    for (i = 0; i < len2; i++) newstr[i+len1] = str2[i];
    newstr[len1 + len2 + 1] = 0;
    return newstr;
}
char* _strappendchar(char ch1, uint16_t times, const char* str2) { return
      _strappendchar(ch1, times, (char*)str2); }
char* _strappendchar(char ch1, uint16_t times, char* str2) {
    int32_t len2 = _strlen(str2);
    size_t i;
    char* newstr = (char*)calloc(times + len2 + 1, 1);
    for (i = 0; i < times; i++) newstr[i] = ch1;
    for (i = 0; i < len2; i++) newstr[times+i] = str2[i];
    newstr[times + len2] = 0;
    return newstr;
}
char* _strappendchar(const char* str1, char ch2, uint16_t times) { return
      _strappendchar((char*)str1, ch2, times); }
char* _strappendchar(char* str1, char ch2, uint16_t times) {
    int32_t len1 = _strlen(str1);
    size_t i;
    char* newstr = (char*)calloc(len1 + times + 1, 1);
    for (i = 0; i < len1; i++) newstr[i] = str1[i];
    for (i = 0; i < times; i++) newstr[len1+i] = ch2;
    newstr[len1 + times] = 0;
    return newstr;
}
char* _charrepeat(char _ch, uint16_t times) {
    size_t i;
    char* newstr = (char*)calloc(times + 1, 1);
    for (i = 0; i < times; i++) newstr[i] = _ch;
    newstr[times] = 0;
    return newstr;
}
char* _substr(const char* _str, int position, int length) { return
      _substr((char*)_str, position, length); }
char* _substr(char* _str, int position, int length) {
    int32_t newlen = _strlen(_str);
    if (length > 0) if (position+length < newlen) newlen = position+length;
    char* result = (char*)calloc(newlen, 1);
    for (int i = position; i < newlen; i++) {
        result[i-position] = _str[i];
    }
    return result;
}

class superlong {
private:
    static inline bool isminus(const char*);
    static inline bool isminus(char*);
    static bool iszero(const char*);
    static bool iszero(char*);
    static bool iseven(const char*);
    static bool iseven(char*);
    static int16_t mod2(const char*, bool minus2 = false);
    static int16_t mod2(char*, bool minus2 = false);
    static int16_t mod3(const char*, bool minus3 = false);
    static int16_t mod3(char*, bool minus3 = false);
    static int16_t mod4(const char*, bool minus4 = false);
    static int16_t mod4(char*, bool minus4 = false);
    static int16_t mod5(const char*, bool minus5 = false);
    static int16_t mod5(char*, bool minus5 = false);
    static int16_t mod8(const char*, bool minus8 = false);
    static int16_t mod8(char*, bool minus8 = false);
    static int16_t mod9(const char*, bool minus9 = false);
    static int16_t mod9(char*, bool minus9 = false);
    static int16_t mod10(const char*, bool minus10 = false);
    static int16_t mod10(char*, bool minus10 = false);
    static int16_t mod11(const char*, bool minus11 = false);
    static int16_t mod11(char*, bool minus11 = false);
    static inline char* zerostr();
    static void trimzeros(const char*&);   // modify value
    static void trimzeros(char*&);         // modify value
    static void matchzeros(const char*&, const char*&, bool numberized = false);  // modify value
    static void matchzeros(char*&, char*&, bool numberized = false);              // modify value
    static void numberize(const char*&);   // modify value
    static void numberize(char*&);         // modify value
    static char* flipsign(const char*, bool numberized = true);
    static char* flipsign(char*, bool numberized = true);
    static char* absstrnum(const char*, bool numberized = true);
    static char* absstrnum(char*, bool numberized = true);
    static char comparenumstr(const char*, const char*, bool numberized = true);
    static char comparenumstr(char*, char*, bool numberized = true);
    static char* addnumstr(const char* &str1, const char* &str2, bool numberized = true, bool zeromatched = true);
    static char* addnumstr(char* &str1, char* &str2, bool numberized = true, bool zeromatched = true);
    static char* subtractnumstr(const char* &str1, const char* &str2, bool numberized = true, bool zeromatched = true, bool absolute = false);
    static char* subtractnumstr(char* &str1, char* &str2, bool numberized = true, bool zeromatched = true, bool absolute = false);
public:
    size_t length;
    bool sign;
    bool minus;
    const char* str;
    superlong();
    superlong(const char*);
    superlong(char*);
    superlong& refresh();
    superlong& trimzeros();
    superlong& matchzeros(superlong&);
    superlong& numberize();
    superlong& flipsign();
    superlong& abs();
    static inline bool isnumeric(const char*);
    static inline bool isnumeric(char*);
    static char compare(const superlong&, const superlong&);

    superlong& operator=(const char* newstr) {
        if (!isnumeric(newstr)) superlong();
        else { str = newstr; refresh(); }
        return *this;
    }
    superlong& operator=(char* newstr) {
        if (!isnumeric(newstr)) superlong();
        else { str = newstr; refresh(); }
        return *this;
    }
    superlong& operator=(const superlong& number) {
        length = number.length;
        sign = number.sign;
        minus = number.minus;
        str = number.str;
        return *this;
    }
    friend bool operator>(const superlong& num1, const char* str2) { return
                operator>(num1, (char*)str2); }
    friend bool operator>(const superlong& num1, char* str2) {
        char* chstr2 = _strduplicate(str2);
        bool result = (comparenumstr(num1.str, chstr2) == '>');
        free(chstr2);
        return result;
    }
    friend bool operator>(const superlong& num1, const superlong& num2) {
        return (comparenumstr(num1.str, num2.str) == '>');
    }
    friend bool operator<(const superlong& num1, const char* str2) { return
                operator<(num1, (char*)str2); }
    friend bool operator<(const superlong& num1, char* str2) {
        char* chstr2 = _strduplicate(str2);
        bool result = (comparenumstr(num1.str, chstr2) == '<');
        free(chstr2);
        return result;
    }
    friend bool operator<(const superlong& num1, const superlong& num2) {
        return (comparenumstr(num1.str, num2.str) == '<');
    }
    friend bool operator==(const superlong& num1, const char* str2) { return
                operator==(num1, (char*)str2); }
    friend bool operator==(const superlong& num1, char* str2) {
        char* chstr2 = _strduplicate(str2);
        bool result = (comparenumstr(num1.str, chstr2) == '=');
        free(chstr2);
        return result;
    }
    friend bool operator==(const superlong& num1, const superlong& num2) {
        return (comparenumstr(num1.str, num2.str) == '=');
    }
    friend bool operator!=(const superlong& num1, const char* str2) { return
                operator!=(num1, (char*)str2); }
    friend bool operator!=(const superlong& num1, char* str2) {
        char* chstr2 = _strduplicate(str2);
        bool result = (comparenumstr(num1.str, chstr2) != '=');
        free(chstr2);
        return result;
    }
    friend bool operator!=(const superlong& num1, const superlong& num2) {
        return (comparenumstr(num1.str, num2.str) != '=');
    }
    friend superlong& operator+(const superlong& num1, int num2) { 
        char* numstr1 = _strduplicate(num1.str);
        char* numstr2 = _numtostr(num2);
        matchzeros(numstr1, numstr2);
        superlong* result = new superlong(addnumstr(numstr1, numstr2));
        free(numstr1);
        free(numstr2);
        return *result;
    }
    friend superlong& operator+(const superlong& num1, const char* str2) { return
                      operator+(num1, (char*)str2); }
    friend superlong& operator+(const superlong& num1, char* str2) {
        char* numstr1 = _strduplicate(num1.str);
        char* numstr2 = _strduplicate(str2);
        numberize(numstr2);
        matchzeros(numstr1, numstr2);
        superlong* result = new superlong(addnumstr(numstr1, numstr2));
        free(numstr1);
        free(numstr2);
        return *result;
    }
    friend superlong& operator+(const superlong& num1, const superlong& num2) {
        char* numstr1 = _strduplicate(num1.str);
        char* numstr2 = _strduplicate(num2.str);
        matchzeros(numstr1, numstr2);
        superlong* result = new superlong(addnumstr(numstr1, numstr2));
        free(numstr1);
        free(numstr2);
        return *result;
    }
    friend superlong& operator-(const superlong& num1, int num2);
    friend superlong& operator-(const superlong& num1, const char* str2);
    friend superlong& operator-(const superlong& num1, char* str2);
    friend superlong& operator-(int num1, const superlong& num2);
    friend superlong& operator-(const char* str1, const superlong& num2);
    friend superlong& operator-(char* str1, const superlong& num2);
    friend superlong& operator-(const superlong& num1, const superlong& num2) {
        char* numstr1 = _strduplicate(num1.str);
        char* numstr2 = _strduplicate(num2.str);
        matchzeros(numstr1, numstr2);
        superlong* result = new superlong(subtractnumstr(numstr1, numstr2, true, true, false));
        free(numstr1);
        free(numstr2);
        return *result;
    }
    friend int16_t operator%(const superlong& num1, int num2) {
        if (num2 == 2) return mod2(num1.str);
        else if (num2 == 3) return mod3(num1.str);
        else if (num2 == 4) return mod4(num1.str);
        else if (num2 == 5) return mod5(num1.str);
        else if (num2 == 8) return mod8(num1.str);
        else if (num2 == 9) return mod9(num1.str);
        else if (num2 == 10) return mod10(num1.str);
        else if (num2 == 11) return mod11(num1.str);
        if (num2 == -2) return mod2(num1.str, true);
        else if (num2 == -3) return mod3(num1.str, true);
        else if (num2 == -4) return mod4(num1.str, true);
        else if (num2 == -5) return mod5(num1.str, true);
        else if (num2 == -8) return mod8(num1.str, true);
        else if (num2 == -9) return mod9(num1.str, true);
        else if (num2 == -10) return mod10(num1.str, true);
        else if (num2 == -11) return mod11(num1.str, true);
        else return -99;
    }
    friend int16_t operator%(const superlong& num1, const char* str2) {
        char* chstr2 = _strduplicate(str2);
        numberize(chstr2);
        if (comparenumstr(absstrnum(chstr2), (char*)"11") == '>') return -99;
        int16_t result = operator%(num1, atoi((const char*)chstr2));
        free(chstr2);
        return result;
    }
    friend int16_t operator%(const superlong& num1, char* str2) {
        char* chstr2 = _strduplicate(str2);
        numberize(chstr2);
        if (comparenumstr(absstrnum(chstr2), (char*)"11") == '>') return -99;
        int16_t result = operator%(num1, atoi((const char*)chstr2));
        free(chstr2);
        return result;
    }
    friend int16_t operator%(const superlong& num1, const superlong& str2) {
        if (comparenumstr(str2.str, (char*)"11", true) == '>') return -99;
        int16_t result = operator%(num1, atoi(str2.str));
        return result;
    }
    //#ifdef _LIBCPP_ISTREAM
        friend std::istream& operator>>(std::istream &ist, superlong &number) {
            char* _str = new char[SUPERLONG_MAXLENGTH];
            ist >> _str;
            number = _str;
            return ist;
        }
    //#endif
    //#ifdef _LIBCPP_OSTREAM
        friend std::ostream& operator<<(std::ostream &ost, const superlong &number) {
            ost << number.str;
            return ost;
        }
    //#endif
};

inline bool superlong::isminus(const char* _str) { return
                       isminus((char*)_str); }
inline bool superlong::isminus(char* _str) {
    if (_str == NULL) return false;
    if (_str[0] == '-') return true;
    return false;
}
bool superlong::iszero(const char* _str) { return
                iszero((char*)_str); }
bool superlong::iszero(char* _str) { // zerostr() == {'0'(48), '\0'(0)};
    if (_str == NULL) return false;
    if ((_str[0] != '0') && (_str[0] != '-')) return false;
    size_t l = 0;
    while (_str[++l] != 0) {
        if (_str[l] != '0') return false;
    }
    return true;
}
bool superlong::iseven(const char* _str) { return
                iseven((char*)_str); }
bool superlong::iseven(char* _str) {
    if (_str == NULL) return false;
    if (_str[0] == 0) return false;
    size_t l = _strlen(_str);
    return ((_str[l-1] == '0')
         || (_str[l-1] == '2')
         || (_str[l-1] == '4')
         || (_str[l-1] == '6')
         || (_str[l-1] == '8'));
}
int16_t superlong::mod2(const char* _str, bool minus2) { return
                   mod2((char*)_str, minus2); }
int16_t superlong::mod2(char* _str, bool minus2) {
    if (_str == NULL) return 0;
    if (_str[0] == 0) return 0;
    size_t l = _strlen(_str);
    switch (_str[l-1]) {
        case '1':
        case '3':
        case '5':
        case '7':
        case '9':
        if (minus2) return -1;
        else return 1;
    }
    return 0;
}
int16_t superlong::mod3(const char* _str, bool minus3) { return
                   mod3((char*)_str, minus3); }
int16_t superlong::mod3(char* _str, bool minus3) {
    if (_str == NULL) return 0;
    if (_str[0] == 0) return 0;
    bool isnega = (_str[0] == '-');
    size_t l = _strlen(_str);
    int32_t digitssum = 0;
    for (int i = isnega; i < l; i++) digitssum += _str[i] - '0';
    int16_t result = digitssum % 3;
    if (isnega != minus3) {
        result = 3 - result;
        if (result == 3) result = 0;
    }
    if (result >= 3) result -= 3;
    else if (result < 0) result += 3;
    if (minus3) result = -result;
    return result;
}
int16_t superlong::mod4(const char* _str, bool minus4) { return
                   mod4((char*)_str, minus4); }
int16_t superlong::mod4(char* _str, bool minus4) {
    if (_str == NULL) return 0;
    if (_str[0] == 0) return 0;
    size_t l = _strlen(_str);
    int8_t last2digits = 0;
    bool isnega = (_str[0] == '-');
    if (isnega+1 < l) last2digits = ((int8_t)_str[l-2]-'0')*10;
    last2digits += (int8_t)_str[l-1]-'0';
    int16_t result = last2digits % 4;
    if (isnega != minus4) {
        result = 4 - result;
        if (result == 4) result = 0;
    }
    if (result >= 4) result -= 4;
    else if (result < 0) result += 4;
    if (minus4) result = -result;
    return result;
}
int16_t superlong::mod5(const char* _str, bool minus5) { return
                   mod5((char*)_str, minus5); }
int16_t superlong::mod5(char* _str, bool minus5) {
    if (_str == NULL) return 0;
    if (_str[0] == 0) return 0;
    size_t l = _strlen(_str);
    int16_t result = (int16_t)_str[l-1]-'0'; // lastdigit
    bool isnega = (_str[0] == '-');
    if (isnega != minus5) {
        result = 5 - result;
        if (result == 5) result = 0;
    }
    if (result >= 5) result -= 5;
    else if (result < 0) result += 5;
    if (minus5) result = -result;
    return result;
}
int16_t superlong::mod8(const char* _str, bool minus8) { return
                   mod8((char*)_str, minus8); }
int16_t superlong::mod8(char* _str, bool minus8) {
    if (_str == NULL) return 0;
    if (_str[0] == 0) return 0;
    size_t l = _strlen(_str);
    int16_t last3digits = 0;
    bool isnega = (_str[0] == '-');
    if (isnega+2 < l) last3digits = ((int8_t)_str[l-3]-'0')*100;
    if (isnega+1 < l) last3digits += ((int8_t)_str[l-2]-'0')*10;
    last3digits += (int8_t)_str[l-1]-'0';
    int16_t result = last3digits % 8;
    if (isnega != minus8) {
        result = 8 - result;
        if (result == 8) result = 0;
    }
    if (result >= 8) result -= 8;
    else if (result < 0) result += 8;
    if (minus8) result = -result;
    return result;
}
int16_t superlong::mod9(const char* _str, bool minus9) { return
                   mod9((char*)_str, minus9); }
int16_t superlong::mod9(char* _str, bool minus9) {
    if (_str == NULL) return 0;
    if (_str[0] == 0) return 0;
    bool isnega = (_str[0] == '-');
    size_t l = _strlen(_str);
    int32_t digitssum = 0;
    for (int i = isnega; i < l; i++) digitssum += _str[i] - '0';
    int16_t result = digitssum % 9;
    if (isnega != minus9) {
        result = 9 - result;
        if (result == 9) result = 0;
    }
    if (result >= 9) result -= 9;
    else if (result < 0) result += 9;
    if (minus9) result = -result;
    return result;
}
int16_t superlong::mod10(const char* _str, bool minus10) { return
                   mod10((char*)_str, minus10); }
int16_t superlong::mod10(char* _str, bool minus10) {
    if (_str == NULL) return 0;
    if (_str[0] == 0) return 0;
    bool isnega = (_str[0] == '-');
    size_t l = _strlen(_str);
    int16_t result = (int16_t)_str[l-1]-'0'; // lastdigit
    if (isnega != minus10) {
        result = 10 - result;
        if (result == 10) result = 0;
    }
    if (result >= 10) result -= 10;
    else if (result < 0) result += 10;
    if (minus10) result = -result;
    return result;
}
int16_t superlong::mod11(const char* _str, bool minus11) { return
                   mod11((char*)_str, minus11); }
int16_t superlong::mod11(char* _str, bool minus11) {
    if (_str == NULL) return 0;
    if (_str[0] == 0) return 0;
    bool isnega = (_str[0] == '-');
    size_t l = _strlen(_str);
    int32_t digitssum = 0;
    bool do_minus = false;
    for (size_t i = l-1; i >= isnega; i--) { // 거꾸로, 첫째자리수 더하고 둘째자리수 빼고...
        if (do_minus) digitssum -= _str[i] - '0';
        else digitssum += _str[i] - '0';
        do_minus = !do_minus;
        if (i <= isnega) break;
    }
    int16_t result = (digitssum % 11);
    if (isnega != minus11) {
        result = 11 - result;
        if (result == 11) result = 0;
    }
    if (result >= 11) result -= 11;
    else if (result < 0) result += 11;
    if (minus11) result = -result;
    return result;
}
inline char* superlong::zerostr() {
    char* _zero = (char*)calloc(2,1);
    _zero[0] = '0';
    _zero[1] = 0;
    return _zero;
}
void superlong::trimzeros(const char* &_str) {
     char* _chstr = (char*)_str;
     trimzeros(_chstr);
     _str = _chstr;
}
void superlong::trimzeros(char* &_str) {
    if (_str == NULL) return;
    if (_str[0] == 0) return;
    if (iszero(_str)) {
        free(_str);
        _str = zerostr();
        return;
    }
    bool minus = (_str[0] == '-');
    int zerocount = 0;
    int len = _strlen(_str);
    if (minus) zerocount++;
    for (; zerocount < len-1; zerocount++)
        if (_str[zerocount] != '0') break;
    _str = _substr(_str, zerocount);
    if (minus) _str = _strappendchar('-', 1, _str);
}
void superlong::matchzeros(const char* &str1, const char* &str2, bool numberized) {
    char* chstr1 = (char*)str1;
    char* chstr2 = (char*)str2;
    matchzeros(chstr1, chstr2, numberized);
    str1 = chstr1;
    str2 = chstr2;
}
void superlong::matchzeros(char* &str1, char* &str2, bool numberized) {
    if (numberized) { numberize(str1); numberize(str2); }
    bool str1m = isminus(str1);
    bool str2m = isminus(str2);
    int sizediff = (_strlen(str1)-str1m) - (_strlen(str2)-str2m);
    if (sizediff > 0) {
        str2 = _strappendchar('0', sizediff, str2);
        if (str2m) {
            str2[0] = '-';
            str2[sizediff] = '0';
        }
    } else if (sizediff < 0) {
        sizediff = -sizediff;
        str1 = _strappendchar('0', sizediff, str1);
        if (str1m) {
            str1[0] = '-';
            str1[sizediff] = '0';
        }
    }
}
void superlong::numberize(const char* &_str) {
    char* _chstr = (char*)_str;
    numberize(_chstr);
    _str = _chstr;
}
void superlong::numberize(char* &_str) {
    if (_str == NULL) return;
    if (_str[0] == 0) return;
    bool minus = (_str[0] == '-');
    int32_t l = _strlen(_str);
    for (uint32_t i = minus; i < l; i++)
        if ((_str[i] < '0') || (_str[i] > '9')) _str[i] = 0;
    trimzeros(_str);
}
char* superlong::flipsign(const char* _str, bool numberized) {
    return flipsign((char*)_str, numberized);
}
char* superlong::flipsign(char* _str, bool numberized) {
    if (_str == NULL) return 0;
    if (_str[0] == 0) return 0;
    if (!numberized) numberize(_str);
    size_t len = _strlen(_str);
    if (_str[0] == '-') {
        char* _plusstr = (char*)calloc(len, 1);
        for (size_t i = 0; i <= len; i++) _plusstr[i] = _str[i+1];
        return _plusstr;
    } else {
        char* _minusstr = (char*)calloc(len+2, 1);
        _minusstr[0] = '-';
        for (size_t i = 0; i <= len; i++) _minusstr[i+1] = _str[i];
        return _minusstr;
    }
}
char* superlong::absstrnum(const char* _str, bool numberized) { return
                 absstrnum((char*)_str, numberized); }
char* superlong::absstrnum(char* _str, bool numberized) {
    if (_str == NULL) return 0;
    if (_str[0] == 0) return 0;
    if (!numberized) numberize(_str);
    if (_str[0] != '-') return _str;
    return flipsign(_str);
}
char superlong::comparenumstr(const char* str1, const char* str2, bool numberized) { return
                comparenumstr((char*)str1, (char*)str2, numberized); }
char superlong::comparenumstr(char* str1, char* str2, bool numberized) {
    if (str1 == NULL) return 0;
    if (str2 == NULL) return 0;
    if (str1[0] == 0) return 0;
    if (str2[0] == 0) return 0;
    // true if str1 > str2
    bool arepositive = true;
    if (!numberized) { numberize(str1); numberize(str2); }
    if (!isminus(str1) && isminus(str2)) return '>';
    else if (isminus(str1) && !isminus(str2)) return '<';
    else if (isminus(str1) && isminus(str1)) arepositive = false;
    size_t len = _strlen(str1);
    if (len > _strlen(str2)) return (arepositive? '>' : '<'); // XOR (reverse if !arepositive)
    else if (len < _strlen(str2)) return (arepositive? '<' : '>');
    else { // len1=len2, digit-by-digit comparison
        for (size_t i = !arepositive; i < len; i++) {
            if (str1[i] > str2[i]) return (arepositive? '>' : '<');
            else if (str1[i] < str2[i]) return (arepositive? '<' : '>');
        }
    }
    // str1[end]==str2[end] (same until the end)
    return '=';
}
char* superlong::addnumstr(const char* &str1, const char* &str2, bool numberized, bool zeromatched) {
    char* chstr1 = (char*)str1;
    char* chstr2 = (char*)str2;
    return addnumstr(chstr1, chstr2, numberized, zeromatched);
}
char* superlong::addnumstr(char* &str1, char* &str2, bool numberized, bool zeromatched) {
    if (str1 == NULL) return 0;
    if (str2 == NULL) return 0;
    if (str1[0] == 0) return 0;
    if (str2[0] == 0) return 0;
    if (!numberized) { numberize(str1); numberize(str2); }
    if (!zeromatched) matchzeros(str1, str2);
    if (iszero(str1)) return str2;
    if (iszero(str2)) return str1;
    size_t len = _strlen(str1);
    uint8_t calc; // 숫자(10진수) 2개의 합, 끝에서부터 한자리씩 처리
    bool carry10 = false; // 다음자리로 넘기기 (19이상 넘어가지 않으니 bool)
    bool str1m = isminus(str1);
    bool str2m = isminus(str2);
    if (str1m == str2m) {
        char* result = (char*)calloc(len+1, 1); // 뒤 '\0' 공간 확보 (앞 '-' 공간 포함)
        result[len] = 0;
        for (size_t i = len-1; i >= str1m; i--) {
            calc = (str1[i]-'0') + (str2[i]-'0') + carry10;
            if (calc >= 10) {
                calc -= 10;
                carry10 = true;
            } else carry10 = false;
            result[i] = (calc+'0');
            if (i == str1m) break;
        }
        if (!str1m) {
            // + + 일때 한자리 넘치는 경우 {'1'} 추가
            if (carry10) result = _strappendchar('1', 1, result);
        } else {
            // - - 일때 한자리 넘치는 경우 {'-', '1'} 추가
            if (carry10) {
                result[0] = '1';
                result = _strappendchar('-', 1, result);
            } else result[0] = '-'; 
        }
        return result;
    } else { // (str1m != str2m)
        if (str2m) { // (+A) + (-B)
            char* absstr2 = absstrnum(str2);
            char compared = comparenumstr(str1, absstr2);
            if (compared == '>') { // 5 + -3 = (5-3)
                return subtractnumstr(str1, absstr2, true, true, true);
            } else if (compared == '<') { // 5 + -7 = -(7-5)
                return _strappendchar('-', 1, subtractnumstr(absstr2, str1, true, true, true));
            } else if (compared == '=') { // 5 + -5 = 0
                return zerostr();
            }
        } else { // (-A) + (+B)
            char* absstr1 = absstrnum(str1);
            char compared = comparenumstr(absstr1, str2);
            if (compared == '>') { // -5 + 3 = -(5-3)
                return _strappendchar('-', 1, subtractnumstr(absstr1, str2, true, true, true));
            } else if (compared == '<') {  // -5 + 7 = (7-5)
                return subtractnumstr(str2, absstr1, true, true, true);
            } else if (compared == '=') { // -5 + 5 = 0
                return zerostr();
            }
        }
    }
    return 0; // invalid calculation
}
char* superlong::subtractnumstr(const char* &str1, const char* &str2, bool numberized, bool zeromatched, bool absolute) {
    char* chstr1 = (char*)str1;
    char* chstr2 = (char*)str2;
    return subtractnumstr(chstr1, chstr2, numberized, zeromatched, absolute);
}
char* superlong::subtractnumstr(char* &str1, char* &str2, bool numberized, bool zeromatched, bool absolute) {
    bool str1zero, str2zero, str1m, str2m;
    char comparison = 0;
    if (!absolute) {
        if (str1 == NULL) return 0;
        if (str2 == NULL) return 0;
        if (str1[0] == 0) return 0;
        if (str2[0] == 0) return 0;
        if (!numberized) { numberize(str1); numberize(str2); }
        if (!zeromatched) matchzeros(str1, str2);
        str1zero = iszero(str1);
        str2zero = iszero(str2);
        if (str1zero && str2zero) return zerostr();
        if (str1zero) { flipsign(str2); return str2; }
        if (str2zero) return str1;
        str1m = isminus(str1);
        str2m = isminus(str2);
        if (str1m != str2m) {
            char* absstr2 = absstrnum(str2);
            if (str2m) { // (+A) - (-B) = A + |B|
                return addnumstr(str1, absstr2, true, true);
            } else { // (-A) - (+B) = -(|A| + |B|)
                char* absstr1 = absstrnum(str1);
                return _strappendchar('-', 1, addnumstr(absstr1, absstr2, true, true));
            }
        } else {
            comparison = comparenumstr(str1, str2);
            if (comparison == '=') return zerostr();
            if (str1m) { // (-A) - (-B)
                char* absstr1 = absstrnum(str1);
                char* absstr2 = absstrnum(str2);
                if (comparison == '<') { // (-5) - (-3) = -(|5|-|3|)
                    return _strappendchar('-', 1, subtractnumstr(absstr1, absstr2, true, true, true));
                } else if (comparison == '>') { // (-5) - (-7) = |7|-|5|
                    return subtractnumstr(absstr2, absstr1, true, true, true);
                }
            } // else (+A) - (+B) : do below
        }
    } else {
        str1zero = iszero(str1);
        str2zero = iszero(str2);
        if (str1zero && str2zero) return zerostr();
        if (str1zero) { flipsign(str2); return str2; }
        if (str2zero) return str1;
        comparison = comparenumstr(str1, str2);
        if (comparison == '=') return zerostr();
        str1m = str2m = false;
    }
    if (comparison == '<') // (5)-(7) = -(7-5)
        return _strappendchar('-', 1, subtractnumstr(str2, str1, true, true, true));
    // finally, absolute standard subtraction (positive big - positive small) like 5 - 3 = 2
    // or already processed numbers to (+big) - (+small):
    size_t len = _strlen(str1);
    char* result = (char*)calloc(len+1, 1);
    result[len] = 0;
    bool carry10 = false;
    int8_t calc;
    for (size_t i = len-1; i >= 0; i--) {
        calc = (str1[i]-'0') - (str2[i]-'0') - carry10;
        if (calc < 0) {
            calc += 10;
            carry10 = true;
        } else carry10 = false;
        result[i] = (calc+'0');
        if (i==0) break;
    }
    trimzeros(result);
    return result;
}
superlong::superlong() {
    length = 0;
    sign = 1;
    minus = 0;
    str = zerostr();
}
superlong::superlong(const char* newstr) {
    if (!isnumeric(newstr)) superlong();
    else {
        str = newstr;
        refresh();
    }
}
superlong::superlong(char* newstr) {
    if (!isnumeric(newstr)) superlong();
    else {
        str = (const char*)newstr;
        refresh();
    }
}
superlong& superlong::refresh() {
    numberize(str);
    sign = !isminus(str);
    minus = !sign;
    length = _strnumlen(str);
    return *this;
}
superlong& superlong::trimzeros() {
    trimzeros(str);
    return *this;
}
superlong& superlong::matchzeros(superlong &othernum) {
    matchzeros(str, othernum.str, true);
    return *this;
}
superlong& superlong::numberize() {
    numberize(str);
    return *this;
}
superlong& superlong::flipsign() {
    flipsign(str);
    return *this;
}
superlong& superlong::abs() {
    if (str[0] != '-') flipsign(str);
    return *this;
}
inline bool superlong::isnumeric(const char* _str) { return
                       isnumeric((char*)_str); }
inline bool superlong::isnumeric(char* _str) {
    return (_strnumlen(_str) != 0);
}
char superlong::compare(const superlong& num1, const superlong& num2) {
    return comparenumstr(num1.str, num2.str, true);
}

#endif

void _debug_printstrchar(const char* _str) {
     _debug_printstrchar((char*)_str); }
void _debug_printstrchar(char* _str) {
    std::cout << "char* = \"" << _str << "\":\n";
    if (_str == NULL) {
        std::cout << "NULL" << std::endl;
        return;
    }
    int len = _strlen(_str)+1;
    std::cout << "{ ";
    for (int i = 0; i < len; i++) {
        std::cout << (int16_t)_str[i];
        if (i < len-1) std::cout << ", ";
    }
    std::cout << " } [" << len << "]" << std::endl;
}
void _debug_printstr(const char* _str) {
     _debug_printstr((char*)_str); }
void _debug_printstr(char* _str) {
    if (_str == NULL) {
        std::cout << "NULL" << std::endl;
        return;
    }
    int len = _strlen(_str);
    std::cout << "char* = \"" << _str << "\" [" << len << "] *" << (uint64_t)_str % 10000 << std::endl;
}

using namespace std;
int main() {
    superlong mynum1, mynum2;
    char mystr1[10005], mystr2[10005];
    while (true) {
        while (true) {
            cout << "Your number 1: ";
            cin >> mystr1;
            if (mystr1[0] == 'e') return 0;
            if (!superlong::isnumeric(mystr1)) cout << "\"" << mystr1 << "\" is not a valid number! Please type again.\n";
            else break;
        }
        while (true) {
            cout << "Your number 2: ";
            cin >> mystr2;
            if (mystr1[0] == 'e') return 0;
            if (!superlong::isnumeric(mystr2)) cout << "\"" << mystr2 << "\" is not a valid number! Please type again.\n";
            else break;
        }
        cout << "   input string: " << mystr1 << "     " << mystr2 << "\n";
        mynum1 = mystr1;
        mynum2 = mystr2;
        cout << "   numberized:   " << mynum1 << "     " << mynum2 << "\n";

        cout << "   comparison:   " << mynum1 << " '" << superlong::compare(mynum1, mynum2) << "' " << mynum2 << "\n";
        cout << "   if(A > B)?    " << mynum1 << "  >  " << mynum2 << " ? " << (mynum1>mynum2? "true(1)\n":"false(0)\n");
        cout << "   if(A == B)?   " << mynum1 << "  == " << mynum2 << " ? " << (mynum1==mynum2? "true(1)\n":"false(0)\n");
        cout << "   if(A != B)?   " << mynum1 << "  != " << mynum2 << " ? " << (mynum1!=mynum2? "true(1)\n":"false(0)\n");
        cout << "   if(A < B)?    " << mynum1 << "  <  " << mynum2 << " ? " << (mynum1<mynum2? "true(1)\n":"false(0)\n");
    
        cout << "   add:          " << mynum1 << "  +  " << mynum2 << "\n"
             << "               = " << mynum1 + mynum2 << "\n";
    
        cout << "   subtract:     " << mynum1 << "  -  " << mynum2 << "\n"
             << "               = " << mynum1 - mynum2 << "\n";
    
        //cout << "   modulo:       " << mynum1 << "  %  " << mynum2 << "\n"
        //     << "               = " << mynum1 % mynum2 << "\n";
    }
}
