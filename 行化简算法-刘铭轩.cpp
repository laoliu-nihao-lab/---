#include<iostream>
using namespace std;
#include<vector>


/* claude code 的优化 

#include <cmath>
const double EPS = 1e-9;
bool isZero(double x) { return std::fabs(x) < EPS; }
修浮点判零：把 forwardSteps / backwardStep 里所有 != 0 换成 !isZero(...)，
并在向前消元的内层循环结束后显式置零：
*/

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
	cout << endl; 
	cout << "矩阵的行数为：" << endl;
	int m = 0;
	cin >> m;
	cout << "矩阵的列数为：" << endl;
	int n = 0;
	cin >> n;
	cout << "请逐行输入一个增广矩阵或系数矩阵：" << endl;
	//初始为空时，迭代器不能指向v.begin()，因为为空。
	//resize()，重新指定大小，可用来初始化 
	v.resize(m);
	//比起判断全为零，不如判断不能有非零，有一个就退出 
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
	for(int i  = 0;i < Pioneer.size();i++)
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
}

/*claude code优化
 
// 在 initMatrix 里替换原来那段 Pioneer 判断：
int last_lead = -1;
bool zero_seen = false;
for (int i = 0; i < m; i++) {
    int lead = -1;
    for (int j = 0; j < n; j++)
        if (!isZero(v[i][j])) { lead = j; break; }   // 首个非零即先导
    if (lead == -1) zero_seen = true;                 // 零行
    else {
        if (zero_seen || lead <= last_lead) { cn_echelon = false; break; }
        last_lead = lead;
    }
}
*/

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
					//(v[i0 + cn_main]).at(j) = 0.0;
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
 
 //consst保证只读，可用范围for 
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

