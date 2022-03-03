#include <iostream>
#include <algorithm>
#include <array>
using namespace std;

 

int main()
{

 

    auto f{ [n = 0]() mutable {cout << ++n << "\n"; } };

 

    f();
    f();
    f();
    f();

 

}
