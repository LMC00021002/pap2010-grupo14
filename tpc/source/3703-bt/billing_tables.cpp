//#define FILEINPUT
#ifdef FILEINPUT
#include <fstream>
#else
#include <iostream>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

using namespace std;

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

long long int cardinal; 

inline int ord( char c ) {
	return c - '0';
}

inline long long int pow10( int i )
{
    long long int res = 1;
    while( i-- > 0 )
        res *= 10;

    return res;
}

void agregar( char *t, char *b ) {
	nodo *actual, *previo;
	char *p;

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
			nodo *nuevo = new nodo;
			nuevo->s = new char[ strlen(p) + 1 ];
			strcpy( nuevo->s, p );
			for( int i = 0; i < MAXHIJOSNODO; i++ ) {
				nuevo->hijos[i] = actual->hijos[i];
				actual->hijos[i] = NULL;
			}
            nuevo->b = actual->b;
            actual->b = NULL;
			actual->hijos[ ord(*p) ] = nuevo;
			*p = '\0';
		}

		previo = actual;
        if( *t )
            actual = previo->hijos[ ord(*t) ];
	}

    if( !*t ) {
        /*
         * Si viene un numero que es prefijo de los demas
         * tengo que extender el arbol en todos los lugares
         * donde se pueden agregar hojas
         */
        int top = 0;
        nodo *pila[ 10*11 ];
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
			        nodo *nuevo = new nodo;
			        nuevo->s = new char[ strlen(actual->s + 1) + 1 ];
                    strcpy( nuevo->s, actual->s + 1 );
			        for( int i = 0; i < MAXHIJOSNODO; i++ ) {
				        nuevo->hijos[i] = actual->hijos[i];
				        actual->hijos[i] = NULL;
			        }
                    nuevo->b = actual->b;
                    actual->b = NULL;
                    actual->s[1] = '\0';
			        actual->hijos[ ord( *nuevo->s ) ] = nuevo;
                }

                for( int i = MAXHIJOSNODO - 1; i >= 0; i-- )
                {
                    if( actual->hijos[i] )
                        pila[ ++top ] = actual->hijos[i];
                    else
                    {
			            nodo *nuevo = new nodo;
			            nuevo->s = new char[ 2 ];
                        sprintf( nuevo->s, "%d", i );
                        nuevo->b = new char[ strlen(b) + 1 ];
                        strcpy( nuevo->b, b );
			            for( int j = 0; j < MAXHIJOSNODO; j++ )
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
            nodo *nuevo = new nodo;
	        nuevo->s = new char[ strlen(t) + 1 ];
	        nuevo->b = new char[ strlen(b) + 1 ];
	        strcpy( nuevo->s, t );
            strcpy( nuevo->b, b );
	        for( int i = 0; i < MAXHIJOSNODO; i++ )
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
        delete[] padre->b;
		delete padre;
	}
}

void generarTest( const char* nombreArchivo ) {
}

inline int digitos( long long int n )
{
    int d;
    
    if( n == 0 )
        return 1;

    for( d = 0; n > 0; d++ )
        n /= 10;
    return d;
}

inline long long int primerosNDigitos( int n, long long int numero )
{
    return numero % ( pow10(n) );
}

void printAux( nodo* t, char* p )
{
	int i;
    char conc[11];

    if(t && ( !t->b || strcmp( t->b, "invalid" ) != 0 ) ) {
        if( t->b )
            cout << p << t->s << " " << t->b << endl;
        else {
            for( i = 0; i < MAXHIJOSNODO; i++ ) {
                if( p ) {
                    strcpy( conc, p );
                    strcat( conc, t->s );
                    printAux( t->hijos[i], conc );
                }
                else {
                    printAux( t->hijos[i], p );
                }
            }
        }
    }
}

void print( nodo* t )
{
    int i;
    for( i = 0; i < MAXHIJOSNODO; i++ )
        printAux( raiz.hijos[i], "" );
}

int main() {
#ifdef FILEINPUT
    generarTest("test");
    ifstream entrada( "test", ios_base::in );
#else
    istream& entrada = cin;
#endif

    int n, i;
    long long int pi, pj, dif;
    char p[11], bp[1024];

    while( entrada >> n )
    {
        cardinal = 0;
        while( n-- > 0 )
        {
            entrada.ignore(); // ignoramos el '\n'
            entrada >> pi;
            entrada.ignore();
            entrada.ignore();
            entrada >> pj;
            entrada >> bp;

            pj = pi - primerosNDigitos( digitos(pj), pi ) + pj;
            dif = pj - pi + 1;
            i = 0;
            while( dif > 0 ) {
                while( !( pi % 10 > 0 ) && dif >= pow10(i) )
                {
                    i++;
                    pi /= 10;
                }

                while( dif < pow10(i) )
                {
                    i--;
                    pi *= 10;
                }

                sprintf( p, "%lld", pi );
                agregar( p, bp );
                dif -= pow(10.0f, i);
                pi++;
            }
        }
        cout << cardinal << endl;

        /* Imprimo el arbol por el stdout */
        print( &raiz );
        cout << endl;

        destruir( &raiz );
    }

#ifdef FILEINPUT
    entrada.close();
#endif
    return 0;
}
