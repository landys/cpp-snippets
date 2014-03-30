/***********************************************
* For LG cell phone game 
* 
* Algorithm: Depth-first search and trace back
* The complexity is more than 2^n
* 
* Lesson: Forget to initialize Halloween.p!!!
* Release is about ten time quicker than debug.
*
* Maybe it is quicker to check the left number (n) of chosen number before
* every eliminate, if 0 < n < 3, then ignore the position.
************************************************/

#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <functional>
#include <stack>
#include <vector>
#include <ctime>
#include <list>
#include <queue>

using namespace std;

#define MAX_NROW 8
#define MAX_NCOL 8

class Halloween;
int Nrow = 0;
int Ncol = 0;
int Num = 0;
int I, J, K;	// temp for replaceAll
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

	int replaceAll(int i, int j)
	{
		int t = 1;
		if (j-1 >= 0 && a[i][j-1] == K)	// left
		{
			if (I >= i && J >= j-1)
			{
				I = i;
				J = j-1;
 			}
			a[i][j-1] = 0;
			t += replaceAll(i, j-1);
		}
		if (i-1 >= 0 && a[i-1][j] == K)	// up
		{
			if (I >= i-1 && J >= j)
			{
				I = i-1;
				J = j;
 			}
			a[i-1][j] = 0;
			t += replaceAll(i-1, j);
		}
		if (j+1 < Ncol && a[i][j+1] == K)	// right
		{
			a[i][j+1] = 0;
			t += replaceAll(i, j+1);
		}
		if (i+1 < Nrow && a[i+1][j] == K)	// down
		{
			a[i+1][j] = 0;
			t += replaceAll(i+1, j);
		}
		
		return t;
	}

// even slower without recursive
// 	int replaceAll(pair<int, int> pp)
// 	{
// 		deque<pair<int, int> > dq;
// 		int k = a[pp.first][pp.second];
// 		int t = 1;
// 		int i, j;
// 		pair<int, int> pi;
// 		int im = pp.first;
// 		int jm = pp.second;
// 		dq.push_back(pp);
// 		a[pp.first][pp.second] = 0;
// 		while (!dq.empty())
// 		{
// 			pi = dq.front();
// 			dq.pop_front();
// 			i = pi.first;
// 			j = pi.second;
// 			if (j-1 >= 0 && a[i][j-1] == k)	// left
// 			{
// 				if (im >= i && jm >= j-1)
// 				{
// 					return 0;
// 				}
// 				a[i][j-1] = 0;
// 				t++;
// 				dq.push_back(pair<int, int>(i, j-1));
// 			}
// 			if (i-1 >= 0 && a[i-1][j] == k)	// up
// 			{
// 				if (im >= i-1 && jm >= j)
// 				{
// 					return 0;
// 				}
// 				a[i-1][j] = 0;
// 				t++;
// 				dq.push_back(pair<int, int>(i-1, j));
// 			}
// 			if (j+1 < Ncol && a[i][j+1] == k)	// right
// 			{
// 				a[i][j+1] = 0;
// 				t++;
// 				dq.push_back(pair<int, int>(i, j+1));
// 			}
// 			if (i+1 < Nrow && a[i+1][j] == k)	// down
// 			{
// 				a[i+1][j] = 0;
// 				t++;
// 				dq.push_back(pair<int, int>(i+1, j));
// 			}
// 		}
// 		return t;
// 	}

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
		K = a[p.first][p.second];	

		// ignore the position when up or left is tried, for is same as k
		// It makes speed thicker more than 700 times
		if ((p.first-1 >= 0 && a[p.first-1][p.second] == K) || 
			(p.second-1 >=0 && a[p.first][p.second-1] == K))
		{
			p = getNextPos(ignoreOnePos(p));
			return false;	
		}

 		a[p.first][p.second] = 0;
 		I = p.first;
		J = p.second;
		int c = replaceAll(p.first, p.second);

//		int c = replaceAll(p);

		if (c >= 3 && (I==p.first && J==p.second))
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
				}
			}
		}
	}

	return true;
}

int main()
{
	ifstream cin("input.txt");

	int i, j;
	clock_t start, end;
	while (cin >> Nrow >> Ncol)
	{
		Num = Nrow * MAX_NCOL;
		Halloween h;	// h.n, h.p will be initialized in constructor now

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
			out << "(" << it->first+1 << "," << it->second+1 << ")" << endl; 
		}
		out << "####################################" << endl;
	}

	return 0;
}

/****************************
Input:
5 7
0 1 1 1 0 1 2
0 2 3 3 1 2 1
2 2 1 3 1 2 1
1 3 2 3 1 2 1
1 3 2 2 1 3 1

7 8
1 1 1 2 3 1 2 2
3 4 4 3 4 2 2 1
3 4 2 2 1 1 4 3
2 2 2 1 1 1 2 4
4 3 1 3 4 2 2 2
4 3 2 4 4 1 3 4
4 1 1 2 3 2 3 4

8 8
1 1 1 2 3 1 1 1
1 1 3 2 2 4 1 1
1 3 1 3 1 2 3 2
3 2 4 2 4 4 1 3
1 1 1 3 3 1 1 2
4 4 4 3 3 4 4 4
2 1 2 4 1 3 2 1
2 1 2 1 4 4 4 2

output:
Yes. It costs: 0 millisecondes.
(1,2)
(2,2)
(2,3)
(2,7)
(4,3)
(2,4)
(4,2)
(3,2)
(4,1)
####################################
Yes. It costs: 0 millisecondes.
(1,1)
(1,7)
(2,2)
(2,6)
(3,3)
(4,5)
(4,7)
(5,1)
(5,2)
(5,3)
(5,5)
(6,2)
(6,4)
(6,2)
####################################
Yes. It costs: 54000 millisecondes.
(1,1)
(1,6)
(5,1)
(6,6)
(5,4)
(3,4)
(5,3)
(8,5)
(6,7)
(6,6)
(7,5)
(7,8)
(8,5)
(7,3)
(7,2)
(7,1)
(8,1)
####################################
*****************************/