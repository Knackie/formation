#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

template<class ForwardIt, class T>
void Myiota(ForwardIt first, ForwardIt last, T value)
{
    while (first != last) {
        *first++ = value;
        ++value;
    }
}



int main()
{
	int tab[10];
	vector<int> v(10);

    Myiota(begin(tab), end(tab), 0);
    Myiota(v.begin(), v.end(), 0);

    for (int i = 0; i < v.size(); i++)
    {
        cout << "v de i : " << v[i] << endl;
    }

}
