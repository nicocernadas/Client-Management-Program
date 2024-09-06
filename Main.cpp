//Archivo MAIN del 1° trabajo integrador
//milibreria.h contiene los headers de las funciones utilizadas
//milibreria.cpp las funciones definidas

#include "miLibreria.cpp"

int main() {
    FILE * ptfile;
    FILE * pbfile;
    char linea[2000];
    //Generacion de estructura de tiempo para cargar automaticamente si es deseado mas adelante
    time_t fecha = time(NULL);
    struct tm date = *localtime(&fecha);

    bienvenida();
    menu(date, pbfile, ptfile, linea);

    printf("\n");
    return 0;
}
