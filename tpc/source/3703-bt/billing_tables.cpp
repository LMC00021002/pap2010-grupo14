#define _CRT_SECURE_NO_WARNINGS
//#define FILEINPUT
#ifdef FILEINPUT
#include <time.h>
#include <fstream>
#endif

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#define forn(i, n) for(int i = 0; i < (int)(n); ++i)

using namespace std;

#define MAXHIJOSNODO 10
#define MAXLONGPREFIJO 11
#define CANTCARACTERES 26

inline int ord( char c ) {
    if( 'a' < c && c < 'z' )
	    return c - 'a';
    else
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

class Nodo
{
public:
    Nodo( char *billPlan = NULL, Nodo *padre = NULL, char *prefijo = NULL )
    {
        if( billPlan )
        {
            mBP = new char[ strlen(billPlan) + 1 ];
		    strcpy( mBP, billPlan );
        }
        else
            mBP = NULL;

        mBPQueDefine = mBP;

        if( prefijo )
        {
            mPrefijo = new char[ strlen(prefijo) + 1 ];
		    strcpy( mPrefijo, prefijo );
        }
        else
            mPrefijo = NULL;

        if( padre )
            mPadre = padre;

        mEsHoja = true;
        mCantHijosHojaAtomica = 0;
    }

    void destruir() {
        for( int i = 0; i < MAXHIJOSNODO; i++ ) {
		    if( mHijos[i] ) {
			    mHijos[i]->destruir();
                mHijos[i] = NULL;
		    }
	    }
        if( mPrefijo )
		    delete [] mPrefijo;

        if( mBP )
            delete [] mBP;

        mBP = NULL;
        mPrefijo = NULL;
        mBPQueDefine = NULL;
        mPadre = NULL;
        mCantHijosHojaAtomica = 0;
        mEsHoja = true;
    }

    ~Nodo(){ destruir(); }

    // prefijo
	char *mPrefijo;

	// bill plan
	char *mBP;

	// punteros a hijos
	Nodo *mHijos[ MAXHIJOSNODO ];

    // puntero al padre
    Nodo *mPadre;

    // si el nodo es hoja, es decir, si tiene todos hijos NULL
    bool mEsHoja;

    /*
     * auxiliar al momento de comprimir
     * guarda la cantidad de hijos que son
     * hoja y que son atomicos, es decir,
     * que la longitud del prefijo es 1
     */
    int mCantHijosHojaAtomica;

    /*
     * auxiliar al momento de comprimir
     * guarda el nombre del Bill Plan
     * que esta definido pasando por este Nodo
     * En caso de existir dos o más BP
     * diferentes que se definen pasando
     * por este Nodo, el valor de
     * la variable es NULL
     */
    char *mBPQueDefine;
};

Nodo raiz;

void comprimir( Nodo *nodo )
{
    while( nodo != &raiz && nodo->mBPQueDefine )
    {
        nodo->mBP = new char[ strlen(nodo->mBPQueDefine) + 1 ];
        strcpy(nodo->mBP, nodo->mBPQueDefine );
        nodo->mBPQueDefine = nodo->mBP;

        forn( i, MAXHIJOSNODO )
        {
            if( nodo->mHijos[i] )
            {
                nodo->mHijos[i]->destruir();
                nodo->mHijos[i] = NULL;
            }
        }

        nodo->mCantHijosHojaAtomica = 0;
        nodo->mEsHoja = true;

        nodo->mPadre->mCantHijosHojaAtomica++;
        nodo = nodo->mPadre;
    }
}

void completar( Nodo* inicial, char* billPlan )
{
	int top = 0;
	Nodo *pila[ MAXHIJOSNODO*MAXLONGPREFIJO ];
	pila[ top ] = inicial;
	while( top >= 0 )
	{
        Nodo *actual = pila[ top-- ];
        if( actual->mBPQueDefine && strcmp(actual->mBPQueDefine, billPlan) == 0 && !actual->mEsHoja )
            comprimir( actual );
        else
        {
            int longPref = 0;
            if( actual->mPrefijo )
                longPref = strlen(actual->mPrefijo);
		    if( !actual->mBP || longPref > 1 ) {
			    if( longPref > 1 ) {
				    // parto el Nodo
				    Nodo *nuevo = new Nodo( NULL, actual, actual->mPrefijo + 1 );
                    nuevo->mCantHijosHojaAtomica = actual->mCantHijosHojaAtomica;
                    nuevo->mEsHoja = actual->mEsHoja;
                    nuevo->mBP = actual->mBP;
				    for( int i = 0; i < MAXHIJOSNODO; i++ ) {
					    nuevo->mHijos[i] = actual->mHijos[i];
					    actual->mHijos[i] = NULL;
				    }

                    actual->mBP = NULL;
                    actual->mEsHoja = false;
				    actual->mPrefijo[1] = '\0';
				    actual->mHijos[ ord( *nuevo->mPrefijo ) ] = nuevo;
                    actual->mCantHijosHojaAtomica = actual->mEsHoja && nuevo->mPrefijo[1] == 0;
			    }

			    for( int i = 0; i < MAXHIJOSNODO; i++ ) {
				    if( actual->mHijos[i] )
					    pila[ ++top ] = actual->mHijos[i];
				    else
                    {
					    Nodo *nuevo = new Nodo( billPlan, actual );
					    nuevo->mPrefijo = new char[ 2 ];
					    sprintf( nuevo->mPrefijo, "%d", i );
					    for( int j = 0; j < MAXHIJOSNODO; j++ )
						    nuevo->mHijos[j] = NULL;

                        actual->mHijos[i] = nuevo;
                        actual->mEsHoja = false;
                        actual->mCantHijosHojaAtomica++;
				    }
			    }

                if( actual->mBPQueDefine && actual->mBPQueDefine != billPlan )
                    actual->mBPQueDefine = NULL;
		    }
        }
	}
}

void agregar( char *key, char *billPlan )
{
    Nodo *previo = &raiz;
	char *p;
    Nodo *actual = previo->mHijos[ ord(*key) ];

    while( actual && *key )
    {
		for( p = actual->mPrefijo; *key == *p && *key; key++,p++ );

		// Si el prefijo que quiero agregar tiene
		// como prefijo a otro que ya agregue
		if( actual->mBP && !*p )
			return;

		// Si tienen prefijo en comun pero ninguno de
		// los dos es prefijo del otro parto el Nodo
		if( *p )
        {
			Nodo *nuevo = new Nodo( NULL, actual, p );
            nuevo->mCantHijosHojaAtomica = actual->mCantHijosHojaAtomica;
            nuevo->mEsHoja = actual->mEsHoja;
            nuevo->mBP = actual->mBP;
			for( int i = 0; i < MAXHIJOSNODO; i++ )
            {
				nuevo->mHijos[i] = actual->mHijos[i];
				actual->mHijos[i] = NULL;
			}

			actual->mBP = NULL;
            actual->mCantHijosHojaAtomica = actual->mEsHoja && nuevo->mPrefijo[1] == 0;
            actual->mEsHoja = false;
			actual->mHijos[ ord(*p) ] = nuevo;
			*p = '\0';
		}

        if( actual->mBPQueDefine && strcmp(actual->mBPQueDefine, billPlan) != 0 )
            actual->mBPQueDefine = NULL;

        previo = actual;
		if( *key )
			actual = previo->mHijos[ ord(*key) ];
	}

	/*
	 * Si viene un numero que es prefijo de los demas
	 * tengo que completar el arbol en todos los lugares
	 * donde se pueden agregar hojas
	 */
    if( !*key ) {
        completar( previo, billPlan );
	} else {
		if( !actual ) {
            if( previo->mBPQueDefine && strcmp(previo->mBPQueDefine, billPlan) == 0 && previo->mCantHijosHojaAtomica == 9 && key[1] == 0 )
                comprimir( previo );
            else
            {
			    Nodo *nuevo = new Nodo( billPlan, previo, key );
			    for( int i = 0; i < MAXHIJOSNODO; i++ )
				    nuevo->mHijos[i] = NULL;

                previo->mEsHoja = false;
                previo->mCantHijosHojaAtomica += key[1] == 0;
                previo->mHijos[ ord(*key) ] = nuevo;
            }
		}
	}
}

long long int card;
void cardinal( Nodo* t )
{
	if(t && ( !t->mBP || strcmp(t->mBP, "invalid") != 0 ) ){
		if( t->mBP )
    	    card++;
		else {
			forn( i, MAXHIJOSNODO )
			    cardinal( t->mHijos[i] );
		}
	}
}

void printAux( Nodo* t, char* p, ostream& salida )
{
	int i;
	char conc[MAXLONGPREFIJO + 1];

	if(t && ( !t->mBP || strcmp(t->mBP, "invalid") != 0 ) ) {
		if( t->mBP )
    	    salida << p << t->mPrefijo << " " << t->mBP << endl;
		else
        {
			for( i = 0; i < MAXHIJOSNODO; i++ )
            {
                if( t->mHijos[i] )
                {
			        strcpy( conc, p );
			        strcat( conc, t->mPrefijo );
				    printAux( t->mHijos[i], conc, salida );
                }
			}
		}
	}
}

inline void print( Nodo* t, ostream& salida ) {
    printAux( &raiz, "", salida );
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
/*

int longPi;
int longOriginalPi;
int longPj;

inline void simplificar( char* pi, char* pj )
{
    for( int i = longPi - 1; i > 0 && pi[i] == '0'; i-- )
    {
        bool sePuedeSacar = false;
        for( int j = 0; !sePuedeSacar && j < longPi; j++ )
            sePuedeSacar = sePuedeSacar || pi[j] <= pj[j];

        if( sePuedeSacar )
        {
            pi[i] = '\0';
            longPi--;
        }
    }
}

void siguiente( char* pi, char* pj )
{
    // sumo uno
    pi[longPi - 1]++;
    int i;
    for( i = longPi - 1; pi[i] > '9' && i > 0; i-- )
    {
        pi[i] = '0';
        pi[i - 1]++;
    }

    if( i <= longOriginalPi - longPj - 1 || pi[longPi - longPj] > pj[0] )
    {
        pi[0] = '\0';
        return;
    }

    bool menor = pi[longOriginalPi - longPj] < pj[0];
    int j;
    for( j = longOriginalPi - longPj + 1; !menor && j < longPi; j++ )
        menor = pi[j] < pj[j - longOriginalPi + longPj];

    if( !menor )
    {
        if( pi[longPi-1] > pj[longPj-1] )
            pi[0] = '\0';
        else
        {
            while( pi[longPi-1] >= pj[longPj-1] && longPi < longPj )
            {
                pi[longPi] = '0';
                pi[longPi + 1] = '\0';
                longPi++;
            }
        }
    }
    else
    {
        for( i = longPi - 1; i >= j && pi[i] == '0'; i-- )
        {
            pi[i] = '\0';
            longPi--;
        }
    }
}
*/

void llitoChar( char* c, long long int p, int digitosPi )
{
    c[digitosPi] = 0;
    int i = digitosPi - 1;
    while( p > 0 )
    {
        c[i] = p % 10 + '0';
        p /= 10;
        i--;
    }
}

int main() {
#ifdef FILEINPUT
//    generarTest("testGenerado");
	ifstream entrada( "test", ios_base::in );
	ofstream salida( "testOut", ios_base::out );
#else
	istream& entrada = cin;
	ostream& salida = cout;
#endif

	int n, numTest = 0;
    long long int pi, pj;
	char p[MAXLONGPREFIJO + 1], bp[21];

	while( entrada >> n )
    {
        raiz.mPrefijo = new char[1];
        *raiz.mPrefijo = '\0';
        card = 0;

        if ( numTest++ > 0 )
            salida << endl;

		while( n-- > 0 )
        {
			entrada.ignore(); // ignoramos el '\n'
			entrada >> p;
            entrada.ignore(); /* Guion y espacio */
			entrada.ignore();
			entrada >> pj;
			entrada >> bp;
/*

            // completo el prefijo pj
            longOriginalPi = strlen(pi);
            longPi = longOriginalPi;
            longPj = strlen(pj);

            simplificar( pi, pj );
            while( *pi )
            {
                agregar( pi, bp );
                siguiente( pi, pj );
            }
*/
            int di = strlen(p);
            pi = atoll(p);

            pj = pi - primerosNDigitos( digitos(pj), pi ) + pj;
            long long int dif = pj - pi + 1;
		    int i = 0;
		    while( dif > 0 )
            {
			    while( di > 1 && !( pi % 10 > 0 ) && dif > pow10(i) )
                {
				    i++;
				    pi /= 10;
			    }

			    while( dif < pow10(i) )
                {
				    i--;
				    pi *= 10;
			    }

                int dpi = digitos(pi);
			    int cerosNuevo = di - dpi - i;
		        llitoChar( p + cerosNuevo, pi, dpi );
			    agregar( p, bp );
			    dif -= pow10(i);
			    pi++;
		    }

		}

        cardinal( &raiz );
		salida << card << endl;

		/* Imprime árbol por stdout */
		print( &raiz, salida );

		raiz.destruir();
    }

#ifdef FILEINPUT
	entrada.close();
	salida.close();
#endif
	return 0;
}
