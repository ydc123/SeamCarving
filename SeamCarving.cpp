#include"stdafx.h"
#include<cstdio>
#include<cstdlib>
#include<algorithm>
#include<cmath>
#include<cstring>
#define inf 100000
#include<opencv2/opencv.hpp>
#define maxn 4000
using namespace std;
using namespace cv;
typedef long long LL;
int seq[maxn*maxn][4];
int mat[maxn][maxn][3],from1[maxn][maxn],from2[maxn][maxn],dp1[maxn][maxn],dp2[maxn][maxn],E[maxn][maxn];
bool isDelete[maxn][maxn];
pair<int,int> val[maxn];
void CalcEnergyFunction(int mat[maxn][maxn][3],int nRows,int nCols,int E[maxn][maxn])
{
	const int INF=1000000;
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
					E[i][j]=E[i][j]+abs(mat[i][j][k]-mat[i][j+1][k])+abs(mat[i][j][k]-mat[i+1][j][k]);
					E[i][j]=E[i][j]+abs(mat[i][j][k]-mat[i][j-1][k])+abs(mat[i][j][k]-mat[i-1][j][k]);
				}
			}
		}
	for(int i=0;i<nRows;++i)
		for(int j=0;j<nCols;++j)
		{
			if(i==0||j==0||i==nRows-1||j==nCols-1)
				E[i][j]=INF;
			else
			{
				int val=0;
				for(int k=0;k<3;++k)
				{
					val+=abs(mat[i-1][j][k]-mat[i+1][j][k]);
					val+=abs(mat[i][j-1][k]-mat[i][j+1][k]);
				}
				E[i][j]=val;
			}
		}

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
	for(int i=0;i<nRows;++i)
		for(int j=0;j<nCols;++j)
		{
			if(i==0||j==0||i==nRows-1||j==nCols-1)
				E[i][j]=INF;
			else
			{
				int val=0;
				for(int k=0;k<3;++k)
					val+=abs(mat[i-1][j][k]+mat[i+1][j][k]+mat[i][j+1][k]+mat[i][j-1][k]-4*mat[i][j][k]);
				E[i][j]=val;
			}
		}
	
}
void Reduce()
{
	string name="2.png";
	Mat M=imread(name);
	int nRows=M.rows,nCols=M.cols,goalCol=nCols/10*8,goalRow=nRows,times=nRows-goalRow+nCols-goalCol;
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
			printf("ап %d ",min_element(dp1[nRows-1],dp1[nRows-1]+nCols)-dp1[nRows-1]);
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
			printf("пп %d ",min_element(dp2[nCols-1],dp2[nCols-1]+nRows)-dp2[nCols-1]);
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
	imwrite("2_reduce.jpg",M);	
}
void Enlarging()
{
	string name="test9.png";
	Mat M=imread(name);
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
			for(int i=nRows-1,j=min_element(dp1[nRows-1],dp1[nRows-1]+nCols)-dp1[nRows-1];i>=0;--i)
			{
				++nSeq;
				seq[nSeq][3]=j;
				for(int k=0;k<3;++k)
					seq[nSeq][k]=mat[i][j][k];
				for(int k=j+1;k<nCols;++k)
					for(int l=0;l<3;++l)
						mat[i][k-1][l]=mat[i][k][l];
				j=from1[i][j];
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
	imwrite("test_9.png",M);
}

void SeamGraph()
{
	string name="2.png";
	Mat M=imread(name);
	int nRows=M.rows,nCols=M.cols,k=200;
	for(int i=0;i<nRows;++i)
		for(int j=0;j<nCols;++j)
			for(int k=0;k<3;++k)
				mat[i][j][k]=M.at<Vec3b>(i,j)[k];
	int delta=k;
	int nSeq=0;
	for(int T=1;T<=delta;++T)
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
		for(int i=nRows-1,j=min_element(dp1[nRows-1],dp1[nRows-1]+nCols)-dp1[nRows-1];i>=0;--i)
		{
			++nSeq;
			seq[nSeq][3]=j;
			for(int k=0;k<3;++k)
				seq[nSeq][k]=mat[i][j][k];
			for(int k=j+1;k<nCols;++k)
				for(int l=0;l<3;++l)
					mat[i][k-1][l]=mat[i][k][l];
			j=from1[i][j];
		}
		printf("ап %d\n",delta-T+1);
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
	M=Mat(nRows,nCols,CV_8UC3);
	for(int i=0;i<nRows;++i)
		for(int j=0;j<nCols;++j)
		{
			Vec3b pixel;
			if(isDelete[i][j])
				pixel=Vec3b(255,0,0);
			else
				for(int k=0;k<3;++k)
					pixel[k]=mat[i][j][k];
			M.at<Vec3b>(i,j)=pixel;
		}
	imwrite("2_seam.png",M);
}
struct Marker
{
	Mat ori,img,type;
	static void on_Mouse(int event,int x,int y,int flags,void *obj)
	{
		Marker* now=static_cast<Marker*>(obj);
		if(event==CV_EVENT_LBUTTONDOWN||(event==CV_EVENT_MOUSEMOVE&&(flags&CV_EVENT_FLAG_LBUTTON)))
		{
			circle(now->img,Point(x,y),20,Scalar(0,255,0),-1);
			circle(now->type,Point(x,y),20,1,-1);
			imshow("img",now->img);
		}
		if(event==CV_EVENT_RBUTTONDOWN||(event==CV_EVENT_MOUSEMOVE&&(flags&CV_EVENT_FLAG_RBUTTON)))
		{
			circle(now->img,Point(x,y),20,Scalar(255,0,0),-1);
			circle(now->type,Point(x,y),20,2,-1);
			imshow("img",now->img);
		}
	}
	Marker(){}
	Marker(const string &name)
	{
		ori=imread(name);
		ori.copyTo(img);
		type=Mat::zeros(img.size(),CV_8UC1);
		namedWindow("img");
		setMouseCallback("img",on_Mouse,this);
		imshow("img",img);
		while(1)
		{
			int c=waitKey(0);
			if(c==27)
				break;
		}
	}
};
uchar type[maxn][maxn];
void ObjectRemove()
{
	Marker *rem=new Marker("3.jpg");
	Mat M=rem->ori,Mt=rem->type;
	int nRows=M.rows,nCols=M.cols;
	for(int i=0;i<nRows;++i)
		for(int j=0;j<nCols;++j)
		{
			for(int k=0;k<3;++k)
				mat[i][j][k]=M.at<Vec3b>(i,j)[k];
			type[i][j]=Mt.at<uchar>(i,j);
		}
	while(1)
	{
		int cntRemove=0;
		for(int i=0;i<nRows;++i)
			for(int j=0;j<nCols;++j)
				if(type[i][j]==1)
					++cntRemove;
		if(!cntRemove)
			break;
		printf("%d\n",cntRemove);
		CalcEnergyFunction(mat,nRows,nCols,E);
		for(int i=0;i<nRows;++i)
			for(int j=0;j<nCols;++j)
			{
				int e=E[i][j];
				if(type[i][j]==1)
					e=-inf;
				if(type[i][j]==2)
					e=inf;
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
		LL cost_col=*min_element(dp1[nRows-1],dp1[nRows-1]+nCols);
		for(int i=nRows-1,j=min_element(dp1[nRows-1],dp1[nRows-1]+nCols)-dp1[nRows-1];i>=0;--i)
		{
			for(int k=j+1;k<nCols;++k)
			{
				for(int l=0;l<3;++l)
					mat[i][k-1][l]=mat[i][k][l];
				type[i][k-1]=type[i][k];
			}
			j=from1[i][j];
		}
		--nCols;
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
	imwrite("test3_remove.jpg",M);	
}
int _tmain(int argc, _TCHAR* argv[])
{
	//Reduce();
	//Enlarging();
	//SeamGraph();
	ObjectRemove();
	return 0;
}
