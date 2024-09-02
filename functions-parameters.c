//Libreria.h propia
//Headers de funciones
//No de necesaria utilidad, solo declaracion y parametros

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>

typedef struct {
    int orden, dia, mes, anio, cuotas, importe;
    char cliente[40], electro[20];
    float impCuota, Iva, totalCuota;
}creditos;

//Funcione de bienvenida y menu principal
void bienvenida();
void menu(struct tm x, FILE * punteroArchivoBin, FILE * punteroArchivo, char * lineaTexto);

//Imprime el archivo TXT
void imprime_creditoscsv(FILE * x, char * y);

//Escritura y funciones del archivo binario
void genera_binario(FILE * x); //Genera el archivo binario
void carga_binario(FILE * xx, FILE * x, char * nombre, char * lineaTexto); //Cargo el .dat con los datos extraidos
void opciones_binario(FILE * xx, char * y);
void lista_binario(FILE * xx); //Lista el archivo.dat completo
void lista_heladera(FILE * xx); //Los 3 siguientes, filtran por heladera, televisor y lavarropa
void lista_televisor(FILE * xx);
void lista_lavarropa(FILE * xx);

//Operaciones con clientes
void operaciones_clientes(struct tm x, FILE * xx); //Menu principal
int verifica_orden(int x, FILE * xx); //Pasa por todo el archivo en busca del orden ingresado
void emite_cliente(int ord, FILE * xx); //Emite el cliente segun el orden ingresado
void genera_cliente(struct tm x, int ord, FILE * xx);
int verifica_apellido(char * buscado, FILE * xx); //Idem 2 anteriores, pero por apellido
void emite_cliente_apellido(char * buscado, FILE * xx);
void modifica_importe(int ord, FILE * xx); //Modificacion de importes
creditos baja_cliente(int ord, FILE * xx); //Segun el orden ingresado, elimina el cliente. Automaticamente es cargado en el archivo de bajas
void genera_archivo_bajas(FILE * bajas); //Genera un archivo.xyz desde 0
void carga_archivo_bajas(creditos cargar, FILE * bajas);
void lista_archivo_bajas(FILE * bajas); //Imprime los clientes eliminados por pantalla