// SeamCarving.cpp : 定义控制台应用程序的入口点。
//

#include"stdafx.h"
#include<cstdio>
#include<cstdlib>
#include<algorithm>
#include<cmath>
#include<cstring>
#include<opencv2/opencv.hpp>
#define maxn 1300
using namespace std;
using namespace cv;
int E[maxn][maxn],mat[maxn][maxn][3];
void CalcEnergyFunction(int M[maxn][maxn][3],int nRows,int nCols)
{
	for(int i=0;i<nRows;++i)
		for(int j=0;j<nCols;++j)
		{
			int val1=0,val2=0,pi=i==0?i+1:i-1,pj=j==0?j+1:j-1;
			int *P=M[i][j],*Qx=M[pi][j],*Qy=M[i][pj];
			for(int k=0;k<3;++k)
			{
				int p=P[k],qx=Qx[k],qy=Qy[k];
				val1+=abs(p-qx),val2+=abs(p-qy);
			}
			E[i][j]=val1+val2;
		}
}
void SeamCarving()//将列缩小10%
{
	string name;
	cin>>name;
	static int dp[maxn][maxn],from[maxn][maxn];
	Mat M=imread(name);
	for(int i=0;i<M.rows;++i)
		for(int j=0;j<M.cols;++j)
			for(int k=0;k<3;++k)
				mat[i][j][k]=M.at<Vec3b>(i,j)[k];
	int goal=M.cols/10*9,nRows=M.rows,nCols=M.cols;
	CalcEnergyFunction(mat,nRows,nCols);
	for(int T=M.cols-goal;T>=1;--T)
	{
		--nCols;
		CalcEnergyFunction(mat,nRows,nCols);
		for(int i=0;i<nRows;++i)
			for(int j=0;j<nCols;++j)
			{
				int e=E[i][j];
				if(i==0)
					dp[i][j]=e;
				else
				{
					dp[i][j]=dp[i-1][j],from[i][j]=j;
					if(j>0&&dp[i][j]>dp[i-1][j-1])
						dp[i][j]=dp[i-1][j-1],from[i][j]=j-1;
					if(j+1<nRows&&dp[i][j]>dp[i-1][j+1])
						dp[i][j]=dp[i-1][j+1],from[i][j]=j+1;
					dp[i][j]+=e;
				}
			}
		for(int i=nRows-1,j=min_element(dp[nRows-1],dp[nRows-1]+nCols)-dp[nRows-1];i>=0;--i)
		{
			if(i==nRows-1)
				printf("%d ",j);
			for(int k=j+1;k<nCols;++k)
				for(int l=0;l<3;++l)
					mat[i][k-1][l]=mat[i][k][l];
		}
		printf("%d\n",T);
	}
	M=Mat(nRows,nCols,CV_8UC3);
	for(int i=0;i<nRows;++i)
		for(int j=0;j<nCols;++j)
			for(int k=0;k<3;++k)
				M.at<Vec3b>(i,j)[k]=mat[i][j][k];
	imwrite("1.png",M);
}
int _tmain(int argc, _TCHAR* argv[])
{
	SeamCarving();
	return 0;
}

