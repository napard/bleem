#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    FILE *salida;
    char buff_temp[50];
    
    if(argc < 2)
    {
        printf("Uso: ./cr_c <nombre_de_archivo> <descripción>\n");
        exit(0);
    }
    
    if((salida = fopen(argv[1], "w")) == NULL)
    {
        printf("Error al crear archivo '%s'.\n", argv[1]);
        exit(-1);
    }

    fprintf(salida, "/*\n");
    fprintf(salida, " * %s\n", argv[1]);
    fprintf(salida, " * %s\n", argv[2]);
    fprintf(salida, " *\n");
    fprintf(salida, " * ");
    fclose(salida);
    
    sprintf(buff_temp, "echo `date \"+%%d %%b %%Y\"` -- `date \"+%%H:%%M %%Z\"` >> %s", argv[1]);
    system(buff_temp);
    
    salida = fopen(argv[1], "a");
    fprintf(salida, " * Notes:\n");
    fprintf(salida, " */\n");
    
    fclose(salida);
}

