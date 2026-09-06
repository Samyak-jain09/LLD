#include <bits/stdc++.h>
using namespace std;

class BreatheImplementor{
    public:
        virtual void breathe() = 0;
};

class LandBreatheImplementor : public BreatheImplementor{
    public:
        void breathe() override{
            cout << "Breathing on land" << endl;
        }
};

class WaterBreatheImplementor : public BreatheImplementor{
    public:
        void breathe() override{
            cout << "Breathing in water" << endl;
        }
};

class TreeBreatheImplementor : public BreatheImplementor{
    public:
        void breathe() override{
            cout << "Breathing through leaves" << endl;
        }
};

class LivingThings{
    protected:
        std::shared_ptr<BreatheImplementor> breatheImplementor;
    public:
        LivingThings(shared_ptr<BreatheImplementor>breatheImpl): 
            breatheImplementor(std::move(breatheImpl)) {}
        virtual void breatheProcess() = 0;
};

class Dog : public LivingThings{
    public:
        Dog(shared_ptr<BreatheImplementor> impl) :
                LivingThings(move(impl)){}
        void breatheProcess() override{
            breatheImplementor->breathe();
        }
};

class Fish : public LivingThings{
    public:
        Fish(shared_ptr<BreatheImplementor> impl) :
            LivingThings(move(impl)){}
        void breatheProcess() override{
            breatheImplementor->breathe();
        }
};

class Tree : public LivingThings{
    public:
        Tree(shared_ptr<BreatheImplementor> impl) :
            LivingThings(move(impl)){}
        void breatheProcess() override{
            breatheImplementor->breathe();
        }
};

int main(){
    Dog dog(make_shared<LandBreatheImplementor>());
    Fish fish(make_shared<WaterBreatheImplementor>());
    Tree tree(make_shared<TreeBreatheImplementor>());

    dog.breatheProcess();
    fish.breatheProcess();
    tree.breatheProcess();

    return 0;
}