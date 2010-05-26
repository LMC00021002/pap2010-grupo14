#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_HAYSTACK 1024000

int *f; /* Failure function */

int make_f(const char *needle) {
	int t = 0; /* think of t+1 as the length of the current valid prefix. */
	int s,m;

	m = strlen(needle);
	f = malloc((m+1)*sizeof(int));

	f[1] = 0; /* When first character is received there is no proper suffix. */

	for (s=1; s < m; s++) { /* Create f[s] */
		while ((t > 0) && (needle[s] != needle[t]))
			t = f[t];
		if (needle[t] == needle[s]) {
			/* Next character can be added to current prefix */
			t++;
			f[s+1] = t;
		} else
			f[s+1] = 0; /* while loop must have exited (t=0) */
	}
}

int main(int argc, char **argv) {
	char haystack[MAX_HAYSTACK],*p;
	int c,i,length = 0;
	int lines = 0;
	char *ll;

	while (scanf("%i", &length) == 1) {
		int encontro = 0;
		char needle[length];
		scanf("%s", needle);
		scanf("%s", haystack);

		make_f(needle);
		p = haystack;
		while (p = strstr(p,needle)) {
			encontro = 1;
			p++;
			ll = haystack; /* Position of start of most recent line */
			for (i=0; i < p-haystack; i++) {
				if (haystack[i] == '\n') {
					ll = haystack + i + 1;
					lines++;
				}
			}
			printf("%d\n", p-ll-1);
		}
		if(!encontro) putchar('\n');
	}
	return 0;
}
