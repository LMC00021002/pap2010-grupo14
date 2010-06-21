#include<stdio.h>
#include<stdlib.h>
#define forn(i, n) for(i=0; i < (int)(n); ++i)
#define MAX_RULE 94

int M[MAX_RULE][MAX_RULE];
int M2[MAX_RULE][MAX_RULE];

void show_matrix(int (*a)[MAX_RULE]) {
	int i, j;
	forn(i, MAX_RULE) {
		printf("%i\t", i);
		forn(j, MAX_RULE)
			printf("%d ", a[i][j]);
		printf("\n");
	}
	printf("\n");
}

void matrix_exp(int (*orig)[MAX_RULE], int exp) {
	int i, j, k;
	int b[MAX_RULE][MAX_RULE];
	forn(i, MAX_RULE)
		forn(j, MAX_RULE)
			b[i][j] = orig[i][j];
	while (--exp) {
		forn(i, MAX_RULE)
			forn(j, MAX_RULE)
				M2[i][j] = 0;
		forn(i, MAX_RULE)
			forn(j, MAX_RULE)
				forn(k, MAX_RULE)
					M2[i][j] += b[i][k] * orig[k][j];
		forn(i, MAX_RULE)
			forn(j, MAX_RULE)
				b[i][j] = M2[i][j];
	}
}

int main(int argc, char **argv) {
	int i, rules, queries, its, res=0;
	unsigned char rule[MAX_RULE], str[MAX_RULE], c;
	int length;
	scanf("%i", &length); /* #Cases */
	while(length--) {
		scanf("%i", &rules);
		while (rules--) {
			scanf("%s", rule);
			for(i=3; rule[i] != 0; i++)
				M[rule[0]-33][rule[i]-33] += 1;
		}

		scanf("%i", &queries);
		while (queries--) {
			scanf("%s %c %i", str, &c, &its);
			matrix_exp(M,its);
			forn(i, MAX_RULE)
				res += M2[i][c-33];
			printf("%i\n", res);
		}
	}
	return 0;
}
