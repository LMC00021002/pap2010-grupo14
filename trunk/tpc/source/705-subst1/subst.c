#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define forn(N,X) for(X=0; X<(int)(N); ++X)
#define fromto(N,M,X) for (X=N; X<M; X++)

#define MAXN 65536
#define MAXLG 17

typedef struct {
	int nr[2], p;
} entry;

entry L[MAXN];
char A[MAXN];
int P[MAXLG][MAXN], R[MAXN], H[MAXN], N, i, stp, cnt, final, k, h;

int cmp(entry *a, entry *b) {
	return ((*a).nr[0] == (*b).nr[0] && (*a).nr[1] < (*b).nr[1]) || ((*a).nr[0] < (*b).nr[0]);
}

int main(void) {
	int cases;
	scanf("%d", &cases);
	for (;cases > 0; cases--) {
		scanf("%s", A);
		N = strlen(A);
		forn (N, i)
			P[0][i] = A[i] - 'a';
		for (stp = 1, cnt = 1; cnt >> 1 < N; stp ++, cnt <<= 1) {
			forn(N,i) {
				L[i].nr[0] = P[stp - 1][i];
				L[i].nr[1] = i + cnt < N ? P[stp - 1][i + cnt] : -1;
				L[i].p = i;
			}
			qsort(L, sizeof(entry)*N, sizeof(entry), cmp);
			
			forn(N,i)
				P[stp][L[i].p] = i > 0 && L[i].nr[0] == L[i - 1].nr[0] && L[i].nr[1] == L[i - 1].nr[1] ? P[stp][L[i - 1].p] : i;
		}

		forn(N,i)
			R[P[stp-1][i]] = i;

		h = 0;
		final = 0;
		forn(N,i) {
			k = P[stp-1][i];
			if (k > 0) {
				int j = R[k-1];
				while(A[i+h] == A[j+h] && i+h < N && j+h < N)h++;
				H[k] = h;
				if (h > 0)h--;
			}
			final += N - i - H[k];
		}

		printf("%d", final);
		if (cases > 0) printf("\n");
	}
	return 0;
}
