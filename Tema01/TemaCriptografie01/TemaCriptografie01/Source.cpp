#include <iostream>
#include <fstream>
#include <string> 
#include <sstream>
#include <vector>

using namespace std;

char caracter[100] = { 0 };
int N = 0;

void citeste_alfabet() {
	N = 0;
	ifstream in("alfabet.txt");
	if (!in.good())
		perror("Fisier inexistent");
	char c;
	while (in >> noskipws >> c) {
		caracter[N] = c;
		N++;
	}
	if (N == 0)
		cout << "Alfabetul dat are 0 caractere" << endl;
	in.close();
}

int modulo(int k, int n) {
	if (k < 0)k = n - (-k) % n;
	if (k >= n) return k % n;
	return k;
}

char da_caracter(int cod) {
	return caracter[modulo(cod, N)];
}

int da_cod(char c) {
	for (int i = 0; i < N; i++)
		if (caracter[i] == c)return i;
	return -1;
}

int invers(int a, int n) {
	int q, r, x0 = 1, x1 = 0, copy_n = n;
	a = modulo(a, n);
	while (n != 0)
	{
		r = n;
		q = a / n;
		n = a % n;
		a = r;

		r = x1;
		x1 = x0 - q * x1;
		x0 = r;
	}
	if (a == 1)
		return modulo(x0, copy_n);
	return -1;
}


void find_plaintext() {
	int a, b;

	citeste_alfabet();

	ifstream in("sursa.txt");
	ofstream out("output.txt");

	string input_str; char c;
	while (in >> noskipws >> c) {
		input_str.push_back(c);
	}

	const char* alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789():; .,!?-'";

	for (unsigned int i = 0; i < strlen(alph); i++)
	{
		for (unsigned int j = 0; j < strlen(alph); j++)
		{
			if (alph[i] == alph[j]) continue;

			cout << "index: (" << i << ", " << j << ")\n";

			a = i; b = j;

			// calculate decryption key
			a = invers(a, N);
			b = modulo(-a * b, N);

			// decrypt
			string result_str = "";
			cout << modulo(da_cod(input_str[0]) * a + b, N) << ' '  << da_cod(';') << '\n';

			string simboluri = "():; .,!?-'";
			string constante = "BDFGHIJKLMPQRSTVWXYZbcdfghjklmpqrstvwxy():; ,?-'";
			bool ok = false;
			for (auto c : simboluri) {
				if (modulo(da_cod(input_str[0]) * a + b, N) == da_cod(c)) {
					ok = true;
					break;
				}
			}
			if (ok) continue;
			for (int i = 0; i < input_str.length() - 1; i++) {
				for (auto c : constante) {
					if (modulo(da_cod(input_str[i]) * a + b, N) == da_cod(c) && modulo(da_cod(input_str[i + 1]) * a + b, N) == da_cod(c)) {
						ok = true;
						break;
					}
				}
			}
			if (ok) continue;
			for (auto c : input_str) {
				result_str.push_back(da_caracter(da_cod(c) * a + b));
			}
			vector<string> result;
			stringstream s_stream(result_str);
			while (s_stream.good()) {
				string substr;
				getline(s_stream, substr, ' '); 
				result.push_back(substr);
			}
			for (auto c : result) {
				if (c.length() > 15) ok = true;
			}
			if (ok) continue;
			out << "input: " << input_str << '\n';
			out << "key temp: (" << i << ", " << j << ")\n";
			out << "key e: a = " << invers(a, N) << "; b = " << modulo(-a * b, N) << '\n';
			out << "key d: a = " << a << "; b = " << b << '\n';
			out << "output: ";
			cout << '\n';
			out << result_str << "\n\n";
		}
	}
}

int main() {
	find_plaintext();

	return 0;
}