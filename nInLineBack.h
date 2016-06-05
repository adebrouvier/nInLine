#ifndef _nInLineBack_h

#define _nInLineBack_h

#define BORRAR_BUFFER while(getchar() != '\n')

typedef struct
{
	int modojuego;
	int proximoturno;
	int N;
	int cantFilas;
	int cantColumnas;
	char  ** tablero;
	int gameload;
} tJuego;

/****************PROTOTIPOS****************/


/*Cambia el turno de jugada*/
void cambiarjugador(int * jugador);

/*Inicializa el vector de fila disponible en las partidas que son cargadas
* Retorna la direccion del vector */
int * cargarvector( tJuego * partida );

/*Reserva memoria dinamica para el tablero, si no hay memoria disponible retorna NULL*/
char ** crearTablero (const int cantFilas , const int cantColumnas);

/*Crea un vector dinámico y lo completa con la cantidad de filas libres por columna. Si no hay memoria disponible retorna NULL. */
int * crearvector (const int cantFilas , const int cantColumnas);

/*Recorre el tablero en todas las direcciones basándose en la ultima jugada. Retorna 1 si hubo linea y 0 en caso contrario*/
int comprobarLinea (  char ** tablero , const int cantFilas , const int cantCol, int columna , int fila, int N, int jugador);

/*Retorna 1 si una par de coordenadas (una jugada) esta dentro de los limites del tablero.
De lo contrario retorna 0.*/
int estaenelTablero (  char ** tablero , int cantFilas , int intColumnas, int columna , int fila, int movimientof, int );

/*Consta de 3 estrategias distintas:
**Estrategia 1: Busca una jugada ganadora para la pc.
**Estrategia 2: Evita que el jugador gane en la proxima jugada.
**Estrategia 3: Genera una jugada aleatoria, verificando además que esta no produzca una jugada ganadora para el usuario.
*Retorna 1 si la PC consiguio linea, 0 en el caso contrario. */
int estrategiaPc ( char ** tablero, int cantFilas , int cantColumnas ,int N, int jugador, int * vector);

/*Recibe una jugada y decrementa el vector de proxima jugada. Retorna la fila donde coloco la ficha. Si no hay espacio en la columna retorna -2*/
int ponerFicha ( char ** tablero, int jugada , int jugador , int * vector );


#endif
