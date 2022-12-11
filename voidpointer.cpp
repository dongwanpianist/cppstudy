// 오픈채팅방 질문자에게 답변하기 위해 만든 void pointer 사용예제
int main() {
    void* everything;

    long bignum = 1234567890123456789;
    everything = &bignum;
    cout << "*Everything in <long> type:\t" << *(uint64_t*)everything << "\tptr: " << everything << " (copied from " << &bignum << ")\n";

    int numarray[5] = {11, 22, 33, 44};
    everything = &numarray;
    cout << "*Everything in <int[]> type:\t" << *(uint32_t*)everything << "\t\t\tptr: " << everything << " (copied from " << &numarray << ")\n";
    // everything[1] 처럼 직접사용불가, cout에 정의된 대로 int배열의 첫번째 원소 출력
    int* arr = (int*)everything;
    cout << "Array access: [" << arr[0] << ", " << arr[1] << ", " << arr[2] << ", " << arr[3] << "]\n";

    char letter = 'A';
    everything = &letter;
    cout << "*Everything in <char> type:\t" << *(uint8_t*)everything << "\t\t\tptr: " << everything << " (copied from " << (void*)&letter << ")\n";

    string sentence = "Hello";
    everything = &sentence;
    cout << "*Everything in <string> type:\t" << *(string*)everything << "\t\t\tptr: " << everything << " (copied from " << &sentence << ")\n";
    // *(string*)everything.size() ->size() 등 클래스 내부기능 직접사용불가

    string another_thing = *(string*)everything;
    cout << "String access: Size: " << another_thing.size() << " | Substr: \"" << another_thing.substr(2,3) << "\"\n";
    // 이렇게 제 형식으로 정의된 변수로 다시 옮겨서 .size() 등 클래스 내부기능 직접사용가능
}
