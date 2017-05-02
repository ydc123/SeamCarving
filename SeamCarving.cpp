#include"stdafx.h"
#include<cstdio>
#include<cstdlib>
#include<algorithm>
#include<cmath>
#include<cstring>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
typedef long long LL;
void CalcEnergyFunction(int ***mat,int nRows,int nCols,int **E)
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
	int **E;
	int ***mat;
	int **dp1,**dp2,**from1,**from2;
	string name="test9.png";
	//cin>>name;
	Mat M=imread(name);
	int maxn=M.rows+10,maxm=M.cols+10;
	mat=new int** [maxn];
	dp1=new int* [maxn],dp2=new int* [maxm],from1=new int* [maxn],from2=new int* [maxm];
	for(int i=0;i<maxm;++i)
		dp2[i]=new int [maxn],from2[i]=new int [maxn];
	for(int i=0;i<maxn;++i)
	{
		dp1[i]=new int [maxm];
		from1[i]=new int [maxm];
		mat[i]=new int* [maxm];
		for(int j=0;j<maxm;++j)
			mat[i][j]=new int [3];
	}
	E=new int* [maxn];
	for(int i=0;i<maxn;++i)
		E[i]=new int [maxm];
	int goalCol=M.cols/10*9,goalRow=M.rows/10*9,nRows=M.rows,nCols=M.cols,times=nRows-goalRow+nCols-goalCol;
	for(int i=0;i<nRows;++i)
		for(int j=0;j<nCols;++j)
			for(int k=0;k<3;++k)
				mat[i][j][k]=M.at<Vec3b>(i,j)[k];
	while(nRows>goalRow||nCols>goalCol)
	{
		CalcEnergyFunction(mat,nRows,nCols,E);
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
			printf("ÁÐ %d ",min_element(dp1[nRows-1],dp1[nRows-1]+nCols)-dp1[nRows-1]);
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
			printf("ÐÐ %d ",min_element(dp2[nCols-1],dp2[nCols-1]+nRows)-dp2[nCols-1]);
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
	for(int i=0;i<maxm;++i)
		delete[] dp2[i],delete[] from2[i];
	for(int i=0;i<maxn;++i)
	{
		delete[] dp1[i];
		delete[] from1[i];
		for(int j=0;j<maxm;++j)
			delete[] mat[i][j];
		delete[] mat[i];
	}
	delete[] mat;
	for(int i=0;i<maxn;++i)
		delete[] E[i];
	delete[] E;
}
void Enlarging()
{
	string name="test9.png";
	Mat M=imread(name);
	int **E,**dp,**from,***mat;
	int maxn=M.rows+10,maxm=M.cols*2+10;
	bool **isDelete;
	mat=new int** [maxn],dp=new int* [maxn],from=new int*[maxn],isDelete=new bool* [maxn];
	for(int i=0;i<maxn;++i)
	{
		mat[i]=new int* [maxm];
		dp[i]=new int [maxm];
		from[i]=new int[maxm];
		isDelete[i]=new bool[maxm];
		for(int j=0;j<maxm;++j)
		{
			mat[i][j]=new int [3];
			isDelete[i][j]=false;
		}
	}
	E=new int* [maxn];
	for(int i=0;i<maxn;++i)
		E[i]=new int [maxm];
	pair<int,int> *val=new pair<int,int> [maxm];
	int** seq;
	seq=new int* [maxn*maxm];
	for(int i=0;i<maxn*maxm;++i)
		seq[i]=new int [4];
	int goalCol=M.cols*2,nRows=M.rows,nCols=M.cols,times=abs(nCols-goalCol),k=nCols/2;
	for(int i=0;i<nRows;++i)
		for(int j=0;j<nCols;++j)
			for(int k=0;k<3;++k)
				mat[i][j][k]=M.at<Vec3b>(i,j)[k];
	while(nCols<goalCol)
	{
		int delta=min(k,goalCol-nCols);
		int nSeq=0;
		for(int T=1;T<=delta;++T)
		{
			CalcEnergyFunction(mat,nRows,nCols,E);
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
						if(j+1<nCols&&dp[i][j]>dp[i-1][j+1])
							dp[i][j]=dp[i-1][j+1],from[i][j]=j+1;
						dp[i][j]+=e;
					}
				}
			for(int i=nRows-1,j=min_element(dp[nRows-1],dp[nRows-1]+nCols)-dp[nRows-1];i>=0;--i)
			{
				++nSeq;
				seq[nSeq][3]=j;
				for(int k=0;k<3;++k)
					seq[nSeq][k]=mat[i][j][k];
				for(int k=j+1;k<nCols;++k)
					for(int l=0;l<3;++l)
						mat[i][k-1][l]=mat[i][k][l];
				j=from[i][j];
			}
			--nCols;
		}
		for(int i=0;nSeq;i=(i+1)%nRows)
		{
			int j=seq[nSeq][3];
			for(int k=nCols;k>=j+1;--k)
			{
				for(int l=0;l<3;++l)
					mat[i][k][l]=mat[i][k-1][l];
				isDelete[i][k]=isDelete[i][k-1];
			}
			isDelete[i][j]=true;
			for(int k=0;k<3;++k)
				mat[i][j][k]=seq[nSeq][k];
			if(i==nRows-1)
				++nCols;
			nSeq--;
		}
		for(int i=0;i<=nRows;++i)
			for(int j=nCols+delta-1,k=nCols-1;k>=0;--k)
			{
				for(int l=0;l<3;++l)
					mat[i][j][l]=mat[i][k][l];
				--j;
				if(isDelete[i][k])
				{
					isDelete[i][k]=false;
					for(int l=0;l<3;++l)
						mat[i][j][l]=mat[i][k][l];
					--j;
				}
			}
		nCols+=delta;
		printf("%d\n",goalCol-nCols+1);
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
	imwrite("2.png",M);
	for(int i=0;i<maxn;++i)
	{
		for(int j=0;j<maxm;++j)
			delete[] mat[i][j];
		delete[] isDelete[i];
		delete[] from[i];
		delete[] dp[i];
		delete[] mat[i];
	}
	delete[] mat;
	delete[] dp;
	delete[] from;
	delete[] isDelete;
	for(int i=0;i<maxn;++i)
		delete[] E[i];
	delete[] E;
	delete[] val;
	for(int i=0;i<maxn*maxm;++i)
		delete[] seq[i];
	delete[] seq;
}
int _tmain(int argc, _TCHAR* argv[])
{
	SeamCarving();
	Enlarging();
	return 0;
}

