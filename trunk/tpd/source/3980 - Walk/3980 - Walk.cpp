#include <stdio.h>
#include <algorithm>
#include <vector>
#ifdef _DEBUG
#include <fstream>
#endif

using namespace std;

#define forn( i, n ) for( int i = 0; i < (n); i++ )
#define POSALICE 0
#define POSBOB 100000
#define NAN -300001

class Desnivel
{
public:
    Desnivel(){};
    Desnivel( double coordX1, double coordX2, int _altura) : x1( coordX1 ), x2( coordX2 ), altura( _altura ) {};
    double x1, x2;
    int altura;
};

//------------------------------------------------------------------------------------------------
// VARIABLES GLOBALES

int n;
vector< Desnivel > poligonosAlice;
vector< Desnivel > poligonosBob;

//------------------------------------------------------------------------------------------------
// METODOS

inline double min( double a, double b ) { return a < b ? a : b; }
inline double max( double a, double b ) { return a < b ? b : a; }

bool contenido( const Desnivel& a, const Desnivel& b )
{
    return b.x1 < a.x1 && a.x2 < b.x2;
}

double cruceEjeX( int x1, int y1, int x2, int y2 )
{
    return x1 - ( x2 - x1 ) * y1/( y2 - y1 );
}

int main()
{
    int cantTests;

#ifdef _DEBUG
    ifstream entrada("test.in", ios_base::in );
    ofstream salida ("test.out", ios_base::out );
#endif

#ifdef _DEBUG
    entrada >> cantTests;
#else
    scanf( "%d", &cantTests );
#endif
    while( cantTests-- > 0 ) {
        poligonosAlice.clear();
        poligonosBob.clear();
#ifdef _DEBUG
        entrada >> n;
#else
        scanf( "%d", &n );
#endif
        while( n-- > 0 ) {
            int altura, cantVertices, xInicial, yInicial, x1, y1, x2, y2;
            vector< double > crucesEjeX;

#ifdef _DEBUG
            entrada >> altura >> cantVertices >> xInicial >> yInicial;
#else
            scanf( "%d %d %d %d", &altura, &cantVertices, &x1, &y1 );
#endif
            x1 = xInicial;
            y1 = yInicial;
            cantVertices--;
            int cantCrucesIzq = 0, cantCrucesMedio = 0;
            double xMasCercanoIzqAlice = POSALICE, xMasCercanoDerAlice = POSALICE;
            double xMasCercanoIzqBob = POSBOB, xMasCercanoDerBob = POSBOB;

            forn( i, cantVertices ) {
#ifdef _DEBUG
                entrada >> x2 >> y2;
#else
                scanf( "%d %d", &x2, &y2 );
#endif
                // si cruza al eje x
                if( ( y1 < 0 && y2 > 0 ) || ( y1 > 0 && y2 < 0 ) || ( y1 == 0 && y2 != 0 ) ) {
                    double x = cruceEjeX( x1, y1, x2, y2 );

                    if ( x < POSALICE ) {
                        cantCrucesIzq++;
                        xMasCercanoIzqAlice = xMasCercanoIzqAlice == POSALICE ? x : max( xMasCercanoIzqAlice, x );
                    }
                    else {
                        if ( x > POSALICE && x < POSBOB ) {
                            cantCrucesMedio++;
                            xMasCercanoDerAlice = xMasCercanoDerAlice == POSALICE ? x : min( xMasCercanoDerAlice, x );
                            xMasCercanoIzqBob = xMasCercanoIzqBob == POSBOB ? x : max( xMasCercanoIzqBob, x );
                        }
                        else
                            xMasCercanoDerBob = xMasCercanoDerBob == POSBOB ? x : min( xMasCercanoDerBob, x );
                    }
                }

                x1 = x2;
                y1 = y2;
            }

            // por ultimo, verificamos si cruza el eje X el segmento formado por el ultimo punto
            // y el primero. En caso de hacerlo, hay que agregar el cruce.
            if( ( y1 < 0 && yInicial > 0 ) || ( y1 > 0 && yInicial < 0 ) || ( y1 == 0 && yInicial != 0 ) ) {
                double x = cruceEjeX( x1, y1, xInicial, yInicial );

                if ( x < POSALICE ) {
                    cantCrucesIzq++;
                    xMasCercanoIzqAlice = xMasCercanoIzqAlice == POSALICE ? x : max( xMasCercanoIzqAlice, x );
                }
                else {
                    if ( x > POSALICE && x < POSBOB ) {
                        cantCrucesMedio++;
                        xMasCercanoDerAlice = xMasCercanoDerAlice == POSALICE ? x : min( xMasCercanoDerAlice, x );
                        xMasCercanoIzqBob = xMasCercanoIzqBob == POSBOB ? x : max( xMasCercanoIzqBob, x );
                    }
                    else
                        xMasCercanoDerBob = xMasCercanoDerBob == POSBOB ? x : min( xMasCercanoDerBob, x );
                }
            }

            // si el poligono actual cruza al eje x en un punto de coordenada x menor a la
            // de alice, y si la cantidad de cruces a la izquierda de alice es impar
            // entonces el poligono contiene a alice
            bool contieneAlice = xMasCercanoIzqAlice != POSALICE && cantCrucesIzq % 2 == 1;

            // analogo para bob pero con cantidad de cruces = cantCrucesIzq + cantCrucesMedio
            bool contieneBob = xMasCercanoDerBob != POSBOB && (cantCrucesIzq + cantCrucesMedio) % 2 == 1;

            if( ( contieneAlice || contieneBob ) && !( contieneAlice && contieneBob ) ) {
                if( contieneAlice )
                    poligonosAlice.push_back( Desnivel( xMasCercanoIzqAlice, xMasCercanoDerAlice, altura ) );
                else
                    poligonosBob.push_back( Desnivel( xMasCercanoIzqBob, xMasCercanoDerBob, altura ) );
            }
        }

        // ordeno los poligonos que contienen a Alice por cercanía a ella
        sort( poligonosAlice.begin(), poligonosAlice.end(), contenido );

        // ordeno los poligonos que contienen a Bob por cercanía a el
        sort( poligonosBob.begin(), poligonosBob.end(), contenido );

        int sizePA = poligonosAlice.size();
        int sizePB = poligonosBob.size();
        int alturaSubida = 0;
        int alturaBajada = 0;
        int ultimaAltura = 0;

        if( sizePA > 0 ) {
            ultimaAltura = poligonosAlice[0].altura;

            for( int i = 1; i < sizePA; i++ ) {
                int h = poligonosAlice[i].altura;
                if( h < ultimaAltura )
                    alturaBajada += ultimaAltura - h;
                else
                    alturaSubida += h - ultimaAltura;

                ultimaAltura = h;
            }
        }

        if( sizePB > 0 ) {
            int i = sizePB - 1;
            if( ultimaAltura == 0 ) {
                ultimaAltura = poligonosBob[i].altura;
                i--;
            }

            for( ; i >= 0; i-- ) {
                int h = poligonosBob[i].altura;
                if( h < ultimaAltura )
                    alturaBajada += ultimaAltura - h;
                else
                    alturaSubida += h - ultimaAltura;

                ultimaAltura = h;
            }
        }

#ifdef _DEBUG
        salida << alturaSubida << " " << alturaBajada << endl;
#else
        printf( "%d %d\n", alturaSubida, alturaBajada );
#endif
    }

#ifdef _DEBUG
    entrada.close();
    salida.close();
#endif
    return 0;
}
