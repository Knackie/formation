#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
#include <functional>
using namespace std;

 

class Alimentation {
protected:
    Alimentation() = default;
    void manger() { cout << "je mange de "; }
};

 

class Carnivore : public Alimentation {
protected:
    Carnivore() = default;
    void manger() {
        Alimentation::manger();
        cout << "la viande\n";
    }
};

 


class Predateur : public Carnivore {
    void tuer() { cout << "je tue ma proie\n"; }
protected:
    Predateur() = default;
    void manger() {
        tuer();
        Carnivore::manger();
    }
};

 

class Omnivore : public Alimentation {
protected:
    Omnivore() = default;
    void manger() {
        Alimentation::manger();
        cout << "tout\n";
    }
};

 

class Herbivore : public Alimentation {
protected:
    Herbivore() = default;
    void manger() { 
        Alimentation::manger();
        cout << "l'herbe\n";
    }
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

 

class Creature {
protected:
    virtual ~Creature() = default;
public:
    virtual void vivre() = 0;
};

 


class TRUC { protected: void manger() { cout << "*****************\n"; } };

 

template <class LOCOMOTION, class ALIMENTATION>
class Animal :
    public Creature,
    protected LOCOMOTION,
    protected ALIMENTATION
{
    static_assert (is_base_of_v<Alimentation, ALIMENTATION>, "alimentation invalide");

 

public:
    void vivre() override {
        LOCOMOTION::bouger();
        ALIMENTATION::manger();
        cout << "\n";
    }
};

 

template <class ALIMENTATION = Omnivore>
class Oiseau : public Animal <Bipede, ALIMENTATION>
{
    void voler() { cout << "je m'envoie en l'air\n"; }
public:
    void vivre() override {
        voler();
        Animal<Bipede, ALIMENTATION>::vivre();
    }
};

 


class Rapace : public Oiseau <Predateur> {};

 

class Gazelle : public Animal<Quadrupede, Herbivore> {};
class Moineau : public Oiseau<> {};
class Singe : public Animal<Bipede, Omnivore> {};
class Aigle : public Rapace {};
class Guepard : public Animal<Quadrupede, Predateur> {};

 

int main()
{
    Gazelle gazelle;
    Moineau moineau;
    Singe singe;
    Aigle aigle;
    Guepard guepard;

 

    vector<reference_wrapper<Creature>> animaux;
    animaux.push_back(gazelle);
    animaux.push_back(moineau);
    animaux.push_back(singe);
    animaux.push_back(aigle);
    animaux.push_back(guepard);

 

    for (Creature& a : animaux)
        a.vivre();
}
