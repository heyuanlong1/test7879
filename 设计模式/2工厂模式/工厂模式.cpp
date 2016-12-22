http://blog.csdn.net/wuzhekai1985/article/details/6660462#comments

//-------------------------------------------简单工厂模式
enum CTYPE {COREA, COREB};   
class SingleCore  
{  
public:  
    virtual void Show() = 0;
};  
//单核A  
class SingleCoreA: public SingleCore  
{  
public:  
    void Show() { cout<<"SingleCore A"<<endl; }  
};  
//单核B  
class SingleCoreB: public SingleCore  
{  
public:  
    void Show() { cout<<"SingleCore B"<<endl; }  
};  
//唯一的工厂，可以生产两种型号的处理器核，在内部判断  
class Factory  
{  
public:   
    SingleCore* CreateSingleCore(enum CTYPE ctype)  
    {  
        if(ctype == COREA)              //工厂内部判断  
            return new SingleCoreA();   //生产核A  
        else if(ctype == COREB)  
            return new SingleCoreB();   //生产核B  
        else  
            return NULL;  
    }  
};  


//-------------------------------------------工厂方法模式
class SingleCore  
{  
public:  
    virtual void Show() = 0;
};  
//单核A  
class SingleCoreA: public SingleCore  
{  
public:  
    void Show() { cout<<"SingleCore A"<<endl; }  
};  
//单核B  
class SingleCoreB: public SingleCore  
{  
public:  
    void Show() { cout<<"SingleCore B"<<endl; }  
};  
class Factory  
{  
public:  
    virtual SingleCore* CreateSingleCore() = 0;
};  
//生产A核的工厂  
class FactoryA: public Factory  
{  
public:  
    SingleCoreA* CreateSingleCore() { return new SingleCoreA; }  
};  
//生产B核的工厂  
class FactoryB: public Factory  
{  
public:  
    SingleCoreB* CreateSingleCore() { return new SingleCoreB; }  
};  


//-------------------------------------------抽象工厂模式
//单核  
class SingleCore   
{  
public:  
    virtual void Show() = 0;
};  
class SingleCoreA: public SingleCore    
{  
public:  
    void Show() { cout<<"Single Core A"<<endl; }  
};  
class SingleCoreB :public SingleCore  
{  
public:  
    void Show() { cout<<"Single Core B"<<endl; }  
};  
//多核  
class MultiCore    
{  
public:  
    virtual void Show() = 0;
};  
class MultiCoreA : public MultiCore    
{  
public:  
    void Show() { cout<<"Multi Core A"<<endl; }  
  
};  
class MultiCoreB : public MultiCore    
{  
public:  
    void Show() { cout<<"Multi Core B"<<endl; }  
};  
//工厂  
class CoreFactory    
{  
public:  
    virtual SingleCore* CreateSingleCore() = 0;
    virtual MultiCore* CreateMultiCore() = 0;
};  
//工厂A，专门用来生产A型号的处理器  
class FactoryA :public CoreFactory  
{  
public:  
    SingleCore* CreateSingleCore() { return new SingleCoreA(); }  
    MultiCore* CreateMultiCore() { return new MultiCoreA(); }  
};  
//工厂B，专门用来生产B型号的处理器  
class FactoryB : public CoreFactory  
{  
public:  
    SingleCore* CreateSingleCore() { return new SingleCoreB(); }  
    MultiCore* CreateMultiCore() { return new MultiCoreB(); }  
}; 


