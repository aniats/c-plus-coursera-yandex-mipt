#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <string>

using namespace std;

//signatures (need them for passing system)
template<typename T> T FuncA(T x);

template <typename type>
vector<type> Sqr(const vector<type>& vec);

template <typename key, typename value>
map<key, value> Sqr(const map<key, value>& m);

template <typename left, typename right>
pair<left, right> Sqr(const pair<left, right>& vec);

//functons
template <typename T> T Sqr(T x) {
    return x * x;
}

template <typename key, typename value>
map<key, value> Sqr(const map<key, value>& m) {
    map<key, value> new_map;
    for (const auto& el: m) {
        new_map.insert(pair(el.first, Sqr(el.second)));
    }
    return new_map;
}

template <typename left, typename right>
pair<left, right> Sqr(const pair<left, right>& p) {
    pair<left, right> new_pair(Sqr(p.first), Sqr(p.second));
    return new_pair;
}

template <typename type>
vector<type> Sqr(const vector<type>& vec) {
    vector<type> new_vec;
    for (const auto& el: vec) {
        new_vec.push_back(Sqr(el));
    }
    return new_vec;
}


int main() {
    vector<int> v = {1, 2, 3};
    cout << "vector:";
    for (int x : Sqr(v)) {
        cout << ' ' << x;
    }
    cout << endl;

    map<int, pair<int, int>> map_of_pairs = {
            {4, {2, 2}},
            {7, {4, 3}}
    };
    cout << "map of pairs:" << endl;
    for (const auto& x : Sqr(map_of_pairs)) {
        cout << x.first << ' ' << x.second.first << ' ' << x.second.second << endl;
    }
}