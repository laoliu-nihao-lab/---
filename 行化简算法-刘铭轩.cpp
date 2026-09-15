#include<iostream>
using namespace std;
#include<vector>

//按步骤运行
void controller(vector< vector<double> > &v);

//初始化矩阵（通过输入） 
int initMatrix(vector< vector<double> > &v);

//向前步骤：1~3
void forwardSteps(vector< vector<double> > &v);

//向后步骤
void backwardStep(vector< vector<double> > &v); 

//打印矩阵
void printMatrix(vector< vector<double> > &v);
 
int main()
{
	vector< vector<double> > v;
	controller(v);
		
	return 0;
}

void controller(vector< vector<double> > &v)
{
	int cn = initMatrix(v); 
	//步骤一:定位主元 
	 if(cn == 0.0)
	 {
	 	//步骤五 :打印 
	 	printMatrix(v);
	 	cout << "该矩阵为零矩阵" << endl;
	 	return;
	 }
	 else if(cn == 1.0)
	 {
	 	//步骤四 
	 	backwardStep(v); 
	 	//步骤五 :打印
	 	printMatrix(v);
	 }
	 else
	 {
	 	//向前步骤：1~3
	 	forwardSteps(v);
	 	//向后步骤
	 	backwardStep(v); 
		//步骤五 :打印 
		printMatrix(v);	 	
	 }
}

int initMatrix(vector< vector<double> > &v)
{
	cout << "请逐行输入一个增广矩阵：" << endl;
	cout << "（若输入系数矩阵，请在最右侧多加一列全为0的常数项列）" << endl;
	cout << endl; 
	int m = 0;
	cout << "矩阵的行数为：" << endl;
	cin >> m;
	int n = 0;
	cout << "矩阵的列数为：" << endl;
	cin >> n;
	cout << "请逐行输入一个增广矩阵：" << endl;
	//resize()，重新指定大小，可用来初始化 
	v.resize(m);
	//初始为空时，迭代器不能指向v.begin()，因为为空。
	int cn = 0;
	int cn1 = 0;
	//第一个数默认为填入个数，第二个数才为要赋的值 
	vector<double> Pioneer(1,0.0);
	for(int i = 0;i < m;i++)
	{
		bool judge = false;
		for(int j = 0;j < n;j++)
		{
			double num = 0.0;
			cin >> num;
			v[i].push_back(num);
			if(num == 0)
			{
				cn++;
			}
			else
			{
				//记录先导元素的位置 
				if(!judge)
				{
					Pioneer.push_back(j);
					judge = true;
				}
			}
		}
	}
	for(int i  = 1;i < Pioneer.size() - 1;i++)
	{
		//判断先导元素是否符合阶梯形定义 
		if(Pioneer[i] < Pioneer[i + 1])
		{
			cn1++;
		}
	}
	//零矩阵 
	if(cn == m * n)
	{
		return 0.0;
	}
	//阶梯形 
	else if(cn1 == Pioneer.size() - 2)
	{
		return 1.0;
	}
	//其他 
	else
	{
		return 2.0;
	}
	/* Claude code 的优化 
		// 读入时记录每行主元列，零行记为 -1
		std::vector<int> pioneer(m, -1);
		for (int i = 0; i < m; ++i) {
		    for (int j = 0; j < n; ++j) {
		        double num; std::cin >> num;
		        v[i].push_back(num);
		        if (pioneer[i] == -1 && !isZero(num)) pioneer[i] = j;
		    }
		}
		
		bool zeroMatrix = true;
		for (int p : pioneer) if (p != -1) { zeroMatrix = false; break; }
		if (zeroMatrix) return 0;
		
		bool echelon = true, seenZeroRow = false;
		int prev = -1;
		for (int p : pioneer) {
		    if (p == -1) seenZeroRow = true;                 // 从此只允许零行
		    else if (seenZeroRow || p <= prev) { echelon = false; break; }
		    else prev = p;
		}
		return echelon ? 1 : 2;
	*/
		
}

void forwardSteps (vector< vector<double> > &v)
{
	//先遍历列，再遍历行，否则就要遍历整个矩阵来判断哪一行的先导元素在最前面，浪费资源 
	int cn = 0;
	for(int j = 0;j < v[0].size();j++)
	{
		//
		bool judge = true; 
		for(int i = cn;i < v.size();i++)
	    {	
	    	if((v[i]).at(j) != 0)
	    	{
	    		vector<double> temp = v[i];
	    		v[i] = v[cn];
	    		v[cn] = temp;
	    		cn++;
	    		judge = false;
	    		break;
			}
		
		}
		if(!judge)
		{
			for(int i0 = 0;i0 < v.size() - cn;i0++)
			{	
				bool judge = false;
				//在外面定义，不然if花括号结束后就消除其的内存了 
				double savenum = 0.0;
				for(int h = 0;h < v[0].size();h++)
		    	{
					//保存倍加与倍乘的系数，不然只有第一次生效，后面系数皆为零。 
					if(!judge)
					{
				    savenum = (v[i0 + cn]).at(j);
					judge = true;
					}
					//'/'是整除，若存在小数，会导致精度丢失问题。
					//所以得用 vector< vector<double> > &v ，而不是 vector< vector<int> > &v 
		    		(v[i0 + cn]).at(h) -= ( savenum / (v[cn - 1]).at(j) ) * ((v[cn - 1]).at(h));
		    	}
			}
			judge = true;
		}				
	}	 
}

void backwardStep(vector< vector<double> > &v)
{
	//行 
	for(int i = v.size() - 1;i >= 0;i--)
	{
		bool judge = false;
		//先导元素的位置 
		int cn1 = v[0].size() - 1;
		double savenum = 1.0;
		//列 
		for(int j = 0;j < v[0].size();j++)
		{
			if((v[i]).at(j) != 0)
			{			
				if(!judge)
				{
					cn1 = j;
				    savenum = (v[i]).at(j);
					judge = true;
				}
				(v[i]).at(j) /= savenum;
			}	
		}
		if(cn1 != v[0].size() - 1)
		{
				//行 
				for(int k = i - 1;k >= 0;k--)
				{
					bool judge1 = false;
					double savenum1 = 0;
					if(!judge1)
					{
						savenum1 = (v[k]).at(cn1);
						judge1 = true;
					}
					for(int j = cn1;j <= v[0].size() - 1;j++)
					{
						(v[k]).at(j) -= ( savenum1 ) * (v[i]).at(j);
					} 
				}
		}	
	}	
}

/* claude code 的优化 

#include <cmath>
const double EPS = 1e-9;
bool isZero(double x) { return std::fabs(x) < EPS; }

void backwardStep(std::vector<std::vector<double>>& v)
{
    for (int i = v.size() - 1; i >= 0; --i)
    {
        int pivotCol = -1;
        for (int j = 0; j < v[0].size(); ++j)
            if (!isZero(v[i][j])) { pivotCol = j; break; }

        if (pivotCol == -1) continue;          // 真·全零行

        double pivot = v[i][pivotCol];
        for (int j = pivotCol; j < v[0].size(); ++j)
            v[i][j] /= pivot;                  // 主元归一化

        for (int k = i - 1; k >= 0; --k)       // 消去上方所有行的主元列
        {
            double factor = v[k][pivotCol];
            for (int c = 0; c < v[0].size(); ++c)
                v[k][c] -= factor * v[i][c];
        }
    }
}
*/

void printMatrix(vector< vector<double> > &v)
{
	//零行移到底部 
	for(int j = 0;j < v.size();j++)
	{
		int cn1 = 0;
		for(int i = 0;i < (v[0]).size();i++)
		{
			if((v[j]).at(i) == 0)
			{
				cn1++;
			}
		}
		if(cn1 == (v[0]).size())
			{
				vector<double> temp = v[j];
	    		v[j] = v[v.size() - 1];
	    		v[v.size() - 1] = temp;
			}
	}	
	cout << "该矩阵对应的简化阶梯矩阵为" << endl;
	for(vector< vector<double> >::iterator it1 = v.begin();it1 != v.end();it1++)
	{
		for(vector<double>::iterator it2 = (*it1).begin(); it2 != (*it1).end();it2++)
		{
			cout << (*it2) << '\t';			
		}
		cout << endl;		
	}
}

//优化:常数 + 范围for 
// void printMatrix(const vector< vector<double> > &v)
//{
//	cout << "该矩阵对应的简化阶梯矩阵为" << endl;
//	for (const auto& row : v) 
//	{
//        for (double x : row) std::cout << x << '\t';
//        std::cout << '\n';
//    }
//}
