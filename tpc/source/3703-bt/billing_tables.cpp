//#define _CRT_SECURE_NO_WARNINGS
//#define FILEINPUT
//#define INVARIANTE
#ifdef FILEINPUT
#include <time.h>
#include <fstream>
#endif

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <set>
#define forn(i, n) for(int i = 0; i < (int)(n); ++i)

using namespace std;

#define MAXHIJOSNODO 10
#define MAXLONGPREFIJO 11

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
    Nodo( char keyChar = '$', Nodo *padre = NULL ) :
      mBP( NULL ), mKeyChar( keyChar ), mPadre( padre ) {
	    forn( i, MAXHIJOSNODO )
		    mHijos[i] = NULL;
    }

    void destruir() {
        for( int i = 0; i < MAXHIJOSNODO; i++ ) {
		    if( mHijos[i] ) {
			    mHijos[i]->destruir();
                delete mHijos[i];
                mHijos[i] = NULL;
		    }
	    }

        mBP = NULL;
        mPadre = NULL;
    }

    ~Nodo(){ destruir(); }

    /*
     * caracter que forma parte
     * de las claves que vamos
     * agregando al arbol
     */
	char mKeyChar;

	// bill plan
	const string *mBP;

    // punteros a hijos
	Nodo *mHijos[ MAXHIJOSNODO ];

    // puntero al padre
    Nodo *mPadre;
};
/*
template <class T>
class cmp {
public:
    cmp(){};

    bool operator() (const T& t1, const T& t2) const {
        return t1.compare(t2) < 0;
    }
};*/
// -------------------- VARIABLES GLOBALES -------------------- //

Nodo raiz;
set<string/*, cmp<string>*/> billPlans;
const string *invalido;

// ------------------------- METODOS -------------------------- //
#ifdef INVARIANTE
void invariante( Nodo* nodo )
{
    if( !nodo )
        return;

    if( nodo->mKeyChar < 'a' || 'z' < nodo->mKeyChar )
        throw "Hay un keyChar invalido";

    bool esHoja = nodo->mBP != NULL;
    if( esHoja && !nodo->mBP )
        throw "esHoja y no tiene mBP";

    if( esHoja && !nodo->mBPQueDefine )
        throw "esHoja y no tiene mBPQueDefine";

    if( nodo->mBPQueDefine )
    {
        bool defineMB = !(nodo->mBP) || ( nodo->mBP && nodo->mBPQueDefine );
        if( nodo->mBP )
            defineMB = defineMB && nodo->mBP == nodo->mBPQueDefine;

        for( int i = 0; i < MAXHIJOSNODO && defineMB; i++ )
        {
            if( nodo->mHijos[i] )
            {
                if( !nodo->mHijos[i]->mBPQueDefine )
                    throw "El mBPQueDefine esta andando mal, su hijo tiene mBPQueDefine NULL";

                defineMB = defineMB && nodo->mHijos[i]->mBPQueDefine == nodo->mBPQueDefine;
                if( nodo->mHijos[i]->mBP )
                    defineMB = defineMB && nodo->mHijos[i]->mBP == nodo->mBPQueDefine;
            }
        }

        if( !defineMB )
            throw "El mBPQueDefine esta andando mal";
    }

    bool padreHijo = false;
    forn( i, MAXHIJOSNODO )
        padreHijo = padreHijo || nodo == nodo->mPadre->mHijos[i];
    if( !padreHijo )
        throw "La relacion padre hijo esta andando mal";

    forn( i, MAXHIJOSNODO )
        invariante( nodo->mHijos[i] );
}
#endif

inline bool esHoja( Nodo *nodo )
{
    if( !nodo )
        return false;

    bool esHoja = true;
    for( int i = 0; i < MAXHIJOSNODO && esHoja; i++ )
        esHoja = esHoja && nodo->mHijos[i] == NULL;

    return esHoja;
}

void completar( Nodo* inicial, const string* billPlan )
{
    Nodo *actual = inicial;
    int top = 0;
	Nodo *pila[ MAXHIJOSNODO*MAXLONGPREFIJO ];
	pila[ top ] = actual;
	while( top >= 0 )
	{
        actual = pila[ top-- ];
	    if( !actual->mBP )
        {
		    forn( i, MAXHIJOSNODO )
            {
			    if( actual->mHijos[i] )
				    pila[ ++top ] = actual->mHijos[i];
			    else
                {
				    Nodo *nuevo = new Nodo( i + '0', actual );
                    nuevo->mBP = billPlan;
                    actual->mHijos[i] = nuevo;
			    }
		    }
	    }
	}
}

void agregar( char *key, const string *billPlan )
{
    if( !*key )
    {
        completar( &raiz, billPlan );
        return;
    }

    Nodo *previo = &raiz;
    Nodo *actual = previo->mHijos[ ord(*key) ];

    // Avanzo la key todo lo que puedo en el trie,
    // posiblemente hasta una hoja
    while( actual && *key && !actual->mBP )
    {
        if( *(++key) )
        {
            previo = actual;
		    actual = actual->mHijos[ ord(*key) ];
        }
	}

    /*
     * Si la clave que estoy agregando contiene
     * otra clave ya agregada anteriormente,
     * no tengo que agregar nada
     */
    if( actual && actual->mBP )
        return;

	/*
	 * Si la clave que estoy agregando es contenida
	 * por otra clave ya agregada anteriormente,
	 * tengo que completar el arbol hacia abajo
     * definiendo el bill plan de parametro
	 */
    if( actual && !*key )
    {
        completar( actual, billPlan );
        return;
    }

    /*
     * Si no pasa ninguna de los casos anteriores
     * quiere decir que tengo que simplemente
     * definirle una nueva rama/hoja al arbol
     */
    if( !actual )
    {
        while( *key )
        {
	        Nodo *nuevo = new Nodo( *key, previo );
	        for( int i = 0; i < MAXHIJOSNODO; i++ )
		        nuevo->mHijos[i] = NULL;

            previo->mHijos[ ord(*key) ] = nuevo;
            previo = nuevo;
            key++;
        }

        previo->mBP = billPlan;
	}
}

long long int card;
void cardinal( Nodo* t )
{
	if(t && ( !t->mBP || t->mBP != invalido ) )
    {
		if( t->mBP )
    	    card++;
		else {
			forn( i, MAXHIJOSNODO )
			    cardinal( t->mHijos[i] );
		}
	}
}

void printAux( Nodo* t, char* p, int pLen, ostream& salida )
{
	if(t && ( !t->mBP || t->mBP != invalido ) )
    {
		if( t->mBP )
    	    salida << p << " " << t->mBP->c_str() << endl;
		else
        {
			forn( i, MAXHIJOSNODO )
            {
                if( t->mHijos[i] )
                {
                	char conc[MAXLONGPREFIJO + 1];
			        strcpy( conc, p );
                    conc[ pLen ] = i + '0';
                    conc[ pLen + 1 ] = '\0';
				    printAux( t->mHijos[i], conc, pLen + 1, salida );
                }
			}
		}
	}
}

inline void print( Nodo* t, ostream& salida ) {
    printAux( &raiz, "", 0, salida );
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
    srand( time(NULL) );
    int cantTests = 20;
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

void comprimir( Nodo *nodo )
{
    if( nodo && !nodo->mBP )
    {
        const string *hojasMismoBP = NULL;

        // las hojas no se comprimen
        if( nodo->mHijos[0] )
        {
            if( !nodo->mHijos[0]->mBP )
               comprimir( nodo->mHijos[0] );
            
            hojasMismoBP = nodo->mHijos[0]->mBP;
        }

        for( int i = 1; i < MAXHIJOSNODO; i++ )
        {
            // las hojas no se comprimen
            if( nodo->mHijos[i] && !nodo->mHijos[i]->mBP )
                comprimir( nodo->mHijos[i] );

            if( !nodo->mHijos[i] || nodo->mHijos[i]->mBP != hojasMismoBP )
                hojasMismoBP = NULL;
        }

        if( !nodo->mBP && hojasMismoBP )
        {
            forn( i, MAXHIJOSNODO )
            {
                if( nodo->mHijos[i] )
                {
                    delete nodo->mHijos[i];
                    nodo->mHijos[i] = NULL;
                }
            }

            nodo->mBP = hojasMismoBP;
        }
    }
}

int main() {
#ifdef FILEINPUT
    generarTest("testGenerado");
	ifstream entrada( "testGenerado", ios_base::in );
	ofstream salida( "testGeneradoOut", ios_base::out );
#else
	istream& entrada = cin;
	ostream& salida = cout;
#endif

    int n, numTest = 0;
    long long int pi, pj;
	char p[MAXLONGPREFIJO + 1], bpInput[21];
    const string* bp;

    while( entrada >> n )
    {
        card = 0;
        billPlans.clear();
        invalido = &*billPlans.insert( "invalid" ).first;

        if ( numTest++ > 0 )
            salida << endl;

		while( n-- > 0 )
        {
			entrada.ignore(); // ignoramos el '\n'
			entrada >> p;
            entrada.ignore(); /* Guion y espacio */
			entrada.ignore();
			entrada >> pj;
			entrada >> bpInput;

            bp = &*billPlans.insert( bpInput ).first;

            int di = strlen(p);
            pi = atoll(p);

            pj = pi - pi % ( pow10(digitos(pj)) ) + pj;
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

#ifdef INVARIANTE
                forn( j, MAXHIJOSNODO )
                    invariante(raiz.mHijos[j]);
#endif
    		    dif -= pow10(i);
			    pi++;
		    }

		}

        // comprimo el arbol
        forn( i, MAXHIJOSNODO )
            comprimir( raiz.mHijos[i] );

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
