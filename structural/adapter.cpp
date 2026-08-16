#include <bits/stdc++.h>
using namespace std;

class WeightMachine{
    public:
        virtual double getWeightInPounds() = 0;
};

class WeightMachineInPounds : public WeightMachine{
    private:
        double weight;
    public:
        WeightMachineInPounds(double w) : weight(w) {}
        double getWeightInPounds() override {
            return weight;
        }
};

class WeightMachineAdapter{
    public:
        virtual double getWeightInKilograms() = 0;
};

class WeightMachineAdapterImpl : public WeightMachineAdapter{
    private:
        WeightMachine* weightMachine;
    public:
        WeightMachineAdapterImpl(WeightMachine* wm) : weightMachine(wm) {}
        double getWeightInKilograms() override {
            return weightMachine->getWeightInPounds() * 0.453592;
        }
};

class Client{
    public:
        void displayWeight(){
            WeightMachineAdapter* adapter = new WeightMachineAdapterImpl(new WeightMachineInPounds(100));
            double weight = adapter->getWeightInKilograms();
            cout<<"Weight is: "<< weight << endl;
        }
};

int main(){
    Client cl;
    cl.displayWeight();
    return 0;
}

