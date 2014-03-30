#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <functional>
#include <stack>
#include <vector>
#include <ctime>

using namespace std;

#define MAX_NROW 8
#define MAX_NCOL 8

class Halloween;
int Nrow = 0;
int Ncol = 0;
int Num = 0;
stack<pair<int,int> > S;
stack<Halloween> SH;
ofstream out("output.txt");

class Halloween
{
public:
	int a[MAX_NROW][MAX_NCOL]; 	// 0 means vacancy
	pair<int, int> p;
	int n;

	Halloween()
	{
		p.first = 0;
		p.second = 0;	
		n = 0;
		fill_n((int*)a, MAX_NROW*MAX_NCOL, 0);
	}

	void print()
	{
		out << "*******************" << endl;
		out << "(" << p.first << "," << p.second << ")" << endl;
		out << "n=" << n << endl;
		for (int i=0; i<Nrow; i++)
		{
			copy (a[i], a[i]+Ncol, ostream_iterator<int>(out, " "));
			out << endl;
		}
	}
	
	bool checkWin()
	{
		return (n == 0);
//		return (find_if((int*)a, (int*)a+Num, bind2nd(not_equal_to<int>(), 0)) == (int*)a+Num);
	}

	int replaceAll(int i, int j, int k, int r=0)
	{
		int t = 1;
		if (j+1 < Ncol && a[i][j+1] == k)	// right
		{
			a[i][j+1] = r;
			t += replaceAll(i, j+1, k);
		}
		if (j-1 >= 0 && a[i][j-1] == k)	// left
		{
			a[i][j-1] = r;
			t += replaceAll(i, j-1, k);
		}
		if (i+1 < Nrow && a[i+1][j] == k)	// down
		{
			a[i+1][j] = r;
			t += replaceAll(i+1, j, k);
		}
		if (i-1 >= 0 && a[i-1][j] == k)	// up
		{
			a[i-1][j] = r;
			t += replaceAll(i-1, j, k);
		}
		return t;
	}

	void moveDown()
	{
		vector<int> ve;
		int i, j;
		for (j=0; j<Ncol; j++)
		{
			int i1 = -1;
			int i2 = -1;
			for (i=Nrow-1; i>=0; i--)
			{
				if (a[i][j] == 0)
				{
					i1 = i;
					break;
				}
			}
			for (i--; i>=0; i--)
			{
				if (a[i][j] != 0)
				{
					i2 = i;
					break;
				}
			}

			if (i2 == -1 && i1 == Nrow-1)
			{
				ve.push_back(j);	// the whole column is 0
			}
			else if (i2 >= 0 && i1 > i2)
			{
				for (; i2>=0; i2--,i1--)
				{
					a[i1][j] = a[i2][j];
				}
				for (; i1>=0; i1--)
				{
					a[i1][j] = 0; 
				}
			}
		}
		int m = ve.size();
		if (m == 0)
		{
			return;
		}
		ve.push_back(Ncol);
		int t = 0;
		int jc = ve[0];
		for (t=0; t<m; t++)
		{
			for (j=ve[t]+1; j<ve[t+1]; j++,jc++)
			{
				for (i=0; i<Nrow; i++)
				{
					a[i][jc] = a[i][j];
				}
			}
		}
		
		int jjc = jc;
		for (i=0; i<Nrow; i++)
		{
			for (jc=jjc; jc<Ncol; jc++)
			{
				a[i][jc] = 0;
			}
		}
	}

	pair<int, int> getNextPos(pair<int, int> pp, int k=0)
	{
		int* ap = find_if((int*)a+(pp.first*MAX_NCOL+pp.second), (int*)a+Num, bind2nd(not_equal_to<int>(), k));
		if (ap == (int*)a+Num)
		{
			pp.first = -1;
			pp.second = -1;
		}
		else
		{
			int at = ap - (int*)a;
			pp.first = at / MAX_NCOL;
			pp.second = at % MAX_NCOL;
		}
		return pp;
	}

	pair<int, int> ignoreOnePos(pair<int, int> pp)
	{
		if (pp.second == Ncol-1)
		{
			if (pp.first == Nrow-1)
			{
				pp.first = -1;
				pp.second = -1;
			}
			else
			{
				pp.first++;
				pp.second = 0;
			}
		}
		else
		{
			pp.second++;
		}
		return pp;
	}

	bool eliminate()
	{
		int k = a[p.first][p.second];
		a[p.first][p.second] = 0;
		int c = replaceAll(p.first, p.second, k);

		if (c >= 3)
		{
			n -= c;
//	 		print();
			moveDown();
			p = getNextPos(pair<int, int>(0, 0));
//			print();
			return true;
		}
		else
		{
//			print();
			p = getNextPos(p);
//			print();
			return false;
		}
	}
};

bool eliminate(Halloween h)
{
	SH.push(h);
	S.push(h.p);
//	out << "********push: " << SH.size() << endl;
//	SH.top().print();
	while (!h.checkWin())
	{	
// 		out << "-----------------------------------------" << endl;
// 		out << "*** : (" << S.top().first << "," << S.top().second << ") " << S.size() << endl;
// 		out << "********: " << SH.size() << endl;
// 		SH.top().print();
		while (true)
		{
			pair<int, int> t = h.p;
			if (h.eliminate())
			{
				SH.push(h);
//				out << "********push: " << SH.size() << endl;
//				SH.top().print();
				S.push(t);
//				out << "*** push: (" << t.first << "," << t.second << ") " << S.size() << endl;
				break;
			}
			else
			{
				if (h.p != pair<int, int>(-1, -1))
				{
					t = h.p;
					h = SH.top();
					h.p = t;
				}
				else
				{
					while (h.p == pair<int, int>(-1, -1))
					{
//						out << "********pop: " << SH.size()-1<< endl;
//						SH.top().print();
						SH.pop();
						if (SH.empty())
						{
							return false;
						}
						h = SH.top();
						h.p = h.getNextPos(h.ignoreOnePos(S.top()));
//						out << "*** pop: (" << S.top().first << "," << S.top().second << ") " << S.size()-1 << endl;
						S.pop();
					}
//					out << "********pop: " << SH.size()-1<< endl;
//					SH.top().print();
					SH.pop();
	
					SH.push(h);
//					out << "********push: " << SH.size() << endl;
//					SH.top().print();
				}
			}
		}
	}

	return true;
}

// bool eliminate(Halloween h)
// {
// 	if (h.checkWin())
// 	{
// 		return true;
// 	}
// 	
// 	Halloween hh = h;
// 
// 	if (h.eliminate() && eliminate(h))
// 	{
// 		return true;
// 	}
// 	else
// 	{
// 		if (h.p != pair<int, int>(-1, -1))
// 		{
// 			hh.p = h.p;
// 			return eliminate(hh);
// 		}
// 		else
// 		{
// 			return false;	
// 		}
// 	}
// }

int main()
{
	ifstream cin("input.txt");

	int i, j;
	Halloween h;
	clock_t start, end;
	while (cin >> Nrow >> Ncol)
	{
		Num = Nrow * MAX_NCOL;
		h.n = 0;
		for (i=0; i<Nrow; i++)
		{
			for (j=0; j<Ncol; j++)
			{
				cin >> h.a[i][j];
				if (h.a[i][j] != 0)	// initialize h.num for checking win
				{
					h.n++;
				}
			}
		}
// 		h.print();	// test moveDown()
// 		h.moveDown();
// 		h.print();

		cout << "Begin... " << endl;
		//h.print();

		while (!S.empty())
		{
			S.pop();
		}
		while (!SH.empty())
		{
			SH.pop();
		}
		
		start = clock();
		h.p = h.getNextPos(h.p);
		bool flag = eliminate(h);
		end = clock();
		if (flag)
		{
			cout << "Yes";
		}
		else
		{
			cout << "No";
		}
		cout << ". It costs: " << end-start << " millisecondes." << endl;

		while (!S.empty())
		{
			cout << "(" << S.top().first << "," << S.top().second << ")" << endl;
			S.pop();
		}
		cout << "####################################" << endl;
	}

	return 0;
}
