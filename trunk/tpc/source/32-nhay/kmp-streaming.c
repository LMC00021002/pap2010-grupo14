#include<stdio.h>
#include<stdlib.h>
#define BUFFER_SIZE 2048
int *pi;

void make_pi(const char *needle, int length) {
	int i=2, j=0;
	pi = malloc((length+1) * sizeof(int));
	pi[0] = -1; pi[1] = 0;

	while (needle[i] != '\0')
		if (needle[i-1] == needle[j]) {
			pi[i] = j+1;
			i++; j++;
		} else if (j > 0) {
			j = pi[j];
		} else {
			pi[i] = 0;
			j = 0;
			i++;
		}
}

int main(int argc, char **argv) {
	char haystack[BUFFER_SIZE];
	int length = 0;
	while (scanf("%i", &length) == 1) {
		int i=0, m=0, it=0, en_bloque=1, encontro=0;
		char needle[length];
		scanf("%s", needle);
		make_pi(needle, length);
		getchar(); /* Lee '\n' remanente */

		fgets(haystack, BUFFER_SIZE, stdin);
		while (haystack[it] != '\0' && needle[i] != '\0') {
			if (haystack[it] == needle[i]) {
				i++;
				if (i == length) { /* string found */
					printf("%i\n", m); encontro = 1;
					i = 0;
					m++;
				}
			} else {
				m += i - pi[i];
				if (i > 0) i = pi[i];
			}
			it = (m + i) % (BUFFER_SIZE-1);
			if ((m + i) >= (en_bloque * (BUFFER_SIZE-1))) {
				en_bloque++;
				fgets(haystack, BUFFER_SIZE, stdin);
			}
		}
		free(pi);
		if(!encontro) putchar('\n');
	}
	return 0;
}
