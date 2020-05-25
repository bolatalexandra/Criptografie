#include "CriptTools.h"
/*
    luam msg1 sau msg2, folosim cheia secreta si decriptam mesajul folosind Vigenere
    mesajul il impartim in vectori de biti cate 128 si 160
    facem sha1 din 128
    daca sunt egale cu biti de 160 atunci e bine
*/

typedef struct { long n, de; } RSA_KEY;


void RSA_cript_decript(const char* sursa, const char* destinatie, int lc, int ls, RSA_KEY& key) {
    long m;
    long i;
    char* c = new char[lc >= ls ? lc : ls];
    ifstream in(sursa);
    ofstream out(destinatie);
    i = 0;
    m = 0;
    while (in >> noskipws >> c[i]) {
        m = m * N + da_cod(c[i]);
        if (i == lc - 1) {
            m = a_la_b_mod_c(m, key.de, key.n);
            i = ls - 1;
            while (m > 0) {
                c[i] = da_caracter(m % N);
                m = m / N;
                i--;
            }
            while (i >= 0)
                c[i--] = da_caracter(0);
            for (i = 0; i < ls; i++)
                out << c[i];
            i = 0;
        }
        else
            i++;
    }  out.close();
    in.close();
    delete[] c;
}


void decriptareVigenere(const char *cheie) {
    int i, k;
    int corect = 1;
    for (i = 0; cheie[i] != '\0'; i++) {
        int temp = da_cod(cheie[i]);
        if (temp < 0 || temp >= N)
            corect = 0;
    }
    k = i;
    char c;
    ifstream cmsg1("msg1.txt");
    ofstream amsg1("decriptatmsg1.txt");
    i = 0;
    while (cmsg1 >> noskipws >> c) {
        amsg1 << da_caracter(da_cod(c) - da_cod(cheie[i]));
        i++;
        if (i == k)
            i = 0;
    }
    cmsg1.close();
    amsg1.close();
    ifstream cmsg2("msg2.txt");
    ofstream amsg2("decriptatmsg2.txt");
    i = 0;
    while (cmsg2 >> noskipws >> c) {
        amsg2 << da_caracter(da_cod(c) - da_cod(cheie[i]));
        i++;
        if (i == k)
            i = 0;
    }
    cmsg2.close();
    amsg2.close();
}

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
    cout << "S-a folosit numarul prim: " << p << endl;
}


void reconstituire_secret() {
    long long S = 0, k, p, * x, * y; 
    cout << "Dati numarul prim: "; 
    cin >> p;  cout << "Dati numarul de fragmente pentru reconstituirea secretului: ";  
    cin >> k;  
    x = new long long[k];
    y = new long long[k];
    cout << "Dati fragmentele: " << endl; 
    for (int i = 0; i < k; i++) { 
        cout << "x[" << (i + 1) << "]=";   
        cin >> x[i];   
        cout << "y[" << (i + 1) << "]=";   
        cin >> y[i];
    }
    for (long long i = 0; i < k; i++) {
        long long l = 1;
        for (int j = 0; j < k; j++)
            if (j != i)l = modulo(-x[j] * l * invers(x[i] - x[j], p), p);
        S = modulo(S + y[i] * l, p);
    }
    cout << "Secretul este: " << S << "\n\n";
}

template <size_t N> bitset<N> rotire_st(bitset<N> bloc, int nr) { 
    return bloc << nr | bloc >> (N - nr); 
}

template <size_t N1, size_t N2> void permutare(bitset<N1> bloc_in, int P[], bitset<N2>& bloc_fin) { 
    for (int i = 0; i < N2; i++) { 
        bloc_fin[N2 - 1 - i] = bloc_in[N1 - P[i]];
    } 
}

template <size_t N> bitset<N> XOR(bitset<N> bloc1, bitset<N> bloc2) {
    return (bloc1 | bloc2) & (~bloc1 | ~bloc2);
}

void decriptareMesaj(const char* sursa)
{
    SHA1 sha1, sha2;  
    unsigned long* rezultat1 = sha1.Valoare(sursa, 128);
    unsigned long* rezultat2 = sha2.Valoare(sursa, 160);
    cout << rezultat1 << " " << rezultat2 << '\n';
    //sha1.transforma_h(rezultat1);
    sha2.transforma_h(rezultat2);
    cout << rezultat1 << " " << rezultat2 << '\n';
    if (rezultat1 == rezultat2)
        cout << "Gasiti aici " << sursa << " mesajul lui Bob!\n";
}

/* int main() {//pentru generarea cheilor
  citeste_alfabet();
  RSA_KEY p_key, s_key;
  int lc = 2, ls = 3;
  cout << "Programul va genereaza o cheie pentru criptare;";
  RSA_key_gen(pow(N,lc)-1,pow(N,ls),p_key, s_key);
  cout << "\nCheia publica--> Ke=(" << p_key.n << "," << p_key.de << ")";
  cout << "\nCheia secreta--> Kd=(" << s_key.n << "," << s_key.de << ")" << endl;
  } */

//int main() {//pentru criptare - decriptare
//    citeste_alfabet();
//    RSA_KEY p_key, s_key;
//    int lc = 1, ls = 1;
//    RSA_key_gen(pow(N, lc) - 1, 40000, p_key, s_key);//generam cheile pentru bloc de 2 caractere la citire si trei la scriere
//    //p_key.de = 8093;
//    //p_key.n = 33227;
//    RSA_cript_decript("sursa.txt", "destinatie.txt", 1, 1, p_key);//textul din "sursa.txt" este criptat in "destinatie.txt"
//    RSA_cript_decript("destinatie.txt", "decriptat.txt", 1, 1, s_key);//textul din "destinatie.txt" este decriptat si trecut in fisierul "decriptat.txt" } */ /*
//}

//int main() {//pentru criptare cu semnatura- decriptare  
//    citeste_alfabet();
//    RSA_KEY p_key, s_key, p_s_key;
//    int lc = 2, ls = 3;
//    RSA_key_gen(10000, 10000000, p_key, s_key);
//    RSA_cript_cu_semnatura("Alexandra", "sursa.txt", "destinatie.txt", lc, ls, p_key, p_s_key);
//    RSA_cript_decript("destinatie.txt", "decriptat.txt", ls, lc, s_key);
//    RSA_verifica_semnatura("decriptat.txt", 72, 2, 3, p_s_key);
//}


int main() {
    citeste_alfabet();
    decriptareVigenere("01111110101100000000011011100010001011010011");
    decriptareMesaj("decriptatmsg1.txt");
    decriptareMesaj("decriptatmsg2.txt");

    return 1;
}