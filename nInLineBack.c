#include <stdlib.h>
#include <stdio.h>
#include "rand.h"
#include "nInLineBack.h"

/*Recibe una columna y quita la ultima ficha colocada en la  misma.*/
void sacarFicha ( char ** tablero ,  int columna ,  int * vector );

/*Libera memoria dinamica*/
void liberar (char ** tablero, int i);

void cambiarjugador(int * jugador)
{
	if (*jugador==1)
		*jugador=2;
	else
		*jugador=1;
}

int * cargarvector( tJuego * partida )

{
	int * vector;

	int i,j;

	vector = crearvector(partida->cantFilas,partida->cantColumnas);

	for (i=0,j=0 ; i<(partida->cantColumnas) ; i++)
		{
			if ( partida->tablero[i][j] == 1 || partida->tablero[i][j] == 2 )
				{
					vector[j]=i-1;
					j++;
					i=0;
				}
			else if ( partida->tablero[i][j] == 0 && i==partida->cantFilas-1 )
				{
					j++;
					i=0;
				}

		}

	return vector;
}

char ** crearTablero(const int cantFilas,const int cantColumnas)
{
	int i;
	char ** tablero;

	tablero = calloc(cantFilas,sizeof(*tablero));

	if ( tablero == NULL )
		return NULL;

	for ( i=0 ; i<cantFilas ; i++ )

	{
		tablero[i] = calloc( (cantColumnas),sizeof(**tablero) );
		if ( tablero[i] == NULL )
		{
			liberar(tablero,i);
			return NULL;
		}
	}

	return tablero;
}

 int * crearvector( const int cantFilas, const int cantColumnas )
{
	int * vector;

	int i;

	vector = malloc(cantColumnas*sizeof(*vector));

	if ( vector == NULL )
	{
		free (vector);
		return NULL;
	}

	for (i=0; i<cantColumnas;i++) /*Inicializo el vector con el numero de fila donde se puede colocar la sigiuiente ficha*/
		vector[i]=cantFilas-1;       /*Es invocada al iniciar una partida nueva*/
	return vector;
}


int comprobarLinea (  char ** tablero , const int cantFilas , const int cantCol, int columna , int fila, int N, int jugador)
{
	int i,j, enlinea, vectordireccionf, vectordireccionc, corte;

	int vec[4][2] = { {1,0}, {0,1} , {1,1} , {1,-1} }; /* Vector con las 4 posibles direcciones de busqueda*/

	for ( i=0 ; i<4 ; i++ )
	{
			vectordireccionf=vec[i][0];
			vectordireccionc=vec[i][1];
		 	enlinea=1;                      /* cantidad de fichas en linea */
			corte=0;

			for (j=1 ; corte < 2 ;  )
			{/*Comparo la jugada con vecinos*/
				if ( estaenelTablero(tablero,cantFilas,cantCol,columna,fila+1,vectordireccionf*j,vectordireccionc*j) 
				&& tablero[fila+1][columna] == tablero[fila+1+vectordireccionf*j][columna+vectordireccionc*j] ) 
					{
						enlinea++;
						j++;
					}
				else
				{
					corte++;
					j=1;
					vectordireccionf=-vectordireccionf; /*Se cambia el sentido de busqueda*/
					vectordireccionc=-vectordireccionc;
				}
			}
			if (enlinea>=N)
				return 1;


	}
	return 0;

}

int estaenelTablero (  char ** tablero , int cantFilas , int cantCol, int columna , int fila, int movimientof, int movimientoc)
{
	if ( (fila+movimientof)<cantFilas && (columna+movimientoc)<cantCol && (fila+movimientof)>=0 && columna+movimientoc>=0)
		return 1;

	return 0;
}

int estrategiaPc ( char ** tablero, int cantFilas , int cantColumnas ,int N, int jugador, int * vector)
{
	int fila, * vectoraux,aux,random,cantColaux=cantColumnas-1;
	int i;

	/*ESTRATEGIA 1*/
	for (i=0; i < cantColumnas; i++)
	{
		if (estaenelTablero(tablero,cantFilas,cantColumnas,i, vector[i]-1 ,0,0) == 1)
		{
			fila=ponerFicha(tablero,i,2,vector); /* Se coloca Ficha PC */
			if ( comprobarLinea (tablero,cantFilas,cantColumnas,i,fila,N,2) )
				return 1;
			else
				sacarFicha(tablero,i,vector); /*Si no hay linea se quita la ficha y se intenta en la sig. columna*/
		}
	}
	/*ESTRATEGIA 2*/
	/*Se coloca ficha del jugador. Si hay linea, la misma se retira y se coloca la de la PC
	caso contrario tambien se quita la del jugador y se repite el proceso en la sig. columna*/
	for ( i=0 ; i < cantColumnas; i++)
	{

		if ( estaenelTablero(tablero,cantFilas,cantColumnas,i, vector[i] ,0,0) == 1)
		{

			fila=ponerFicha(tablero,i,1,vector);
			if ( comprobarLinea (tablero,cantFilas,cantColumnas,i,fila,N,1) == 1 )
			{
				sacarFicha(tablero,i,vector);
				ponerFicha(tablero,i,2,vector);
				return 0;
			}
			else
			sacarFicha(tablero,i,vector);
		}

	}
	/*ESTRATEGIA 3*/

	/*Vector que almacena los numeros de columnas disponibles
	* y se achica en cada ciclo para no repetir numeros aleatorios*/
	vectoraux=crearvector(cantFilas,cantColumnas); 
	for (i=0; i < cantColumnas; i++)
		vectoraux[i]=i;
	do
	{
		random = randInt(0,cantColaux);
		if ( vector[random] == 0) 
		/* Si la columna posee un solo espacio, se coloca ficha ya 
		* que es imposible generar jugada ganadora del jugador*/
		{
			ponerFicha(tablero, random , 2 , vector);
			return 0;
		}
		if ( estaenelTablero(tablero,cantFilas,cantColumnas, vectoraux[random] , vector[vectoraux[random]] ,0,0) == 1 
		&&  estaenelTablero( tablero , cantFilas , cantColumnas , vectoraux[random] , vector[vectoraux[random]]-1 , 0 , 0 ) == 1  
		&& vector[random]!=0)
		{
			ponerFicha(tablero, vectoraux[random] , 2 , vector);/*Pone pc*/
			fila=ponerFicha(tablero, vectoraux[random] , 1 , vector);/*Pone jugador*/
			if (
comprobarLinea (tablero,cantFilas,cantColumnas,vectoraux[random],fila,N,1) == 0) /*Si no hizo linea jugador*/
			{
				sacarFicha(tablero,vectoraux[random],vector); /*Saco jugador*/
				return 0;
			}
			else
			{
				sacarFicha(tablero,vectoraux[random],vector);
				sacarFicha(tablero,vectoraux[random],vector);
			}

		}

		aux=vectoraux[cantColaux];
		vectoraux[cantColaux]=vectoraux[random];
		vectoraux[random]=aux;
		cantColaux--;


	}
	while ( cantColaux>0 );

	if (cantColaux==0)
	{
	ponerFicha(tablero, vectoraux[random] , 2 , vector);
	}

return 0;

}

void liberar (char ** tablero, int i)
{
	for (i--;i>=0;i--)
		free(tablero[i]);
	free(tablero);

}

int ponerFicha ( char ** tablero, int columna , int jugador , int * vector )
{

	while ( vector[columna] >= 0 )
	{

		tablero[ (vector[columna]) ][columna] = jugador;

		--vector[columna];

		return vector[columna];
	}

	return -2;

}

void sacarFicha ( char ** tablero, int columna ,  int * vector )
{
	vector[columna]++;
	tablero [ vector[columna] ] [ columna ] = 0;

}

