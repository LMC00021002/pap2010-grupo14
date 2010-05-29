#define FILEINPUT
#ifdef FILEINPUT
#include <fstream>
#else
#include <iostream>
#endif

#include <math.h>
#include <string.h>

using namespace std;

typedef pair< int, int > pii;
typedef unsigned int uint;

#define MAXHIJOSNODO 10

struct nodo {
    // prefijo
	char *s;

    // bill plan
    char *b;

    // punteros a hijos
	nodo *hijos[ MAXHIJOSNODO ];
};

nodo raiz;

int cardinal; 

inline bool number( char c )
{
    return '0' <= c && c <= '9';
}

inline int ord( char c ) {
	return c - '0';
} 

inline char ordInv( int i ) {
	return i + '0';
} 

void agregar( char *t, char *b ) {
	nodo *actual, *nuevo, *previo, *pila[ 10*11 ];
	char *p;
	int i, j, top;

	previo = &raiz;

    actual = previo->hijos[ ord(*t) ];
    while( actual && *t ) {
		for( p = actual->s; *t == *p && *t; t++,p++ );

        // Si el prefijo que quiero agregar tiene
        // como prefijo a otro que ya agregue
        if( actual->b && !*p )
            return;

        // Si tienen prefijo en comun pero ninguno de
        // los dos es prefijo del otro parto el nodo
		if( *p ) {
			nuevo = new nodo;
			nuevo->s = new char[ strlen(p) + 1 ];
			strcpy( nuevo->s, p );
			for( i = 0; i < MAXHIJOSNODO; i++ ) {
				nuevo->hijos[i] = actual->hijos[i];
				actual->hijos[i] = NULL;
			}
            nuevo->b = actual->b;
            actual->b = NULL;
			actual->hijos[ ord(*p) ] = nuevo;
			*p = '\0';
		}

		previo = actual;
        actual = previo->hijos[ ord(*t) ];
	}

    if( !*t ) {
        // extender el arbol si viene un
        // numero que es prefijo de los demas
        top = 0;
        pila[ top ] = previo;
        while( top >= 0 )
        {
            actual = pila[ top ];
            top--;
            if( !actual->b || ( actual->s && strlen(actual->s) > 1 ) )
            {
                if( actual->s && strlen(actual->s) > 1 )
                {
                    // parto el nodo
			        nuevo = new nodo;
			        nuevo->s = new char[ strlen(actual->s + 1) + 1 ];
                    strcpy( nuevo->s, actual->s + 1 );
			        for( i = 0; i < MAXHIJOSNODO; i++ ) {
				        nuevo->hijos[i] = actual->hijos[i];
				        actual->hijos[i] = NULL;
			        }
                    nuevo->b = actual->b;
                    actual->b = NULL;
                    actual->s[1] = '\0';
			        actual->hijos[ ord( *nuevo->s ) ] = nuevo;
                }

                for( i = MAXHIJOSNODO - 1; i >= 0; i-- )
                {
                    if( actual->hijos[i] )
                        pila[ ++top ] = actual->hijos[i];
                    else
                    {
			            nuevo = new nodo;
			            nuevo->s = new char[ 2 ];
                        sprintf( nuevo->s, "%d", i );
                        nuevo->b = new char[ strlen(b) + 1 ];
                        strcpy( nuevo->b, b );
			            for( j = 0; j < MAXHIJOSNODO; j++ )
				            nuevo->hijos[j] = NULL;
                        actual->hijos[i] = nuevo;
                        if( strcmp( b, "invalid" ) != 0 )
                            cardinal++;
                    }
                }
            }
        }
    } else {
        if( !actual ) {
            nuevo = new nodo;
	        nuevo->s = new char[ strlen(t) + 1 ];
	        nuevo->b = new char[ strlen(b) + 1 ];
	        strcpy( nuevo->s, t );
            strcpy( nuevo->b, b );
	        for( i = 0; i < MAXHIJOSNODO; i++ )
		        nuevo->hijos[i] = NULL;
            previo->hijos[ ord(*t) ] = nuevo;
            if( strcmp( b, "invalid" ) != 0 )
                cardinal++;
        }
    }
} 

void destruir( nodo *padre ) {
	int i;

	for( i = 0; i < MAXHIJOSNODO; i++ ) {
		if( padre->hijos[i] ) {
			destruir( padre->hijos[i] );
			padre->hijos[i] = NULL;
		}
	}
	if( padre != &raiz ) {
		delete[] padre->s;
		delete padre;
	}
}

void generarTest( const char* nombreArchivo ) {
}

inline int digitos( int n )
{
    int d;
    
    if( n == 0 )
        return 1;

    for( d = 0; n > 0; d++ )
        n /= 10;
    return d;
}

inline int primerosNDigitos( int n, int numero )
{
    return numero % ((int) pow( 10.0f,n ) );
}

void print( nodo* t, char* p )
{
	int i;
    char conc[11];

    if( t->b != "invalid" )
    {
        if( t->b )
            printf( "%s%s %s\n", p, t->s, t->b );
        else
        {
            for( i = 0; i < MAXHIJOSNODO; i++ )
            {
                if( t->hijos[i] && ( !t->hijos[i]->b || strcmp( t->hijos[i]->b, "invalid" ) != 0 ) )
                {
                    if( p )
                    {
                        strcpy( conc, p );
                        strcat( conc, t->s );
                        print( t->hijos[i], conc );
                    }
                    else
                    {
                        print( t->hijos[i], p );
                    }
                }
            }
        }
    }
}

int main() {
#ifdef FILEINPUT
    generarTest("test");
    ifstream entrada( "test", ios_base::in );
#endif

    int n, pi, pj, dif, i;
    char p[11], bp[1024];

#ifdef FILEINPUT
    while( entrada >> n )
    {
#else
    while( scanf( "%d", n ) )
    {
#endif
        cardinal = 0;
        while( n-- > 0 )
        {
#ifdef FILEINPUT
            entrada.ignore(); // ignoramos el '\n'
            entrada >> pi;
            entrada.ignore();
            entrada.ignore();
            entrada >> pj;
            entrada >> bp;
#else
            getchar(); // ignoramos el '\n'
            scanf( "%d - %d %s", pi, pj, bp );
#endif

            pj = pi - primerosNDigitos( digitos(pj), pi ) + pj;
            dif = pj - pi + 1;
            i = 0;
            while( dif > 0 ) {
                while( !( pi % 10 > 0 ) && dif >= pow(10.0f, i) )
                {
                    i++;
                    pi /= 10;
                }

                while( dif < pow(10.0f, i) )
                {
                    i--;
                    pi *= 10;
                }

                sprintf( p, "%d", pi );
                agregar( p, bp );
                dif -= pow(10.0f, i);
                pi++;
            }
        }
        printf( "%d\n", cardinal );

        char vacio[1] = "";
        for( i = 0; i < MAXHIJOSNODO; i++ )
        {
            if( raiz.hijos[i] && ( !raiz.hijos[i]->b || strcmp(raiz.hijos[i]->b, "invalid") != 0 ) )
                print( raiz.hijos[i], vacio );
        }
        printf( "\n" );
        destruir(&raiz);
    }

#ifdef FILEINPUT
    entrada.close();
#endif
    return 0;
}
