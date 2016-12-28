//抽象接口  
class ReplaceAlgorithm  
{  
public:  
    virtual void Replace() = 0;  
};  
//三种具体的替换算法  
class LRU_ReplaceAlgorithm : public ReplaceAlgorithm  
{  
public:  
    void Replace() { cout<<"Least Recently Used replace algorithm"<<endl; }  
};  
  
class FIFO_ReplaceAlgorithm : public ReplaceAlgorithm  
{  
public:  
    void Replace() { cout<<"First in First out replace algorithm"<<endl; }  
};  
class Random_ReplaceAlgorithm: public ReplaceAlgorithm  
{  
public:  
    void Replace() { cout<<"Random replace algorithm"<<endl; }  
};  



//----------------------------------------------------------------
//-------------------方式一：直接通过参数指定，传入一个特定算法的指针。
class Cache  
{  
private:  
    ReplaceAlgorithm *m_ra;  
public:  
    Cache(ReplaceAlgorithm *ra) { m_ra = ra; }  
    ~Cache() { delete m_ra; }  
    void Replace() { m_ra->Replace(); }  
};  

int main()  
{  
    Cache cache(new LRU_ReplaceAlgorithm()); //暴露了算法的定义  
    cache.Replace();  
    return 0;  
}  





//----------------------------------------------------------------
//-------方式二：也是直接通过参数指定，只不过不是传入指针，而是一个标签。
enum RA {LRU, FIFO, RANDOM}; //标签  
class Cache  
{  
private:  
    ReplaceAlgorithm *m_ra;  
public:  
    Cache(enum RA ra)   
    {   
        if(ra == LRU)  
            m_ra = new LRU_ReplaceAlgorithm();  
        else if(ra == FIFO)  
            m_ra = new FIFO_ReplaceAlgorithm();  
        else if(ra == RANDOM)  
            m_ra = new Random_ReplaceAlgorithm();  
        else   
            m_ra = NULL;  
    }  
    ~Cache() { delete m_ra; }  
    void Replace() { m_ra->Replace(); }  
}; 

//相比方式一，这种方式用起来方便多了。其实这种方式将简单工厂模式与策略模式结合在一起，算法的定义使用了策略模式，而Cache的定义其实使用了简单工厂模式
int main()  
{  
    Cache cache(LRU); //指定标签即可  
    cache.Replace();  
    return 0;  
}  




//----------------------------------------------------------------
//---------------------方式三：利用模板实现。算法通过模板的实参指定。
template <class RA>  
class Cache  
{  
private:  
    RA m_ra;  
public:  
    Cache() { }  
    ~Cache() { }  
    void Replace() { m_ra.Replace(); }  
};  

int main()  
{  
    Cache<Random_ReplaceAlgorithm> cache; //模板实参  
    cache.Replace();  
    return 0;  
}  


