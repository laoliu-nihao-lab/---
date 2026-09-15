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
void printMatrix(const vector< vector<double> > &v);

 
int main()
{
	vector< vector<double> > v;
	controller(v);
		
	return 0;
}

void controller(vector< vector<double> > &v)
{
	int cn_matrix_type = initMatrix(v); 
	//步骤一:定位主元 
	 if(cn_matrix_type == 0.0)
	 {
	 	//步骤五 :打印 
	 	printMatrix(v);
	 	cout << "该矩阵为零矩阵" << endl;
	 	return;
	 }
	 else if(cn_matrix_type == 1.0)
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
	//cout << "请逐行输入一个增广矩阵：" << endl;
	//cout << "（若输入系数矩阵，请在最右侧多加一列全为0的常数项列）" << endl;
	cout << endl; 
	cout << "矩阵的行数为：" << endl;
	int m = 0;
	cin >> m;
	cout << "矩阵的列数为：" << endl;
	int n = 0;
	cin >> n;
	cout << "请逐行输入一个增广矩阵：" << endl;
	//resize()，重新指定大小，可用来初始化 
	v.resize(m);
	//初始为空时，迭代器不能指向v.begin()，因为为空。
	bool cn_echelon = true;
	bool cn_zero_matrix = true;
	bool judge_matrix = false;
	//储存先导位置
	vector<int> Pioneer;
	//行
	for(int i = 0;i < m;i++)
	{
		bool cn_zero_row = true;
		//列
		for(int j = 0;j < n;j++)
		{
			double num = 0.0;
			cin >> num;
			v[i].push_back(num);
			if(num != 0)
			{
				//记录先导元素的位置 
					Pioneer.push_back(j);
					if(cn_zero_row)
					{
						cn_zero_row = false;
					}
					if(!judge_matrix)
					{
						cn_zero_matrix = false;
						judge_matrix = true;
					}
			}
		}
		//零行须要全为零，包括常数项列 
		if(cn_zero_row)
		{
			//之后可考虑通过移位区分 
			Pioneer.push_back(n + i);	
		}
	}
	for(int i  = 1;i < Pioneer.size() - 1;i++)
	{
		//判断先导元素是否符合阶梯形定义 
		if(Pioneer[i] >= Pioneer[i + 1])
		{
			cn_echelon = false;
			break;
		}
	}
	//零矩阵 
	if(cn_zero_matrix)
	{
		return 0.0;
	}
	//阶梯形 
	else if(cn_echelon)
	{
		return 1.0;
	}
	//一般矩阵 
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
		
		//比起判断全为零，不如判断不能有非零，有一个就退出 
		bool zeroMatrix = true;
		for (int p : pioneer) if (p != -1) { zeroMatrix = false; break; }
		if (zeroMatrix) return 0;
		
		bool echelon = true, seenZeroRow = false;
		int prev = -1;
		for (int p : pioneer) {
			//发现零行 
		    if (p == -1) seenZeroRow = true; // 从此只允许零行
		    //否则，先导位置 比零小（存在零行时）或比上面的小 ->普通 
		    //好像只要第二点判断就好了 
		    else if (seenZeroRow || p <= prev) { echelon = false; break; }
		    else prev = p;
		}
		return echelon ? 1 : 2;
	*/
		
}

void forwardSteps (vector< vector<double> > &v)
{
	//先遍历列，再遍历行，否则就要遍历整个矩阵来判断哪一行的先导元素在最前面，浪费资源 
	int cn_main = 0;
	//列 
	for(int j = 0;j < v[0].size();j++)
	{
		bool judge_zero_row = true; 
		//行 
		for(int i = cn_main;i < v.size();i++)
	    {	
	    	if((v[i]).at(j) != 0)
	    	{
	    		vector<double> temp = v[i];
	    		v[i] = v[cn_main];
	    		v[cn_main] = temp;
	    		cn_main++;
	    		judge_zero_row = false;
	    		break;
			}
		
		}
		if(!judge_zero_row)
		{
			for(int i0 = 0;i0 < v.size() - cn_main;i0++)
			{	
				bool judge_savenum = false;
				//在外面定义，不然if花括号结束后就消除其的内存了 ,且无法保存值。 
				double savenum = 0.0;
				for(int h = 0;h < v[0].size();h++)
		    	{
					//保存倍加与倍乘的系数，不然只有第一次生效，后面系数皆为零。 
					if(!judge_savenum)
					{
				    savenum = (v[i0 + cn_main]).at(j);
					judge_savenum = true;
					}
					//'/'是整除，若存在小数，会导致精度丢失问题。
					//所以得用 vector< vector<double> > &v ，而不是 vector< vector<int> > &v 
		    		(v[i0 + cn_main]).at(h) -= ( savenum / (v[cn_main - 1]).at(j) ) 
											   * 
											   ( (v[cn_main - 1]).at(h) );
		    	}
			}
			judge_zero_row = true;
		}				
	}	 
}

void backwardStep(vector< vector<double> > &v)
{
	//行 
	for(int i = v.size() - 1;i >= 0;i--)
	{
		bool judge_main = false;
		//先导元素的位置 
		int cn_leading = -1;
		double savemain = 1.0;
		//列 
		for(int j = 0;j < v[0].size();j++)
		{
			//主元归一 
			if((v[i]).at(j) != 0)
			{			
				if(!judge_main)
				{
					//先导元素的位置
					cn_leading = j;
				    savemain = (v[i]).at(j);
					judge_main = true;
				}
				(v[i]).at(j) /= savemain;
			}	
		}
		if(cn_leading != -1)
		{
				//行 
				for(int k = i - 1;k >= 0;k--)
				{
					bool judge_target = false;
					double savetarget = 0;
					if(!judge_target)
					{
						savetarget = (v[k]).at(cn_leading);
						judge_target = true;
					}
					for(int j = cn_leading;j <= v[0].size() - 1;j++)
					{
						(v[k]).at(j) -= ( savetarget ) * (v[i]).at(j);
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

void printMatrix(const vector< vector<double> > &v)
{	
	cout << "该矩阵对应的简化阶梯矩阵为" << endl;
	for(int i = 0;i < v.size();i++)
	{
		for(int j = 0;j < v[i].size();j++)
		{
			cout << v[i].at(j) << '\t';			
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
