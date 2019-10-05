/* Pareja 2: Juan Martin, David Palomo */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char *laca = NULL;
	int i, j, flag, origen, destino, actual = 0, max_input, max_table, est_final;
	FILE *f = NULL;
	char *line = NULL;
	size_t len = 0;
	char **array = NULL;
	char simbolo;

	f = fopen("dia1.txt", "r");
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

	/* Reservar para cadena y tabla */
	laca = (char *) calloc(max_input, sizeof(char));
	if(!laca) return 1;
	array = (char **) calloc(max_table, sizeof(char*));
	if(!array) {
		free(laca);
		return 1;
	}
	for(i=0; i<max_table; i++) {
		array[i] = (char *) calloc(max_table, sizeof(char));
		if(!(array[i])) {
			free(laca);
			for(j=0; j<i; j++) free(array[j]);
			free(array);
			return 1;
		}
	}

	/* Rellena el array con los valores del archivo */
	while ((getline(&line, &len, f)) != -1) {
		origen = line[0] - '0';
		simbolo = line[1];
		destino = line[2] - '0';
		array[origen][destino] = simbolo;
	}
	fclose(f);

	/* Pide cadena a usuario */
	printf("Introduce una cadena de a's y b's, de hasta %d caracteres:\n", max_input);
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
		if(laca[i]!='a' && laca[i]!='b') {
			printf("\nError, Introduce solo a's o b's\n");
			free(laca);
			for(j=0; j<max_table; j++) free(array[j]);
			free(array);
			return 1;
		}
	}

	/* Lectura de simbolos y salto por estados */
	for(i=0; i<strlen(laca); i++) {
		printf("\nEstado:\tq%d\n", actual);
		flag = 0;
		printf("Leido:\t%c\n", laca[i]);
		for(j=0; j<max_table; j++) {
			if(array[actual][j] == laca[i]) {
				actual = j;
				flag = 1;
				printf("Salto:\tq%d\n", actual);
				break;
			}
		}
		/* Si no habia ese simbolo en la fila, game over */
		if(flag == 0) {
			printf("Salto:\tNo.\n\nCadena no reconocida\n");
			free(laca);
			for(j=0; j<max_table; j++) free(array[j]);
			free(array);
			return 1;
		}
	}

	/* Ya se ha leido toda la cadena. Estado es final? */
	if(actual == est_final)
		printf("\nCADENA RECONOCIDA!!\n");
	else
		printf("\nCadena no reconocida\n");

	free(laca);
	for(j=0; j<max_table; j++) free(array[j]);
	free(array);
	return 0;
}
