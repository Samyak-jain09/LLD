#include <bits/stdc++.h>

using namespace std;

class Vehicle{
    public:
        virtual void getVehicleName() const = 0;
        virtual void getCapacity() const= 0;
        virtual ~Vehicle() = default;
};

class Car : public Vehicle{
    public:
        void getVehicleName() const override{
            cout<<"CAR"<<endl;
        }
        void getCapacity() const override{
            cout<<100<<endl;
        }
};

class NullVehicle : public Vehicle{
    public:
        void getVehicleName() const override{
            cout<<"0"<<endl;
        }
        void getCapacity() const override{
            cout<<0<<endl;
        }
};

class VehicleFactory{
    public:
        static std::unique_ptr<Vehicle> getVehicleObject(const string &s){
            if(s == "CAR")
                return make_unique<Car>();
            else{
                return make_unique<NullVehicle> ();
            }
        }
};

int main(){
    auto vehicle = VehicleFactory::getVehicleObject("CAR");
    vehicle->getCapacity();
    vehicle->getVehicleName();
    auto vehicle2 = VehicleFactory::getVehicleObject("BIKE");
    vehicle2->getCapacity();
    vehicle2->getVehicleName();
    return 0;
}
