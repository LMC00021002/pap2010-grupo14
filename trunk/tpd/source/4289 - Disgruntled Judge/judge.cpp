#include<iostream>

using namespace std;

#define forn(N,X) for(int X=0; X<(int)(N); ++X)
#define forinvn(N,X) for(int X=(int)(N)-1; X>=0; --X)
#define fromto(N,M,X) for (int X=N; X<M; X++)

#define MAXN 10001

//bool ab[MAXN][MAXN];

int main(void)
{
	int cases;
	cin >> cases;
	int input[100];
	int input73[100];
	int input137[100];
	
	bool escandidato = false;
	int outputa;
	int outputb;
	int i73,i137,j73,j137;

	forn (cases,i)
	{
		cin >> input[i]; 
		input73[i] = input[i] % 73;
		input137[i] = input[i] % 137;
	}
	
	i73 = i137 = -1;
	forn(MAXN,i)
	{
		//i73 = i % 73;
		i73++;
		if (i73 == 73) i73 = 0;
		//ii73 = i73*i73%73;
		i137++;
		if (i137 == 137) i137 = 0;

		//ii137 = i137*i137%137;
		j73 = j137 = -1;
		forn(MAXN,j)
		{
			j73++;
			if (j73 == 73) j73 = 0;
			//ij73 = j73 * i73 % 73;
			j137++;
			if (j137 == 137) j137 = 0;
			//ij137 = j137 * i137 % 137;
			forn(cases-1,t)
			{
/*				escandidato = 
					(ii73 + input73[t] *i73 + ij73+ j73) % 73 == input73[t+1]
					&& (ii137 + input137[t] * i137 + ij137 + j137) % 137 == input137[t+1];*/
					escandidato = 
					((i73 * input73[t]  + j73 )*i73+ j73) % 73 == input73[t+1]
					&& ((i137 * input137[t]  + j137 )*i137+ j137) % 137 == input137[t+1];
				if (!escandidato)break;
			}
			
			if (escandidato)
			{
				outputa = i;
				outputb = j;
				break;	
			}
		}
		if (escandidato)break;
	}

	forn (cases,t)
		cout << (outputa * input[t] + outputb) % MAXN << endl;
	
	return 0;
}
