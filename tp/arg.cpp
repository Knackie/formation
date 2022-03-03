#include <iostream>
using namespace std;

 

int somme() { return 0; }

 

template <typename T, typename... Args>
auto somme(T t, Args... args) {return t + somme(args...);}

 

int main()
{
    cout << somme(12, -56.7, 67LL, 6U, 'z') << "\n";
    cout << somme(89.f, 12, -56.7, 67LL, 6U, 'z') << "\n";
    cout << somme(-56.7, 67LL, 6U) << "\n";
}
