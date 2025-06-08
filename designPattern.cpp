#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>

class Singleton{
Singleton(){};
~Singleton(){};

static Singleton* ins_;

public:
Singleton(const Singleton&) = delete;
Singleton& operator=(const Singleton&) = delete;
Singleton(Singleton&&) = delete;
static Singleton* getInstance(){
    if(ins_ == nullptr){
        ins_ = new Singleton();
    }
    return ins_;
}

static void destroyInstance(){
    if(ins_ != nullptr){
        delete ins_;
        ins_ = nullptr;
    }
}

};

Singleton* Singleton::ins_ = nullptr;

void testSingleton(){
    auto single = Singleton::getInstance();
}


class ProductBase{
public:
    ProductBase(){};
    virtual ~ProductBase(){};
    virtual void doSomething() = 0;
};

class ProductionA : public ProductBase{
public:
    ProductionA(){};
    ~ProductionA(){};
    void doSomething(){
        
    }
};

class ProductionB : public ProductBase{
public:
    ProductionB(){};
    ~ProductionB(){};
    void doSomething(){
        
    }
};

class Factory{
public:
    enum ProductType{
        PRODUCTA,
        PRODUCTB
    };
    Factory(){};
    ~Factory(){};
    static ProductBase* createProduct(ProductType type){
        switch(type){
            case PRODUCTA:
                return new ProductionA();
            case PRODUCTB:
                return new ProductionB();
            default:
                return nullptr;
        }
    }
};

void runProduct(){
    auto product = Factory::createProduct(Factory::PRODUCTA);
    product->doSomething();
}



class ObserverBase{
public:
    ObserverBase(){};
    virtual ~ObserverBase(){};
    virtual void notify(std::string msg) = 0;
};

class ConcreteObserver: public ObserverBase{
std::string name_;
public:
    ConcreteObserver(std::string name): name_(name){};
    ~ConcreteObserver(){};
    void notify(std::string msg){
        
    }
};

class Subject{
std::vector<ObserverBase*> observers;

public:
    Subject(){};
    ~Subject(){};

    void attach(ObserverBase* observer){
        observers.emplace_back(observer);
    }

    void detach(ObserverBase* observer){
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notify(std::string msg){
        for(auto observer : observers){
            observer->notify(msg);
        }
    }
};

void testObserver(){
    Subject subject;
    ConcreteObserver* observer1 = new ConcreteObserver("a");
    ConcreteObserver* observer2 = new ConcreteObserver("b");
    subject.attach(observer1);
    subject.attach(observer2);
    subject.detach(observer2);
    subject.notify("detached observer2");
}


class StrategyBase{
public:
    StrategyBase(){};
    virtual ~StrategyBase(){};

    virtual void execute() = 0;
};

class StrategyA : public StrategyBase{
public:
    StrategyA(){};
    ~StrategyA(){};

    void execute(){
        
    }
};

class StrategyB : public StrategyBase{
public:
    StrategyB(){};
    ~StrategyB(){};

    void execute(){
        
    }
};

class Context{
StrategyBase*  strategy_;
public:
    Context(StrategyBase* strategy=nullptr) : strategy_(strategy){}
    
    void setStrategy(StrategyBase* strategy){
        strategy_ = strategy;
    }

    void executeStrategy(){
        if(strategy_ != nullptr){
            strategy_->execute();
        }
    }
};

void testStrategy(){
    StrategyA* strategyA = new StrategyA;
    StrategyB* strategyB = new StrategyB;
    Context context(strategyA);
    context.executeStrategy();
    context.setStrategy(strategyB);
    context.executeStrategy();
}

class ComponentBase {
public:
    virtual ~ComponentBase(){};

    virtual void operate() = 0;
};

class ConcreteComponent : public ComponentBase{
public:
    void operate(){
        printf("ConcreteComponentA operate...\n");
    };
};

class DecoratorBase : public ComponentBase{
ComponentBase* comp_ = nullptr;
public:
    DecoratorBase(ComponentBase* comp): comp_(comp){};
    void operate(){
        if(nullptr != comp_){
            comp_->operate();
        }
    }
};

class ConcreteDecoratorA: public DecoratorBase{
public:
    ConcreteDecoratorA(ComponentBase* comp): DecoratorBase(comp){};
    void operate(){
        DecoratorBase::operate();
        addBehaviour();
    }

    void addBehaviour(){
        printf("ConcreteDecoratorA addBehavior\n");
    };
};

class ConcreteDecoratorB: public DecoratorBase{
public:
    ConcreteDecoratorB(ComponentBase* comp): DecoratorBase(comp){};

    void operate(){
        DecoratorBase::operate();
        addBehavior();
    }

    void addBehavior(){
        printf("ConcreteDecoratorB addBehavior\n");
    };
};

void testDecorator(){
    ConcreteComponent* comp1 = new ConcreteComponent;
    ConcreteDecoratorA* deco1 = new ConcreteDecoratorA(comp1);
    ConcreteDecoratorB* deco2 = new ConcreteDecoratorB(deco1);
    deco2->operate();
}

class TargetAPI{
public:
    virtual ~TargetAPI(){};
    virtual void request() = 0;
};

class Adaptee{
public:
    void specificRequest(){};
};

// 类适配器(多重继承)
class AdaptorClass: public TargetAPI, private Adaptee{
public:
    void request(){
        specificRequest();
    }
};

class AdaptorObj: public TargetAPI{
Adaptee* adaptee_ = nullptr;

public:
    AdaptorObj(Adaptee* adaptee): adaptee_(adaptee){};
    void request(){
        if(nullptr != adaptee_){
            adaptee_->specificRequest();
        }
    }
};

void testAdaptor(){
    AdaptorClass adCls;
    adCls.request();

    Adaptee* tee = new Adaptee;
    AdaptorObj adObj(tee);
    adObj.request();
}