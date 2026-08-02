#include <bits/stdc++.h>
using namespace std;

enum Direction {
    UP,
    DOWN
};

enum Status {
    MOVING,
    IDLE
};

class InternalButton;
class ElevatorController;
class ElevatorCreator;

class Display {
public:
    Display() : floor(0), dir(Direction::UP) {}

    Display(Direction dir, int floor)
        : dir(dir), floor(floor) {}

    void printDisplay() {
        cout << "Floor is " << floor
             << " direction is " << getDirection() << endl;
    }

    void setDirection(Direction dir) {
        this->dir = dir;
    }

    void setFloor(int floor) {
        this->floor = floor;
    }

    void setDisplay(int floor, Direction dir) {
        setFloor(floor);
        setDirection(dir);
    }

private:
    int floor;
    Direction dir;

    string getDirection() {
        return (dir == Direction::UP) ? "UP" : "DOWN";
    }
};

class ElevatorCar {
public:
    ElevatorCar(int id);

    bool move(int destFloor, Direction dir);

    void showDisplay();

    void pressButton(int dest);

    void setDisplay();

    int getId() {
        return id;
    }

    int getCurrentFloor() {
        return currentFloor;
    }

private:
    int id;
    Display display;
    int currentFloor;
    Direction dir;
    Status status;

    InternalButton* button;
};

class ElevatorController {
public:
    ElevatorController(ElevatorCar* elevator)
        : elevator(elevator) {}

    void submitInternalRequest(int floor) {
        if (floor > elevator->getCurrentFloor())
            upQueue.push(floor);
        else
            downQueue.push(floor);
    }

    void submitExternalRequest(int floor, Direction dir) {
        if (dir == Direction::UP)
            upQueue.push(floor);
        else
            downQueue.push(floor);
    }

    void controlElevator() {
        while (!upQueue.empty()) {
            int floor = upQueue.top();
            upQueue.pop();

            elevator->move(floor, Direction::UP);
        }

        while (!downQueue.empty()) {
            int floor = downQueue.top();
            downQueue.pop();

            elevator->move(floor, Direction::DOWN);
        }
    }

    ElevatorCar& getElevator() {
        return *elevator;
    }

private:
    ElevatorCar* elevator;

    priority_queue<int> downQueue; // max-heap
    priority_queue<int, vector<int>, greater<int>> upQueue; // min-heap
};

class ElevatorCreator {
public:
    inline static vector<ElevatorController> elevators = {
        ElevatorController(new ElevatorCar(1)),
        ElevatorController(new ElevatorCar(2))
    };
};

class InternalButtonDispatcher {
public:
    void submitRequest(int floor, ElevatorCar &elevator) {
        for (auto &controller : ElevatorCreator::elevators) {
            if (controller.getElevator().getId() == elevator.getId()) {
                controller.submitInternalRequest(floor);
                break;
            }
        }
    }
};

class InternalButton {
public:
    InternalButton() = default;

    bool pressButton(int dest, ElevatorCar &elevator) {
        bool found = false;

        for (int floor : availableButtons) {
            if (floor == dest) {
                found = true;
                break;
            }
        }

        if (!found)
            return false;

        dispatcher.submitRequest(dest, elevator);
        return true;
    }

private:
    vector<int> availableButtons = {0,1,2,3,4,5,6,7,8};
    InternalButtonDispatcher dispatcher;
};

class ExternalButtonDispatcher {
public:
    void submitRequest(int floor, Direction dir) {
        for (auto &controller : ElevatorCreator::elevators) {
            if (controller.getElevator().getId() % 2 == 0 &&
                floor % 2 == 0) {
                controller.submitExternalRequest(floor, dir);
            }
            else if (controller.getElevator().getId() % 2 != 0 &&
                     floor % 2 != 0) {
                controller.submitExternalRequest(floor, dir);
            }
        }
    }
};

class Floor {
public:
    Floor(int id) : id(id) {}

    void pressButton(Direction dir) {
        dispatcher.submitRequest(id, dir);
    }

    int getId() {
        return id;
    }

private:
    int id;
    ExternalButtonDispatcher dispatcher;
};

class Building {
public:
    Building(vector<Floor> floors)
        : floors(floors) {}

    void addFloor(Floor floor) {
        floors.push_back(floor);
    }

    void removeFloor(int id) {
        floors.erase(
            remove_if(floors.begin(),
                      floors.end(),
                      [&](Floor &f) {
                          return f.getId() == id;
                      }),
            floors.end());
    }

private:
    vector<Floor> floors;
};

/*-------------------------------------------------
        ElevatorCar implementation
--------------------------------------------------*/

ElevatorCar::ElevatorCar(int id) {
    this->id = id;
    currentFloor = 0;
    dir = Direction::UP;
    status = Status::IDLE;
    display = Display();
    button = new InternalButton();
}

bool ElevatorCar::move(int destFloor, Direction dir) {
    status = Status::MOVING;
    this->dir = dir;

    if (dir == Direction::UP) {
        for (int i = currentFloor; i <= destFloor; i++) {
            currentFloor = i;
            setDisplay();
            showDisplay();
        }
    }
    else {
        for (int i = currentFloor; i >= destFloor; i--) {
            currentFloor = i;
            setDisplay();
            showDisplay();
        }
    }

    status = Status::IDLE;
    return true;
}

void ElevatorCar::showDisplay() {
    display.printDisplay();
}

void ElevatorCar::setDisplay() {
    display.setDisplay(currentFloor, dir);
}

void ElevatorCar::pressButton(int dest) {
    button->pressButton(dest, *this);
}

/*-------------------------------------------------
                    main
--------------------------------------------------*/

int main() {
    vector<Floor> floors;
    for (int i = 0; i <= 8; i++)
        floors.emplace_back(i);

    Building building(floors);

    cout << "External request from floor 3\n";
    floors[3].pressButton(Direction::UP);

    for (auto &controller : ElevatorCreator::elevators)
        controller.controlElevator();

    cout << "\nInternal request to floor 7\n";
    ElevatorCreator::elevators[0]
        .getElevator()
        .pressButton(7);

    ElevatorCreator::elevators[0].controlElevator();

    return 0;
}
