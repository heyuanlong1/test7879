单例模式又分为两种基本的情形：饿汉式和懒汉式



//----------懒汉式  单例模式示例   非线程安全
class Singleton
{
public:
    static Singleton * getInstance()
    {
        if (instance == NULL) {
            instance = new Singleton();
        }   
        return instance;
    }  
private:
    Singleton()
    {
    }

    static Singleton *instance;

};
Singleton * Singleton::instance = NULL;


//----------懒汉式  单例模式示例   非线程安全
class Singleton
{
public:
    static Singleton * getInstance()
    {
       	static Singleton obj;
        return &obj;
    }  
private:
    Singleton()
    {
    }

    static Singleton *instance;

};



//----------懒汉式  单例模式示例   线程安全
class Singleton
{
public:
    static Singleton * getInstance()
    {
    	if (instance == NULL) {
    		lock();
	        if (instance == NULL) {
	            instance = new Singleton();
	        }   
	        unlock();
        }
        return instance;
    }  
private:
    Singleton()
    {
    }

    static Singleton *instance;

};
Singleton * Singleton::instance = NULL;


//----------饿汉式  单例模式示例   线程安全
class Singleton
{
public:
    static Singleton * getInstance()
    {
        return instance;
    }  
private:
    Singleton()
    {
    }

    static Singleton *instance;

};
Singleton * Singleton::instance =  new Singleton();


//--------------析构单例模式的对象    利用：系统会自动析构所有的全局变量。

class Singleton
{
public:
    static Singleton * getInstance(){
        if (NULL == instance) {
            instance = new Singleton();
        }
        return instance;
    }

    class Garbage
    {
    public:
        ~Garbage(){
            if (Singleton::instance != NULL) {
                delete Singleton::instance;
            }
        }
    };
    
private:
    static Garbage garbage;
    static Singleton *instance;

    Singleton(){
    }
    ~Singleton(){
    }
};
Singleton::Garbage Singleton::garbage;
Singleton * Singleton::instance = NULL;

//------------------------------





