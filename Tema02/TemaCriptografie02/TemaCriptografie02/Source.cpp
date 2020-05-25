
/*
-avem numarul prim p declarat long long(este dat in problema)
-fragmentele sunt declarate tot de tip long long
-se ia functia lui de reconstituire secret si se modifica deoarece nu se introduce de la tastaura
-in functia reconstituire secret se pune ca x si y sa fie de tip long long, de asemenea unde sunt cele doua foruri (tot in functie) se pune :
	for (long long i = 0; i < m; i++) {
		long long l = 1;
		for (int j = 0; j < m; j++)
-se face o functie pentru a genera toate combinarile
-dupa ce se reconstituire secrete pentru fiecare combinare de 15 luate cate 10  fragmente (ar trebui sa fie combinari de 15 luate cate 10=3303 variabile posibile, dar cum sunt doua fragmente eronate combinari de 13 luate cate 10=286 chei bune care se repeta)
deci trebuie se vede ce cheie se repeta de 286 de ori ca mai apoi sa verifica care sunt fragementele eronate.

*/

#include "CriptTools.h"

using namespace std;

void fragmentare_secret() { 
	int S, n, k, p, * a;  
	cout << "Dati secretul: "; 
	cin >> S;  
	cout << "Dati numarul total de fragmente: ";  
	cin >> n; 
	cout << "Dati numarul de fragmente pentru reconstituirea secretului: ";  
	cin >> k; 
	p = da_prim(S > n ? S : n, 100000, 0); 
	a = new int[k]; 
	a[0] = S; 
	for (int i = 1; i < k - 1; i++) 
		a[i] = rand() % p;  
	a[k - 1] = 1 + rand() % (p - 1);
	cout << "\nFragmente generate:\n"; 
	for (int i = 1; i <= n; i++) 
		cout << "(" << i << "," << val_pol(a, k - 1, i, p) << ")" << " "; 
	cout << "S-a folosit numarul prim: " << p << endl; }



ofstream out("destinatie.txt");

void reconstituire_secret(long long p, long long k, long long *x, long long*y) {
	long long S = 0;
	for (long long i = 0; i < k; i++) { 
		long long l = 1;   
		for (int j = 0; j < k; j++)
			if (j != i)l = modulo(-x[j] * l * invers(x[i] - x[j], p), p);  
		S = modulo(S + y[i] * l, p); 
	} 
	for (int i = 1; i <= 10; i++)
	{
		out << "(" << x[i] << "," << y[i] << ") ";
	}
	out << "\nSecretul este: " << S << "\n\n";
}

int  st[20], n = 15, m = 10;
long long p, k;
long long* xcombinari;
long long* ycombinari;
vector<int> v;

void Afisare()
{
	int i;
	for (i = 1; i <= m; i++)
	{
		xcombinari[i] = st[i];
		ycombinari[i] = v[st[i] - 1];
		cout << "(" << xcombinari[i] << "," << ycombinari[i] << ") ";
	}
	cout << '\n';
	reconstituire_secret(p, k, xcombinari, ycombinari);
}

bool valid(int k)
{
	if (k > 1)
		if (st[k - 1] >= st[k]) return 0;
	return 1;
}

void Back(int k)
{
	int i;
	for (i = 1; i <= n; i++)
	{
		st[k] = i;
		if (valid(k) == 1)
		{
			if (k == m) {
				Afisare();
			}
			else Back(k + 1);
		}
	}
}


void  citireFragmente(vector<int> &v, long long&p, long long&k) {
	ifstream fin("sursa.txt");
	fin >> p;
	fin >> k;
	int x;
	while (fin >> x)
		v.push_back(x);
}

int main() {
	citireFragmente(v,p,k);
	xcombinari = new long long[k];
	ycombinari = new long long[k];
	Back(1);
	//fragmentare_secret();
	//reconstituire_secret();

	return 1;
}