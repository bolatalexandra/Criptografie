#include <iostream>
#include<fstream>
#include <stdlib.h>
#include <vector>
#include "CriptTools.h"

using namespace std;
/*
pasi:
o functie pentru a afla toate cheile posibile:
-parcurgem numerele de la 5000 la 5999(sau cum ai tu cheile) verificam care se termina cu 1 si factorizam acel n pentru a afla p si q
-avand p si q aflam phi
-nr. cifrelor lui p trebuie sa fie egal sau aproxiumativ egal cu cel al lui q
-de la 3 pana la phi(n)-1 alegem e
-daca e si phi sunt numere prime, incepe cu 16 si e^{-1}(modulo phi(n))!=-1

Mai apoi facem:
-cat timp mai avem chei facem decriptarea mesajului din sursa

*/

void cautareChei(string text, int e, int n, int phi, vector<int>& nvector, vector<int>& dvector) {
	int d = invers(e, phi);
	while (d < 0) {
		e++;
		d = invers(e, phi);
	}
	bool adaugare = true;
	for (int i = 0; i < dvector.size(); i++)
		if (dvector[i] == d) {
			adaugare = false;
			break;
		}
	if (adaugare)
	{
		dvector.push_back(d);
		nvector.push_back(n);
	}
}

void decriptRSA(vector<int> dvector, string dtext, vector<int> nvector)
{
	ofstream out("destinatie.txt");
	int j, l, m, i;
	for (int c = 0; c < dvector.size(); c++) {
		out << "d:" << dvector[c] <<" n:"<< nvector[c] << "\n";
		//for (i = 0, m = 1; m < nvector[c]; i++)
		//	m *= N;
		//for (j = 1; j <= i; j++)
		//	for (l = i; l <= 5; l++)
		j = 3;
		l = 2;
		char* v = new char[l >= j ? l : j];
		{
			ofstream co("citire.txt");
			for (int i = 0; i < dtext.length(); i++) {
				co << dtext[i];
			}
			co.close();
			ifstream in("citire.txt");
			m = i = 0;
			while (in >> noskipws >> v[i]) {
				//cout << dvector[c] << ' ' << nvector[c] << '\n';
				m = m * N + da_cod(v[i]);
				if (i == j - 1) {
					m = a_la_b_mod_c(m, dvector[c], nvector[c]);
					i = l - 1;
					while (m > 0) {
						v[i] = da_caracter(m % N);
						m = m / N;
						i--;
					}
					while (i >= 0)
						v[i--] = da_caracter(0);
					for (i = 0; i < l; i++) {
						out << v[i];
					}
					i = 0;
				}
				else
					i++;
			}
			in.close();
			out << '\n';
		}
		out << '\n';
	}
	out.close();
}


string Citire() {
	string c;
	citeste_alfabet();
	ifstream in("sursa.txt");
	while (in >> noskipws >> c)
		//in.close();
		return c;
}

int numarCifre(int p)
{
	int nr = 0;
	while (p) {
		p /= 10;
		nr++;
	}
	return nr;
}

void setCheie(int& e, int& p, int& q, int& n, int& phi) {
	vector<int> dvector, nvector;
	string textCriptat = Citire();
	srand((int)time(NULL));
	//n = rand() % 99 + 1500;
	for (int i = 15001; i <= 15991; i+=10){
		n = i;
		factorizare(n, p, q);
		if (p != -1 && q != -1 && p!= 1 && q!=1) {
			phi = (p - 1) * (q - 1);
			if (numarCifre(p) == numarCifre(q) + 1 || numarCifre(q) == numarCifre(p) + 1 || numarCifre(q) == numarCifre(p))
			for (int j = 5000; j < 5999; j++)
			{
				//e = 3 + rand() % (phi - 1);
				e = j;
				//cout << e << '\n';
					if (cmmdc(e, phi) == 1 && a_la_b_mod_c(e,-1,phi) != -1 && e < phi)
					{
						cautareChei(textCriptat, e, n, phi , nvector, dvector);
						cout << e << ' ' << p << ' ' << q << ' ' << n << ' ' << phi << '\n';
						//setCheie(e, p, q, n, phi);

					}
			}
		}
	}
	decriptRSA(dvector, textCriptat, nvector);
}


int main() {
	int e, p, q, n, phi;
	setCheie(e, p, q, n, phi);



	return 0;
}


