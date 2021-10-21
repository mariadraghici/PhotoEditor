#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
typedef struct {
	int r, g, b;
} color;
// Functia de salvare
void save(char text[3], color **clr,
	int line, int col, char a[1000][1000], int intensity, int l)
{
	if (l == 0) {
		printf("No image loaded\n");
	} else {
		printf("Saved %s\n", a[1]);
		// Daca trebuie salvat in format ascii
		if (strcmp(a[2], "ascii") == 0) {
			if (strcmp(text, "P2") == 0 || strcmp(text, "P5") == 0) {
				FILE *out = fopen(a[1], "wt");
				fprintf(out, "P2\n");
				fprintf(out, "%d ", col);
				fprintf(out, "%d\n", line);
				fprintf(out, "%d\n", intensity);
				for (int i = 0; i < line; i++) {
					for (int j = 0; j < col; j++)
						fprintf(out, "%d ", clr[i][j].r);
					fprintf(out, "\n");
				}
				fclose(out);
				}
			if (strcmp(text, "P3") == 0 || strcmp(text, "P6") == 0) {
				FILE *out = fopen(a[1], "wt");
				fprintf(out, "P3\n");
				fprintf(out, "%d ", col);
				fprintf(out, "%d\n", line);
				fprintf(out, "%d\n", intensity);
				for (int i = 0; i < line; i++) {
					for (int j = 0; j < col; j++) {
						fprintf(out, "%d ", clr[i][j].r);
						fprintf(out, "%d ", clr[i][j].g);
						fprintf(out, "%d ", clr[i][j].b);
					}
				fprintf(out, "\n");
				}
				fclose(out);
			}
		} else {//daca trebuie salvat in format binar
			if (strcmp(text, "P2") == 0 || strcmp(text, "P5") == 0) {
				FILE *out = fopen(a[1], "wb");
				fprintf(out, "P5\n");
				fprintf(out, "%d %d\n", col, line);
				fprintf(out, "%d\n", intensity);
				// afisez un singur camp deoarece e nu e poza color
				for (int i = 0; i < line; i++)
					for (int j = 0; j < col; j++)
						fwrite(&clr[i][j].r, sizeof(char), 1, out);
				fclose(out);
				}

			if (strcmp(text, "P3") == 0 || strcmp(text, "P6") == 0) {
				FILE *out = fopen(a[1], "wb");
				fprintf(out, "P6\n");
				fprintf(out, "%d %d\n", col, line);
				fprintf(out, "%d\n", intensity);
				for (int i = 0; i < line; i++)
					for (int j = 0; j < col; j++) {
						// afisez toate campurile deoarece e poza colora
						fwrite(&clr[i][j].r, sizeof(char), 1, out);
						fwrite(&clr[i][j].g, sizeof(char), 1, out);
						fwrite(&clr[i][j].b, sizeof(char), 1, out);
					}
			fclose(out);
		}
	}
}
}
// Efect grayscale
void grayscale(color **clr, int x1, int y1,
				int x2, int y2, char text[3], int l)
{// Daca nu este incarcata imaginea
	if (l == 0) {
		printf("No image loaded\n");
	} else {
		if ((strcmp(text, "P2") == 0) || strcmp(text, "P5") == 0)
			printf ("Grayscale filter not available\n");
		else {
			printf("Grayscale filter applied\n");
			for (int i = y1; i < y2; i++)
				for (int j = x1; j < x2; j++) {
					double new_value;
					// Calculez noua valoare a culorilor
					new_value = (clr[i][j].r + clr[i][j].g + clr[i][j].b) / 3.0;
					// Verific daca trece valoarea maxima
					if (new_value > 255)
						new_value = 255;
					// Atribui noile valori
					clr[i][j].r = round(new_value);
					clr[i][j].g = round(new_value);
					clr[i][j].b = round(new_value);
				}
			}
	}
}
void sepia(color **clr, int x1, int y1, int x2, int y2, char text[3], int l)
{
	if (l == 0)
		printf("No image loaded\n");
	else {
	if ((strcmp(text, "P2") == 0) || strcmp(text, "P5") == 0)
		printf ("Sepia filter not available\n");
	else {
			printf("Sepia filter applied\n");
			for (int i = y1; i < y2; i++)
				for (int j = x1; j < x2; j++) {
					double new_red, new_green, new_blue;
					// Calculez noile valori
					new_red = 0.393 * clr[i][j].r + 0.769 * clr[i][j].g + 0.189 * clr[i][j].b;
					new_green = 0.349 * clr[i][j].r + 0.686 * clr[i][j].g + 0.168 * clr[i][j].b;
					new_blue = 0.272 * clr[i][j].r + 0.534 * clr[i][j].g + 0.131 * clr[i][j].b;
					// Verific daca intrec maximul; le atribui valoarea maxima in caz ca da
					if (new_red > 255)
						new_red = 255;
					if (new_green > 255)
						new_green = 255;
					if (new_blue > 255)
						new_blue = 255;
					// Atribui noile valori
					clr[i][j].r = round(new_red);
					clr[i][j].g = round(new_green);
					clr[i][j].b = round(new_blue);

				}
		}
	}
}
// Functia de crop
void crop(color ***clr, int *line, int *col, int *x1, int *y1, int *x2, int *y2, int l)
{	
	if (l == 0) {
		printf("No image loaded\n");
	} else {
		printf("Image cropped\n");
		int old_line;
		old_line = *line;// Salvez nr vechi de linii
		*line = *y2 - *y1;// Calculez noul nr de linii
		*col = *x2 - *x1;// Calculez noul nr de coloane
		// Alocare matrice
		color **mat = (color **)malloc(*line * sizeof(color *));
		for (int q = 0; q < *line; q++)
			mat[q] = calloc(*col, sizeof(color));
		int t = 0, q = 0;
		// Salvez submatricea selectata
		for (int i = *y1; i < *y2; i++) {
			for (int j = *x1; j < *x2; j++) {
				mat[q][t] = (*clr)[i][j];
				t++;
					}
		q++;
		t = 0;
				}
		// Dezalocare matrice structura
		for (int q = 0; q < old_line; q++)
			free((*clr)[q]);
		free(*clr);
		*clr = (color **)malloc(*line * sizeof(color *));
		// Aloc matricea cu noile dimensiuni
		for (int i = 0; i < *line; i++)
			(*clr)[i] = calloc(*col , sizeof(color));
		for (int i = 0; i < *line; i++)
			for (int j = 0; j < *col; j++)
				(*clr)[i][j] = mat[i][j];
		for (int q = 0; q < *line; q++)
			free(mat[q]);
		free(mat);
		// Reinitializez coordonatele pt a fi selectata toata imaginea
		*x1 = 0;
		*y1 = 0;
		*x2 = *col;
		*y2 = *line;
	}
}
// Functia de rotire
void rotate_function(color ***clr, int *line, 
						int *col, int *x1, int *y1, int *x2, int *y2, int *rot)
{	// Verific daca este selectata toata imaginea
		if (*x1 == 0 && *y1 == 0 && *x2 == *col && *y2 == *line) {
			*rot = 1;// Variabila ce devine 1 daca a fost rotita imaginea
			*x2 = *line;// Nr de linii devine nr de coloane
			*y2 = *col;// Nr de coloane devine nr de linii
			// Alocare matrice cu noile dimensiuni
			color **mat = (color **)malloc(*y2 * sizeof(color *));
			for (int q = 0; q < *y2; q++)
				mat[q] = calloc(*x2, sizeof(color));
			int q = 0;
			// Salvez elementele in matricea noua incepand cu ultimul element de pe prima linie
			for (int j = *x2 - 1; j >= 0; j--) {
				for (int i = 0; i < *y2; i++)
					mat[i][j] = (*clr)[q][i];
				q++;
		}
		for (int q = 0; q < *line; q++)
			free((*clr)[q]);
		free(*clr);
		*line = *y2;
		*col = *x2;
		*clr = (color **)malloc(*line * sizeof(color *));
		for (int i = 0; i < *line; i++)
			(*clr)[i] = calloc(*col , sizeof(color));
		// Copiez matricea rotita
		for (int i = 0; i < *line; i++)
			for (int j = 0; j < *col; j++)
				(*clr)[i][j] = mat[i][j];
		for (int i = 0; i < *line; i++)
			free(mat[i]);
		free(mat);
		} else {// Verific daca selectia e patratica
			if ((*x2 - *x1) != (*y2 - *y1)) {
				printf("The selection must be square\n");
			} else {
				*rot = 1;
				int dim = *x2 - *x1;// Dimensiunea submatricei
				int q = 0;
				// Alocare memorie pentru submatrice
				color **temp = (color **)malloc(dim * sizeof(color *));
					for (int q = 0; q < dim; q++)
						temp[q] = calloc(dim, sizeof(color));
				int t = 0;
				// Copiez elementelele din submatricea selectata
				//intr-o alta matrica
				for (int i = *y1; i < *y2; i++) {
					for (int j = *x1; j < *x2; j++) {
						temp[q][t] = (*clr)[i][j];
						t++;
					}
					q++;
					t = 0;
				}
				color **mat = (color **)malloc(dim * sizeof(color *));
					for (int q = 0; q < dim; q++)
						mat[q] = calloc(dim, sizeof(color));
				q = 0;
				for (int j = dim - 1; j >= 0; j--) {
					for(int i = 0; i < dim; i++)
						mat[i][j] = temp[q][i];
					q++;
										}
				for (int i = 0; i < dim; i++)
					free(temp[i]);
				free(temp);
				q = 0;
				t = 0;
				// Scriu submatricea rezultata in cea initiala
				for (int i = *y1; i < *y2; i++) {
					for (int j = *x1; j < *x2; j++) {
						(*clr)[i][j] = mat[q][t];
						t++;
					}
					q++;
					t = 0;
				}
				for (int i = 0; i < dim; i++)
					free(mat[i]);
				free(mat);
			}
	}
}
void select_function(int col, int line, int *x1, int *x2, int *y1, int *y2,
					int *new_x1, int *new_x2, int *new_y1, int *new_y2, int l)
{	
	if (l == 0)
		printf("No image loaded\n");
	// Verific coordonatele
	else {
		int temp;
		if (*x1 > *x2) {
			temp = *x1;
			*x1 = *x2;
			*x2 = temp;
		}
		if (*y1 > *y2) {
			temp = *y1;
			*y1 = *y2;
			*y2 = temp;
		}
		// Verific coordonatele, daca depasesc dimensiunea matricii
		// sau daca sunt egale
		if (*x2 > col || *y2 > line || *x1 < 0 || *y1 < 0 || *x1 == *x2 || *y1 == *y2)
			printf("Invalid set of coordinates\n");
		else {// Salvez noile coordonate
			*new_x1 = *x1;
			*new_x2 = *x2;
			*new_y1 = *y1;
			*new_y2 = *y2;
			printf("Selected %d %d %d %d\n", *new_x1, *new_y1, *new_x2, *new_y2);
		}
	}
}
void verify_coord(char x1[1000], char y1[1000], char x2[1000],
					char y2[1000], int *vf)
{// Verific daca sunt corecte coordonatele, adica daca sunt numere
	if((atoi(x1) == 0 && strcmp(x1, "0") != 0) || (atoi(x2) == 0 && strcmp(x2, "0") != 0) || (atoi(y1) == 0 && strcmp(y1, "0") != 0) || (atoi(y2) == 0 && strcmp(y2, "0") != 0))
		*vf = 0;
}
void select_all(int col, int line, int *new_x1,
				int *new_x2, int *new_y1, int *new_y2, int l)
{
	if (l == 0) {
		printf("No image loaded\n");
	} else {
		*new_x1 = 0;
		*new_x2 = col;
		*new_y1 = 0;
		*new_y2 = line;
		printf("Selected ALL\n");
	}
}
void select_all_load(int col, int line, int *new_x1,
						int *new_x2, int *new_y1, int *new_y2)
{// Selectez toata imaginea dupa ce dau load
	*new_x1 = 0;
	*new_x2 = col;
	*new_y1 = 0;
	*new_y2 = line;
		}
void read_P2(color **clr, FILE *in, int col, int line, int n, char a[1000][1000])
{	// Citesc elementele din fisier si le salvez in campurile matricei clr, lucrand
	// doar cu campul r
	fopen(a[1], "rt");
	fseek(in, n, SEEK_SET);
	for (int i = 0; i < line; i++)
		for (int j = 0; j < col; j++) {
			fscanf(in, "%d", &clr[i][j].r);
			clr[i][j].g = clr[i][j].r;
			clr[i][j].b = clr[i][j].r;;
			}
	fclose(in);
}
void read_P3(FILE *in, int col, int line, color **clr, int n, char a[1000][1000])
{	// Citesc elementele din fisier si salvez fiecare culoare intr-un cam al matricei clr
	fopen(a[1], "rt");
	fseek(in, n, SEEK_SET);
	for (int i = 0; i < line; i++)
		for (int j = 0; j < col; j++) {
			fscanf(in, "%d", &clr[i][j].r);
			fscanf(in, "%d", &clr[i][j].g);
			fscanf(in, "%d", &clr[i][j].b);
			}
	fclose(in);
}
void read_P5(FILE *in, int col, int line, color **clr, int n, char a[1000][1000])
{// Analog read_P2
	fopen(a[1], "rb");
	fseek(in, n, SEEK_SET);
	unsigned char x;

	for (int i = 0; i < line; i++) {
		for (int j = 0; j < col; j++){
			fread(&x, 1, 1, in);
			clr[i][j].r = x;
			clr[i][j].g = x;
			clr[i][j].b = x;
		}
		}
}
void read_P6(FILE *in, int col, int line, color **clr, int n, char a[1000][1000])
{
// Analog read_P3
	fopen(a[1], "rb");
	fseek(in, n, SEEK_SET);
	unsigned char x;

	for (int i = 0; i < line; i++) {
		for (int j = 0; j < col; j++){
			fread(&x, 1, 1, in);
			clr[i][j].r = x;
			fread(&x, 1, 1, in);
			clr[i][j].g = x;
			fread(&x, 1, 1, in);
			clr[i][j].b = x;
		}
		}
}
void load(char text[3], char cpy[7], char li[7],char tensi[5], int *n,
			char a[1000][1000], int *col,int *line, FILE *in, int *intensity)
{
	printf("Loaded %s\n", a[1]);
	fgets(text, 3, in);
	fseek(in, strlen(text) + 1, SEEK_SET);
	fscanf(in, "%s", cpy);// Citesc nr de coloane
	fscanf(in, "%s", li);// Citesc nr de linii
	*col = atoi(cpy);// Transform in int
	*line = atoi(li);// Transform in int
	*n = strlen(cpy) + strlen(li) + 4 + strlen(text);// Calculez cu cate pozitii trebuie sa ma mut in fisier
	fscanf(in, "%s", tensi);
	*intensity = atoi(tensi);
	*n = *n + strlen(tensi);
}
int main(void)
{
	int ok = 1, n, line, col, intensity, x1, x2, y1, y2, new_x1, new_x2, new_y1, new_y2, l = 0;
	char s[1000];
	char a[1000][1000];
	color **clr;
	char li[7], cpy[7], tensi[5];
	char text[3];

	while (ok == 1) {

		int i = 0;
		fgets(s, 1000, stdin);// Citesc datele de intrare
		char *p;
		p = strtok(s, " \n");
		while (p != NULL) {
			strcpy(a[i], p);// Salvez fiecare element al sirului pe o linie a matricei
			i++;
			p = strtok(NULL, " \n");
		}
		if (strcmp(a[0], "LOAD") == 0) {
			FILE *in = fopen(a[1], "r");
			if (in == NULL) {
				printf("Failed to load %s\n", a[1]);
				in = NULL;
				l = 0;
			} else {
				load(text, cpy, li, tensi, &n, a, &col, &line, in, &intensity);
				select_all_load(col, line, &new_x1, &new_x2, &new_y1, &new_y2);
				l = 1;
				clr = (color **)malloc(line * sizeof(color *));
				for(int q = 0; q < line; q++)
					clr[q] = calloc(col, sizeof(color));
				if (strcmp(text, "P2") == 0) {
					fclose(in);
					read_P2(clr, in, col, line, n, a);
											}
				if (strcmp(text, "P3") == 0) {
					fclose(in);
					read_P3(in, col, line, clr, n, a);
				}
				if (strcmp(text, "P5") == 0) {
					fclose(in);
					read_P5(in, col, line, clr, n, a);
				}
				if (strcmp(text, "P6") == 0){
					fclose(in);
					read_P6(in, col, line, clr, n, a) ;
				}
											}
										}
		else if (strcmp(a[0], "SELECT") == 0 && strcmp(a[1], "ALL") == 0)
			select_all(col, line, &new_x1, &new_x2, &new_y1, &new_y2, l);
		else if (strcmp(a[0], "SELECT") == 0 && strcmp(a[1], "ALL") != 0)
			{	
				if (l == 0)
					printf("No image loaded\n");
				else {
					int vf = 1;
					verify_coord(a[1], a[2], a[3], a[4], &vf);
					if (vf == 0)
						printf("Invalid command\n");
					else {
						x1 = atoi(a[1]);// Transform coordonatele citite in int
						y1 = atoi(a[2]);
						x2 = atoi(a[3]);
						y2 = atoi(a[4]);
						select_function(col, line, &x1, &x2, &y1, &y2, &new_x1, &new_x2, &new_y1, &new_y2, l);
				}
			}
			}
		else if (strcmp(a[0], "ROTATE") == 0) {
			char angle[4];
			int rot = 0;
			strcpy(angle, a[1]);
			if (l == 0)
				printf("No image loaded\n");
			else {
				if (strcmp(angle, "90") == 0 || strcmp(angle, "-270") == 0)
					rotate_function(&clr, &line, &col, &new_x1, &new_y1, &new_x2, &new_y2, &rot);
				else if (strcmp(angle, "-180") == 0 || strcmp(angle, "180") == 0)
					for(int q = 0; q < 2; q++)// Accesez de doua ori functia de rotire 90
						rotate_function(&clr, &line, &col, &new_x1, &new_y1, &new_x2, &new_y2, &rot);
				else if (strcmp(angle, "-90") == 0 || strcmp(angle, "270") == 0)
					for(int q = 0; q < 3; q++)
						rotate_function(&clr, &line, &col, &new_x1, &new_y1, &new_x2, &new_y2, &rot);
				else if (strcmp(angle, "0") == 0 || strcmp(angle, "360") == 0 || strcmp(angle, "-360") == 0)
					for(int q = 0; q < 4; q++)
						rotate_function(&clr, &line, &col, &new_x1, &new_y1, &new_x2, &new_y2, &rot);
				else printf("Unsupported rotation angle\n");
				if (rot == 1)
					printf("Rotated %s\n", angle);
			}
		}
		else if (strcmp(a[0], "CROP") == 0)
			crop(&clr, &line, &col, &new_x1, &new_y1, &new_x2, &new_y2, l);
		else if (strcmp(a[0], "GRAYSCALE") == 0)
			grayscale(clr, new_x1, new_y1, new_x2, new_y2, text, l);
		else if (strcmp(a[0], "SEPIA") == 0)
			sepia(clr, new_x1, new_y1, new_x2, new_y2, text, l);
		else if (strcmp(a[0], "SAVE") == 0)
			save(text, clr, line, col, a, intensity, l);
		else if (strcmp(a[0], "EXIT") == 0) {
			if(l == 0) {
				printf("No image loaded\n");
				for(int q = 0; q < line; q++)
					free(clr[q]);
				free(clr);
				return 0;
				} else {
					for (int q = 0; q < line; q++)
						free(clr[q]);
					free(clr);
					return 0;
				}
			}
		else printf("Invalid command\n");
}
	return 0;
}
