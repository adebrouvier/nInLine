#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "getnum.h"
#include "rand.h"
#include "nInLineBack.h"
#define MAXDIM 30
#define MINFICHAS 3
#define LIM_JUGADA 3
#define LIM_NOMBRE 20

/*Funcion que carga los datos de una partida guardada*/
char** cargarPartida ( tJuego * partida, int * turno , int * error);

/*Funcion que guarda los parametros de la partida y el contenido del tablero en un archivo*/
int grabarPartida ( tJuego * partida , char * nombreArchivo);

/*Funcion que imprime en pantalla errores de ejecucion*/
void imprimirerror(int error);

/*Funcion que se encarga de la logica del juego, pidiendo jugada o invocandoala estrategia de la PC*/
int jugar (tJuego * partida, int * turno, int * error);

/*Lee caracteres desde entrada estandar*/
int leerString (char * string, int lim );

/*Funcion que imprime en pantalla el menu principal del juego*/
int menu (void);

/*Funcion que imprime en pantalla el menu de salida de partida*/
int menu2 (void);

/*Imprime en pantalla el tablero.*/
void mostrarTablero(tJuego * partida);

/*Interactua con el usuario solicitando el numero de filas y de columnas del tablero*/
void pedirDimensiones (tJuego * partida);

/*Solicita el numero de columna a jugar.*/
int pedirJugada (int cantColumnas,int jugador, int * jugada);

/*Interaccion con el usuario para solicitar el numero fichas para hacer linea*/
void pedirN (tJuego * partida);

/*Funcion que solicita el nombre de archivo para guardar o cargar una partida*/
char * pedirNombre(void);

/*Invoca a pedirN y pedirDimensiones en nuevas partidas*/
void pedirParametros (tJuego * partida);

/*Interaccion con el usuario para pedir confirmacion*/
int siNo(void);

/*Verifica si el archivo existe o no. Consulta sobre-escritura*/
int validarNombre(char * nombre);

int main (void)
{
	int seleccion;
	tJuego partida;
	int turno=1,error=1;
	randomize(); /* Se setea semilla para random de la Estrategia de la PC*/

	do
	{
		seleccion=menu();
		switch (seleccion)
		{
			case 1: /*Partida de dos jugadores*/
			{ 
				pedirParametros(&partida);
				partida.modojuego=0;
				partida.gameload=0;
				jugar(&partida,&turno,&error);
        			imprimirerror(error);
				break;
			}

			case 2: /*Partida contra la PC*/
			{ 
				pedirParametros(&partida);
				partida.modojuego=1;
				partida.gameload=0;
				jugar(&partida,&turno,&error);
            			imprimirerror(error);
				break;
			}

			case 3: /*Cargar Partida*/
			{
				do
           			{
                			partida.tablero=cargarPartida(&partida,&turno,&error);
                			imprimirerror(error);
            			}
				while (error!=1);
				partida.gameload=1;
				jugar(&partida,&turno,&error);
			        imprimirerror(error);
				break;
			}
			case 4:
				exit(0);

			default:
				printf("Introduzca una opcion valida\n" ) ;
		}
	}

while (seleccion<1 || seleccion>4);


return 0;
}

char** cargarPartida ( tJuego * partida , int * turno, int * error)
{
	char **tablero, *nombre,aux;
	FILE * archivo;
	int i,j,turnoaux=1;
	nombre=pedirNombre();

	if ( nombre == NULL)
	{
		*error=-4;
		return NULL;
	}

	archivo=fopen(nombre,"r");

	if (archivo == NULL)
	{
		*error=-1; /* Archivo inexistente */
		return NULL;
	}

	/*Inicializa la estructura partida*/
	if ( fread(&(partida->modojuego),sizeof(int),1,archivo)!=1 || ( partida->modojuego!=0 && partida->modojuego!=1 ) )
	{
		*error=-2; /*Archivo invalido*/
		fclose(archivo);
		return NULL;
	}

	if ( fread(&(partida->proximoturno),sizeof(int),1,archivo) != 1 )
	{
		*error=-2; /*Archivo invalido*/
		fclose(archivo);
		return NULL;
	}

	if ( fread(&(partida->cantFilas),sizeof(int),1,archivo) != 1 )
	{
		*error=-2; /*Archivo invalido*/
		fclose(archivo);
		return NULL;
	}

	if (  fread(&(partida->cantColumnas),sizeof(int),1,archivo) != 1 )
	{
		*error=-2; /*Archivo invalido*/
		fclose(archivo);
		return NULL;
	}

	if (  fread(&(partida->N),sizeof(int),1,archivo) !=1)
	{
		*error=-2; /*Archivo invalido*/
		fclose(archivo);
		return NULL;
	}

	tablero = crearTablero(partida->cantFilas,partida->cantColumnas);

	if (tablero == NULL)
	{
		*error=-4; /* Falta de memoria*/
		fclose(archivo);
		return NULL;
	}
	
	for (i=0; i < (partida->cantFilas) ; i++)
	{
		for (j=0; j < (partida->cantColumnas) ; j++)
			{
				if( fread(&aux,sizeof(char),1,archivo)!=1 || !(aux>=0 && aux<=2) )
				{
					*error=-2; /*Archivo invalido*/
					fclose(archivo);
					return NULL;
				}
				tablero[i][j]=aux;
				if (tablero[i][j] == 1 || tablero[i][j] == 2) /* Se cuenta cuantos turnos se jugaron */
					turnoaux++;
			}
	}
	
	*turno=turnoaux;
	*error=1;	/*OK*/
	fclose(archivo);
	return tablero;


}

int grabarPartida ( tJuego * partida, char * nombreArchivo)
{
	int i,j;
	FILE * archivo;



	if ( (archivo=fopen(nombreArchivo,"w"))==NULL)
		return -1;
	

	/*Escribe en el archivo los campos de la estructura partida*/
	fwrite(&(partida->modojuego),sizeof(int),1,archivo);


	fwrite(&(partida->proximoturno),sizeof(int),1,archivo);


	fwrite(&(partida->cantFilas),sizeof(int),1,archivo);

	fwrite(&(partida->cantColumnas),sizeof(int),1,archivo);

	fwrite(&(partida->N),sizeof(int),1,archivo);


	for (i=0; i < (partida->cantFilas) ; i++)
	{
		for (j=0; j< (partida->cantColumnas) ; j++)
			fwrite(&(partida->tablero[i][j]),sizeof(char),1,archivo);
	}


	return fclose(archivo);
}

void imprimirerror (int error)
{

	switch(error)
	{
		case -1: printf("Archivo inexistente.\n"); break;
		case -2: printf("Archivo invalido.\n"); break;
		case -4: printf("Error: Falta de memoria.\n"); break;
	}

}

int jugar (tJuego * partida, int * turno, int * error)
{
	int columna, *vector=NULL, jugador=1, i, hayLinea=0, fila, seleccion;

	char * nombreArchivo;

	/*Si es una partida nueva, se crea el tablero y se inicializa el vector con la próxima fila libre.
	Si es una partida guardada, se setea el turno de quien juega y se arma el vector de próxima jugada*/
	if (partida->gameload == 0)
		{
			partida->tablero = crearTablero(partida->cantFilas,partida->cantColumnas);
            		if (partida->tablero == NULL)
            		{
            			*error=-4; /* Falta de memoria*/
            			return 0;
            		}
			vector = crearvector(partida->cantFilas,partida->cantColumnas);
            		if (vector == NULL)
                	{
           			*error=-4; /* Falta de memoria*/
	            		return 0;
            		}
		}
	else
		{
			jugador=partida->proximoturno;
			vector=cargarvector(partida);
		}

	for (i=(*turno); !hayLinea  && i<=(partida->cantFilas)*(partida->cantColumnas) ; i++)
	{
		mostrarTablero(partida);
        /* Si es una partida de dos jugadores o contra la PC y el turno es impar, pide jugada*/
		if ( partida->modojuego == 0 || (partida->modojuego == 1 && i%2!=0  ) )
		{


			if ( pedirJugada(partida->cantColumnas,jugador,&columna)==-1 )
			{
				do
				{           /*Menu para guardar la partida o salir*/
		                	seleccion = menu2();
        		       		switch (seleccion)
                			{
                    				case 1:
                    				{
							do
			                     			nombreArchivo=pedirNombre();
							while( !validarNombre(nombreArchivo) );
							partida->proximoturno=jugador;
                        				grabarPartida(partida , nombreArchivo);
                        				printf("Partida Guardada con éxito\n");
							return 1;


                    				}

                    				case 2:
              						exit(0);
                				default:
                					printf("Introduzca una opcion valida\n" ) ;

                			}
                		}
				while(seleccion < 1  || seleccion > 2);
            		}

			/*Como es jugada valida, coloca la ficha en el tablero*/
			fila = ponerFicha(partida->tablero, columna,jugador,vector);

            		/*Si la columna está llena, se decrementa el indice de turno y vuelve a pedir jugada.
            		Sino comprueba si hay linea*/
			if ( fila == -2)
			{
				printf(">>>¡ NO hay lugar en la columna %d !<<<\n",columna+1);//PONER FLAG
				i--;
			}
			else
			{

				if ( comprobarLinea (partida->tablero,partida->cantFilas,partida->cantColumnas,columna,fila,partida->N,jugador) )
				{
					hayLinea=1;
					mostrarTablero(partida);
					printf("¡Ganador Jugador %d!\n", jugador );
					return 1;
				}

			cambiarjugador(&jugador);

			}
		}

		else
			{ /* Juega la PC */
				printf("Jugada de la PC:\n");
				if( estrategiaPc(partida->tablero,partida->cantFilas,partida->cantColumnas,partida->N,jugador,vector)==1 )
				{
					hayLinea=1;
					mostrarTablero(partida);
					printf("¡Juego perdido!\n");
					return 1;
				}
				else
				{
					cambiarjugador(&jugador);
				}
			}


	}
	mostrarTablero(partida);
	printf("EMPATE!\n");
	return 1;
}

int leerString (char * string, int lim )
{
	char c;
	int i=0;

	while ( (c=getchar())!='\n')
	{
		if (i<lim)
		string[i++]=c;
	}
	string[i]='\0';
	return i;
}

int menu (void)
{
	int opcion;

	printf("\n*****************************\n");
	printf("N en Línea\n");
	printf("Menú:\n");
	printf("1.Juego de dos jugadores\n");
	printf("2.Juego contra la computadora\n");
	printf("3.Recuperar un juego grabado\n");
	printf("4.Terminar\n");
	printf("*****************************\n\n");

	opcion = getint("Introduzca una opcion: ");
	return opcion;

}

int menu2 (void)
{
    int opcion;

    printf("\nELija una opción:\n");
    printf("1.Grabar juego\n");
    printf("2.Salir\n");

    opcion = getint("");
    return opcion;


}

void mostrarTablero(tJuego * partida)
{
	int i,j,aux;
        for( i=0 ; i < (partida->cantFilas) ; i++ )
        { 		putchar('\t');
		for( j=0 ; j < (partida->cantColumnas) ; j++ )
		{
			if (partida->tablero[i][j]==1)
				aux='X';
			else if (partida->tablero[i][j]==2)
					aux='O';
				else
					aux='.';
			printf("%c   ",aux);
		}
		putchar('\n');
	}
	putchar('\n');
	putchar('\t');
	for( j=1 ; j <= (partida->cantColumnas) ; j++ ) /*Imprime numero de columna*/
		printf("%d  %s", j, j>=10?"":" ");
	putchar('\n');
}

void pedirDimensiones (tJuego * partida)
{
	printf("Introduzca el numero de FILAS. ");
	do
	{
		printf("Debe ser mayor o igual a %d y menor a %d: ", partida->N+1,MAXDIM);
		partida->cantFilas=getint("");
	}
	while (partida->cantFilas<(partida->N+1) || partida->cantFilas>MAXDIM);

	printf("Introduzca el numero de COLUMNAS. ");
	do
	{
		printf("Debe ser mayor o igual a %d y menor a %d: ", partida->N+1, MAXDIM);
		partida->cantColumnas=getint("");
	}
	while( partida->cantColumnas<(partida->N+1) || partida->cantColumnas>MAXDIM);
}

int pedirJugada (int cantColumnas,int jugador, int * jugada)
{
	char line[LIM_JUGADA], auxquit;
	int aux;

    	*jugada=0;

	printf("Turno del jugador %d (%c) \n", jugador, (jugador==1)?'X':'O' );

	/*Lee de la entrada estandar la jugada o la opcion para salir*/
	do
	{
		printf("Introduzca la columna [1-%d] (q o Q para salir): ",cantColumnas);

		leerString(line,LIM_JUGADA);

		aux=sscanf(line,"%d",jugada);
		if (aux!=1)
		{
			sscanf(line,"%c",&auxquit);
			if (auxquit=='q' || auxquit=='Q')
				return -1;
		}

	}
	while ( *jugada<1 || *jugada>cantColumnas );

	(*jugada)--;

	return 1;
}

void pedirN (tJuego * partida)
{
	int N;
	printf("Introduzca el numero de FICHAS para hacer LINEA. ");
	do
	{
		printf("Debe ser mayor o igual a %d y menor a %d: ", MINFICHAS , MAXDIM-1);
		N=getint("");
	}
	while (N<3 || N>MAXDIM-1);
	partida->N=N;
}

char * pedirNombre( void )
{
	char namefile[LIM_NOMBRE], *namefilefinal;
	int len;

	printf("Introduzca nombre de archivo. El limite es %d caracteres: ",LIM_NOMBRE);

    	/*Lee el nombre del archivo de la entrada standar*/
	fgets(namefile, LIM_NOMBRE-1, stdin);

	len=strlen(namefile);

    	/*Reemplaza \n por un \0 */
	namefile[len-1]='\0';

	namefilefinal = malloc((strlen(namefile)+4)*sizeof(int) );

	strcpy(namefilefinal,namefile);

    	/*Agrega la extension del archivo*/
	strcat(namefilefinal,".sav");

	return namefilefinal;

}

void pedirParametros (tJuego * partida)
{
	pedirN(partida);
	pedirDimensiones(partida);
}

int siNo(void)
{
	int c;
	do
	{
		if(toupper(c = getchar()) == 'N')
		{
			BORRAR_BUFFER;
			return 0;
		}
		putchar('\n');
	}
	while(toupper(c) != 'S');

	BORRAR_BUFFER;
	return 1;
}

int validarNombre(char * nombre)
{
	FILE * archivo;
	if( (archivo = fopen(nombre, "rb")) != NULL)
	{
		printf("El archivo ya existe. Desea sobreescribirlo?(S/N)\n");
		return siNo();
	}
	return 1;
}

















