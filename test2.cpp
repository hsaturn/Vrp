#include <iostream>
#include <cstdarg>

using namespace std;

template<size_t index, typename... Ts>
void resetElementN(Ts&&... ts) {
    get<index>(std::forward<Ts>(ts)...) = {}; // assuming element N has an empty ctor
}

int main() {
    int i = 0;
    string s = "hello";
    get<0>(i,2,"hello","hello"s, 'a') += get<0>(2);
    get<1>(1,i,"hello",4) += get<1>(1, 2);
    get<3>(1,2,"hello",i) += get<2>(0, 1, 2);    
    get<2>(1,2,s,4) = get<2>(0, 1, "hi");
    cout << i << ' ' << s << endl;    
    resetElementN<1>(0, i, 2);
    resetElementN<0>(s, 1, 2);
    cout << i << ' ' << s << endl;    

    // not ok - and do not compile
    // get<0>(1,i,"hello","hello"s) = 5;
    // get<1>(1,i*2,"hello") = 5;
    // get<2>(1,i*2,"hello")[4] = '!';
    // resetElementN<1>(s, 1, 2);

    // ok
    const int j = 2;
    cout << get<0>(j,i,3,4) << endl;

    // not ok - and do not compile
    // get<0>(j,i,3,4) = 5;    

    // not ok - and do not compile
    // with a readable compilation error
    // cout << get<-1>("one", 2, '3') << endl;
    // cout << get<3>("one", 2, '3') << endl;
}
