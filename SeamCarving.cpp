 SeamCarving.cpp : 定义控制台应用程序的入口点。


#include"stdafx.h"
#include<cstdio>
#include<cstdlib>
#include<algorithm>
#include<cmath>
#include<cstring>
#include<opencv2/opencv.hpp>
#define maxn 3000
using namespace std;
using namespace cv;
typedef long long LL;
int E[maxn][maxn],mat[maxn][maxn][3];
void CalcEnergyFunction(int M[maxn][maxn][3],int nRows,int nCols)
{
	const int INF=1000000;
	//for(int i=0;i<nRows;++i)
	//	for(int j=0;j<nCols;++j)
	//	{
	//		if(i==0||j==0||i==nRows-1||j==nCols-1)
	//			E[i][j]=INF;
	//		else
	//		{
	//			E[i][j]=0;
	//			for(int k=0;k<3;++k)
	//			{
	//				E[i][j]=E[i][j]+abs(mat[i][j][k]-mat[i][j+1][k])+abs(mat[i][j][k]-mat[i+1][j][k]);
	//				E[i][j]=E[i][j]+abs(mat[i][j][k]-mat[i][j-1][k])+abs(mat[i][j][k]-mat[i-1][j][k]);
	//			}
	//		}
	//	}
	//for(int i=0;i<nRows;++i)
	//	for(int j=0;j<nCols;++j)
	//	{
	//		if(i==0||j==0||i==nRows-1||j==nCols-1)
	//			E[i][j]=INF;
	//		else
	//		{
	//			int val=0;
	//			for(int k=0;k<3;++k)
	//			{
	//				val+=abs(mat[i-1][j][k]-mat[i+1][j][k]);
	//				val+=abs(mat[i][j-1][k]-mat[i][j+1][k]);
	//			}
	//			E[i][j]=val;
	//		}
	//	}

	for(int i=0;i<nRows;++i)
		for(int j=0;j<nCols;++j)
		{
			if(i==0||j==0||i==nRows-1||j==nCols-1)
				E[i][j]=INF;
			else
			{
				E[i][j]=0;
				for(int k=0;k<3;++k)
				{
					int val1=mat[i-1][j+1][k]-mat[i-1][j-1][k]+2*(mat[i][j+1][k]-mat[i][j-1][k])+mat[i+1][j+1][k]-mat[i+1][j-1][k];
					int val2=mat[i-1][j-1][k]-mat[i+1][j-1][k]+2*(mat[i-1][j][k]-mat[i+1][j][k])+mat[i-1][j+1][k]-mat[i-1][j+1][k];
					E[i][j]=abs(val1)+abs(val2);
				}
			}
		}
	//for(int i=0;i<nRows;++i)
	//	for(int j=0;j<nCols;++j)
	//	{
	//		if(i==0||j==0||i==nRows-1||j==nCols-1)
	//			E[i][j]=INF;
	//		else
	//		{
	//			int val=0;
	//			for(int k=0;k<3;++k)
	//				val+=abs(mat[i-1][j][k]+mat[i+1][j][k]+mat[i][j+1][k]+mat[i][j-1][k]-4*mat[i][j][k]);
	//			E[i][j]=val;
	//		}
	//	}
}
void SeamCarving()
{
	string name="test.png";
	//cin>>name;
	static int dp1[maxn][maxn],from1[maxn][maxn];
	static int dp2[maxn][maxn],from2[maxn][maxn];
	Mat M=imread(name);
	int goalCol=M.cols/10*8,goalRow=M.rows/10*8,nRows=M.rows,nCols=M.cols,times=nRows-goalRow+nCols-goalCol;
	for(int i=0;i<nRows;++i)
		for(int j=0;j<nCols;++j)
			for(int k=0;k<3;++k)
				mat[i][j][k]=M.at<Vec3b>(i,j)[k];
	while(nRows>goalRow||nCols>goalCol)
	{
		CalcEnergyFunction(mat,nRows,nCols);
		for(int i=0;i<nRows;++i)
			for(int j=0;j<nCols;++j)
			{
				int e=E[i][j];
				if(i==0)
					dp1[i][j]=e;
				else
				{
					dp1[i][j]=dp1[i-1][j],from1[i][j]=j;
					if(j>0&&dp1[i][j]>dp1[i-1][j-1])
						dp1[i][j]=dp1[i-1][j-1],from1[i][j]=j-1;
					if(j+1<nCols&&dp1[i][j]>dp1[i-1][j+1])
						dp1[i][j]=dp1[i-1][j+1],from1[i][j]=j+1;
					dp1[i][j]+=e;
				}
			}
		for(int j=0;j<nCols;++j)
			for(int i=0;i<nRows;++i)
			{
				int e=E[i][j];
				if(j==0)
					dp2[j][i]=e;
				else
				{
					dp2[j][i]=dp2[j-1][i],from2[j][i]=i;
					if(i>0&&dp2[j][i]>dp2[j-1][i-1])
						dp2[j][i]=dp2[j-1][i-1],from2[j][i]=i-1;
					if(i+1<nRows&&dp2[j][i]>dp2[j-1][i+1])
						dp2[j][i]=dp2[j-1][i+1],from2[j][i]=i+1;
					dp2[j][i]+=e;
				}
			}
		LL cost_col=*min_element(dp1[nRows-1],dp1[nRows-1]+nCols);
		LL cost_row=*min_element(dp2[nCols-1],dp2[nCols-1]+nRows);
		if(nCols>goalCol&&(nRows==goalRow||cost_col*nRows<cost_row*nCols))
		{
			for(int i=nRows-1,j=min_element(dp1[nRows-1],dp1[nRows-1]+nCols)-dp1[nRows-1];i>=0;--i)
			{
				for(int k=j+1;k<nCols;++k)
					for(int l=0;l<3;++l)
						mat[i][k-1][l]=mat[i][k][l];
				j=from1[i][j];
			}
			printf("列 %d ",min_element(dp1[nRows-1],dp1[nRows-1]+nCols)-dp1[nRows-1]);
			--nCols;
		}
		else
		{
			for(int j=nCols-1,i=min_element(dp2[nCols-1],dp2[nCols-1]+nRows)-dp2[nCols-1];j>=0;--j)
			{
				for(int k=i+1;k<nRows;++k)
					for(int l=0;l<3;++l)
						mat[k-1][j][l]=mat[k][j][l];
				i=from2[j][i];
			}
			printf("行 %d ",min_element(dp2[nCols-1],dp2[nCols-1]+nRows)-dp2[nCols-1]);
			--nRows;
		}
		printf("%d\n",times--);
	}
	M=Mat(nRows,nCols,CV_8UC3);
	for(int i=0;i<nRows;++i)
		for(int j=0;j<nCols;++j)
		{
			Vec3b pixel;
			for(int k=0;k<3;++k)
				pixel[k]=mat[i][j][k];
			M.at<Vec3b>(i,j)=pixel;
		}
	imwrite("1.png",M);
}
int _tmain(int argc, _TCHAR* argv[])
{
	SeamCarving();
	return 0;
}

