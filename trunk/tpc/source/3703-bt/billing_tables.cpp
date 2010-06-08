#include <stdio.h>
#include <string>
#include <set>

using namespace std;

#define MAXHIJOSNODO 10
#define MAXLONGPREFIJO 11
#define forn(i, n) for(int i = 0; i < (int)(n); ++i)

// ------------- METODOS BASICOS NECESARIOS ---------------- //

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

inline long long atoll( char* n )
{
    long long number = 0;
    while( *n != '\0' )
        number = number * 10 + *n++ - '0';

    return number;
}

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

// ------------------- ESTRUCTURA DEL TRIE -------------------- //

class Nodo
{
public:
    Nodo( char keyChar = '$', Nodo *padre = NULL ) :
      mBP( NULL ), mKeyChar( keyChar ) {
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
};

// -------------------- VARIABLES GLOBALES -------------------- //

Nodo raiz;
set<string> billPlans;
const string *invalido;
long long int cardinal;

// --------------------- METODOS DEL TRIE --------------------- //

void printAux( Nodo* t, char* p, int pLen )
{
	if(t && ( !t->mBP || t->mBP != invalido ) )
    {
		if( t->mBP )
    	    printf( "%s %s\n", p, t->mBP->c_str() );
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
				    printAux( t->mHijos[i], conc, pLen + 1 );
                }
			}
		}
	}
}

inline void print( Nodo* t ) {
    printAux( &raiz, "", 0 );
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
                    if( billPlan != invalido )
                        cardinal++;
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
        if( billPlan != invalido )
            cardinal++;
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
                    if( hojasMismoBP != invalido )
                        cardinal--;
                }
            }

            nodo->mBP = hojasMismoBP;
            if( hojasMismoBP != invalido )
                cardinal++;
        }
    }
}

int main() {

    int n, numTest = 0;
    long long int pi, pj;
	char p[MAXLONGPREFIJO + 1], bpInput[21];
    const string* bp;

    while( scanf("%d", &n ) == 1 )
    {
        cardinal = 0;
        invalido = &*billPlans.insert( "invalid" ).first;

        if ( numTest++ > 0 )
            printf("\n");

		while( n-- > 0 )
        {
            scanf("%s - %lld %s", p, &pj, bpInput );

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

    		    dif -= pow10(i);
			    pi++;
		    }

		}

        // comprimo el arbol
        forn( i, MAXHIJOSNODO )
            comprimir( raiz.mHijos[i] );

		printf("%d\n", cardinal);

		/* Imprime árbol por stdout */
		print( &raiz );

		raiz.destruir();
        billPlans.clear();
    }

	return 0;
}
