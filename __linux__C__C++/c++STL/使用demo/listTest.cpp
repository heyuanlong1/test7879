#include <stdio.h>
#include <iostream> 
#include <list>
using namespace std;

void showList(list<int> &mylist){
    list<int>::iterator i;   
    cout<<"mylist.begin()--- mylist.end():"<<endl;   
    for (i = mylist.begin(); i != mylist.end(); ++i)   
        cout << *i << " ";   
    cout << endl;   
}

int main(int argc, char const *argv[])
{
	//增

	//删

	//改

	//查

    list<int> mylist;   
    list<int>::iterator i;   
      
                                    //从前面向mylist容器中添加数据   
    mylist.push_front (2);   
    mylist.push_front (1);   
      
                                    //从后面向mylist容器中添加数据   
    mylist.push_back (3);   
    mylist.push_back (4);   
      
                                    //从前向后显示mylist中的数据  
    showList(mylist); 

                                    //从后向后显示mylist中的数据   
    list<int>::reverse_iterator ir;   
    cout<<"mylist.rbegin()---mylist.rend():"<<endl;   
    for (ir =mylist.rbegin(); ir!=mylist.rend();ir++) {   
        cout << *ir << " ";   
    }   
    cout << endl;

    cout<<"front:"<<mylist.front()<<endl;       //取头元素
    cout<<"back:"<<mylist.back()<<endl;         //取尾元素

    mylist.pop_front();                         //扔头元素
    mylist.pop_back();                          //扔尾元素

    mylist.insert(mylist.begin(),0);            //插入
    showList(mylist); 

    mylist.erase(mylist.begin());               //删除
    showList(mylist); 

    mylist.remove(3);                           //删除
    showList(mylist); 

    std::list<int> second;
    mylist.swap(second);                        //交换
    cout<<"mylist";
    showList(mylist); 
    cout<<"second";
    showList(second); 
    mylist.swap(second);

    if (mylist.empty()){                        //空判断
        cout<<"empty"<<endl;
    }else{
        cout<<"no empty"<<endl;
    }

    cout<<"list size:"<<mylist.size()<<endl;    //元素数量
	showList(mylist); 
	return 0;
}