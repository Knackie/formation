#include <iostream>
#include <array>
using namespace std;

 

struct MoteurElectrique {void demarrer() { cout << "bbzzzz\n"; }};
struct MoteurEssence {void demarrer() { cout << "vrrrouumm\n"; }};

 

struct RoueA {void rouler() { cout << "je roule en A\n"; }};
struct RoueB {void rouler() { cout << "je roule en B\n"; }};

 

struct ClimatisationBasique { void demarrer() { cout << "je ventile\n"; } };
struct ClimatisationDeLuxe { void demarrer() { cout << "je climatise\n"; } };
struct ClimatisationNulle { void demarrer() {} };

 

struct Portiere { void fermer() { cout << "clac\n"; } };

 

template <class MOTEUR, class ROUE, class CLIMATISATION, size_t NB_PORTIERES>
class Voiture {
    MOTEUR moteur;
    array<ROUE, 4> roues;
    CLIMATISATION climatisation;
    array<Portiere, NB_PORTIERES> portieres;
public:
    void demarrer() {
        for (Portiere& p : portieres)
            p.fermer();
        moteur.demarrer();
        for (ROUE& r : roues)
            r.rouler();
        climatisation.demarrer();
        cout << "\n";
    }
};

 

template <class MOTEUR, class ROUE, class CLIMATISATION>
using Coupe = Voiture<MOTEUR, ROUE, CLIMATISATION, 2>;

 

template <class ROUE, class CLIMATISATION>
using CoupeElectrique = Voiture<MoteurElectrique, ROUE, CLIMATISATION, 2>;

 

template <class ROUE, class CLIMATISATION>
using CoupeEssence = Voiture<MoteurEssence, ROUE, CLIMATISATION, 2>;

 

template <class MOTEUR, class ROUE, class CLIMATISATION>
using Berline = Voiture<MOTEUR, ROUE, CLIMATISATION, 4>;

 

using CoupeMegane = Coupe<MoteurElectrique, RoueA, ClimatisationNulle>;
using BerlineClio = Berline<MoteurEssence, RoueB, ClimatisationDeLuxe>;

 

int main()
{
    CoupeMegane v1;
    v1.demarrer();

 

    BerlineClio v2;
    v2.demarrer();
}
