/**************************
For LG cell phone game 
Algorithm: Depth-first search and trace back
*****************************/

#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <functional>
#include <stack>
#include <vector>
#include <ctime>
#include <list>

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

		// ignore the position when up or left is tried, for is same as k
		// It makes speed thicker more than 700 times
		if ((p.first-1 >= 0 && a[p.first-1][p.second] == k) || 
			(p.second-1 >=0 && a[p.first][p.second-1] == k))
		{
			p = getNextPos(ignoreOnePos(p));
			return false;	
		}

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
			out << "Yes";
			cout << "Yes";
		}
		else
		{
			out << "No";
			cout << "No";
		}
		cout << ". It costs: " << end-start << " millisecondes." << endl;
		out << ". It costs: " << end-start << " millisecondes." << endl;

		list<pair<int, int> > ls;
		while (S.size()>1)
		{
			ls.push_front(S.top());
			S.pop();
		}
		for (list<pair<int, int> >::iterator it=ls.begin(); it!=ls.end(); it++)
		{
			out << "(" << it->first << "," << it->second << ")" << endl; 
		}
		out << "####################################" << endl;
	}

	return 0;
}

/****************************
8 8
0 0 0 2 3 0 0 0
0 0 3 2 2 4 0 0
0 3 1 3 1 2 3 2
3 2 4 2 4 4 1 3
1 1 1 3 3 1 1 2
4 4 4 3 3 4 4 4
2 1 2 4 1 3 2 1
2 1 2 1 4 4 4 2
results:
Begin...
Yes. It costs: 36171 millisecondes.
(4,0)
(5,5)
(4,3)
(2,3)
(4,2)
(7,4)
(5,6)
(5,5)
(6,4)
(6,7)
(7,4)
(6,2)
(6,1)
(6,0)
(7,0)

But:
8 8
1 1 1 2 3 1 1 1
1 1 3 2 2 4 1 1
1 3 1 3 1 2 3 2
3 2 4 2 4 4 1 3
1 1 1 3 3 1 1 2
4 4 4 3 3 4 4 4
2 1 2 4 1 3 2 1
2 1 2 1 4 4 4 2

It runs more than one hour but with no results.
*****************************/