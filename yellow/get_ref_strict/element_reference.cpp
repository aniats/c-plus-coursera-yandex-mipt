#include <iostream>
#include <map>

using namespace std;

template <typename key, typename value>
value& GetRefStrict(map<key, value>& m, const key& el) {
    if (m.count(el) == 0) {
        throw runtime_error("no such key in dictionary");
    }
    return m.at(el);
}

int main() {
    map<int, string> m = {{0, "value"}};
    string& item = GetRefStrict(m, 0);
    item = "newvalue";
    cout << m[0] << endl; // выведет newvalue
    return 0;
}