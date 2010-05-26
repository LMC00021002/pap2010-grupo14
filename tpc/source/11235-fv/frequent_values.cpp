//#define FILEINPUT
#ifdef FILEINPUT
#include <fstream>
#include <time.h>
#else
#include <iostream>
#endif

#include <vector>
using namespace std;

typedef pair< int, int > pii;
typedef unsigned int uint;

vector< vector< int > > tabla;
vector< int > enteros;
vector< int > mapRangos;
vector< pii > rangos;

inline int max( int a, int b ) { return a > b ? a : b; }
inline int log2( int a ){ int potencia = 0; while( 1 << ++potencia <= a ){} return potencia - 1; }
inline pii& rangoEnQueCae( int a ) { return rangos[ mapRangos[ a ] ]; }

void convertirARMQ( const vector< int >& arreglo )
{
    int n = arreglo.size();
    int k = 0;
    int ultimoDiferente = 0;
    enteros.push_back(1);
    mapRangos.resize( n, 0 );

    rangos.push_back( pii( 0, 0 ) );
    for( int i = 1; i < n; i++ )
    {
        if( arreglo[ ultimoDiferente ] != arreglo[i] )
        {
            rangos[ k ].second = i - 1;
            ultimoDiferente = i;
            enteros.push_back(0);
            k++;
            rangos.push_back( pii( i, i ) );
        }

        enteros[ k ]++;
        mapRangos[ i ] = k;
    }

    rangos[ k ].second = n - 1;
}

void procesarTabla()
{
    int n = enteros.size();

    tabla.resize( n, vector< int >( log2(n) + 1 ) );

    for( int i = 0; i < n; i++ )
        tabla[i][0] = enteros[ i ];

    for( int j = 1; 1 << j <= n; j++ )
        for( int i = 0; i + (1 << j) - 1 < n; i++ )
            tabla[i][j] = max( tabla[i][j - 1], tabla[i + (1 << (j - 1))][j - 1] );
}

int calcularMaximo( int i, int j )
{
    if ( i > j )
        return 1;

    i = mapRangos[ i ];
    j = mapRangos[ j ];

    int k = log2( j - i + 1 );
    return max( tabla[ i ][ k ], tabla[ j - (1<<k) + 1 ][ k ] );
}

int calcularFV( int i, int j )
{
    pii& a = rangoEnQueCae( i );
    pii& b = rangoEnQueCae( j );

    if ( a == b )
        return j - i + 1;

    int maximoParcial = calcularMaximo( a.second + 1, b.first - 1 );
    return max( maximoParcial, max( a.second - i + 1, j - b.first + 1) );
}
#ifdef FILEINPUT
void generarTest( const char* nombreArchivo )
{
    srand ( time(NULL) );

    int NMAX = 50;
    int MAXINCREMENTO = 2;
    int PROBABILIDADIGUALES = 65;

    ofstream salida( nombreArchivo, ios_base::out );
    int n = rand() % NMAX + 1;
    int q = n*(n+1)/2;

    salida << n << " " << q << endl;
    
    int num = -NMAX*MAXINCREMENTO/10;
    salida << num;

    for( int i = 1; i < n; i++ )
    {
        if( (rand() % 100) > PROBABILIDADIGUALES )
            num += rand() % MAXINCREMENTO + 1;
        
        salida << " " << num;
    }

    salida << endl;
    for( int i = 1; i <= n; i++ )
        for( int j = i; j <= n; j++ )
            salida << i << " " << j << endl;
    salida << "0";
    salida.close();
}
#endif

int main()
{
#ifdef FILEINPUT
    generarTest("test");
    ifstream entrada( "test", ios_base::in );
    ofstream salida( "testOut", ios_base::out );
#else
    istream& entrada = cin;
    ostream& salida = cout;
#endif
    int n, q;

    while( entrada >> n )
    {
        if( n == 0 )
            break;
        
        tabla.clear();
        enteros.clear();
        mapRangos.clear();
        rangos.clear();

        entrada >> q;
        vector< int > arregloEntrada( n );
        for( int i = 0; i < n; i++ )
        {
            entrada >> arregloEntrada[ i ];
        }

        convertirARMQ( arregloEntrada );
        procesarTabla();

        for( int i = 0; i < q; i++ )
        {
            int desde, hasta;
            entrada >> desde >> hasta;

            salida << calcularFV(--desde, --hasta) << endl;
        }
    }
#ifdef FILEINPUT
    entrada.close();
    salida.close();
#endif
    return 0;
}
