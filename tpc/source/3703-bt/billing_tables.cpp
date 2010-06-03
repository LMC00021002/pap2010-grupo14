#define _CRT_SECURE_NO_WARNINGS
//#define FILEINPUT
#ifdef FILEINPUT
#include <time.h>
#include <fstream>
#endif

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define forn(i, n) for(int i = 0; i < (int)(n); ++i)

using namespace std;

#define MAXHIJOSNODO 10
#define MAXLONGPREFIJO 11

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

inline long long atoll( char* n )
{
    long long number = 0;
    while( *n != '\0' )
        number = number * 10 + *n++ - '0';

    return number;
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
		nodo *pila[ MAXHIJOSNODO*MAXLONGPREFIJO ];
		pila[ top ] = previo;
		while( top >= 0 )
		{
			actual = pila[ top ];
			top--;
			if( !actual->b || ( actual->s && strlen(actual->s) > 1 ) ) {
				if( actual->s && strlen(actual->s) > 1 ) {
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

				for( int i = MAXHIJOSNODO - 1; i >= 0; i-- ) {
					if( actual->hijos[i] )
						pila[ ++top ] = actual->hijos[i];
					else {
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

void printAux( nodo* t, char* p, ostream& salida )
{
	int i;
	char conc[MAXLONGPREFIJO + 1];

	if(t && ( !t->b || strcmp( t->b, "invalid" ) != 0 ) ) {
		if( t->b )
    	    salida << p << t->s << " " << t->b << endl;
		else {
			for( i = 0; i < MAXHIJOSNODO; i++ ) {
				if( p ) {
					strcpy( conc, p );
					strcat( conc, t->s );
					printAux( t->hijos[i], conc, salida );
				}
				else {
					printAux( t->hijos[i], p, salida );
				}
			}
		}
	}
}

void print( nodo* t, ostream& salida )
{
	for( int i = 0; i < MAXHIJOSNODO; i++ )
		printAux( raiz.hijos[i], "", salida );
}

#ifdef FILEINPUT
void crearPrefijos( char* pi, char* pj )
{
    for( int i = 0; i < MAXLONGPREFIJO; i++ )
        pi[i] = rand() % 10 + '0';
    pi[11] = '\0';

    int offset = rand() % MAXLONGPREFIJO;
    for( int i = offset; i < MAXLONGPREFIJO; i++ ) {
        pj[i - offset] = pi[i] + rand() % 3;
        if( pj[i - offset] > '9' )
            pj[i - offset] = '9';
    }
    pj[MAXLONGPREFIJO - offset] = '\0';
}

void generarTest( char* nombreArchivo )
{
    ofstream salida( nombreArchivo, ios_base::out );
    srand(time(NULL));
    int cantTests = rand() % 5 + 1;
    while( cantTests-- > 0 )
    {
        int n = rand() % 100;

        salida << n << endl;

        while( n-- > 0 ) {
            char pi[12], pj[12];
            crearPrefijos( pi, pj );
            salida << pi << " - " << pj << " ";
            (rand() % 100 > 30) ? salida << (char)(rand() % 26 + 'a') : salida << "invalid";
            salida << endl;
        }
        salida << endl;
    }
    salida.close();
}
#endif

int main() {
#ifdef FILEINPUT
    generarTest("testGenerado");
	ifstream entrada( "testGenerado", ios_base::in );
	ofstream salida( "testGeneradoOut", ios_base::out );
#else
	istream& entrada = cin;
	ostream& salida = cout;
#endif

	int n, i, numTest=0;
	long long int pi, pj, dif;
	char p[MAXLONGPREFIJO + 1], bp[21];

	while( entrada >> n ) {
		if (numTest++ > 0)
			salida << endl;

		cardinal = 0;
		while( n-- > 0 ){
			entrada.ignore(); // ignoramos el '\n'
			entrada >> p;

			pi = atoll(p);
			int di = strlen(p);

			entrada.ignore(); /* Guion y espacio */
			entrada.ignore();
			entrada >> pj;
			entrada >> bp;

			pj = pi - primerosNDigitos( digitos(pj), pi ) + pj;
			dif = pj - pi + 1;
			i = 0;
			while( dif > 0 ) {
				while( !( pi % 10 > 0 ) && dif >= pow10(i) ) {
					i++;
					pi /= 10;
				}

				while( dif < pow10(i) ) {
					i--;
					pi *= 10;
				}

				int cerosNuevo = di - digitos(pi) - i;
				sprintf( p + cerosNuevo, "%lld", pi );
				agregar( p, bp );
				dif -= pow10(i);
				pi++;
			}
		}
		salida << cardinal << endl;

		/* Imprime árbol por stdout */
		print( &raiz, salida );

		destruir( &raiz );
	}

#ifdef FILEINPUT
	entrada.close();
	salida.close();
#endif
	return 0;
}
