#include <stdio.h>
#include <iostream> 
#include <map>
#include <string>
using namespace std;

void showmap(map<int,string> &mymap){
    map<int,string>::iterator i;   
    cout<<"mymap.begin()--- mymap.end():"<<endl;   
    for (i = mymap.begin(); i != mymap.end(); ++i)   
        cout << i->first << " => " << i->second  << " ";   
    cout << endl;   
}

int main(int argc, char const *argv[])
{
	//增

	//删

	//改

	//查

    map<int,string> mymap;   
    map<int,string>::iterator i;       
    map<int,string>::iterator it;       
  	std::pair<std::map<int,string>::iterator,bool> ret;

  	for (int i=1; i<=5; ++i) 
  		mymap.insert(  std::pair<int,string>(i*10, "ii" ) );  

  	ret = mymap.insert( std::pair<int,string>(20,"202020") ); 
  	if (ret.second==false) 
  		it=ret.first;  				  		// "it" now points to element 20

    mymap.insert(mymap.begin(), std::pair<int,string>(0,"000") );
    showmap(mymap); 

    mymap[33] = "3333";
    cout<<"mymap[3] = 5 -> "<<mymap.at(33)<<endl;

    mymap.erase(++mymap.begin());         		//删除 map的iterator没有 +1 运算
    mymap.erase(0);         					//删除
    showmap(mymap); 

    std::map<int,string> second;
    mymap.swap(second);                        //交换
    cout<<"mymap";
    showmap(mymap); 
    cout<<"second";
    showmap(second); 
    mymap.swap(second);

    it = mymap.find(20);						//find
    cout<<"mymap.find(20):"<<it->first<<" -> "<<it->second<<endl;

    int n = mymap.count(20);					//count
    cout<<"mymap.count(20):"<<n<<endl;


    if (mymap.empty()){                        //空判断
        cout<<"empty"<<endl;
    }else{
        cout<<"no empty"<<endl;
    }

    cout<<"map size:"<<mymap.size()<<endl;    //元素数量
	showmap(mymap); 
	return 0;
}