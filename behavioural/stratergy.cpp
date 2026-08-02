#include <bits/stdc++.h>

class DriveStratergy{
    public:
        virtual ~DriveStratergy() = default;
        virtual void drive() = 0;
};

class NormalDrive : public DriveStratergy{
    public:
        void drive() override{
            std::cout<<"It is normal drive stratergy";
        }
};

class SpecialDrive: public DriveStratergy{
    public:
        void drive() override{
            std::cout<<"It is special drive stratergy";
        }
};

class Vehicle{
    private:
        DriveStratergy* driveSt;
    public:
        Vehicle(DriveStratergy* drive): driveSt(drive){};
        void drive(){
            driveSt->drive();
        }
};                  

int main(){
    DriveStratergy* normal = new NormalDrive();
    Vehicle* car = new Vehicle(normal);
    car->drive();
    return 0;
}

