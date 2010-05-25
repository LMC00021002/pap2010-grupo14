#define FILEINPUT
#ifdef FILEINPUT
#include <fstream>
#else
#include <iostream>
#endif

#include <vector>
#include <math.h>
using namespace std;

typedef pair< int, int > pii;

inline int max( int a, int b ) { return a > b ? a : b; }
inline pii& max( pii& a, pii& b ){ return a.second > b.second ? a : b; }

class Data
{
public:
    Data(){};
    Data( pii primero, pii ultimo, pii mejor ) : prim( primero ), ult( ultimo ), mejor( mejor ){};

    pii prim;
    pii mejor;
    pii ult;
};

vector< vector< Data > > tabla;


void procesarTabla( const vector<int>& enteros )
{
    int n = enteros.size();
    int log2n = log10f( n )/log10f(2) + 1;
    tabla = vector< vector< Data > >( n, vector< Data >( log2n ) );
    // inicializo la tabla para los intervalos de longitud 1
    for( int i = 0; i < n; i++ )
        tabla[i][0] = Data( pii(enteros[i], 1), pii(enteros[i], 1), pii(enteros[i], 1) ) ;

    // ahora calculo los valores a partir de intervalos de la mitad de tamaño
    for( int j = 1; 1 << j <= n; j++ )
    {
        int powj = 1 << (j - 1);
        for( int i = 0; i + (1 << j) - 1 < n; i++ )
        {
            if( tabla[i][j - 1].ult.first == tabla[i + powj][j - 1].prim.first)
            {
                if( tabla[i][j - 1].prim.first == tabla[i + powj][j - 1].ult.first )
                {
                    int cantOcurrencias = tabla[i][j - 1].prim.second + tabla[i + powj][j - 1].ult.second;
                    pii tupla( tabla[i][j - 1].ult.first, cantOcurrencias );
                    tabla[i][j].prim = tupla;
                    tabla[i][j].ult = tupla;
                    tabla[i][j].mejor = tupla;
                }
                else
                {
                    pii tuplaMedio( tabla[i][j - 1].ult.first, tabla[i][j - 1].ult.second + tabla[i + powj][j - 1].prim.second );
                    tabla[i][j].prim = tuplaMedio.first == tabla[i][j - 1].prim.first ? tuplaMedio : tabla[i][j - 1].prim;
                    tabla[i][j].ult = tuplaMedio.first == tabla[i + powj][j - 1].ult.first ? tuplaMedio : tabla[i + powj][j - 1].ult;
                    tabla[i][j].mejor = max( tabla[i][j].prim, max( tabla[i][j].ult, tuplaMedio ) );
                }
            }
            else
            {
                tabla[i][j].prim = tabla[i][j - 1].prim;
                tabla[i][j].ult = tabla[i + powj][j - 1].ult;
                tabla[i][j].mejor = max( tabla[i][j - 1].mejor, tabla[i + powj][j - 1].mejor );
            }
        }
    }
}



int calcularFV( int i, int j )
{
    int k = static_cast<int>( log10f( static_cast<float>(j - i + 1) )/log10f(2) );
    int dosALaK = 1 << k;
    Data& parte1 = tabla[i][k];
    Data& parte2 = tabla[j - dosALaK + 1][k];

    return max( parte1.mejor.second, parte2.mejor.second );
}


int main()
{
#ifdef FILEINPUT
    ifstream entrada("test2", ios_base::in);
#else
    istream& entrada = cin;
#endif
    int n, q;

    entrada >> n >> q;

    vector< int > enteros( n );
    for( int i = 0; i < n; i++ )
    {
        entrada >> enteros[ i ];
    }

    procesarTabla( enteros );

    for( int i = 0; i < q; i++ )
    {
        int desde, hasta;
        entrada >> desde >> hasta;

        printf("%d\n", calcularFV(--desde, --hasta) );
    }
#ifdef FILEINPUT
    entrada.close();
    system("PAUSE");
#endif
    return 0;
}
