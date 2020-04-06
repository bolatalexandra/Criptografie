#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

//#include "CriptTools.h" 

using namespace std;

void citire(string& text, const char* fisier) {
	ifstream in(fisier);
	if (!in.good()) cerr << ("Fieier '" + string(fisier) + "' inexistent!") << endl;
	text.clear();
	char caracter;
	while (in >> noskipws >> caracter) text.push_back(caracter);
}

map<char, double> frecvente_alfabet(const string& alfabet, const char* fisier, bool sortat = false) {
	ifstream in(fisier);
	if (!in.good()) cerr << ("Fieier '" + string(fisier) + "' inexistent!") << endl;

	map<char, double> bloc = map<char, double>();
	unsigned int numar_caractere = 0;

	char caracter;
	while (in >> noskipws >> caracter) {
		numar_caractere++;

		// adaug in map-ul de frecevente 
		// doar daca am un asemenea caracter in alfabet
		if (alfabet.find_first_of(caracter) != string::npos) {
			bloc[caracter]++;
		}
	}

	for (auto& element : bloc) {
		bloc[element.first] = element.second * 1. / numar_caractere;
	}
	
	return bloc;
}

map<char, double> frecvente_extract(string text) {
	map<char, double> bloc = map<char, double>();
	for (auto caracter : text) {
		bloc[caracter]++;
	}
	for (auto & element : bloc) {
		bloc[element.first] = element.second * 1. / text.size();
	}
	return bloc;
}

string extrage_text(const string& text, size_t start, size_t pas, map<char, double>& frecenta_text) {
	string rezultat = "";
	frecenta_text = frecvente_extract(text);
	for (auto i = start; i < text.size(); i += pas) {
		if (rezultat.find_first_of(text[i]) == string::npos) {
			rezultat.push_back(text[i]);
		}
	}
	return rezultat;
}

char caracter(const string& alfabet, int cod);

size_t calculeaza_coincidente(
	const string& alfabet, 
	map<char, double>& frecventa_alfabet,
	string text, 
	map<char, double>& frecventa_text,
	char caracter_neexistent_in_alfabet = '#') 
{
	// adaug in frecvente caracterul neexistent in alfabet 
	frecventa_alfabet[caracter_neexistent_in_alfabet] = 0;

	// paddez text-ul cu acest caracter
	size_t diferenta = alfabet.size() - text.size();
	for (size_t i = 0; i < diferenta; i++) {
		text.push_back(caracter_neexistent_in_alfabet);
	}

	double maxim_suma = 0;
	size_t cate_siftari;
	size_t lungime = alfabet.size();
	for (size_t j = 0; j < lungime; j++) {
		 //cout << alfabet << '\n';
		 //cout << text << '\n';

		double suma = 0;
		for (size_t i = 0; i < lungime; i++) {
			suma += frecventa_alfabet[alfabet[i]] * frecventa_text[text[i]];
		}
		// cout << suma << ' ' << j << '\n';

		// siftez cu o pozitie la stanga
		char caracter = text[0];
		for (size_t i = 0; i < lungime - 1; i++) {
			text[i] = text[i + 1];
		}
		text[lungime - 1] = caracter;
		//cout << '\n';

		if (maxim_suma < suma) {
			maxim_suma = suma;
			cate_siftari = j;
		}
	}

	cout << maxim_suma << ' ' << caracter(alfabet, cate_siftari)  << ' ' << cate_siftari << '\n';
	
	return cate_siftari;
}

char caracter(const string& alfabet, int cod) {
	size_t n = alfabet.size();
	if (cod < 0) return alfabet[n - (-cod) % n];
	return alfabet[cod % n];
}

int cod(const string& alfabet, char caracter) {
	size_t n = alfabet.size();
	for (int i = 0; i < n; i++)
		if (alfabet[i] == caracter) return i;
	return -1;
}

//void lungime_cheie(string sursa) {
//	ofstream out("lungime_cheie.txt");
//
//	for (int i = 1; i < sursa.size(); i++) {
//		int nr_coincidente = 0;
//		for (int j = i; j < sursa.size(); j++) {
//			// cout << sursa[j] << ' ' << sursa[j - i] << '\n';
//			if (sursa[j] == sursa[j - i]) nr_coincidente++;
//		}
//		out << i << ": " << nr_coincidente << "\n";
//	}
//}

string generateKey(string str, string key)
{
	int x = str.size();

	for (int i = 0; ; i++)
	{
		if (x == i)
			i = 0;
		if (key.size() == str.size())
			break;
		key.push_back(key[i]);
	}
	return key;
}

string cipherText(string alfabet, string str, string key)
{
	string cipher_text;

	for (int i = 0; i < str.size(); i++)
	{
		int x = (cod(alfabet, str[i]) + cod(alfabet, key[i])) % alfabet.size();
		
		cipher_text.push_back(caracter(alfabet,x));

	}
	return cipher_text;
}

string originalText(string alfabet, string cipher_text, string key)
{
	string orig_text;

	for (int i = 0; i < cipher_text.size(); i++)
	{
		// converting in range 0-25 
		int x = (cod(alfabet, cipher_text[i]) - cod(alfabet, key[i]) + alfabet.size()) % alfabet.size();
		orig_text.push_back(caracter(alfabet, x));
	}
	return orig_text;
}

//int main() {
//	string str = "Ana are mere!";
//	string keyword = "Deny";
//
//	cout << "Str: " << str << '\n';
//
//	string alfabet;
//	citire(alfabet, "alfabet.txt");
//
//	string key = generateKey(str, keyword);
//	string cipher_text = cipherText(alfabet, str, key);
//
//	cout << "Ciphertext: " << cipher_text << "\n";
//
//	cout << "Decrypted Text: " << originalText(alfabet, cipher_text, key);
//}

int main() {
	static string ALFABET;
	string text_criptat;
	size_t lungime_cheie;

	// citire alfabet
	citire(ALFABET, "alfabet.txt");

	// frecventa alfabet
	auto frecventa_alfabet = frecvente_alfabet(ALFABET, "harap_alb.txt");

	// citire text criptat
	citire(text_criptat, "sursa.txt");

	// presupun lungea cheii 6
	for (int i = 4; i < 5; i++) {
		lungime_cheie = i;
		string cheie;
		for (size_t i = 0; i < lungime_cheie; i++) {
			map<char, double> frecventa_extract;
			auto extract = extrage_text(text_criptat, i, lungime_cheie, frecventa_extract);
			auto cod = calculeaza_coincidente(ALFABET, frecventa_alfabet, extract, frecventa_extract);
			// cout << parte << ' ' << caracter(ALFABET, parte) << '\n';
			cheie.push_back(caracter(ALFABET, cod));
		}
		cout << "Cheie: " << cheie << "\n\n";

		for (size_t i = lungime_cheie; i < text_criptat.size(); i++) {
			cheie.push_back(cheie[i % lungime_cheie]);
		}

		// dupa repetarea cheii
		// cout << cheie;

		cout << "Criptotext: " << text_criptat << '\n';

		string text_decriptat;

		//for (size_t i = 0; i < text_criptat.size(); i++) {
		//	text_decriptat.push_back(
		//		caracter(ALFABET, text_criptat[i] - cheie[i] + ALFABET.size())
		//	);
		//}

		cout << "Decrypted Text: " << originalText(ALFABET, text_criptat, cheie);
		//cout << "Plaintext: " << text_decriptat;

	}
	

	return 0;
}