#include <iostream>
#include <string>
using namespace std;

 

class Carnivore {
protected:
    Carnivore() = default;
    void manger() { cout << "je mange de la viande\n"; }
};

 

class Omnivore {
protected:
    Omnivore() = default;
    void manger() { cout << "je mange de tout\n"; }
};

 

class Bipede {
protected:
    Bipede() = default;
    void bouger() { cout << "je marche\n"; }
};

 

class Quadrupede {
protected:
    Quadrupede() = default;
    void bouger() { cout << "je galope\n"; }
};

 

template <class LOCOMOTION, class ALIMENTATION>
class Animal :
    protected LOCOMOTION,
    protected ALIMENTATION
{
public:
    void vivre() {
        LOCOMOTION::bouger();
        ALIMENTATION::manger();
        cout << "\n";
    }
};

 

class Singe : public Animal<Bipede, Omnivore> {};
class Lion : public Animal<Quadrupede, Carnivore> {};

 


int main()
{
    Lion lion;
    lion.vivre();

 

    Singe singe;
    singe.vivre();
}
