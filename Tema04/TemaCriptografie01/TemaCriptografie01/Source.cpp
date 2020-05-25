#include "CriptTools.h"

typedef struct { BigInt<1024> n, de; } RSA_KEY;

void RSA_key_gen(BigInt<1024> MIN, BigInt<1024> MAX, RSA_KEY& p_key, RSA_KEY& s_key) {
    //functia va genera o pereche de chei pentru RSA, cu n intre MIN si MAX  
    if (MAX > 4000000)MAX = 4000000;//nu permitem MAX prea mare pentru a nu depasi prin calcule domeniul maxim admis pentru variabile 
    srand((int)time(NULL)); 
    BigInt<1024> p, q, phi;
    p = da_prim2(Sqrt(MIN), Sqrt(MAX), Sqrt(0));
    q = da_prim2(Sqrt(MIN), Sqrt(MAX), p);
    p_key.n = s_key.n = p*q;  
    phi = (p - 1)*(q - 1); 
    p_key.de = 3 + rand() % (phi - 3); 
    s_key.de = invers(p_key.de, phi);  
    while (s_key.de<0) {//pentru ca e sa fie inversabil  
        p_key.de++; 
        s_key.de = invers(p_key.de, phi); 
    } 
} 

    void RSA_cript_decript(const char* sursa, const char* destinatie, int lc, int ls, RSA_KEY & key) {
        BigInt<1024> m;
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
                    c[i] = da_caracter2(m % N);    
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

    void RSA_cript_cu_semnatura(const char* semnatura, const char* sursa, const char* destinatie, int lc, int ls, RSA_KEY p_key, RSA_KEY & s_p_key) {//semnatura de max 8 caractere 
        const int DIM = 48+1;//dimensiunea blocului din final (cu semnatura) 
        int i, m;  char c[DIM]; 
        RSA_KEY s_s_key;
        //RSA_key_gen(pow(N,lc) - 1, pow(N,ls), s_p_key, s_s_key);//generam cheile pentru semnatura
        RSA_key_gen(10000, 10000000, s_p_key, s_s_key);
       

        ifstream in(sursa); 
        ofstream temp("temp.txt"); 
        while (in.get(c, DIM)) {
            temp << c;
        } 
        in.close();

        SHA1 sha1;  
        unsigned long* rezultat = sha1.Valoare(sursa);
        sprintf(c, "%8s%08lx%08lx%08lx%08lx%08lx", semnatura, rezultat[0], rezultat[1], rezultat[2], rezultat[3], rezultat[4]);

        for (int i = 0; i + lc < DIM; i += lc) { 
            BigInt<1024> m = 0;
            int j;
            char bloc[DIM];  
            for (j = 0; j < lc; j++)
                m = m * N + da_cod(c[i + j]);  
            m = a_la_b_mod_c(m, s_s_key.de, s_s_key.n);  
            j = ls - 1;  
            while (m > 0) {
                bloc[j] = da_caracter2(m % N); 
                m = m / N;   
                j--; 
            }   
            while (j >= 0)
                bloc[j--] = da_caracter(0);  
            for (j = 0; j < ls; j++)
                temp << bloc[j]; 
            j = 0; 
        }  
        in.close(); 
        temp.close(); 
        RSA_cript_decript("temp.txt", "destinatie.txt", lc, ls, p_key); 
        remove("temp.txt");//stergem fisierul temporar
    } 
        void RSA_verifica_semnatura(const char* sursa,int DIM_SEMNATURA,int lc, int ls, RSA_KEY s_p_key) {//lc,ls - lungimea la citire,scriere in momentul criptarii  
            char* c = new char[DIM_SEMNATURA + 1], *semnatura=new char[DIM_SEMNATURA + 1]; 
            int index = 0;
            FILE *in= fopen(sursa, "r"); 
            fseek(in, 0, SEEK_END);  
            long lungime = ftell(in); 
            fseek(in, (lungime - DIM_SEMNATURA), SEEK_SET); 
            for(int i=0;i<DIM_SEMNATURA;i++)
                c[i]=fgetc(in);  fclose(in); 
            for (int i = 0; i + ls < DIM_SEMNATURA; i += ls) {  
                BigInt<1024> m = 0;
                long j;
            char bloc[10]; 
            for (j = 0; j < ls; j++)
                m = m * N + da_cod(c[i + j]); 
            m = a_la_b_mod_c(m, s_p_key.de, s_p_key.n); 
            j = lc - 1;   
            while (m > 0) { 
                bloc[j] = da_caracter2(m % N);   
                m = m / N;   
                j--; 
            }  
            while (j >= 0)
                bloc[j--] = da_caracter(0); 
            for (j = 0; j < lc; j++)
                semnatura[index++] = bloc[j]; 
            j = 0;
    }

    semnatura[index] = '\0';  cout << semnatura << endl;//semnatura decriptata, compusa din concatenarea cuvantului folosit la semnatura si sha1() din textul in clar  
    delete [] c;  
    delete [] semnatura;
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

    /* int main() {//pentru criptare - decriptare 
    citeste_alfabet();  
    RSA_KEY p_key, s_key;  
    int lc = 2, ls = 3;  
    RSA_key_gen(pow(N,lc)-1,40000,p_key, s_key);//generam cheile pentru bloc de 2 caractere la citire si trei la scriere 
    RSA_cript_decript("sursa.txt","destinatie.txt",2,3,p_key);//textul din "sursa.txt" este criptat in "destinatie.txt" 
    RSA_cript_de
    cript("destinatie.txt", "decriptat.txt", 3, 2, s_key);//textul din "destinatie.txt" este decriptat si trecut in fisierul "decriptat.txt" } */ /*
    */ 

    int main() {//pentru criptare cu semnatura- decriptare  
        citeste_alfabet();  
        RSA_KEY p_key,s_key, p_s_key; 
        int lc = 2, ls = 3; 
        RSA_key_gen(10000, 10000000, p_key, s_key);
        RSA_cript_cu_semnatura("Alexandra", "sursa.txt", "destinatie.txt", lc, ls, p_key, p_s_key);  
        RSA_cript_decript("destinatie.txt", "decriptat.txt", ls, lc, s_key);  
        RSA_verifica_semnatura("decriptat.txt", 72,2,3, p_s_key); 
    } 