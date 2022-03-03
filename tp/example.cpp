#include <iostream>
#include <algorithm>
#include <array>
using namespace std;

 

int main()
{
    array<int, 5> myArray{ 34, -67, 99, -1, 999 };
    {
        struct functor {
            bool operator()(int n) const { return n < 0; }
        };
        cout << count_if(cbegin(myArray), cend(myArray), functor{}) << "\n";
    }
    {
        cout << count_if(cbegin(myArray), cend(myArray), [](int n) { return n < 0; }) << "\n";
    }
}
