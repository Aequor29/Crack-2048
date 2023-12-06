#include <bits/stdc++.h>
#include <time.h>
using namespace std;
const int DEPTH=5;
//我们将棋盘用16行的字母表示
//verify if to continue before computer add new number
pair<double,double> csearch(vector<int>arr,int depth,double beta, double alpha);
pair<double,double> msearch(vector<int>arr,int depth,double beta, double alpha);
int to_end(vector<int> arr){
	for(int i=0;i<16;i++)if(arr[i]==2048)return 1;
	for(int i=0;i<16;i++)if(arr[i]==0)return 0;
	return -1;
}
int log2 (int a){
	if(a==0)return 0;
	int to_return=0;
	while(a!=1){
		//cout<<a<<endl;
		to_return++;
		a/=2;
	}
	return to_return;
}
vector<int> cadd(vector<int> arr){
	vector<int>indices;
	for(int i=0;i<16;i++)if(arr[i]==0)indices.push_back(i);
	if(indices.size()==0)cout<<"error in cadd"<<endl;
	if(rand()%10==0)arr[indices[rand()%indices.size()]]=4;
	else arr[indices[rand()%indices.size()]]=2;
	return arr;
}
vector<vector<int>> cmove(vector<int> arr){
	vector<vector<int>> to_return;
	for(int i=0;i<16;i++)if(arr[i]==0){
		vector<int> arr2(arr);
		vector<int> arr4(arr);
		arr2[i]=2;
		arr4[i]=4;
		to_return.push_back(arr2);
		to_return.push_back(arr4);
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
		for(int j=0;j<4;j++)cout<<arr[i*4+j]<<" ";
		cout<<endl;
	}
	cout<<endl;
}
int diff(vector<int> arr){
	int to_return=0;
	vector<int> carr;
	for(int i=0;i<4;i++)if(arr[i]!=0)carr.push_back(arr[i]);
	//if(carr.size()==0)return 0;
	int upb=carr.size()-1;
	for(int i=0;i<upb;i++){
		if(carr.size()==0&&i==0)cout<<"nwe"<<endl;
		to_return+=abs(log2(carr[i])-log2(carr[i+1]));
	}
	//cout<<carr.size()<<endl;
	return to_return;
}
double smooth(vector<int>arr){
	//cout<<"smooth"<<endl;
	int result=0;
	for(int i=0;i<4;i++){
		//cout<<i<<endl;
		vector<int>carr {arr[i],arr[4+i],arr[8+i],arr[12+i]};
		result+=diff(carr);
		
		carr={arr[4*i],arr[4*i+1],arr[4*i+2],arr[4*i+3]};
		//cout<<carr[0]<<" "<<carr[1]<<" "<<carr[2]<<" "<<carr[3]<<endl;
		result+=diff(carr);
		//cout<<i<<endl;
	}
	return double(result);
}
pair<int,int> diff2(vector<int>arr){
	int l=0;
	int g=0;
	for(int i=0;i<3;i++)if(arr[i]<arr[i+1])l+=arr[i+1]-arr[i];else g+=arr[i]-arr[i+1];
	return make_pair(l,g);
}
double mono(vector<int>arr){
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
	return (double)(max(ds[0],ds[1])+max(ds[2],ds[3]));
}
double emp_size(vector<int>arr){
	int to_return=0;
	for(int i=0;i<16;i++)if(arr[i]==0)to_return++;
	return (double)to_return;
}	
double max_arr(vector<int>arr){
	int to_return=0;
	for(int i=0;i<16;i++)if(to_return<arr[i])to_return=arr[i];
	return (double)to_return;
}
double score(vector<int> arr){
	
	double to_return=0;
	to_return=smooth(arr)*(-1);
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
	if(candidates.size()==0)cout<<"error in madd"<<endl;
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
bool game(){
	vector<int> arr(16);
	
	arr=cadd(arr);
	
	arr=madd(arr);

	while(true){
		//printg(arr);
		
		arr=cadd(arr);
		int sig=to_end(arr);
		if(sig==1)return true;
		if(sig==-1){
			cout<<max_arr(arr)<<endl;
			return false;
		}
		arr=madd(arr);
	}
	cout<<"error in game"<<endl;
	
	return true;
}
		
int main(){
	
	for(int i=0;i<10;i++){
		time_t start_time,end_time;
		start_time=time(NULL);
		
		bool ok=game();
		end_time=time(NULL);
		cout<<end_time-start_time<<" "<<ok<<endl;
	}
	return 0;
}
