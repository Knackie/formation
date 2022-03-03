#include <iostream>
#include <thread>
#include <chrono>
using namespace std;
using namespace std::chrono;

 

int func(ostream& o, const char* msg) {
//    throw 123;
    int n{100};
    while (n-- > 0) {
        o << msg << "\n";
    }
    return 99;
}

 

int main()
{
    cout << thread::hardware_concurrency() << "\n";
    try {
        thread th{ func, ref(cout), "bonjour" };
        thread th2{ func, ref(cout) , "    guten tag" };
        thread th3{ func, ref(cout), "                        hello" };
        th.join();
        th2.join();
        th3.join();
    }
    catch (...) {
        cout << "oupsss\n";
    }
}
