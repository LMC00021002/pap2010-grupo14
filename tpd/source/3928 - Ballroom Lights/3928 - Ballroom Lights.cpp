#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>

#ifdef _DEBUG
#include <fstream>
#include <iomanip>
#include <assert.h>
#endif

using namespace std;

#define forn( i, n ) for( int i = 0; i < (n); i++ )
#define EPSILON 0.0000001l

inline double min( double a, double b ) { return a < b ? a : b; }
inline double max( double a, double b ) { return a < b ? b : a; }

// ------------------------------------------------------------------------- //
// ESTRUCTURAS

class Par {
public:
    Par(){};
    Par( double _x, double _y ) : x( _x ), y( _y ) {};
    Par( const Par& p ) : x( p.x ), y( p.y ) {};
    Par operator-( const Par& p ) const { return Par( x - p.x, y - p.y ); }
    bool operator<( const Par& p ) const { return x < p.x || ( fabs(x - p.x) < EPSILON && y < p.y ); }
    bool operator==( const Par& p ) const { return fabs(x - p.x) < EPSILON && fabs(y - p.y) < EPSILON; }
    bool operator!=( const Par& p ) const { return fabs(x - p.x) > EPSILON || fabs(y - p.y) > EPSILON; }

    double x;
    double y;
};

inline double prodInterno( const Par& p1, const Par& p2 ) { return p1.x * p2.x + p1.y * p2.y; }

class Segmento {
public:
    Segmento(){};
    Segmento( const Segmento& s ) : p1( s.p1 ), p2( s.p2 ) {};
    Segmento( const Par& _p1, const Par& _p2 ) : p1( _p1 ), p2( _p2 ) {};

    Par p1;
    Par p2;
};

class Semirrecta {
public:
    Semirrecta(){};
    Semirrecta( const Par& _p1, const Par& _p2 ) : origen( _p1 ), v( _p2 - _p1 ) {};
    Semirrecta( const Segmento& s, bool origenP1 = true ) { origen = origenP1 ? s.p1 : s.p2; v = origenP1 ? s.p2 - s.p1 : s.p1 - s.p2; }
    Semirrecta( const Semirrecta& sr ) : origen( sr.origen ), v( sr.v ) {};

    Par origen;
    Par v;
};

struct Columna {
    int x;
    int y;
    int r;
};

struct Luz {
    int x;
    int y;
};


// ------------------------------------------------------------------------- //
// VARIABLES GLOBALES

int cantLuces, cantColumnas, ancho, alto;
vector< Luz > luces;
vector< Columna > columnas;
vector< Segmento > pared;

// ------------------------------------------------------------------------- //
// METODOS

enum IdPared{ INFERIOR = 0, DERECHA, SUPERIOR, IZQUIERDA };

bool interSemirrectas( const Semirrecta& sr1, const Semirrecta& sr2, Par& res )
{
    double k = ( sr2.origen.x - sr1.origen.x ) * sr1.v.y + ( sr1.origen.y - sr2.origen.y ) * sr1.v.x;
    double det = sr1.v.x * sr2.v.y - sr1.v.y * sr2.v.x;
    if( fabs(det) < EPSILON )
    {
        // caso particular:
        // semirectas de sentido opuesto con mismo origen y misma direccion
        // para ver si tienen sentido opuesto o no usamos el producto interno
        if( sr1.origen == sr2.origen && prodInterno( sr1.v, sr2.v ) < 0 )
        {
            res = sr1.origen;
            return true;
        }
        return false;
    }

    double m = k / det;

    // p es el punto de interseccion
    // verificamos si el punto de interseccion esta hacia
    // el mismo sentido que los vectores de sr1 y sr2
    Par p( sr2.origen.x + m*(sr2.v.x), sr2.origen.y + m*(sr2.v.y) );
    if( prodInterno( p - sr1.origen, sr1.v ) < 0 )
        return false;

    if( prodInterno( p - sr2.origen, sr2.v ) < 0 )
        return false;

    res = p;
    return true;
}

bool interSemirrectaSeg( const Semirrecta& sr, const Segmento& s, Par& res )
{
    Par p;
    if( interSemirrectas( sr, s, p ) )
    {
        // para verificar si el punto de interseccion esta dentro de la semirecta
        // veo si el producto interno de los vectores s.p2 - s.p1 y p - s.p1 es positivo
        // y si el producto interno de los vectores s.p1 - s.p2 y p - s.p2 es positivo
        Par v1( s.p2.x - s.p1.x, s.p2.y - s.p1.y );
        Par v2( p.x - s.p1.x, p.y - s.p1.y );
        if( prodInterno( v1, v2 ) < 0 )
            return false;

        v1.x = s.p1.x - s.p2.x;
        v1.y = s.p1.y - s.p2.y;
        v2.x = p.x - s.p2.x;
        v2.y = p.y - s.p2.y;
        if( prodInterno( v1, v2 ) < 0 )
            return false;

        res = p;
        return true;
    }

    return false;
}

Par intervaloSombra( const Luz& l, const Columna& c )
{
    Par v( c.x - l.x, c.y - l.y );
    double hipot = sqrt( v.x * v.x + v.y * v.y );					// hypot( v.x, v.y );
    double sinAlf = c.r / hipot;									// sin(alfa) = opuesto / hipotenusa
//    double ady = sqrt( hipot * hipot - (double)(c.r * c.r) );		// adyacente = raiz( hipotenusa^2 - opuesto^2 )
//    double cosAlf = ady / hipot;									// cos(alfa) = adyacente / hipotenusa
    double cosAlf = sqrt(1 - sinAlf * sinAlf);						// cos(alfa) = sqrt( 1 - sin^2(alfa) )

    /* las tangentes las calculamos rotando el vector multiplicandolo con la matriz de rotación
     *
     * cos(alfa)   -sin(alfa)  *   v.x   =   t.x
     * sin(alfa)    cos(alfa)      v.y       t.y
     *
     * y luego movemos el punto sumandole la ubicacion de la luz
     */
    Par vTangente1( v.x * cosAlf - v.y * sinAlf + l.x, v.x * sinAlf + v.y * cosAlf + l.y );
    Par vTangente2( v.x * cosAlf + v.y * sinAlf + l.x, v.y * cosAlf - v.x * sinAlf + l.y );

    // construyo las tangentes
    Semirrecta tangente1( Par(l.x, l.y), vTangente1 );
    Semirrecta tangente2( Par(l.x, l.y), vTangente2 );

    Segmento* paredTang1 = NULL;
    Segmento* paredTang2 = NULL;
    Par itscc1, itscc2;

    int cantParedes = pared.size();
    int i;
    double x1 = 0;
    double x2 = 0;
    for( i = 0; i < cantParedes && !paredTang2; i++ ) {
        if( interSemirrectaSeg( tangente2, pared[i], itscc2 ) ) {
            paredTang2 = &pared[i];
            switch( i )
            {
            case INFERIOR:
                x1 += itscc2.x;
                break;
            case DERECHA:
                x1 += itscc2.y;
                break;
            case SUPERIOR:
                x1 += (double)ancho - itscc2.x;
                break;
            case IZQUIERDA:
                x1 += (double)alto - itscc2.y;
                break;
            }
        }
        else {
            x1 += ( (i + 1) % 2 ) * ancho + (i % 2) * alto;
            x2 += ( (i + 1) % 2 ) * ancho + (i % 2) * alto;
        }
    }

    int j = i - 1;
    while( !paredTang1 ) {
        if( j % cantParedes == 0 ) {
            j = 0;
            x2 = 0;
        }

        if( interSemirrectaSeg( tangente1, pared[j], itscc1 ) ) {
            paredTang1 = &pared[j];
            switch( j )
            {
            case INFERIOR:
                x2 += itscc1.x;
                break;
            case DERECHA:
                x2 += itscc1.y;
                break;
            case SUPERIOR:
                x2 += (double)ancho - itscc1.x;
                break;
            case IZQUIERDA:
                x2 += (double)alto - itscc1.y;
                break;
            }

        }
        else {
            x2 += ( (j + 1) % 2 ) * ancho + (j % 2) * alto;
        }

        j++;
    }

    double maximo = (ancho << 1) + (alto << 1);
    if( x1 >= maximo )
        x1 -= maximo;

    if( x2 >= maximo )
        x2 -= maximo;

    return Par( x1, x2 );
}

void comprimir( vector< Par >& pares )
{
    int n = pares.size();
    if( n > 0 ) {
        vector< Par > res;

        sort( pares.begin(), pares.end() );

        res.push_back( pares[0] );
        int ultimo = 0;

        for( int i = 1; i < n; i++ ) {
            if( res[ ultimo ].y >= pares[ i ].x ) {
                if( res[ ultimo ].y < pares[ i ].y )
                    res[ ultimo ].y = pares[ i ].y;
            }
            else {
                res.push_back( pares[ i ] );
                ultimo++;
            }
        }

        pares = res;
    }
}

void complemento( vector< Par >& conj, double minVal, double maxVal )
{
    comprimir( conj );

    int n = conj.size();
    vector< Par > res;

    if( n == 0 )
        res.push_back( Par( minVal, maxVal ) );
    else {
        if( fabs(minVal - conj[0].x) > EPSILON )
            res.push_back( Par( minVal, conj[0].x ) );

        for( int i = 0; i < n - 1; i++ )
            res.push_back( Par( conj[i].y, conj[i + 1].x ) );

        if( fabs( conj[n-1].y - maxVal ) > EPSILON )
            res.push_back( Par( conj[n-1].y, maxVal ) );
    }

    conj = res;
}

double perimIluminado()
{
    int minVal = 0, maxVal = (ancho << 1) + (alto << 1);

	if( cantLuces == 0 )
		return minVal;

	if( cantColumnas == 0 )
		return maxVal;

    vector< Par > iluminado;

    forn( i, cantLuces ) {
        vector< Par > intervalos;

        forn( j, cantColumnas ) {
            Par sombra = intervaloSombra( luces[i], columnas[j] );
            if( sombra.x > sombra.y ) {
                intervalos.push_back( Par( minVal, sombra.y ) );
                intervalos.push_back( Par( sombra.x, maxVal ) );
            }
            else
                intervalos.push_back( sombra );
        }
        
        complemento( intervalos, minVal, maxVal );
        
        int sizeIntervalos = intervalos.size();
        forn( i, sizeIntervalos )
            iluminado.push_back( intervalos[i] );
    
        comprimir( iluminado );
    }

    int sizeIluminado = iluminado.size();
    double res = 0;
    forn( i, sizeIluminado )
        res += iluminado[i].y - iluminado[i].x;

    return res;
}

int main()
{
    while( true ) {
        columnas.clear();
        luces.clear();
        pared.clear();

        scanf( "%d %d %d %d", &cantLuces, &cantColumnas, &ancho, &alto );

        if( cantLuces == 0 && cantColumnas == 0 && ancho == 0 && alto == 0 )
            break;

        pared.resize(4);
        pared[INFERIOR] = Segmento( Par( 0, 0 ), Par( ancho, 0 ) );
        pared[DERECHA] = Segmento( Par( ancho, 0 ), Par( ancho, alto ) );
        pared[SUPERIOR] = Segmento( Par( ancho, alto ), Par( 0, alto ) );
        pared[IZQUIERDA] = Segmento( Par( 0, alto ), Par( 0, 0 ) );

        forn( i, cantLuces ) {
            Luz l;
            scanf( "%d %d", &l.x, &l.y );
            luces.push_back( l );
        }

        forn( i, cantColumnas )
        {
            Columna nuevaCol;
            scanf( "%d %d %d", &nuevaCol.x, &nuevaCol.y, &nuevaCol.r );
            columnas.push_back( nuevaCol );
        }

        printf( "%.4f\n", perimIluminado() );
    }

    return 0;
}
