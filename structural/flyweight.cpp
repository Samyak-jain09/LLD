#include <bits/stdc++.h>
using namespace std;

class IRobot{
    public:
        virtual ~IRobot() = default; 
        virtual void draw(int x, int y) = 0;
};

class HumanRobot : public IRobot{
    private:
        string type;
        string body;
    public:
        HumanRobot(string type, string body) : type(type), body(body) {}
        void draw(int x, int y){
            cout<<"Human Rendered at "<< x << " " << y << endl;
        }
};

class RoboticDog : public IRobot{
    private:
        string type;
        string body;
    public:
        RoboticDog(string type, string body) : type(type), body(body) {}
        void draw(int x, int y){
            cout<<"Dog Rendered at "<< x << " " << y << endl;
        }
};

class RobotFactory{
    private:
        static inline unordered_map<string, unique_ptr<IRobot> > cache;
    public:
        static IRobot* createRobot(string type){
            if(cache.find(type)!=cache.end()){
                return cache[type].get();
            }
            else if(type == "Human"){
                cache[type] = make_unique<HumanRobot>("HUMANBODYTYPE","HUMANBODY");
            }
            else{
                cache[type] = make_unique<RoboticDog>("ROBOTBODYTYPE","ROBOTBODY");
            }
            return cache[type].get();
        }
};

int main() {
    // Both clients get a pointer to the EXACT SAME instance in memory
    IRobot* robot1 = RobotFactory::createRobot("Human");
    IRobot* robot2 = RobotFactory::createRobot("Human");
    
    // Extrinsic state (coordinates) is passed externally during execution
    robot1->draw(10, 20);
    robot2->draw(30, 45);
    
    cout << "Are they pointing to the same instance? " << (robot1 == robot2 ? "Yes" : "No") << "\n";
    
    return 0;
}