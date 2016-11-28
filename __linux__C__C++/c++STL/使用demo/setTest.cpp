#include <stdio.h>
#include <iostream> 
#include <set>
using namespace std;

void showset(set<int> &myset){
    set<int>::iterator i;   
    cout<<"myset.begin()--- myset.end():"<<endl;   
    for (i = myset.begin(); i != myset.end(); ++i)   
        cout << *i << " ";   
    cout << endl;   
}

int main(int argc, char const *argv[])
{
	//增

	//删

	//改

	//查

    set<int> myset;   
    set<int>::iterator i;       
    set<int>::iterator it;       
  	std::pair<std::set<int>::iterator,bool> ret;

  	for (int i=1; i<=5; ++i) 
  		myset.insert(i*10);    		  		// set: 10 20 30 40 50

  	ret = myset.insert(20);           		// no new element inserted  	
  	if (ret.second==false) 
  		it=ret.first;  				  		// "it" now points to element 20

    myset.insert(myset.begin(),0);          //插入
    showset(myset); 

    myset.erase(++myset.begin());         		//删除 set的iterator没有 +1 运算
    myset.erase(0);         					//删除
    showset(myset); 

    std::set<int> second;
    myset.swap(second);                        //交换
    cout<<"myset";
    showset(myset); 
    cout<<"second";
    showset(second); 
    myset.swap(second);

    it = myset.find(20);						//find
    cout<<"myset.find(20):"<<*it<<endl;

    int n = myset.count(20);					//count
    cout<<"myset.count(20):"<<n<<endl;


    if (myset.empty()){                        //空判断
        cout<<"empty"<<endl;
    }else{
        cout<<"no empty"<<endl;
    }

    cout<<"set size:"<<myset.size()<<endl;    //元素数量
	showset(myset); 
	return 0;
}