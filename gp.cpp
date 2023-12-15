#include <bits/stdc++.h>
#include <time.h>
using namespace std;
ofstream fout;
int DEPTH=8;

int research_per_move=20;
int research_depth=10;


int dir[]={-4,4,-1,1};
int to_end(vector<int> arr){
	for(int i=0;i<16;i++)if(arr[i]==0)return 0;
	return -1;
}





vector<int> cadd(vector<int> arr){
	vector<int>indices;
	for(int i=0;i<16;i++)if(arr[i]==0)indices.push_back(i);
	if(indices.size()==0)fout<<"error in cadd"<<endl;
	if(rand()%10==0)arr[indices[rand()%indices.size()]]=4;
	else arr[indices[rand()%indices.size()]]=2;
	return arr;
}


pair<int,vector<int>> eli(vector<int> arr){
	vector<int>narr;
	vector<int>tarr;
	int return_v=0;
	for(int i=0;i<4;i++)if(arr[i]!=0)narr.push_back(arr[i]);
	int nsize=narr.size();
	for(int i=0;i<nsize;i++){
		if(i!=nsize-1&&narr[i]==narr[i+1]){tarr.push_back(narr[i]+narr[i+1]);return_v+=narr[i]+narr[i+1];i+=1;}
		else tarr.push_back(narr[i]);
	}
	for(int i=tarr.size();i<4;i++)tarr.push_back(0);
	return make_pair(return_v,tarr);
}
vector<pair<int,vector<int>>> mmove(vector<int> arr){
	vector<pair<int,vector<int>>> to_return;
	vector<int> arru(arr);
	int vu=0;
	for(int i=0;i<4;i++){
		vector<int>to_eli {arr[i],arr[i+4],arr[i+8],arr[i+12]};
		pair<int,vector<int>>pa=eli(to_eli);
		to_eli=pa.second;
		for(int j=0;j<4;j++)arru[i+4*j]=to_eli[j];
		vu+=pa.first;
	}
	if(arru!=arr)to_return.push_back(make_pair(vu,arru));
	vector<int> arrd(arr);
	int vd=0;
	for(int i=0;i<4;i++){
		vector<int>to_eli {arr[i+12],arr[i+8],arr[i+4],arr[i]};
		pair<int,vector<int>>pa=eli(to_eli);
		to_eli=pa.second;
		for(int j=0;j<4;j++)arrd[i+12-4*j]=to_eli[j];
		vd+=pa.first;
	}
	if(arrd!=arr)to_return.push_back(make_pair(vd,arrd));
	vector<int> arrl(arr);
	int vl=0;
	for(int i=0;i<4;i++){
		vector<int>to_eli {arr[i*4],arr[i*4+1],arr[i*4+2],arr[i*4+3]};
		pair<int,vector<int>>pa=eli(to_eli);
		to_eli=pa.second;
		for(int j=0;j<4;j++)arrl[i*4+j]=to_eli[j];
		vl+=pa.first;
	}
	if(arrl!=arr)to_return.push_back(make_pair(vl,arrl));
	vector<int> arrr(arr);
	int vr=0;
	for(int i=0;i<4;i++){
		vector<int>to_eli {arr[i*4+3],arr[i*4+2],arr[i*4+1],arr[i*4]};
		pair<int,vector<int>>pa=eli(to_eli);
		to_eli=pa.second;
		for(int j=0;j<4;j++)arrr[i*4+3-j]=to_eli[j];
		vr+=pa.first;
	}
	if(arrr!=arr)to_return.push_back(make_pair(vr,arrr));
	return to_return;
}

void printg(vector<int> arr){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++)fout<<arr[i*4+j]<<" ";
		fout<<endl;
	}
	fout<<endl;
}


int max_arr(vector<int>arr){
	int to_return=0;
	for(int i=0;i<16;i++)if(to_return<arr[i])to_return=arr[i];
	return to_return;
}


vector<int> madd(vector<int>arr){
	
	vector<pair<int,vector<int>>>candidates=mmove(arr);
	if(candidates.size()==0)return arr;
	vector<int> to_return;
	int c_score=-1;
	for(pair<int,vector<int>> pa:candidates){
		int new_score=pa.first;
		//cout<<pa.first<<endl;
		vector<int>new_board=pa.second;
		vector<int>cur_board;
		for(int i=0;i<research_per_move;i++){
			vector<int>cur_board=new_board;
			for(int j=0;j<research_depth;j++){
				cur_board=cadd(cur_board);
				vector<pair<int,vector<int>>>new_candidates=mmove(cur_board);
				if(new_candidates.size()==0)break;
				int ind=rand()%new_candidates.size();
				new_score+=new_candidates[ind].first;
				cur_board=new_candidates[ind].second;
			}
		}
		if(new_score>c_score){c_score=new_score;to_return=new_board;}
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
			return max_arr(arr);
		}
	}
	fout<<"error in game"<<endl;
	
	return 0;
}
		
int main(){
	fout.open("test_result2.txt");

	for(int i=0;i<10;i++){
		time_t start_time,end_time;
		start_time=time(NULL);
		
		int result=game();
		end_time=time(NULL);
		fout<<end_time-start_time<<" "<<result<<endl;
	}
	fout.close();
	return 0;
}