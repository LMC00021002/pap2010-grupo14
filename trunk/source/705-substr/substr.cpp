#include <algorithm>
#include <stdio.h>
#include <string.h>

#define forn( i, n ) for ( int i = 0; i < (n); i++ )

#define MAXN 50000

int N, delta;
char cadena[MAXN + 1];
int suffixArray[MAXN + 1];
int rank[MAXN + 1];
int tmp[MAXN + 1];
int LCP[MAXN + 1];

bool comp( const int& i, const int& j ) {
	if ( rank[i] != rank[j] )
		return rank[i] < rank[j];
	if ( i + delta < N && j + delta < N )
		return rank[i + delta] < rank[j + delta];
	return i > j;
}

long long sumatoria( const int arreglo[], int size )
{
    long long resultado = 0;
    forn( i, size )
        resultado += arreglo[i];
    return resultado;
}

int main() {
    int T;
    scanf( "%d\n", &T );
    while( T-- > 0 )
    {
	    scanf( "%s", &cadena );
	    N = strlen( cadena );

	    forn( i, N )
        {
		    suffixArray[i] = i;
		    rank[i] = cadena[i];
	    }

	    for ( delta = 1; delta == 1 || tmp[N - 1] != N - 1; delta *= 2 )
        {
            std::sort( suffixArray, suffixArray + N, comp );
		    forn( i, N - 1 )
                tmp[i + 1] = tmp[i] + comp( suffixArray[i], suffixArray[i + 1] );
		    forn( i, N )
                rank[ suffixArray[i] ] = tmp[i];
	    }

	    for ( int i = 0, k = 0; i < N; i++ ) {
		    if ( rank[i] != N - 1 )
            {
		        for ( int j = suffixArray[ rank[i] + 1 ]; cadena[i + k] == cadena[j + k]; k++ );
		            LCP[ rank[i] ] = k;
		        if ( k > 0 )
                    k--;
            }
	    }

        printf( "%lld\n", ((long long) N)*((long long) N+1)/2 - sumatoria(LCP, N) );
    }
	return 0;
}
