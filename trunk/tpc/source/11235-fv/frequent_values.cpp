#include <iostream>
#include <vector>
using namespace std;

typedef pair< int, int > pii;
vector< vector< int > > tabla;
vector< int > enteros;
vector< int > mapIndices;
vector< pii > rangos;

inline int max( int a, int b ) { return a > b ? a : b; }
inline int log2( int a ){ int potencia = 0; while( 1 << ++potencia <= a ){} return potencia - 1; }
inline pii& rangoEnQueCae( int a ) { return rangos[ mapIndices[ a ] ]; }

void convertirARMQ( const vector< int >& arreglo )
{
    int n = arreglo.size();
    int k = 0;
    int ultimoDiferente = 0;
    enteros.push_back(1);
    mapIndices.resize( n, 0 );

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
        mapIndices[ i ] = k;
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

    i = mapIndices[ i ];
    j = mapIndices[ j ];

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

int main()
{
    int n, q;

    while( scanf("%d", &n) == 1 )
    {
        if( n == 0 )
            break;
        
        tabla.clear();
        enteros.clear();
        mapIndices.clear();
        rangos.clear();

        scanf("%d", &q);
        vector< int > arregloEntrada( n );
        for( int i = 0; i < n; i++ )
            scanf("%d", &arregloEntrada[ i ]);

        convertirARMQ( arregloEntrada );
        procesarTabla();

        for( int i = 0; i < q; i++ )
        {
            int desde, hasta;
            scanf("%d %d", &desde, &hasta);

            printf("%d\n", calcularFV(--desde, --hasta));
        }
    }

    return 0;
}
