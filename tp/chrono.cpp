#include <iostream>
#include <vector>
using namespace std;

 

template <typename Itor, typename T>
void iota(Itor begin, Itor end, T seed) {
    while (begin != end) {
        *begin = seed++;
        ++begin;
    }
}

 

class Chrono {
    unsigned counter{};
public:
    Chrono(const Chrono&) { cout << "copie\n"; }
    Chrono() { cout << "construction\n"; };
    ~Chrono() { cout << "destruction\n"; };
    
    Chrono& operator++() {
        ++counter;
        return *this;
    }

 

    Chrono operator++(int unused) {
        Chrono oldChrono{ *this };
        ++counter;
        return oldChrono; // RVO return value optimization
    }

 

    friend ostream& operator<< (ostream& o, const Chrono& chrono) {
        return o << chrono.counter;
    }
};

 


int main()
{
    Chrono c;
    //cout << c << "\n";
    //++c;
    cout << c << "\n";
    c++;
}
