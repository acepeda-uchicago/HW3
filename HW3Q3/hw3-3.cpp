#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

struct Nth_Power {
    private:
        int power;
    public:
        Nth_Power(int val) : power(val) {}
        int operator()(int y) const { return pow(y,power); }

};

int main() { 
    vector<int> v = { 1, 2, 3, 4, 5 };
    Nth_Power cube{3};
    cout << cube(7) << endl; // prints 343
    // print first five cubes
    transform(v.begin(), v.end(), ostream_iterator<int>(cout, ", "), cube);
}