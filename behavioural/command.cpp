#include <bits/stdc++.h>
using namespace std;

class AC{
    private:
        bool isOn;
        int temp;
    public:
        void turnOnAc(){
            isOn = true;
            cout<<"AC is turned on"<<endl;
        }

        void turnOfAc(){
            isOn = false;
            cout<<"AC is turned off"<<endl;
        }

        void setTemperature(int temp){
            this->temp = temp;
            cout<<"Temperature set to "<<temp<<endl;
        }
};

class ICommand{
    public:
        virtual ~ICommand() = default;
        virtual void execute() = 0;
        virtual void undo() = 0;
};

class TurnOnACCommand : public ICommand{
    private:
        std::shared_ptr<AC> ac;
    public:
        TurnOnACCommand(std::shared_ptr<AC> ac) : ac(move(ac)) {}
        void execute(){
            ac->turnOnAc();
        }
        void undo(){
            ac->turnOfAc();
        }
};

class TurnOffACCommand : public ICommand{
    private:
        std::shared_ptr<AC> ac;
    public:
        TurnOffACCommand(std::shared_ptr<AC> ac) : ac(move(ac)) {}
        void execute(){
            ac->turnOfAc();
        }
        void undo(){
            ac->turnOnAc();
        }
};

class SetTemperatureCommand : public ICommand{
    private:
        std::shared_ptr<AC> ac;
    public:
        SetTemperatureCommand(std::shared_ptr<AC> ac) : ac(move(ac)) {}
        void execute(){
            ac->setTemperature(20);
        }
        void undo(){
            ac->setTemperature(15);
        }
};

class RemoteControl{
    private:
        std::shared_ptr<ICommand> command;
        stack<shared_ptr<ICommand> > history;
    public:
        RemoteControl () = default;
        void setCommand (shared_ptr<ICommand> cmd){
            command = (cmd);
            history.push((cmd));
        }
        void pressButton(){
            command->execute();
        }
        void undo(){
            if(!history.empty()){
                auto lastCommand = history.top().get();
                lastCommand->undo();
                history.pop();
            }
            else{
                cout<<"cannot undo"<<endl;
            }
        }
};

int main(){
    RemoteControl* remote = new RemoteControl();
    remote->setCommand(std::make_shared<TurnOffACCommand>(std::make_shared<AC>()));
    remote->pressButton();
    remote->undo();
    return 0;
}
