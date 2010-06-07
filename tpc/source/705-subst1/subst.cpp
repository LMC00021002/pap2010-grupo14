#include<iostream>
#include<string>
#include<cstdio>
#include<cstring>
#include<algorithm>

using namespace std;

#define forn(N,X) for(int X=0; X<(int)(N); ++X)
#define forinvn(N,X) for(int X=(int)(N)-1; X>=0; --X)
#define fromto(N,M,X) for (int X=N; X<M; X++)
#define inf 0x3f3f3f3f

#define MAXN 65536
#define MAXLG 17
char A[MAXN];

struct entry {
	int nr[2], p;
}
L[MAXN];
int P[MAXLG][MAXN],R[MAXN],H[MAXN], N, i, stp, cnt,final,k,h;

int cmp(struct entry a, struct entry b) {
	return a.nr[0] == b.nr[0] ? (a.nr[1] < b.nr[1] ? 1 : 0) : (a.nr[0] < b.nr[0] ? 1 : 0);
}

int lcp(int x, int y) {
	int k, ret = 0;
	if (x == y) return N - x;

	for (k = stp-1; (k >= 0) && (x < N) && (y < N); k--)
		if (P[k][x] == P[k][y]) {
			x += (1 << k);
			y += (1 << k);
			ret += (1 << k);
		}

	return ret;
}

int main(void) {
	int cases;
	cin >> cases;
	for (; cases > 0; cases--) {
		cin >> A;
		N = strlen(A);
		forn (N, i)
			P[0][i] = A[i] - 'a';
		for (stp = 1, cnt = 1; cnt >> 1 < N; stp ++, cnt <<= 1) {
			forn(N,i) {
				L[i].nr[0] = P[stp - 1][i];
				L[i].nr[1] = i + cnt < N ? P[stp - 1][i + cnt] : -1;
				L[i].p = i;
			}
			sort(L, L + N, cmp);

			forn(N,i)
				P[stp][L[i].p] = i > 0 && L[i].nr[0] == L[i - 1].nr[0] && L[i].nr[1] == L[i - 1].nr[1] ? P[stp][L[i - 1].p] : i;
		}

		forn(N,i)
			R[P[stp-1][i]] = i;

		h = 0;
		final = 0;
		forn(N,i) {
			k = R[i];
			if (k > 0) {
				int j = P[stp-1][k-1];
				while (A[i+h] == A[j+h] && i+h < N && j+h < N) h++;
				H[k] = h;
				if (h > 0) h--;
			}
			final += N - i - H[k];
		}
/*
1. for(i=1;i<=n;i++) rank[sa[i]] = i;
2. h=0;
3. for(i=1;i<=n;i++) {
4. k = rank[i];
5. if(k==1) lcp[k]=-1;
6. else {
7. j = sa[k-1];
8. while(i+h<=n && j+h<=n && t[i+h]==t[j+h]):
9. h++;:
10. lcp[k] = h;
11. }
12. if(h>0) h--;
13. }
*/

/*
fromto(1,N,i)
	final += N - i - H[i];//lcp(P[stp-1][t],P[stp-1][t+1]);
*/
		cout << final << endl;
	}
	return 0;
}
