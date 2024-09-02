//Archivo principal

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
void limpia_archivo_bajas(FILE * bajas); //Genera un archivo.xyz desde 0
void carga_archivo_bajas(creditos cargar, FILE * bajas);
void lista_archivo_bajas(FILE * bajas); //Imprime los clientes eliminados por pantalla

void bienvenida() {
    printf("-- PRIMER TRABAJO INTEGRADOR -- \n\n");
    printf("Alumno: Cernadas, Nicolas\nComision: C-TI04\nFecha de entrega: 20/05/2024\n");
    printf("Archivos utilizados: Creditos.csv\n                     Creditos.txt\n                     Clientes_bajas.xyz\n");
    printf(" # Aclaraciones # \n Para una mejor visualizacion del trabajo, se recomienda maximizar el tama%co de la consola de comando.\n\n",164);
    printf("===================================================\n");
}
void menu(struct tm x, FILE * punteroArchivoBin, FILE * punteroArchivo, char * lineaTexto) {
    int flag = 0, flag2 = 0, flag3 = 0;
    char * nombre;
    int opcion;
    char s;
    do{
        do{
            opcion = 6; //Inicializo en un valor que no existe, si el usuario ingresa una letra, 'opcion' va a quedar con este valor y repetir el ciclo
            printf("\n -- Menu de opciones -- \n\n1)Para imprimir el archivo: 'creditos.txt' original\n2)Para generar el archivo binario\n3)Para pasar los datos del .txt al .dat\n4)Para imprimir el archivo.dat\n5)Para realizar operaciones con clientes\n0)Para salir del menu de opciones\nSeleccion => ");
            fflush(stdin);
            scanf("%d", &opcion);
            if ((opcion != 1) && (opcion != 2) && (opcion != 3) && (opcion != 4) && (opcion != 5) && (opcion != 0)) {
                printf("\n#ERROR#\nIngresa una opcion valida.");
                printf("\n");
            }
        }while( (48 > opcion) && (opcion > 54));
        printf("\n");
        switch(opcion) {
            case 0: {
                printf("Hasta la proxima!");
                printf("\n");
                getchar();
                exit(0);
            } break;
            case 1: {
                imprime_creditoscsv(punteroArchivo, lineaTexto);
                printf("\n");
            } break;
            case 2: {
                flag = 1;
                genera_binario(punteroArchivoBin);
                getchar();
            } break;
            case 3: {
                if (flag == 0) {
                    printf("\n#ERROR#\nTodavia no generaste el archivo...No tenemos donde cargar los datos!\n");
                } else {
                    flag2 = 1;
                    carga_binario(punteroArchivoBin, punteroArchivo, nombre, lineaTexto);
                    getchar();
                }
            } break;
            case 4: {
                if (flag != 1) {
                    printf("\n#ERROR#\nTodavia no generaste el archivo!\n");
                } else {
                    opciones_binario(punteroArchivoBin,lineaTexto);
                    getchar();
                }
            } break;
            case 5: {
                if (flag != 1) {
                    printf("\n#ERROR#\nTodavia no generaste el archivo!\n");
                } else {
                    operaciones_clientes(x, punteroArchivoBin);
                    getchar();
                }
            } break;
        }
        do {
            printf("\nDesea continuar? (y/n): ");
            fflush(stdin);
            scanf("%c",&s);
            if ((s!='y') && (s!='Y') && (s!='n') && (s!='N')) {
                printf("\n#ERROR#\nIngresa: Y o y para continuar, N o n para salir.\n");
            }
        }while((s!='y') && (s!='Y') && (s!='n') && (s!='N'));
    }while ((s=='y') || (s=='Y'));
}
//Imprimo para visualizacion del archivo: creditos.txt
void imprime_creditoscsv(FILE * x, char * y) {
    int i = 0;
    char * token, * token1, * token2, * token3, * token4, * token5, * token6, * token7, * token8, * token9, * token10, * token11;
    x = fopen("creditos.txt", "r");
    if (x != NULL) {
        printf(" -- Creditos.CSV -- \n\n");
        fgets(y, 2000, x); //Salteo los titulos
        printf("----------------------------------------------------------------------------------------------------------------------------------\n");
        printf("Orden| Cliente            | Importe   | Electro     | Dia  | Mes  | Anio   | Nro.Cuotas  | ImporteCuota  | IVA       | TotalCuota |\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------\n");
        while(!feof(x)) {
            i++; //Fines esteticos
            fgets(y, 2000, x); //Lee la primer linea del archivo
            token = strtok(y, ",;"); //Guarda hasta el primer ';'
            token1 = strtok(NULL, ";"); //guarda hasta el segundo ';'
            token2 = strtok(NULL, ";"); //guarda hasta el tercero ';'
            token3 = strtok(NULL, ";"); //idem para el resto
            token4 = strtok(NULL, ";");
            token5 = strtok(NULL, ";");
            token6 = strtok(NULL, ";");
            token7 = strtok(NULL, ";");
            token8 = strtok(NULL, ";");
            token9 = strtok(NULL, ";");
            token10 = strtok(NULL, ";");
            token11 = strtok(NULL, ";");
            printf("%3s  | %-16s   | %6s    | %9s   | %3s  | %2s   | %5s  | %6s      | %10s    | %-10s| %s ",
                   token, token1, token2, token3, token4, token5, token6, token7, token8, token9, token10, token11);
            if (i == 17) { //Ciclo con fines esteticos, no dar importancia, no influye al codigo
                printf("\n");
            }
            printf("----------------------------------------------------------------------------------------------------------------------------------\n");
        }
    } else {
        printf("\n#ERROR#\nOcurrio algo inesperado...Intentelo nuevamente");
        exit(1);
    }
    fclose(x);
}
//Genera el archivo binario con 'WB'
void genera_binario(FILE * xx) {
    char nombreArch[] = "binario.dat";
    xx = fopen(nombreArch, "wb");
    if (xx == NULL) {
        printf("\n#ERROR#\nOcurrio un error inesperado...Intentelo nuevamente.\n");
        exit(1);
    } else {
        printf("Archivo generado con exito.\n");
        fclose(xx);
    }
}
//Cargo el archivo con los datos del TXT con 'RB+' (lo cual posibilita la carga de clientes previa a la carga de datos del txt al archivo binario)
//NOTA: Los clientes cargados en el orden del 1 al 17, seran pisados por los datos del TXT
void carga_binario(FILE * xx, FILE * x, char * n, char * lineaTexto) {
    creditos personas;
    char * token, * token2;
    x = fopen("creditos.txt", "rt");
    xx = fopen("binario.dat", "rb+");
    if ((x == NULL) || (xx == NULL)) {
        printf("\n#ERROR#\nOcurrio un error inesperado...Intentalo nuevamente.\n");
        exit(1);
    } else {
        rewind(x);
        rewind(xx);
        fgets(lineaTexto, 2000, x);
        while(!feof(x)){
            fgets(lineaTexto, 2000, x);
            token = strtok(lineaTexto, ";");
            personas.orden = atoi(token);
            token = strtok(NULL, ";");
            strcpy(personas.cliente, token);
            token = strtok(NULL, ";");
            personas.importe = atoi(token);
            token = strtok(NULL, ";");
            strcpy(personas.electro,token);
            token = strtok(NULL, ";");
            personas.dia = atoi(token);
            token = strtok(NULL, ";");
            personas.mes = atoi(token);
            token = strtok(NULL, ";");
            personas.anio = atoi(token);
            token = strtok(NULL, ";");
            personas.cuotas = atoi(token);
            token2 = strtok(NULL,",");
            token = strtok(NULL, ";");
            personas.impCuota = atof(token2) + (atof(token)/100);
            token2 = strtok(NULL,",");
            token = strtok(NULL, ";");
            personas.Iva = atof(token2) + (atof(token)/100);
            token2 = strtok(NULL,",");
            token = strtok(NULL, ";");
            personas.totalCuota = atof(token2) + (atof(token)/100);
            fwrite(&personas,sizeof(personas),1,xx);
        }
        printf("Datos cargados exitosamente.\n");
        fclose(x);
        fclose(xx);
    }
}
//Lee el archivo binario con tipos
void opciones_binario(FILE * xx, char * y) {
    int opcion, opcion2;
    do {
        do{
            opcion = 4;
            fflush(stdin);
            printf("\n-- Opciones del archivo -- \n\n");
            printf("1)Listar todos\n2)Por nombre del electrodomestico\n0)Para salir\nSeleccion => ");
            fflush(stdin);
            scanf("%d",&opcion);
            if((opcion!= 1)&&(opcion!= 2)&&(opcion!= 3)&&(opcion!= 0)) {
                printf("\n#ERROR#\nPor favor, selecciona una opcion valida.\n");
            }
        }while((opcion!= 1)&&(opcion!= 2)&&(opcion!= 3)&&(opcion!= 0));
        switch(opcion) {
            case 1: {
                lista_binario(xx);
            } break;
            case 2: {
                do {
                    do{
                        opcion2 = 4; //Valor inexistente previo a la carga, si hay un ingreso erroneo de seleccion, se carga esto y no sale automaticamente
                                    //del while con el valor previamente asignado
                        fflush(stdin);
                        printf("\nElegi el tipo de electrodomestico a visualizar\n1)Heladera\n2)Televisor\n3)Lavarropa\n0)Para volver atras\nSeleccion => ");
                        fflush(stdin);
                        scanf("%d",&opcion2);
                        if ((opcion2 != 1) && (opcion2 != 2) && (opcion2 != 3) && (opcion2 != 0)) {
                            printf("\n#ERROR#\nPor favor, selecciona una opcion valida.\n");
                        }
                    }while((opcion2 != 1) && (opcion2 != 2) && (opcion2 != 3) && (opcion2 != 0));
                    switch(opcion2) {
                        case 1: {
                            lista_heladera(xx);
                            getchar();
                        } break;
                        case 2: {
                            lista_televisor(xx);
                            getchar();
                        } break;
                        case 3: {
                            lista_lavarropa(xx);
                            getchar();
                        } break;
                    }
                }while(opcion2 != 0);
            } break;
            case 0: {
            } break;
        }
    }while(opcion != 0);
}
//FUNCIONES DE IMPRESION BINARIAS
//BINARIO COMPLETO
void lista_binario(FILE * xx){
    creditos personas;
    int i = 0, cant;
    xx = fopen("binario.dat", "rb");
    if (xx == NULL) {
        printf("\n#ERROR#\nOcurrio un error inesperado...Intentelo nuevamente.\n");
        exit(1);
    } else {
        rewind(xx);
        fseek(xx, 0, SEEK_END);
        cant = ftell(xx)/sizeof(personas);
        fseek(xx, 0, SEEK_SET);
        printf("\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------\n");
        printf("Orden| Cliente            | Importe   | Electro     | Dia  | Mes  | Anio   | Nro.Cuotas  | ImporteCuota  | IVA       | TotalCuota |\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------\n");
        while(i < cant) {
            fseek(xx, i*sizeof(personas),SEEK_SET);
            fread(&personas, sizeof(personas),1,xx);
            printf("%3d  | %-16s   | %6d    | %9s   | %3d  | %2d   | %5d  | %6d      | %10.2f    | %-10.2f| %-11.2f|\n",
            personas.orden,personas.cliente,personas.importe,personas.electro,personas.dia,personas.mes,personas.anio,personas.cuotas,personas.impCuota,personas.Iva,personas.totalCuota);
            printf("----------------------------------------------------------------------------------------------------------------------------------\n");
            i++;
        }
        fclose(xx);
    }
}
//BINARIOS INDIVIDUALES, 1)HELADERA, 2)TELEVISOR Y 3)LAVARROPA
void lista_heladera(FILE * xx) {
    creditos personas;
    xx = fopen("binario.dat", "rb");
    if (xx == NULL) {
        printf("\n#ERROR#\nOcurrio un error inesperado...Intentelo nuevamente.\n");
        exit(1);
    } else {
        rewind(xx);
        printf("\n-- Creditos para 'Heladera' --\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------\n");
        printf("Orden| Cliente            | Importe   | Electro     | Dia  | Mes  | Anio   | Nro.Cuotas  | ImporteCuota  | IVA       | TotalCuota |\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------\n");
        fread(&personas, sizeof(personas),1,xx);
        while(!feof(xx)) {
            if ((strcmp(personas.electro,"Heladera")) == 0) {
                printf("%3d  | %-16s   | %6d    | %9s   | %3d  | %2d   | %5d  | %6d      | %10.2f    | %-10.2f| %-11.2f|\n",
                    personas.orden,personas.cliente,personas.importe,personas.electro,personas.dia,personas.mes,personas.anio,personas.cuotas,personas.impCuota,personas.Iva,personas.totalCuota);
                printf("----------------------------------------------------------------------------------------------------------------------------------\n");
                fread(&personas, sizeof(personas),1,xx);
            } else {
                fread(&personas, sizeof(personas),1,xx);
            }
        }
        fclose(xx);
    }
}
void lista_televisor(FILE * xx) {
    creditos personas;
    xx = fopen("binario.dat", "rb");
    if (xx == NULL) {
        printf("\n#ERROR#\nOcurrio un error inesperado...Intentelo nuevamente.\n");
        exit(1);
    } else {
        rewind(xx);
        printf("\n-- Creditos para 'Televisor' --\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------\n");
        printf("Orden| Cliente            | Importe   | Electro     | Dia  | Mes  | Anio   | Nro.Cuotas  | ImporteCuota  | IVA       | TotalCuota |\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------\n");
        fread(&personas, sizeof(personas),1,xx);
        while(!feof(xx)) {
            if ((strcmp(personas.electro,"Televisor")) == 0) {
                printf("%3d  | %-16s   | %6d    | %9s   | %3d  | %2d   | %5d  | %6d      | %10.2f    | %-10.2f| %-11.2f|\n",
                    personas.orden,personas.cliente,personas.importe,personas.electro,personas.dia,personas.mes,personas.anio,personas.cuotas,personas.impCuota,personas.Iva,personas.totalCuota);
                printf("----------------------------------------------------------------------------------------------------------------------------------\n");
                fread(&personas, sizeof(personas),1,xx);
            } else {
                fread(&personas, sizeof(personas),1,xx);
            }
        }
        fclose(xx);
    }
}
void lista_lavarropa(FILE * xx) {
    creditos personas;
    xx = fopen("binario.dat", "rb");
    if (xx == NULL) {
        printf("\n#ERROR#\nOcurrio un error inesperado...Intentelo nuevamente.\n");
        exit(1);
    } else {
        rewind(xx);
        printf("\n-- Creditos para 'Lavarropa' --\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------\n");
        printf("Orden| Cliente            | Importe   | Electro     | Dia  | Mes  | Anio   | Nro.Cuotas  | ImporteCuota  | IVA       | TotalCuota |\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------\n");
        fread(&personas, sizeof(personas),1,xx);
        while(!feof(xx)) {
            if ((strcmp(personas.electro,"Lavarropa")) == 0) {
                printf("%3d  | %-16s   | %6d    | %9s   | %3d  | %2d   | %5d  | %6d      | %10.2f    | %-10.2f| %-11.2f|\n",
                    personas.orden,personas.cliente,personas.importe,personas.electro,personas.dia,personas.mes,personas.anio,personas.cuotas,personas.impCuota,personas.Iva,personas.totalCuota);
                printf("----------------------------------------------------------------------------------------------------------------------------------\n");
                fread(&personas, sizeof(personas),1,xx);
            } else {
                fread(&personas, sizeof(personas),1,xx);
            }
        }
        fclose(xx);
    }
}
//MENU PARA CLIENTES
//Funciones integradas de busqueda en binario
void operaciones_clientes(struct tm x, FILE * xx) {
    creditos bajas_a_cargar;
    FILE * archivo_bajas; //Voy a usar mas adelante
    int flag, orden, opcion, opcion2;
    char apellido[30], selec, cont;
    do {
        do {
            opcion = 8;
            printf(" --  Apartado Clientes --\n");
            printf("\n1)Para dar de alta un cliente nuevo\n2)Para buscar un cliente por numero de orden\n3)Para buscar un cliente por apellido\n4)Modificar el importe del credito de un cliente\n5)Para dar de baja un cliente\n6)Para limpiar el archivo de bajas\n7)Para listar las bajas\n0)Para salir\nSeleccion => ");
            scanf("%d",&opcion);
            if ((opcion!=1) && (opcion!=2) && (opcion!=3) && (opcion != 4) && (opcion != 5) && (opcion != 6) && (opcion != 7) && (opcion != 0)) {
                printf("\n#ERROR#\nIngresa una opcion valida.\n\n");
            }
        }while((48 > opcion) && (opcion > 55));
        switch(opcion){
            case 1: {
                do {
                    printf("\nIngresa el numero de orden: ");
                    fflush(stdin);
                    scanf("%d",&orden);
                    flag = 0;
                    if(orden < 1) {
                        printf("\n#ERROR#\nIngrese un numero de orden valido.\n");
                    }
                }while(orden < 1);
                flag = verifica_orden(orden, xx);
                if (flag == 0) {
                    genera_cliente(x, orden, xx);
                } else {
                    printf("\n#ERROR#\nEse numero de orden ya esta en uso, por favor, elija uno nuevo.\n");
                }
            }break;
            case 2: {
                do {
                    printf("\nIngresa el numero de orden: ");
                    fflush(stdin);
                    scanf("%d",&orden);
                    flag = 0;
                    if(orden < 1) {
                        printf("\n#ERROR#\nIngrese un numero de orden valido.\n");
                    }
                }while(orden < 1);
                flag = verifica_orden(orden, xx);
                if (flag == 1) {
                    emite_cliente(orden, xx);
                } else {
                    printf("\n#ERROR#\nNo hay ningun cliente asociado a ese numero de orden.\n");
                }
            } break;
            case 3: {
                printf("\n#Aclaracion importante#\nEl formato de busqueda es el siguiente: 'nombre' 'segundo nombre' 'apellido'\n");
                printf("Si usted posee 2 apellidos o contiene espacios en el mismo, ingresar la segunda parte: ej => Juan Gomez Perez ('Perez' seria el apellido a ingresar)\n");
                printf("\nIngresa el apellido a buscar: ");
                fflush(stdin);
                scanf("%s", apellido);
                flag = verifica_apellido(apellido, xx);
                if (flag == 1) {
                    emite_cliente_apellido(apellido, xx); //GENERAR FUNCION, YA FUNCIONA (FALTABA ESPACIO)
                } else {
                    printf("\n#ERROR#\nNo existe ningun cliente con ese apellido.\n");
                }
            } break;
            case 4: {
                do {
                    printf("\nIngresa el numero de orden: ");
                    fflush(stdin);
                    scanf("%d",&orden);
                    flag = 0;
                    if(orden < 0) {
                        printf("\n#ERROR#\nIngrese un numero de orden valido.\n");
                    }
                }while(orden < 0);
                flag = verifica_orden(orden, xx);
                if (flag == 1) {
                    modifica_importe(orden, xx);
                } else {
                    printf("\n#ERROR#\nNo hay ningun cliente asociado a ese numero de orden.\n");
                }
            } break;
            case 5: {
                do {
                    printf("\nIngresa el numero de orden: ");
                    fflush(stdin);
                    scanf("%d",&orden);
                    flag = 0;
                    if(orden < 0) {
                        printf("\n#ERROR#\nIngrese un numero de orden valido.\n");
                    }
                }while(orden < 0);
                flag = verifica_orden(orden, xx);
                if (flag == 1) {
                    do {
                        printf("\nEsta seguro de que desea dar de baja al cliente de orden numero: %d ? (Y/n)\n", orden);
                        emite_cliente(orden, xx);
                        printf("\nSeleccion => ");
                        fflush(stdin);
                        scanf("%c",&selec);
                        if ((selec != 'Y') && (selec != 'y') && (selec != 'N') && (selec != 'n')) {
                            printf("\n#ERROR#\nPor favor, ingresa una opcion valida.\n");
                        }
                    }while((selec != 'Y') && (selec != 'y') && (selec != 'N') && (selec != 'n'));
                    switch(selec) {
                        case 'Y': {
                            bajas_a_cargar = baja_cliente(orden, xx);
                            carga_archivo_bajas(bajas_a_cargar, archivo_bajas);
                            do {
                                opcion2 = 4;
                                printf("\nPresione\n1)Si desea visualizar el Archivo.dat actualizado\n2)Si desea visualizar el archivo de bajas actualizado\n0)Si desea continuar sin visualizacion\nSeleccion => ");
                                scanf("%d",&opcion2);
                                if((opcion2 != 1)&&(opcion2 != 2)&&(opcion2 != 0)) {
                                    printf("\n#ERROR#\nPor favor, ingrese una opcion valida\n");
                                }
                            }while((opcion2 < 48) && (opcion2 > 51));
                            switch(opcion2) {
                                case 1: {
                                    lista_binario(xx);
                                } break;
                                case 2: {
                                    lista_archivo_bajas(archivo_bajas);
                                } break;
                                case 0: {

                                }
                            }
                        } break;
                        case 'y': {
                            bajas_a_cargar = baja_cliente(orden, xx);
                            carga_archivo_bajas(bajas_a_cargar, archivo_bajas);
                            do {
                                opcion2 = 4;
                                printf("\nPresione\n1)Si desea visualizar el Archivo.dat actualizado\n2)Si desea visualizar el archivo de bajas actualizado\n0)Si desea continuar sin visualizacion\nSeleccion => ");
                                scanf("%d",&opcion2);
                                if((opcion2 != 1)&&(opcion2 != 2)&&(opcion2 != 0)) {
                                    printf("\n#ERROR#\nPor favor, ingrese una opcion valida\n");
                                }
                            }while((opcion2 < 48) && (opcion2 > 51));
                            switch(opcion2) {
                                case 1: {
                                    lista_binario(xx);
                                } break;
                                case 2: {
                                    lista_archivo_bajas(archivo_bajas);
                                } break;
                                case 0: {

                                }
                            }
                        } break;
                        case 'n': break;
                        case 'N': break;
                    }
                } else {
                    printf("\n#ERROR#\nNo hay ningun cliente asociado a ese numero de orden.\n");
                }
            } break;
            case 6: {
                limpia_archivo_bajas(archivo_bajas);
            } break;
            case 7: {
                lista_archivo_bajas(archivo_bajas);
            } break;
        }
        do {
            printf("\nDesea realizar alguna operacion mas con clientes? (Y/n)\nSeleccion => ");
            fflush(stdin);
            scanf("%c",&cont);
            printf("\n");
            if ((cont!='y') && (cont!='Y') && (cont!='n') && (cont!='N')) {
                printf("\n#ERROR#\nPor favor, ingresa una opcion valida.\n");
            } else {}
        } while((cont!='y') && (cont!='Y') && (cont!='n') && (cont!='N'));
    }while(cont == 'y' || cont == 'Y');
}
//FUNCION DE RECORRIDO
//Recorre todo el archivo desde el principio, si coincide el orden, emite falla.
int verifica_orden(int x, FILE * xx) {
    creditos personas;
	int flag;
	xx = fopen("binario.dat","rb");
    if (xx == NULL) {
        printf("\n#ERROR#\nOcurrio un error inesperado...Intentelo nuevamente.");
        exit(1);
    } else {
    	rewind(xx);
    	flag = 0;
    	fread(&personas,sizeof(personas),1,xx);
        while(!feof(xx)) {
            if (personas.orden == x) {
                flag = 1;
                break; //Rompo el ciclo por que no puede haber 2 personas con el mismo numero de orden, entonces mi busqueda termino
            }
            fread(&personas,sizeof(personas),1,xx);
        }
        fclose(xx);
    }
    return flag;
}
void emite_cliente(int ord, FILE * xx) {
    xx = fopen("binario.dat","rb");
    creditos personas;
    if (xx == NULL) {
        printf("\n#ERROR#\nOcurrio un error inesperado...Intentalo denuevo.\n");
        exit(1);
    } else {
        rewind(xx);
        fread(&personas, sizeof(personas),1, xx);
        while (!feof(xx)) {
            if (personas.orden == ord) {
                printf("\n");
                printf("----------------------------------------------------------------------------------------------------------------------------------\n");
                printf("Orden| Cliente            | Importe   | Electro     | Dia  | Mes  | Anio   | Nro.Cuotas  | ImporteCuota  | IVA       | TotalCuota |\n");
                printf("----------------------------------------------------------------------------------------------------------------------------------\n");
                printf("%3d  | %-16s   | %6d    | %9s   | %3d  | %2d   | %5d  | %6d      | %10.2f    | %-10.2f| %-11.2f|\n",
                    personas.orden,personas.cliente,personas.importe,personas.electro,personas.dia,personas.mes,personas.anio,personas.cuotas,personas.impCuota,personas.Iva,personas.totalCuota);
                printf("----------------------------------------------------------------------------------------------------------------------------------\n");
                break;
            } else {
                fread(&personas, sizeof(personas),1, xx);
            }
        }
        fclose(xx);
    }
}
//Funcion de carga de clientes con 'RB+'
void genera_cliente(struct tm x, int ord, FILE * xx) {
    char nombre[30];
    int opcion, importe, fecha;
    xx = fopen("binario.dat","rb+");
    creditos nuevocliente;
    if (xx == NULL) {
        printf("\n#ERROR#\nOcurrio un error...Intentalo nuevamente");
        exit(1);
    } else {
        fseek(xx, 0, SEEK_SET);
        fseek(xx, (ord-1)*sizeof(nuevocliente), SEEK_CUR);
        nuevocliente.orden = ord;
        printf("Ingresa el nombre del cliente: ");
        fflush(stdin);
        gets(nombre);
        strcpy(nuevocliente.cliente, nombre);
        printf("Ingresa el importe: ");
        fflush(stdin);
        scanf("%d",&nuevocliente.importe);
        printf("Ingresa\n'1' Para 'Heladera'\n'2' Para 'Televisor'\n'3' Para 'Lavarropa'\nSeleccion => ");
        fflush(stdin);
        scanf("%d",&opcion);
        switch(opcion) {
            case 1: {
                strcpy(nuevocliente.electro, "Heladera");
            } break;
            case 2: {
                strcpy(nuevocliente.electro, "Televisor");
            } break;
            case 3: {
                strcpy(nuevocliente.electro, "Lavarropa");
            } break;
        }
        do {
            printf("Ingresa\n1)Si queres cargar la fecha del dia\n2)Si queres cargar una fecha en particular\nSeleccion => ");
            scanf("%d",&fecha);
            switch(fecha) {
                case 1: {
                    nuevocliente.dia = x.tm_mday;
                    nuevocliente.mes = (x.tm_mon + 1);//Por que cuenta desde 0 como cualquier array
                    nuevocliente.anio = (x.tm_year + 1900); //Por que cuenta los anios desde el 1900
                } break;
                case 2: {
                    printf("\nFormato de carga dia / mes / anio = x / x / xxxx\n");
                    do {
                        fflush(stdin);
                        printf("Ingresa el anio de alta: ");
                        scanf("%d",&nuevocliente.anio);
                        if ((nuevocliente.anio < 1900) || (nuevocliente.anio > (x.tm_year + 1900))) {
                            printf("\n#ERROR#\nError en el ingreso!\nO el numero ingresado es muy grande/chico\nO no es un numero!, Intentalo nuevamente\n\n");
                        }
                    }while((nuevocliente.anio < 1900) || (nuevocliente.anio > (x.tm_year + 1900)));
                    if (nuevocliente.anio == 2024) {
                        do {
                            fflush(stdin);
                            printf("Ingresa el mes: ");
                            scanf("%d",&nuevocliente.mes);
                            if ((nuevocliente.mes < 0) || (nuevocliente.mes > (x.tm_mon + 1))) {
                                printf("\n#ERROR#\nError en el ingreso!\nO el numero ingresado es muy grande/chico\nO no es un numero!, Intentalo nuevamente\n\n");
                            }
                        }while((nuevocliente.anio < 0) || (nuevocliente.mes > (x.tm_mon + 1)));
                        do {
                            fflush(stdin);
                            printf("Ingresa el dia: ");
                            scanf("%d",&nuevocliente.dia);
                            if ((nuevocliente.dia < 0) || (nuevocliente.dia > (x.tm_mday))) {
                                printf("\n#ERROR#\nError en el ingreso!\nO el numero ingresado es muy grande/chico\nO no es un numero!, Intentalo nuevamente\n\n");
                            }
                        }while((nuevocliente.anio < 0) || (nuevocliente.dia > (x.tm_mday)));
                    } else {
                        do {
                            fflush(stdin);
                            printf("Ingresa el mes: ");
                            scanf("%d",&nuevocliente.mes);
                            if ((1 > nuevocliente.mes) || (nuevocliente.mes > 12)) {
                                printf("\n#ERROR#\nError en el ingreso!\nO el numero ingresado es muy grande/chico\nO no es un numero!, Intentalo nuevamente\n\n");
                            }
                        }while((1 > nuevocliente.mes) || (nuevocliente.mes > 12));
                        do {
                            fflush(stdin);
                            printf("Ingresa el dia: ");
                            scanf("%d",&nuevocliente.dia);
                            if ((1 > nuevocliente.dia) || (nuevocliente.dia > 31)) {
                                printf("\n#ERROR#\nError en el ingreso!\nO el numero ingresado es muy grande/chico\nO no es un numero!, Intentalo nuevamente\n\n");
                            }
                        }while((1 > nuevocliente.dia) || (nuevocliente.dia > 31));
                    }
                } break;
            }
        }while((fecha!=1) && (fecha!=2));
        printf("En cuantas cuotas lo va a pagar?: ");
        fflush(stdin);
        scanf("%d",&nuevocliente.cuotas);
        nuevocliente.impCuota = (nuevocliente.importe/nuevocliente.cuotas);
        nuevocliente.Iva = ((nuevocliente.importe*1.21) - nuevocliente.importe);
        nuevocliente.totalCuota = nuevocliente.impCuota + nuevocliente.Iva;
        fwrite(&nuevocliente,sizeof(nuevocliente),1,xx);
        printf("\nNuevo cliente generado con exito!\n");
        fclose(xx);
    }
}
//Funcion de recorrido por APELLIDO
int verifica_apellido(char  * buscado, FILE * xx) {
    int flag = 0;
    char * token, * token2, nombre[30];
    creditos personas;
    xx = fopen("binario.dat","rb");
    if (xx == NULL) {
        printf("\n#ERROR#\nOcurrio un error inesperado...Intentalo denuevo.\n");
        exit(1);
    } else {
        fseek(xx, 0, SEEK_SET);
        fread(&personas, sizeof(personas),1, xx);
        while(!feof(xx)) {
            if (strcmp(personas.cliente, "" ) == 0) {
                fread(&personas, sizeof(personas),1,xx);
            } else {
                strcpy(nombre, personas.cliente);
                token = strtok(nombre, " ");
                fflush(stdin);
                token = strtok(NULL, " ");
                fflush(stdin);
                token2 = strtok(NULL, " ");
                 if (token2 == NULL) {
                    if (strcmp(buscado, token) == 0) {
                        flag = 1;
                        break;
                    } else {
                        fread(&personas, sizeof(personas),1,xx);
                    }
                } else {
                    if (strcmp(buscado, token2) == 0) {
                        flag = 1;
                        break;
                    } else {
                        fread(&personas, sizeof(personas),1,xx);
                    }
                }
            }
        }
        fclose(xx);
    }
    return flag;
}

//Emision de clientes cuando se busca por apellido y no por numero de orden
void emite_cliente_apellido(char * buscado, FILE * xx) {
    int i=0;
    char * token, * token2, nombre[30];
    creditos personas;
    xx = fopen("binario.dat","rb");
    if (xx == NULL) {
        printf("\n#ERROR#\nOcurrio un error inesperado...Intentalo denuevo.\n");
        exit(1);
    } else {
        fseek(xx, 0, SEEK_SET);
        fread(&personas, sizeof(personas),1, xx);
        while(!feof(xx)) {
            if (strcmp(personas.cliente, "" ) == 0) {
                fread(&personas, sizeof(personas),1,xx);
            } else {
                strcpy(nombre, personas.cliente);
                token = strtok(nombre, " ");
                fflush(stdin);
                token = strtok(NULL, " ");
                fflush(stdin);
                token2 = strtok(NULL, " ");
                if (token2 == NULL) {
                    if (strcmp(buscado, token) == 0) {
                        if (i==0){
                            printf("\n");
                            printf("----------------------------------------------------------------------------------------------------------------------------------\n");
                            printf("Orden| Cliente            | Importe   | Electro     | Dia  | Mes  | Anio   | Nro.Cuotas  | ImporteCuota  | IVA       | TotalCuota |\n");
                            printf("----------------------------------------------------------------------------------------------------------------------------------\n");
                            printf("%3d  | %-16s   | %6d    | %9s   | %3d  | %2d   | %5d  | %6d      | %10.2f    | %-10.2f| %-11.2f|\n",
                                personas.orden,personas.cliente,personas.importe,personas.electro,personas.dia,personas.mes,personas.anio,personas.cuotas,personas.impCuota,personas.Iva,personas.totalCuota);
                            printf("----------------------------------------------------------------------------------------------------------------------------------\n");
                            i++;
                        } else {
                            printf("%3d  | %-16s   | %6d    | %9s   | %3d  | %2d   | %5d  | %6d      | %10.2f    | %-10.2f| %-11.2f|\n",
                                personas.orden,personas.cliente,personas.importe,personas.electro,personas.dia,personas.mes,personas.anio,personas.cuotas,personas.impCuota,personas.Iva,personas.totalCuota);
                            printf("----------------------------------------------------------------------------------------------------------------------------------\n");
                        }
                        //No rompo el ciclo por si hay mas de 1 persona con el mismo apellido
                        fread(&personas, sizeof(personas),1,xx);
                    } else {
                        fread(&personas, sizeof(personas),1,xx);
                    }
                } else {
                    if (strcmp(buscado, token2) == 0) {
                        if (i==0){
                            printf("\n");
                            printf("----------------------------------------------------------------------------------------------------------------------------------\n");
                            printf("Orden| Cliente            | Importe   | Electro     | Dia  | Mes  | Anio   | Nro.Cuotas  | ImporteCuota  | IVA       | TotalCuota |\n");
                            printf("----------------------------------------------------------------------------------------------------------------------------------\n");
                            printf("%3d  | %-16s   | %6d    | %9s   | %3d  | %2d   | %5d  | %6d      | %10.2f    | %-10.2f| %-11.2f|\n",
                                personas.orden,personas.cliente,personas.importe,personas.electro,personas.dia,personas.mes,personas.anio,personas.cuotas,personas.impCuota,personas.Iva,personas.totalCuota);
                            printf("----------------------------------------------------------------------------------------------------------------------------------\n");
                            i++;
                        } else {
                            printf("%3d  | %-16s   | %6d    | %9s   | %3d  | %2d   | %5d  | %6d      | %10.2f    | %-10.2f| %-11.2f|\n",
                                personas.orden,personas.cliente,personas.importe,personas.electro,personas.dia,personas.mes,personas.anio,personas.cuotas,personas.impCuota,personas.Iva,personas.totalCuota);
                            printf("----------------------------------------------------------------------------------------------------------------------------------\n");
                        }
                        //No rompo el ciclo por si hay mas de 1 persona con el mismo apellido
                        fread(&personas, sizeof(personas),1,xx);
                    } else {
                        fread(&personas, sizeof(personas),1,xx);
                    }
                }
            }
        }
        fclose(xx);
    }
}

void modifica_importe(int ord, FILE * xx) {
    int cant;
    char op;
    xx = fopen("binario.dat","r+b");
    creditos personas;
    if (xx == NULL) {
        printf("\n#ERROR#\nOcurrio un error inesperado...Intentelo nuevamente.\n");
    } else {
        fread(&personas, sizeof(personas), 1, xx);
        while(!feof(xx)) {
            if (personas.orden == ord) {
                do {
                    printf("\nSeguro que desea modificar el importe de %s? (Y/N)\n",personas.cliente);
                    printf("Importe anterior: %d\nSeleccion => ", personas.importe);
                    fflush(stdin);
                    scanf("%c",&op);
                    if ((op != 'y') && (op != 'Y') && (op != 'N') && (op != 'n')) {
                        printf("\n#ERROR#\nEsa no es una opcion valida...Ingresa 'Y' para seguir, o 'N' para cancelar\n");
                    }
                }while((op != 'y') && (op != 'Y') && (op != 'N') && (op != 'n'));
                if ((op == 'y') || (op == 'Y')) {
                    printf("\nIngrese el nuevo importe: ");
                    scanf("%d",&personas.importe);
                    personas.impCuota = (float)(personas.importe/personas.cuotas);
                    personas.Iva = (float)((personas.importe*1.21) - personas.importe);
                    personas.totalCuota = personas.impCuota + personas.Iva;
                    cant = ftell(xx) - sizeof(personas);
                    fseek(xx, cant, SEEK_SET);
                    fwrite(&personas,sizeof(personas),1,xx);
                    printf("\nImporte modificado con exito!\n");
                    break;
                } else {
                    break;
                }
            } else {
                fread(&personas, sizeof(personas), 1, xx);
            }
        }
        fclose(xx);
    }
}
//Baja de clientes
creditos baja_cliente(int ord, FILE * xx) {
    creditos bajas;
    xx = fopen("binario.dat","rb+");
    creditos personas;
    if (xx == NULL) {
        printf("\n#ERROR#\nOcurrio un error inesperado...Intentelo nuevamente.\n");
        exit(1);
    } else {
        fseek(xx, 0, SEEK_SET);
        fseek(xx, (ord-1)*sizeof(personas), SEEK_SET);
        fread(&bajas, sizeof(bajas),1, xx); //cargo el struct con los datos de la persona que se va a eliminar
        fseek(xx, 0, SEEK_SET);
        fseek(xx, (ord-1)*sizeof(personas), SEEK_SET);
        fread(&personas, sizeof(personas),1, xx);
        personas.orden = 0;
        strcpy(personas.cliente, "");
        personas.importe = 0;
        strcpy(personas.electro, "");
        personas.dia = 0;
        personas.mes = 0;
        personas.anio = 0;
        personas.cuotas = 0;
        personas.impCuota = 0;
        personas.Iva = 0;
        personas.totalCuota = 0;
        fseek(xx,-sizeof(personas),SEEK_CUR);
        fwrite(&personas,sizeof(personas),1, xx);
        fclose(xx);
    }
    return bajas;
}
//Limpia el archivo de bajas generandolo nuevamente.
//Se pierde el contenido grabado anteriormente
void limpia_archivo_bajas(FILE * bajas) {
    bajas = fopen("clientes_bajas.xyz","wb");
    if (bajas == NULL) {
        printf("\n#ERROR#\nOcurrio un error inesperado...Intentelo nuevamente.\n");
    } else {
        printf("\nLimpieza finalizada!\n");
        fclose(bajas);
    }
}
//Cargo las bajas en el archivo
void carga_archivo_bajas(creditos cargar, FILE * bajas) {
    bajas = fopen("clientes_bajas.xyz","ab");
    if (bajas == NULL) {
        printf("\n#ERROR#\nOcurrio un error inesperado...Intentelo nuevamente.\n");
    } else {
        fwrite(&cargar, sizeof(cargar),1, bajas);
        fclose(bajas);
    }
}
//Imprimo en pantalla
void lista_archivo_bajas(FILE * bajas) {
    bajas = fopen("clientes_bajas.xyz","rb");
    creditos lista_bajas;
    if (bajas == NULL) {
        printf("\n#ERROR#\nOcurrio un error inesperado...El archivo no existe\nAun no hay clientes dados de baja\nComience por dar de baja a alguien para visualizar el archivo.\n");
    } else {
        fseek(bajas, 0, SEEK_SET);
        fread(&lista_bajas, sizeof(lista_bajas),1, bajas);
        printf("\n -- Clientes dados de baja -- \n");
        printf("----------------------------------------------------------------------------------------------------------------------------------\n");
        printf("Orden| Cliente            | Importe   | Electro     | Dia  | Mes  | Anio   | Nro.Cuotas  | ImporteCuota  | IVA       | TotalCuota |\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------\n");
        while(!feof(bajas)) {
            printf("%3d  | %-16s   | %6d    | %9s   | %3d  | %2d   | %5d  | %6d      | %10.2f    | %-10.2f| %-11.2f|\n",
                lista_bajas.orden,lista_bajas.cliente,lista_bajas.importe,lista_bajas.electro,lista_bajas.dia,lista_bajas.mes,lista_bajas.anio,lista_bajas.cuotas,lista_bajas.impCuota,lista_bajas.Iva,lista_bajas.totalCuota);
            printf("----------------------------------------------------------------------------------------------------------------------------------\n");
            fread(&lista_bajas, sizeof(lista_bajas),1,bajas);
        }
        fclose(bajas);
    }
}