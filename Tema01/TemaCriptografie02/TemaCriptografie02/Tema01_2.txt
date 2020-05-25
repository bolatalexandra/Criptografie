#include <fstream>
#include "CriptTools.h" 

using namespace std;


pair<Matrice<int>, Vector<int>> generare_chei_criptare_afina(int k, int n) {
	Matrice<int> A(k, k);  
	Vector<int> b(k);
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < k; j++) {
			if (i == j)
				A[i][j] = 1;
			else {
				A[i][j] = 0;
			}
		}
	}
	A[0][0] = 1 + rand() % (n - 1);  
	while (invers(A[0][0], n) < 0)
		A[0][0]++;
	for (int nr = 0; nr < 10 * k; nr++) { 
		int l1 = rand() % k;   
		int l2 = rand() % k;   
		if (l1 == l2)l2++;   
		if (l2 >= k)l2 = 0;   
		for (int i = 0; i < k; i++)   
			A[l1][i] = (A[l1][i] + A[l2][i]) % n; 
	}

	for (int i = 0; i < k; i++)   
		b[i] = modulo(rand(), n);  
	return pair<Matrice<int>, Vector<int>>(A, b);
}

pair<Matrice<int>, Vector<int>> calcul_cheie_decriptare_afina(pair<Matrice<int>, Vector<int>> cheie_criptare, int n) { 
	Matrice<int> A = cheie_criptare.first.inv(n);  
	return pair<Matrice<int>, Vector<int>>(A, (-A * cheie_criptare.second) % n); 
}

void criptare_afina(pair<Matrice<int>, Vector<int>> cheie, string text) {
	ofstream out("destinatie.txt"); 
	char c;  
	int i = 0, k = cheie.first.NrLin();  
	Vector<int> v(k); 
	for(auto c : text) {
		v[i++] = da_cod(c);  
		if (i == k) { 
			v = cheie.first * v + cheie.second;
			for (int i = 0; i < k; i++)    
				out << da_caracter(modulo(v[i], N));   
			i = 0; 
		} 
	}
	cout << "\n";
}

vector<pair<int, string>> analiza_frecvente(int k) {
	vector<pair<int, string>> blocuri;
	ifstream in("sursa.txt");
	if (!in.good())   
		cout << "Fisier inexistent (pentru analiza frecventelor)" << endl;  
	char c[100]; 
	while (!in.eof()) { 
		in.read(c, k);  
		c[k] = '\0';   
		string s(c);   
		bool mai_este = false;   
		for (int i = 0; i < blocuri.size(); i++) { 
			if (blocuri[i].second == s) { 
				blocuri[i].first++;     
				mai_este = true;     
				break; 
			} 
		}   
		if (!mai_este)    
			blocuri.push_back(std::pair<int, string>(1, s)); 
	}

	sort(blocuri.begin(), blocuri.end(), greater<>());//pentru sortare descendenta  
	return blocuri; 
} 


string citire()
{
	citeste_alfabet();
	ifstream in("sursa.txt");
	if (!in.good())
		perror("fisier inexistent");
	string input_str; char c;
	while (in >> noskipws >> c) {
		input_str.push_back(c);
	}
	return input_str;
}

//void decriptare(pair<Matrice<int>, Vector<int>> cheie, int n, string input_str) {
//	ofstream out("output.txt");
//	out << "buna\n";
//	int i = 0, k = cheie.first.NrLin();
//	Vector<int> v(k);
//	for (auto c : input_str) {
//		
//		v[i++] = da_cod(c);
//		if (i == k) {
//			v = cheie.first * v + cheie.second;
//			for (int i = 0; i < k; i++)
//				out << da_caracter(modulo(v[i], N));
//			i = 0;
//		}
//	}
//	out << '\n\n';
//}

void plaintext() {
	string text = citire();
	ofstream out("output.txt");
	srand(time(NULL));

	//pair< Matrice<int>, Vector<int>> cheiec = generare_chei_criptare_afina(2, N);
	//criptare_afina(cheiec, text);

	for (int z = 0; z < 10000; z++)
	{
		cout << z << '\n';
		string result_str = "";
		pair< Matrice<int>, Vector<int>> cheiec = generare_chei_criptare_afina(2, N);
		out << "c: " << cheiec.first << cheiec.second << '\n';
		pair<Matrice<int>, Vector<int>> cheied = calcul_cheie_decriptare_afina(cheiec, N);
		out << "d: " << cheied.first << cheied.second << '\n';
		int i = 0, k = cheied.first.NrLin();
		Vector<int> v(k);
		for (int i = 0; i < k; i++)
			v[i] = 0;
		for (auto c : text) {
			v[i++] = da_cod(c);
			if (i == k) {
				v = cheied.first * v + cheied.second;
				for (int i = 0; i < k; i++)
					result_str.push_back(da_caracter(modulo(v[i], N)));
				i = 0;
			}
		}
		// out << "input: " << text << '\n';
		out << result_str << "\n";
	}
}

	


int main() {
	//vector<pair<int, string>> blocuri;
	//blocuri = analiza_frecvente();
	//for (int i = 0; i < blocuri.size(); i++) { 
	//	cout << blocuri[i].second << " - " << blocuri[i].first << endl; 
	//}
	
	plaintext();
	
	//criptare_afina(chei, text);
	return 0;
}