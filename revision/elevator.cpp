#include <bits/stdc++.h>
using namespace std;

// Forward declarations
class ElevatorCar;
class ElevatorController;

enum class Direction
{
    UP,
    DOWN
};

enum class Status
{
    IDLE,
    MOVING
};

class Display
{
private:
    int floor;
    Direction dir;

public:
    void setFloor(int fl)
    {
        floor = fl;
    }
    void setDir(Direction d)
    {
        dir = d;
    }
    int getFloor() const
    {
        return this->floor;
    }
    Direction getDir() const
    {
        return this->dir;
    }
};

class ElevatorController
{
private:
    priority_queue<int, vector<int>, greater<int>> upQueue;
    priority_queue<int> downQueue;
    std::unique_ptr<ElevatorCar> elevator;

public:
    ElevatorController(unique_ptr<ElevatorCar> ele) : elevator(move(ele)) {}

    void acceptExternalRequest(int floor, Direction dir)
    {
        cout << "External Request -> Floor "
             << floor << " Direction "
             << (dir == Direction::UP ? "UP" : "DOWN")
             << endl;
        if (dir == Direction::UP)
            upQueue.push(floor);
        else
            downQueue.push(floor);
    }
    void acceptInternalRequest(int floor);
    void controlElevator();
    ElevatorCar *getElevator()
    {
        return elevator.get();
    }
};

class InternalButtonsDispatcher
{
private:
    vector<std::shared_ptr<ElevatorController>> &elevator;

public:
    InternalButtonsDispatcher(vector<std::shared_ptr<ElevatorController>>  &ele)
        : elevator(ele) {}

    // Declaration only; definition moved below ElevatorCar
    void submitRequest(int elevatorId, int floor);
};

class InternalButtons
{
private:
    unique_ptr<InternalButtonsDispatcher> dispatcher;

public:
    InternalButtons(unique_ptr<InternalButtonsDispatcher> dispatcher) : dispatcher(move(dispatcher)) {}

    void submitRequest(int id, int floor)
    {
        dispatcher->submitRequest(id, floor);
    }
};

class ElevatorCar
{
private:
    int id;
    int currentFloor;
    Direction dir;
    Status status;
    std::unique_ptr<Display> display;
    std::unique_ptr<InternalButtons> internalButtons;

public:
    ElevatorCar(int id, int floor, Direction dir, unique_ptr<Display> display, unique_ptr<InternalButtons> buttons) : id(id), currentFloor(floor), dir(dir), status(Status::IDLE), display(std::move(display)), internalButtons(std::move(buttons)) {}

    void pressButton(int floor)
    {
        internalButtons->submitRequest(this->id, floor);
    }

    void move(int floor, Direction newDir)
    {
        cout << "Elevator " << id
             << " moving from Floor " << currentFloor
             << " to Floor " << floor
             << " Direction : "
             << (newDir == Direction::UP ? "UP" : "DOWN")
             << endl;

        currentFloor = floor;
        dir = newDir;
        status = Status::MOVING;

        display->setDir(dir);
        display->setFloor(floor);
    }

    int getId() const
    {
        return this->id;
    }

    int getCurrentFloor() const
    {
        return this->currentFloor;
    }
};

// Now that ElevatorCar is fully defined, we can define InternalButtonsDispatcher::submitRequest
void InternalButtonsDispatcher::submitRequest(int elevatorId, int floor)
{
    cout << "Button Pressed Inside Elevator "
         << elevatorId
         << " for Floor "
         << floor << endl;

    for (auto &elevatorController : elevator)
    {
        if (elevatorController->getElevator()->getId() == elevatorId)
        {
            elevatorController->acceptInternalRequest(floor);
        }
    }
}

void ElevatorController::acceptInternalRequest(int floor)
{

    cout << "Internal Request -> Elevator "
         << elevator->getId()
         << " wants Floor "
         << floor << endl;

    if (floor > elevator->getCurrentFloor())
        upQueue.push(floor);
    else
        downQueue.push(floor);
}

void ElevatorController::controlElevator()
{
    while (!upQueue.empty())
    {
        int floor = upQueue.top();
        upQueue.pop();

        elevator->move(floor, Direction::UP);
    }

    while (!downQueue.empty())
    {
        int floor = downQueue.top();
        downQueue.pop();

        elevator->move(floor, Direction::DOWN);
    }
}

class ExternalButtonDispatcher
{
protected:
    vector<shared_ptr<ElevatorController>>& elevators;

public:
    ExternalButtonDispatcher(vector<shared_ptr<ElevatorController>> &elevators) : elevators(elevators) {}
    virtual void submitRequest(int floor, Direction dir) = 0;
    virtual ~ExternalButtonDispatcher() = default;
};

class OddEvenDispatcher : public ExternalButtonDispatcher
{
public:
    OddEvenDispatcher(vector<shared_ptr<ElevatorController>>& elevators)
        : ExternalButtonDispatcher(elevators) {}

    void submitRequest(int floor, Direction dir) override
    {

        cout << "Floor " << floor
             << " requested Elevator "
             << (dir == Direction::UP ? "UP" : "DOWN")
             << endl;

        for (auto &controller : ExternalButtonDispatcher::elevators)
        {

            if (controller->getElevator()->getId() % 2 == 0 &&
                floor % 2 == 0)
            {

                cout << "Assigned to Elevator "
                     << controller->getElevator()->getId()
                     << endl;

                controller->acceptExternalRequest(floor, dir);
                break;
            }

            else if (controller->getElevator()->getId() % 2 != 0 &&
                     floor % 2 != 0)
            {

                cout << "Assigned to Elevator "
                     << controller->getElevator()->getId()
                     << endl;

                controller->acceptExternalRequest(floor, dir);
                break;
            }
        }
    }
};

class ExternalButton
{
private:
    unique_ptr<ExternalButtonDispatcher> dispatcher;

public:
    ExternalButton(unique_ptr<ExternalButtonDispatcher> dispatcher) : dispatcher(move(dispatcher)) {}

    void submitRequest(int floor, Direction dir)
    {
        dispatcher->submitRequest(floor, dir);
    }
};

class Floor
{
private:
    int floorNumber;
    shared_ptr<ExternalButton> button;

public:
    Floor(int num, shared_ptr<ExternalButton> button) : floorNumber(num), button(move(button)) {}

    void submitRequest(int floor, Direction dir)
    {
        button->submitRequest(floor, dir);
    }
};

class Building
{
public:
    vector<unique_ptr<Floor>> floors;
};

int main() {

    vector<shared_ptr<ElevatorController>> controllers;

    // ---------- Elevator 1 ----------

    auto display1 = make_unique<Display>();

    auto internalDispatcher1 =
        make_unique<InternalButtonsDispatcher>(controllers);

    auto buttons1 =
        make_unique<InternalButtons>(move(internalDispatcher1));

    auto elevator1 =
        make_unique<ElevatorCar>(
            1,
            0,
            Direction::UP,
            move(display1),
            move(buttons1));

    auto controller1 =
        make_shared<ElevatorController>(move(elevator1));

    controllers.push_back(controller1);

    // ---------- Elevator 2 ----------

    auto display2 = make_unique<Display>();

    auto internalDispatcher2 =
        make_unique<InternalButtonsDispatcher>(controllers);

    auto buttons2 =
        make_unique<InternalButtons>(move(internalDispatcher2));

    auto elevator2 =
        make_unique<ElevatorCar>(
            2,
            0,
            Direction::UP,
            move(display2),
            move(buttons2));

    auto controller2 =
        make_shared<ElevatorController>(move(elevator2));

    controllers.push_back(controller2);

    // ---------- External Dispatcher ----------

    auto externalDispatcher =
        make_unique<OddEvenDispatcher>(controllers);

    auto externalButton =
        make_shared<ExternalButton>(move(externalDispatcher));

    // ---------- Building Floors ----------

    Building building;

    for (int i = 0; i <= 10; i++) {
        building.floors.push_back(
            make_unique<Floor>(i, externalButton));
    }

    cout << "\n========== EXTERNAL REQUESTS ==========\n";

    building.floors[3]->submitRequest(3, Direction::UP);
    building.floors[8]->submitRequest(8, Direction::DOWN);

    cout << "\n========== PROCESS EXTERNAL ==========\n";

    controller1->controlElevator();
    controller2->controlElevator();

    cout << "\n========== INTERNAL REQUESTS ==========\n";

    controller1->getElevator()->pressButton(7);
    controller2->getElevator()->pressButton(2);

    cout << "\n========== PROCESS INTERNAL ==========\n";

    controller1->controlElevator();
    controller2->controlElevator();

    return 0;
}