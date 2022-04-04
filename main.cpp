//
//  main.cpp
//  Extendible_Hashing
//
//  Created by Mansi on 31/03/22.
//

#include <iostream>
#include <vector>
#include<math.h>
#include<bits/stdc++.h>
using namespace std;

int gen_key(int n,int gd){
    int key=0,i=0;
    while(gd){
        key=key+(n%2)*pow(2,i);
        n=n/2;
        ++i;
        --gd;
    }
    return key;
}

int key_present(int key, vector<pair<int,int>> &A){
    for(int i=0;i<A.size();i++){
        if(key==A[i].first){
            return A[i].second;
        }
    }
    return -1;
}

bool insert(int value, int id, vector<pair<int,vector<int>>> &A,int bc){
    bool found=false;
    for(int i=0;i<A.size();i++){
        if(A[i].first==id){
            found=true;
            if(A[i].second.size()<bc){
                A[i].second.push_back(value);
                //cout<<"HOLA"<<endl;
                return true;
            }
        }
    }
    
    if(!found){
        vector<int> temp;
        temp.push_back(value);
        A.push_back(make_pair(id,temp));
        //cout<<"HERE"<<endl;
        return true;
    }
    //cout<<"returning false"<<endl;
    return false;
}

void double_dir(vector<pair<int,int>> &A,int gd){
    int n=A.size();
    for(int i=0;i<n;i++){
        A.push_back(make_pair(i+gd+1,A[i].second));
    }
}
bool key_present(int key, vector<pair<int,vector<int>>> &A){
    for(int i=0;i<A.size();i++){
        if(A[i].first==key){
            return true;
        }
    }
    return false;
}

vector<pair<int,vector<int>>> create_new_buckets(vector<int> &A,int b,int bc){
    vector<pair<int,vector<int>>> ans;
    
    for(int i=0;i<A.size();i++){
        int key=gen_key(A[i], b);
        if(!key_present(key, ans)){
            vector<int> x;
            x.push_back(A[i]);
            ans.push_back(make_pair(key,x));
        }else{
            if(!insert(A[i],key,ans,bc)){
                ans=create_new_buckets(A, b+1, bc);
                return ans;
            }
        }
    }
    return ans;
    
}

void split_bucket(int id, int value, int b, int bc, vector<pair<int,vector<int>>> &A){
    //fetching the bucket
    vector<int> temp;
    vector<pair<int,vector<int>>> :: iterator itr=A.begin();
    for(int i=0;i<A.size();i++,itr++){
        if(A[i].first==id){
            temp=A[i].second;
            A.erase(itr);
        }
    }
    temp.push_back(value);
    vector<pair<int,vector<int>>> temp2;
    temp2=create_new_buckets(temp,b,bc);
    for(int i=0;i<temp2.size();i++){
        A.push_back(make_pair(temp2[i].first,temp2[i].second));
    }
}

void re_configure(vector<pair<int,vector<int>>> &A,vector<pair<int,int>> &B){
    for(int i=0;i<A.size();i++){
        for(int j=0;j<B.size();j++){
            if(A[i].first==B[j].first){
                B[j].second=A[i].first;
            }
        }
    }
    return;
}

bool val_present(int value,int id, vector<pair<int,vector<int>>> &A){
    for(int i=0;i<A.size();i++){
        if(A[i].first==id){
            for(int j=0;j<A[i].second.size();j++){
                if(A[i].second[j]==value){
                    return true;
                }
            }
        }
    }
    return false;
}

void delete_val(int value, int id, vector<pair<int,vector<int>>> &A){
    for(int i=0;i<A.size();i++){
        if(A[i].first==id){
            vector<int> :: iterator itr;
            for(itr=A[i].second.begin();itr!=A[i].second.end();itr++){
                if(*itr==value){
                    A[i].second.erase(itr);
                    break;
                }
            }
        }
    }
}
void print_buckets(vector<pair<int,vector<int>>> &A){
    cout<<"buckets:"<<endl;
    for(int i=0;i<A.size();i++){
        cout<<A[i].first<<" : ";
        for(int j=0;j<A[i].second.size();j++){
            cout<<A[i].second[j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void print_dir(vector<pair<int,int>> &A){
    cout<<"dir:"<<endl;
    for(int i=0;i<A.size();i++){
        cout<<A[i].first<<" "<<A[i].second<<endl;
    }
    cout<<endl;
}
int main() {
    vector<pair<int,int>> dir;
    vector<pair<int,vector<int>>> buckets;
    int gd,bc;
    cin>>gd>>bc;
    int k=0,x=gd,y=gd;
    //initializing the directory
    vector<int> temp;
    for(int i=0;i<pow(2,gd);i++){
        dir.push_back(make_pair(i,i));
    }
    
    while(k!=6){
        cin>>k;
        if(k==2){
            //insert into the list
            int value;
            cin>>value;
            int key=gen_key(value, gd);
            //cout<<"KEY: "<<key<<endl;
            int id=key_present(key, dir);
            //cout<<"ID: "<<id<<endl;
            
            if(id!=-1){
                if(!insert(value,id,buckets,bc)){
                    ++x;
                    
                    //cout<<"new id: "<<id<<endl;
                    //cout<<"new key: "<<key<<endl;
                    split_bucket(id, value, x, bc, buckets);
                    
                    vector<int> temp;
                    for(int i=0;i<buckets.size();i++){
                        temp.push_back(buckets[i].first);
                    }
                    sort(temp.begin(),temp.end());
                    int d=0,bro=temp[temp.size()-1];
                    while(bro){
                        bro=bro/2;
                        ++d;
                    }
                    for(int i=0;i<d-y;i++){
                        double_dir(dir, gd);
                        ++gd;
                    }
                    //print_dir(dir);
                    
                    //cout<<"dir size:"<<dir.size()<<endl;
                    
                    //adding a new bucket corresponding to the newly generated id
                    
                    re_configure(buckets, dir);
                }
                /*print_buckets(buckets);
                cout<<"gd: "<<gd<<endl;
                cout<<"..................."<<endl;*/
            }
        }else if(k==3){
            //search for the value
            int value;
            cin>>value;
            int key=gen_key(value, gd);
            int id=key_present(key, dir);
            if(val_present(value,id,buckets)){
                //cout<<"found!"<<endl;
            }else{
                //cout<<value<<" not found :("<<endl;
            }
        }else if(k==4){
            //delete the value
            int value;
            cin>>value;
            int key=gen_key(value, gd);
            int id=key_present(key, dir);
            if(val_present(value,id,buckets)){
                delete_val(value, id, buckets);
            }else{
                //cout<<value<<" not found :("<<endl;
            }
        }else if(k==5){
            //display status:
    
            cout<<gd<<endl;
            cout<<buckets.size()<<endl;
            vector<pair<int,int>> ans;
            for(int i=0;i<buckets.size();i++){
                int a=buckets[i].second.size();
                //finding the local depth
                int count=0;
                for(int j=0;j<dir.size();j++){
                    if(dir[j].second==buckets[i].first){
                        ++count;
                    }
                }
                int b=gd-count+1;
                ans.push_back(make_pair(a,b));
            }
            sort(ans.begin(),ans.end());
            for(int i=0;i<ans.size();i++){
                cout<<ans[i].first<<" "<<ans[i].second<<endl;
            }
            //print_buckets(buckets);
            //print_dir(dir);
        }
    }
}
