// BOJ10699를 푸느라 만든 시간관리 클래스
// class myclock
// 비공개 변수 및 메소드: 여기에 기재할 필요가 없음
// 직접접근 변수:
//  내부적으로 처리하는 것들은 비공개로 잘 숨겨놓았고,
//  다음과 같이 직접 읽으면 편리한 변수들은 캡슐화하지 않았음. 읽을때 get함수에 인자없이 괄호()만 덧붙이는 게 싫어서...
//    std::chrono::system_clock::time_point& tpvalue
//    uint16_t year, month, day, hour, minute, second, weekday ( weekday = {1부터 7까지 일월화수목금토} )
//    std::string strdate, strweekday, strtime, strfull ( strfull = 앞선 3가지 str을 합침 )
// 메소드:
//  초기화
//    myclock()
//    myclock(std::string timezone_abbreviation)
//    myclock(int16_t utc, int16_t utcminute = 0)
//    myclock(const std::chrono::system_clock::time_point& tp)
//    myclock(const date_t newdate)
//    myclock(const datetime_t newdatetime)
//  현재시간 입력
//    void now()
//    void now(std::string timezone_abbreviation)
//    void now(int16_t utc, int16_t utcminute = 0)
//  임의시간 입력
//    void set(const std::chrono::system_clock::time_point& newtp)
//    void set(const date_t newdate)
//    void set(const datetime_t newdatetime)
//  단위 연산
//    void addsecond(int moresecond, bool calcdate = true)
//    void addminute(int moreminute, bool calcdate = true, bool refresh_str = true)
//    void addhour(int morehour, bool calcdate = true, bool refresh_str = true)
//    void addday(int moreday)
//  누락된 연산을 갱신 (특히 직접접근으로 변수 값을 변경한 직후에 필수)
//    void refresh()
//  시간대 변경
//    void timezone(std::string timezone_abbreviation)
//    void timezone(int16_t utc, int16_t utcminute = 0)

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
typedef struct date_t {
    uint16_t year = 1970;
    uint16_t month = 1;
    uint16_t day = 1;
} date_t;
typedef struct datetime_t {
    uint16_t year = 1970;
    uint16_t month = 1;
    uint16_t day = 1;
    uint16_t hour = 0;
    uint16_t minute = 0;
    uint16_t second = 0;
    friend bool operator==(const datetime_t& A, const datetime_t& B) {
        if (A.year != B.year) return false;
        if (A.month != B.month) return false;
        if (A.day != B.day) return false;
        if (A.hour != B.hour) return false;
        if (A.minute != B.minute) return false;
        if (A.second != B.second) return false;
        return true;
    }
    friend bool operator!=(const datetime_t& A, const datetime_t& B) {
        return !(A == B);
    }
} datetime_t;
class myclock {
private:
    uint64_t second_epoch;
    uint32_t day_epoch;
    int16_t _utc = 0;
    int16_t _utcminute = 0;
    void init_now() {
        tpvalue = std::chrono::system_clock::now();
        decode_tp();
    }
    void init_custom(const date_t& newdate) {
        init_custom(datetime_t{newdate.year, newdate.month, newdate.day, 0, 0, 0});
    }
    void init_custom(const datetime_t& newdatetime) {
        uint16_t carry;
        second = newdatetime.second % 60;
            carry = newdatetime.second / 60;
        minute = (newdatetime.minute + carry) % 60;
            carry = (newdatetime.minute + carry) / 60;
        hour = (newdatetime.hour + carry) % 24;
            carry = (newdatetime.hour + carry) / 24;
        day = newdatetime.day + carry; // day->month carry missing: 다시 디코드할 것
        month = ((newdatetime.month - 1) % 12) + 1;
            carry = (newdatetime.month - 1) / 12;
        year = newdatetime.year + carry;
        encode_day_epoch();
        encode_tp();
        decode_day_epoch(false); // 다시 디코드하여 날짜 유효성 검증 (day부분)
        calcweekday();
        encode_str();
    }
    std::string _fillzero(int num, int digit) {
        std::string result = std::to_string(num);
        return std::string(digit - result.size(), '0') + result;
    }
    void decode_tp(bool calcdate = true) { 
        second_epoch = std::chrono::duration_cast<std::chrono::seconds>(tpvalue.time_since_epoch()).count();
        decode_second_epoch(calcdate);
        encode_str();
    }
    void decode_second_epoch(bool calcdate = false) {
        hour = second_epoch / 3600 % 24; // duration_cast<hours>(tpvalue.time_since_epoch()).count() % 24
        minute = second_epoch / 60 % 60; // duration_cast<minutes>(tpvalue.time_since_epoch()).count() % 60
        second = second_epoch % 60; // duration_cast<seconds>(tpvalue.time_since_epoch()).count() % 60
        if (calcdate) decode_day_epoch();
    }
    #define  DURATION_DAY     std::chrono::duration<uint32_t, decltype(std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>{})>
    #define  DURATION_SECOND  std::chrono::duration<uint64_t, decltype(std::ratio_multiply<std::ratio<1>, std::chrono::seconds::period>{})>
    void decode_day_epoch(bool updatefromsecond = true) {
        // for BOJ10699, with the reference from HowardHinnant.
        if (updatefromsecond) day_epoch = second_epoch / 86400;
        auto const z = day_epoch + 719468;
        auto const era = (z >= 0 ? z : z - 146096) / 146097;
        auto const doe = static_cast<unsigned>(z - era * 146097);          // [0, 146096] // 윤년이 반복되는 주기
        auto const yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;  // [0, 399]
        auto const y = static_cast<DURATION_DAY::rep>(yoe) + era * 400;
        auto const doy = doe - (365*yoe + yoe/4 - yoe/100);                // [0, 365]
        auto const mp = (5*doy + 2)/153;                                   // [0, 11]
        day = doy - (153*mp+2)/5 + 1;                                      // [1, 31]
        month = mp < 10 ? mp+3 : mp-9;                                     // [1, 12]
        year = y + (month <= 2);
        calcweekday();
    }
    void calcweekday() {
        weekday = day_epoch % 7; // 0부터 목금토일월화수
        if (weekday <= 2) weekday += 5;
        else weekday -= 2; // 1부터 일월화수목금토
    }
    void encode_tp() {
        DURATION_SECOND mydur(second_epoch);
        tpvalue = std::chrono::system_clock::time_point(mydur);
    }
    void encode_day_epoch(bool calcsecond = true) {
        // for BOJ10699, with the reference from HowardHinnant.
        auto const y = static_cast<int>(year) - (month <= 2);
        auto const m = static_cast<unsigned>(month);
        auto const d = static_cast<unsigned>(day);
        auto const era = (y >= 0 ? y : y-399) / 400;
        auto const yoe = static_cast<unsigned>(y - era * 400);       // [0, 399]
        auto const doy = (153*(m > 2 ? m-3 : m+9) + 2)/5 + d-1;      // [0, 365]
        auto const doe = yoe * 365 + yoe/4 - yoe/100 + doy;          // [0, 146096]
        day_epoch = era * 146097 + static_cast<int>(doe) - 719468;
        if (calcsecond) encode_second_epoch(true);
    }
    void encode_second_epoch(bool updatefromday = false) {
        if (updatefromday) second_epoch = day_epoch * 86400;
        second_epoch += hour * 3600;
        second_epoch += minute * 60;
        second_epoch += second;
    }
    void encode_str() {
        strdate = _fillzero(year, 4) + '-' + _fillzero(month, 2) + '-' + _fillzero(day, 2);
        strtime = _fillzero(hour, 2) + ':' + _fillzero(minute, 2) + ':' + _fillzero(second, 2);
        if (weekday == 1) strweekday = "Sun";
        else if (weekday == 2) strweekday = "Mon";
        else if (weekday == 3) strweekday = "Tue";
        else if (weekday == 4) strweekday = "Wed";
        else if (weekday == 5) strweekday = "Thu";
        else if (weekday == 6) strweekday = "Fri";
        else strweekday = "Sat";
        strfull = strdate + ' ' + strweekday + ' ' + strtime + " (UTC" + (_utc>=0 ? "+" : "") + std::to_string(_utc) + (_utcminute>0? ":"+_fillzero(_utcminute, 2) : "") + ")";
    }
public:
    std::chrono::system_clock::time_point tpvalue;
    uint16_t year;
    uint16_t month;
    uint16_t day;
    uint16_t hour;
    uint16_t minute;
    uint16_t second;
    uint16_t weekday;
    std::string strfull;
    std::string strdate;
    std::string strtime;
    std::string strweekday;
    myclock() {
        init_now();
    }
    myclock(std::string timezone_abbreviation) {
        init_now();
        timezone(timezone_abbreviation);
    }
    myclock(int16_t utc, int16_t utcminute = 0) {
        init_now();
        timezone(utc, utcminute);
    }
    myclock(const std::chrono::system_clock::time_point& tp) {
        tpvalue = tp;
        decode_tp();
    }
    myclock(const date_t newdate) {
        init_custom(newdate);
    }
    myclock(const datetime_t newdatetime) {
        init_custom(newdatetime);
    }
    void now() {
        init_now();
    }
    void now(std::string timezone_abbreviation) {
        init_now();
        timezone(timezone_abbreviation);
    }
    void now(int16_t utc, int16_t utcminute = 0) {
        init_now();
        timezone(utc, utcminute);
    }
    void set(const std::chrono::system_clock::time_point& newtp) {
        if (&tpvalue == &newtp) return;
        tpvalue = newtp;
        decode_tp();
    }
    void set(const date_t newdate) {
        init_custom(newdate);
    }
    void set(const datetime_t newdatetime) {
        init_custom(newdatetime);
    }
    void addsecond(int moresecond, bool calcdate = true) {
        second_epoch += moresecond;
        decode_second_epoch(calcdate);
        encode_str();
    }
    void addminute(int moreminute, bool calcdate = true, bool refresh_str = true) {
        second_epoch += (moreminute * 60);
        decode_second_epoch(calcdate);
        if (refresh_str) encode_str();
    }
    void addhour(int morehour, bool calcdate = true, bool refresh_str = true) {
        second_epoch += (morehour * 3600);
        decode_second_epoch(calcdate);
        if (refresh_str) encode_str();
    }
    void addday(int moreday) {
        day_epoch += moreday;
        second_epoch += (moreday * 86400);
        decode_day_epoch(false);
        encode_str();
    }
    void refresh() {
        encode_day_epoch();
        encode_tp();
        decode_day_epoch(false);
        calcweekday();
        encode_str();
    }
    void timezone(std::string timezone_abbreviation) {
        if (timezone_abbreviation == "HST") timezone(-10);
        else if (timezone_abbreviation == "AKST") timezone(-9);
        else if (timezone_abbreviation == "PST") timezone(-8);
        else if (timezone_abbreviation == "MST") timezone(-7);
        else if (timezone_abbreviation == "CST") timezone(-6);
        else if (timezone_abbreviation == "EST") timezone(-5);
        else if (timezone_abbreviation == "CET") timezone(1);
        else if (timezone_abbreviation == "EET") timezone(2);
        else if (timezone_abbreviation == "FET") timezone(3);
        else if (timezone_abbreviation == "MSK") timezone(3);
        //else if (timezone_abbreviation == "CST") timezone(9);
        else if (timezone_abbreviation == "KST") timezone(9);
        else if (timezone_abbreviation == "JST") timezone(9);
        else if (timezone_abbreviation == "NZST") timezone(12);
        else timezone(0);
    }
    void timezone(int16_t utc, int16_t utcminute = 0) {
        if (utcminute > 0) addminute(-_utcminute+utcminute, true, false);
        addhour(-_utc+utc, true, false);
        _utcminute = utcminute;
        _utc = utc;
        encode_str();
    }
    friend std::ostream& operator<<(std::ostream& ost, const myclock& cl) {
        ost << cl.strfull;
        return ost;
    }
    myclock operator+(const int32_t moresecond) {
        this->addsecond(moresecond);
        return *this;
    }
    myclock operator+=(const int32_t moresecond) {
        this->addsecond(moresecond);
        return *this;
    }
    myclock operator-(const int32_t lesssecond) {
        this->addsecond(-lesssecond);
        return *this;
    }
    myclock operator-=(const int32_t lesssecond) {
        this->addsecond(-lesssecond);
        return *this;
    }
    myclock operator=(const myclock& newclock) {
        if (this == &newclock) return *this;
        second_epoch = newclock.second_epoch;
        day_epoch = newclock.day_epoch;
        _utc = newclock._utc;
        _utcminute = newclock._utcminute;
        tpvalue = newclock.tpvalue;
        year = newclock.year;
        month = newclock.month;
        day = newclock.day;
        hour = newclock.hour;
        minute = newclock.minute;
        second = newclock.second;
        weekday = newclock.weekday;
        strfull = newclock.strfull;
        strdate = newclock.strdate;
        strtime = newclock.strtime;
        return *this;
    }
    myclock operator=(const std::chrono::system_clock::time_point& newtp) {
        if (&tpvalue == &newtp) return *this;
        tpvalue = newtp;
        decode_tp();
        return *this;
    }
    myclock operator=(const date_t& newdate) {
        init_custom(newdate);
        return *this;
    }
    myclock operator=(const datetime_t& newdatetime) {
        init_custom(newdatetime);
        return *this;
    }
    friend bool operator==(const myclock& A, const myclock& B) {
        if (A.year != B.year) return false;
        if (A.month != B.month) return false;
        if (A.day != B.day) return false;
        if (A.hour != B.hour) return false;
        if (A.minute != B.minute) return false;
        if (A.second != B.second) return false;
        return true;
    }
    friend bool operator==(const myclock& A, const datetime_t& B) {
        if (A.year != B.year) return false;
        if (A.month != B.month) return false;
        if (A.day != B.day) return false;
        if (A.hour != B.hour) return false;
        if (A.minute != B.minute) return false;
        if (A.second != B.second) return false;
        return true;
    }
    friend bool operator!=(const myclock& A, const myclock& B) {
        return !(A == B);
    }
    friend bool operator!=(const myclock& A, const datetime_t& B) {
        return !(A == B);
    }
};
int main() {
    myclock myclock1;
    std::cout << myclock1 << "\n";
    myclock1.timezone("CST");
    std::cout << myclock1 << "\n";
    myclock1.addhour(-23);
    std::cout << myclock1 << "\n";
    myclock1.day = 22; // direct change
    std::cout << myclock1 << "\n"; // not applied to the output string
    myclock1.refresh();
    std::cout << myclock1 << "\n";
    std::cout << "Lastly, let's try the wrong datetime (out of range) correction...\n";
    myclock1.set(datetime_t{2010,14,32,25,61,199});
    std::cout << myclock1 << "\n";
    std::cout << (myclock1 == datetime_t{2011,3,5,2,4,19} ? "corrected!\n" : "Not corrected...\n");
}
