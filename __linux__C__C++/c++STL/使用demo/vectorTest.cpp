#include <stdio.h>
#include <iostream> 
#include <vector>
using namespace std;

void showvector(vector<int> &myvector){
    vector<int>::iterator i;   
    cout<<"myvector.begin()--- myvector.end():"<<endl;   
    for (i = myvector.begin(); i != myvector.end(); ++i)   
        cout << *i << " ";   
    cout << endl;   
}

int main(int argc, char const *argv[])
{
	//增

	//删

	//改

	//查

    vector<int> myvector;   
    vector<int>::iterator i;   
      
                                    //从后面向myvector容器中添加数据   
    myvector.push_back (4);   
    myvector.push_back (3);   
    myvector.push_back (2);   
    myvector.push_back (1);   
      
                                    //从前向后显示myvector中的数据  
    showvector(myvector); 

                                    //从后向后显示myvector中的数据   
    vector<int>::reverse_iterator ir;   
    cout<<"myvector.rbegin()---myvector.rend():"<<endl;   
    for (ir =myvector.rbegin(); ir!=myvector.rend();ir++) {   
        cout << *ir << " ";   
    }   
    cout << endl;

    myvector[3] = 5;
    cout<<"myvector[3] = 5 -> "<<myvector.at(3)<<endl;

    cout<<"front:"<<myvector.front()<<endl;       //取头元素
    cout<<"back:"<<myvector.back()<<endl;         //取尾元素

    myvector.pop_back();                          //扔尾元素

    myvector.insert(myvector.begin(),0);          //插入
    showvector(myvector); 

    myvector.erase(myvector.begin() + 1);         //删除
    showvector(myvector); 

    std::vector<int> second;
    myvector.swap(second);                        //交换
    cout<<"myvector";
    showvector(myvector); 
    cout<<"second";
    showvector(second); 
    myvector.swap(second);

    if (myvector.empty()){                        //空判断
        cout<<"empty"<<endl;
    }else{
        cout<<"no empty"<<endl;
    }

    cout<<"vector size:"<<myvector.size()<<endl;    //元素数量
	showvector(myvector); 
	return 0;
}