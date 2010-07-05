#include<stdio.h>
#include<stdlib.h>
#define forn(i, n) for(i=0; i < (int)(n); ++i)
#define max(a,b) ((a)>(b) ? (a) : (b))
#define MAX_RULE 94
#define OFFSET 33

int orig[MAX_RULE][MAX_RULE];
int aux[MAX_RULE][MAX_RULE];
int aux2[MAX_RULE][MAX_RULE];

void show_matrix(int (*a)[MAX_RULE]) {
	int i, j;
	forn(i, MAX_RULE) {
		printf("%c (%i)\t", i+OFFSET, i+OFFSET);
		forn(j, MAX_RULE)
			printf("%d", a[i][j]);
		printf("\n");
	}
	printf("\n");
}

int num_chars(const unsigned char *str, char c) {
	int i=0, count=0;
	for (; str[i] != 0; i++)
		if (str[i] == c) count++;
	return count;
}

void cp_matrix(int (*a)[MAX_RULE], int (*b)[MAX_RULE]) {
	int i,j;
	forn(i, MAX_RULE)
		forn(j, MAX_RULE)
			a[i][j] = b[i][j];
}

void mult_matrix(int (*a)[MAX_RULE], int (*b)[MAX_RULE]) {
	/* Sobreescribe a */
	int i, j, k, res[MAX_RULE][MAX_RULE];
	forn(i, MAX_RULE)
		forn(j, MAX_RULE)
			res[i][j] = 0;
	forn(i, MAX_RULE)
		forn(j, MAX_RULE)
			forn(k, MAX_RULE)
				res[i][j] += a[i][k] * b[k][j];
	cp_matrix(a, res);
}

void matrix_exp(int exp) {
	for(;;) {
		if (exp % 2 == 1)
			mult_matrix(aux, aux2);
		if ((exp /= 2) == 0) break;
		mult_matrix(aux2, aux2);
	}
}

int main(int argc, char **argv) {
	int i, j, rules, queries, its, res;
	unsigned char rule[MAX_RULE], str[MAX_RULE], c;
	int num_cases;
	scanf("%i", &num_cases);
	while (num_cases--) {
		forn(i, MAX_RULE)
			forn(j, MAX_RULE)
				orig[i][j] = 0; /* Reset orig */

		scanf("%i", &rules);
		while (rules--) {
			scanf("%s", rule);
			for(i=3; rule[i] != 0; i++) {
				orig[rule[0]-OFFSET][rule[i]-OFFSET]++;
				orig[rule[i]-OFFSET][rule[i]-OFFSET] = 1;
			}
		}

		scanf("%i", &queries);
		while (queries--) {
			res = 0;
			scanf("%s %c %i", str, &c, &its);
			/* Reset aux & aux2 */
			forn(i, MAX_RULE)
				forn(j, MAX_RULE)
					aux[i][j] = (i==j);
			cp_matrix(aux2, orig);
			matrix_exp(its); /* aux <- matriz exponenciada */
			forn(i, MAX_RULE)
				res += aux[i][c-OFFSET] * max(num_chars(str, i+OFFSET), 1);
			printf("%i\n", res);
		}
	}
	return 0;
}
