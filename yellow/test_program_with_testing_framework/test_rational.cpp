#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
    if (t != u) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string& hint) {
    AssertEqual(b, true, hint);
}

class TestRunner {
public:
    template <class TestFunc>
    void RunTest(TestFunc func, const string& test_name) {
        try {
            func();
            cerr << test_name << " OK" << endl;
        } catch (exception& e) {
            ++fail_count;
            cerr << test_name << " fail: " << e.what() << endl;
        } catch (...) {
            ++fail_count;
            cerr << "Unknown exception caught" << endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            cerr << fail_count << " unit tests failed. Terminate" << endl;
            exit(1);
        }
    }

private:
    int fail_count = 0;
};

/*class Rational {
public:
    // Вы можете вставлять сюда различные реализации,
    // чтобы проверить, что ваши тесты пропускают корректный код
    // и ловят некорректный

    Rational();
    Rational(int numerator, int denominator) {
    }

    int Numerator() const {
    }

    int Denominator() const {
    }
};*/

void TestReduceFraction() {
    {
        Rational first = Rational(1, 2);
        Rational second = Rational(5, 10);
        AssertEqual(first.Numerator(), second.Numerator(), "Numerators are not equal (Reduce)");
        AssertEqual(first.Denominator(), second.Denominator(), "Denominators are not equal (Reduce)");
    }
}

void TestCreatingFraction() {
    {
        Rational first = Rational();
        AssertEqual(first.Numerator(), 0, "Numerator is not equal 0 (Create)");
        AssertEqual(first.Denominator(), 1, "Denominator is not equal 1 (Create)");
    }

    {
        Rational first = Rational(-1, -5);
        AssertEqual(first.Numerator(), 1, "Numerator is not positive (Create)");
        AssertEqual(first.Denominator(), 5, "Denominator is not positive (Create)");
    }

    {
        Rational first = Rational(-1, 5);
        AssertEqual(first.Numerator(), -1, "Numerator is not negative (Create)");
        AssertEqual(first.Denominator(), 5, "Denominator is not positive (Create)");
    }

    {
        Rational first = Rational(1, -5);
        AssertEqual(first.Numerator(), -1, "Numerator is not negative (Create)");
        AssertEqual(first.Denominator(), 5, "Denominator is not positive (Create)");
    }

    {
        Rational first = Rational(0, 7);
        AssertEqual(first.Numerator(), 0, "Numerator is not 0 (Create)");
        AssertEqual(first.Denominator(), 1, "Denominator is not 1 (Create)");
    }

}

int main() {
    TestRunner runner;
    runner.RunTest(TestCreatingFraction, "TestCreatingFraction");
    runner.RunTest(TestReduceFraction, "TestReduceFraction");
    // добавьте сюда свои тесты
    return 0;
}