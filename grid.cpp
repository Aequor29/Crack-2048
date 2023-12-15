#include <bits/stdc++.h>
#include <time.h>
using namespace std;
ofstream fout;
int DEPTH=8;
//double S1=-1.0,S2=1.0,M=-0.5,E=0.7,W=3.0,I=1.0;
double S1=-1.0,S2=1.0,M=1.0,E=1.0,W=0.0,I=1.0;
double tl[]={36.0,54.0,16.0,36.0,12.0};
double ts[]={0.16,0.32,0.0025,0.0025,0.001};
//M should be positive
//S1 should be negative
//E should be positive
//W is disastrous

pair<double,double> csearch(vector<int>arr,int depth,double beta, double alpha);
pair<double,double> msearch(vector<int>arr,int depth,double beta, double alpha);
int dir[]={-4,4,-1,1};
int to_end(vector<int> arr){
	for(int i=0;i<16;i++)if(arr[i]==0)return 0;
	return -1;
}

int log2 (int a){
	if(a==0)return 0;
	int to_return=0;
	while(a!=1){
		//fout<<a<<endl;
		to_return++;
		a/=2;
	}
	return to_return;
}

int to2 (int a){
	int to_return=1;
	for(int i=0;i<a;i++)to_return*=2;
	return to_return;
}

vector<int> cadd(vector<int> arr){
	vector<int>indices;
	for(int i=0;i<16;i++)if(arr[i]==0)indices.push_back(i);
	if(indices.size()==0)fout<<"error in cadd"<<endl;
	if(rand()%10==0)arr[indices[rand()%indices.size()]]=4;
	else arr[indices[rand()%indices.size()]]=2;
	return arr;
}

vector<int> marki(vector<int>arr,int x){
	arr[x]=-1;
	for(int i=0;i<4;i++){
		int y=x+dir[i];
		if(y>=0&&y<16&&arr[y]==0)arr=marki(arr,y);
	}
	return arr;
}

int island(vector<int> arr){
	int to_return=0;
	for(int i=0;i<16;i++)if(arr[i]==0){
		to_return++;
		arr=marki(arr,i);
	}
	return to_return;
}
pair<int,int> diff2(vector<int>arr){
	int l=0;
	int g=0;
	for(int i=0;i<3;i++)if(arr[i]<arr[i+1])l+=arr[i+1]-arr[i];else g+=arr[i]-arr[i+1];
	return make_pair(l,g);
}
int mono(vector<int>arr){
	vector<int> ds {0,0,0,0};
	for(int i=0;i<4;i++){
		vector<int>carr {arr[i],arr[4+i],arr[8+i],arr[12+i]};
		pair<int,int>pa=diff2(carr);
		ds[0]+=pa.first;
		ds[1]+=pa.second;
		
		carr={arr[4*i],arr[4*i+1],arr[4*i+2],arr[4*i+3]};
		pa=diff2(carr);
		ds[2]+=pa.first;
		ds[3]+=pa.second;
	}
	return max(ds[0],ds[1])+max(ds[2],ds[3]);
}
int diff(vector<int> arr){
	int to_return=0;
	vector<int> carr;
	for(int i=0;i<4;i++)if(arr[i]!=0)carr.push_back(arr[i]);
	//if(carr.size()==0)return 0;
	int upb=carr.size()-1;
	for(int i=0;i<upb;i++){
		if(carr.size()==0&&i==0)fout<<"nwe"<<endl;
		to_return+=abs(log2(carr[i])-log2(carr[i+1]));
	}
	//fout<<carr.size()<<endl;
	return to_return;
}
int smooth(vector<int>arr){
	//fout<<"smooth"<<endl;
	int result=0;
	for(int i=0;i<4;i++){
		//fout<<i<<endl;
		vector<int>carr {arr[i],arr[4+i],arr[8+i],arr[12+i]};
		result+=diff(carr);
		
		carr={arr[4*i],arr[4*i+1],arr[4*i+2],arr[4*i+3]};
		//fout<<carr[0]<<" "<<carr[1]<<" "<<carr[2]<<" "<<carr[3]<<endl;
		result+=diff(carr);
		//fout<<i<<endl;
	}
	return result;
}
double cscore(vector<int> arr){
	return S2*smooth(arr)+I*island(arr);
}
vector<vector<int>> cmove(vector<int> arr){
	vector<vector<int>> to_return;
	double large_score=0;
	for(int i=0;i<16;i++)if(arr[i]==0){
		vector<int> arr2(arr);
		vector<int> arr4(arr);
		arr2[i]=2;
		arr4[i]=4;
		large_score=max(large_score,cscore(arr2));
		large_score=max(large_score,cscore(arr4));
	}
	for(int i=0;i<16;i++)if(arr[i]==0){
		vector<int> arr2(arr);
		vector<int> arr4(arr);
		arr2[i]=2;
		arr4[i]=4;
		if(large_score==cscore(arr2))to_return.push_back(arr2);
		if(large_score==cscore(arr4))to_return.push_back(arr4);
	}
	return to_return;
}


vector<int> eli(vector<int> arr){
	vector<int>narr;
	vector<int>tarr;
	for(int i=0;i<4;i++)if(arr[i]!=0)narr.push_back(arr[i]);
	int nsize=narr.size();
	for(int i=0;i<nsize;i++){
		if(i!=nsize-1&&narr[i]==narr[i+1]){tarr.push_back(narr[i]+narr[i+1]);i+=1;}
		else tarr.push_back(narr[i]);
	}
	for(int i=tarr.size();i<4;i++)tarr.push_back(0);
	return tarr;
}
vector<vector<int>> mmove(vector<int> arr){
	vector<vector<int>> to_return;
	vector<int> arru(arr);
	for(int i=0;i<4;i++){
		vector<int>to_eli {arr[i],arr[i+4],arr[i+8],arr[i+12]};
		to_eli=eli(to_eli);
		for(int j=0;j<4;j++)arru[i+4*j]=to_eli[j];
	}
	if(arru!=arr)to_return.push_back(arru);
	vector<int> arrd(arr);
	for(int i=0;i<4;i++){
		vector<int>to_eli {arr[i+12],arr[i+8],arr[i+4],arr[i]};
		to_eli=eli(to_eli);
		for(int j=0;j<4;j++)arrd[i+12-4*j]=to_eli[j];
	}
	if(arrd!=arr)to_return.push_back(arrd);
	vector<int> arrl(arr);
	for(int i=0;i<4;i++){
		vector<int>to_eli {arr[i*4],arr[i*4+1],arr[i*4+2],arr[i*4+3]};
		to_eli=eli(to_eli);
		for(int j=0;j<4;j++)arrl[i*4+j]=to_eli[j];
	}
	if(arrl!=arr)to_return.push_back(arrl);
	vector<int> arrr(arr);
	for(int i=0;i<4;i++){
		vector<int>to_eli {arr[i*4+3],arr[i*4+2],arr[i*4+1],arr[i*4]};
		to_eli=eli(to_eli);
		for(int j=0;j<4;j++)arrr[i*4+3-j]=to_eli[j];
	}
	if(arrr!=arr)to_return.push_back(arrr);
	return to_return;
}

void printg(vector<int> arr){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++)fout<<arr[i*4+j]<<" ";
		fout<<endl;
	}
	fout<<endl;
}


int emp_size(vector<int>arr){
	int to_return=0;
	for(int i=0;i<16;i++)if(arr[i]==0)to_return++;
	return to_return;
}	
int max_arr(vector<int>arr){
	int to_return=0;
	for(int i=0;i<16;i++)if(to_return<arr[i])to_return=arr[i];
	return log2(to_return);
}

double score(vector<int> arr){
	double to_return=0;
	to_return=S1*smooth(arr)+M*mono(arr)+E*emp_size(arr)+W*max_arr(arr);
	//+log(emp_size(arr))*2.7+max_arr(arr)
	return to_return;
}
pair<double,double> csearch(vector<int>arr,int depth,double beta, double alpha){
	
	if(depth==0)return make_pair(score(arr),alpha);		
	vector<vector<int>>candidates=cmove(arr);
	if(candidates.size()==0)return make_pair(score(arr),alpha);		
	for(vector<int> candi:candidates){
		
		pair<int,int>pa=msearch(candi,depth-1,beta,alpha);
		double calpha=pa.second;
		if(calpha<beta){
			beta=calpha;
		}
		if(alpha>=beta)break;
	}
	
	return make_pair(beta,alpha);
}
pair<double,double> msearch(vector<int>arr,int depth,double beta, double alpha){
	if(depth==0)return make_pair(beta,score(arr));		
	vector<vector<int>>candidates=mmove(arr);
	if(candidates.size()==0)return make_pair(beta,score(arr));
	for(vector<int> candi:candidates){
		pair<int,int>pa=csearch(candi,depth-1,beta,alpha);
		double cbeta=pa.first;
		if(cbeta>alpha){
			alpha=cbeta;
		}
		if(alpha>=beta)break;
	}
	return make_pair(beta,alpha);
}
vector<int> madd(vector<int>arr){
	
	vector<vector<int>>candidates=mmove(arr);
	if(candidates.size()==0)return arr;
	vector<int> to_return;
	double beta=1000000;
	double alpha=-1000000;
	for(vector<int> candi:candidates){
		
		pair<int,int>pa=csearch(candi,DEPTH,beta,alpha);
		
		double cbeta=pa.first;
		
		if(cbeta>alpha){
			alpha=cbeta;
			to_return=candi;
		}
	}
	return to_return;
}
int game(){
	vector<int> arr(16);
	
	arr=cadd(arr);
	
	arr=madd(arr);

	while(true){
		//printg(arr);
		
		arr=cadd(arr);
		arr=madd(arr);
		int sig=to_end(arr);
		if(sig==-1){
			return to2(max_arr(arr));
		}
		
	}
	fout<<"error in game"<<endl;
	
	return 0;
}
		
int main(){
	fout.open("test_result3.txt");
	for(int x=0;x<5;x++){
		fout<<endl<<"M="<<ts[x]<<" E="<<tl[x]<<endl;
		M=ts[x];
		E=tl[x];
		double sum=0;
		for(int i=0;i<10;i++){
			time_t start_time,end_time;
			start_time=time(NULL);
		
			int result=game();
			sum+=result;
			end_time=time(NULL);
			fout<<end_time-start_time<<" "<<result<<endl;
		}
		fout<<endl<<"evaluation: "<<sum/10<<endl;
	}
	fout.close();
	return 0;
}
