/* Pareja 2: Juan Martin, David Palomo */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char *laca = NULL;
	int i, j, t, flag, origen, destino, max_input, max_table, est_final, max_trans, trans, num_e = 1, max_actual, tam_temporal, e;
	FILE *f = NULL;
	char *line = NULL;
	size_t len = 0;
	char ***array = NULL;
	int *temporal = NULL, *actual = NULL;
	char simbolo;

	if (argc < 2) {
		printf("--- HELP ---\nUtiliza \"./programa archivo_de_texto\"\n");
		return 1;
	}

	f = fopen(argv[1], "r");
	if(!f) {
		printf("\nError con el archivo de texto\n");
		return 1;
	}

	/* max_input, max_table, est_final */
	getline(&line, &len, f);
	max_input = atoi(line);
	getline(&line, &len, f);
	max_table = atoi(line);
	getline(&line, &len, f);
	est_final = atoi(line);
	getline(&line, &len, f);
	max_trans = atoi(line); /*Maximo numero de transiciones de q_tal a q_pascual*/
	getline(&line, &len, f);
	max_actual = atoi(line); /*Max numero estados actuales*/

	/* Reservar para cadena y tabla */
	laca = (char *) calloc(max_input, sizeof(char));
	if(!laca) return 1;
	array = (char ***) calloc(max_table, sizeof(char **));
	if(!array) {
		free(laca);
		return 1;
	}
	for(i=0; i<max_table; i++) {
		array[i] = (char **) calloc(max_table, sizeof(char *));
		if(!(array[i])) {
			free(laca);
			for(j=0; j<i; j++) free(array[j]);
			free(array);
			return 1;
		}
		for(j=0; j<max_table; j++) {
			array[i][j] = (char *) calloc(max_trans, sizeof(char));
			if(!(array[i][j])) {
				/* TODO: CONTROL ERRORES AQUI */
				return 1;
			}
		}
	}

	/* Rellena el array con los valores del archivo */
	while ((getline(&line, &len, f)) != -1) {
		origen = line[0] - '0';
		simbolo = line[1];
		destino = line[2] - '0';
		trans = (int) strlen(array[origen][destino]);
		array[origen][destino][trans] = simbolo;
	}
	fclose(f);

	/* Pide cadena a usuario */
	printf("Introduce una cadena de letras de la 'a' a la 'z', de hasta %d caracteres:\n", max_input);
	scanf("%s", laca);

	/* Control de errores de entrada de usuario */
	if(strlen(laca) > max_input) {
		printf("\nTe dije hasta %d caracteres...\n", max_input);
		free(laca);
		for(j=0; j<max_table; j++) free(array[j]);
		free(array);
		return 1;
	}
	for(i=0; i<strlen(laca); i++) {
		if(laca[i]<'a' || laca[i]>'z') {
			printf("\nTe dije letras de la 'a' a la 'z'...\n");
			free(laca);
			for(j=0; j<max_table; j++) free(array[j]);
			free(array);
			return 1;
		}
	}

	/*Reservar para array de estados actuales y temporal*/
	actual = (int *) calloc(max_actual, sizeof(int));
	temporal = (int *) calloc(max_actual, sizeof(int));
	if(!actual || !temporal) {
		/*TODO: CDE*/
		return 1;
	}

	/* Lectura de simbolos y salto por estados */
	for(i=0; i<strlen(laca); i++) {
		printf("__________________\n");
		tam_temporal = 0;
		for(e=0; e<num_e; e++) { /*num_e: numero de estados actuales*/
			printf("\nEstado[%d]:\tq%d\n", e, actual[e]);
			printf("Simbolo leido:\t%c\n", laca[i]);
			for(j=0; j<max_table; j++) {
				trans = (int) strlen(array[actual[e]][j]); /*num transiciones desde ese estado*/
				for(t=0; t<trans; t++) {
					if(array[actual[e]][j][t] == laca[i]) {
						printf("Salto a:\tq%d\n", j);
						temporal[tam_temporal] = j; /*METO EN EL ARRAY TEMPORAL EL ESTADO J*/
						tam_temporal++;
					}
				}
			}
		}
		if(tam_temporal == 0) {
			num_e = 0;
			break; /*SI NO HAY MAS CAMBIOS DE ESTADO, SALGO DEL FOR (CASO SUMIDERO)*/
		}
		/*UNA VEZ RECORRIDOS TODOS LOS ACTUALES, SUSTITUYO ACTUAL POR TEMPORAL Y VACIO TEMPORAL */
		for(e=0; e<tam_temporal; e++) {
			actual[e] = temporal[e];
			num_e = tam_temporal;
		}
	}

	/* Ya se ha leido toda la cadena. Algun estado es final? */
	for(e=0; e<num_e; e++) { /*num_e: numero de estados actuales*/
		if(actual[e] == est_final) {
			printf("__________________\n\nCADENA RECONOCIDA!!\n");
			free(laca);
			for(j=0; j<max_table; j++) free(array[j]);
			free(array);
			return 0;
		}
	}

	printf("__________________\n\nCadena no reconocida\n");
	free(laca);
	for(j=0; j<max_table; j++) free(array[j]);
	free(array);
	return 0;
}
