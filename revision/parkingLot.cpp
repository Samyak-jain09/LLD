#include <bits/stdc++.h>
#include <thread>
using namespace std;

class ParkingSpot;
class Vehicle;
class ParkingSpotManager;
class Ticket;

enum class VehicleType
{
    twoWheeler,
    fourWheeler
};

class Vehicle
{
private:
    int vehicleNumber;
    VehicleType type;

public:
    Vehicle(int num, VehicleType type) : vehicleNumber(num), type(type) {}
    int getVehicleNumber() const
    {
        return this->vehicleNumber;
    }
    VehicleType getVehicleType() const
    {
        return this->type;
    }
};

class Ticket
{
private:
    int id;
    std::chrono::steady_clock::time_point entranceTime;
    ParkingSpot *spot;
    Vehicle *vehicle;

public:
    Ticket(int id, ParkingSpot *spot, Vehicle *vehicle) : id(id), entranceTime(std::chrono::steady_clock::now()),
                                                          spot(spot), vehicle(vehicle) {}
    Vehicle *getVehicle() const
    {
        return this->vehicle;
    }
    ParkingSpot *getPS() const
    {
        return this->spot;
    }
    std::chrono::steady_clock::time_point getEntryTime() const
    {
        return this->entranceTime;
    }
};

class ParkingSpot
{
private:
    int id;
    Vehicle *vehicleParked;
    bool isEmpty;

public:
    ParkingSpot(int id) : id(id), vehicleParked(nullptr), isEmpty(true) {}
    virtual int getPrice() = 0;
    int getId() const
    {
        return this->id;
    }
    void parkVehicle(Vehicle *vehicle)
    {
        if (vehicleParked != nullptr)
        {
            cout << "Spot is not empty\n";
            return;
        }
        else
        {
            isEmpty = false;
            vehicleParked = vehicle;
        }
    }
    void removeVehicle()
    {
        if (vehicleParked == nullptr)
            cout << "Spot already empty\n";
        else
        {
            vehicleParked = nullptr;
            isEmpty = true;
        }
    }
};

class TwoWheelerParkingSpot : public ParkingSpot
{
public:
    TwoWheelerParkingSpot(int id) : ParkingSpot(id) {}
    int getPrice() override
    {
        return 15;
    }
};

class FourWheelerParkingSpot : public ParkingSpot
{
public:
    FourWheelerParkingSpot(int id) : ParkingSpot(id) {}
    int getPrice() override
    {
        return 40;
    }
};

class ParkingStratergy
{
public:
    virtual ParkingSpot *getPS(vector<ParkingSpot *> parkingSpots) = 0;
};

class NearestParkingStrategy : public ParkingStratergy
{
public:
    ParkingSpot *getPS(vector<ParkingSpot *> parkingSpots) override
    {
        return parkingSpots[0];
    }
};

class PricingStatergy
{
public:
    virtual int calculateCost(std::chrono::seconds sec) = 0;
};

class HourlyPricingStrategy : public PricingStatergy
{
public:
    int calculateCost(std::chrono::seconds sec) override
    {
        return sec.count() * 10;
    }
};

class MinutePricingStrategy : public PricingStatergy
{
public:
    int calculateCost(std::chrono::seconds sec) override
    {
        return sec.count() * 10;
    }
};


class ParkingSpotManager
{
protected:
    vector<ParkingSpot *> parkingSpots;
    ParkingStratergy *strategy;

public:
    ParkingSpotManager(vector<ParkingSpot *> list, ParkingStratergy *strategy) : parkingSpots(list), strategy(strategy) {}
    virtual void addParkingSpot(int id) = 0;
    void removeParkingSpot(int id)
    {
        auto it = find_if(parkingSpots.begin(), parkingSpots.end(), [&](const ParkingSpot *spot)
                          { return spot->getId() == id; });
        if (it != parkingSpots.end())
        {
            parkingSpots.erase(it);
        }
    }
    ParkingSpot *getParkingSpace()
    {
        return strategy->getPS(parkingSpots);
    }
    ParkingSpot *parkVehicle(Vehicle *vehicle)
    {
        ParkingSpot *spot = getParkingSpace();
        spot->parkVehicle(vehicle);
        return spot;
    }
    void removeVehicle(ParkingSpot *ps)
    {
        ps->removeVehicle();
    }
    vector<ParkingSpot *> getList() const
    {
        return this->parkingSpots;
    }
};

class TwoWheelerPSM : public ParkingSpotManager
{
public:
    TwoWheelerPSM(vector<ParkingSpot *> list, ParkingStratergy *strategy) : ParkingSpotManager(list, strategy) {}
    void addParkingSpot(int id) override
    {
        parkingSpots.push_back(new TwoWheelerParkingSpot(id));
    }
};

class FourWheelerPSM : public ParkingSpotManager
{
public:
    FourWheelerPSM(vector<ParkingSpot *> list, ParkingStratergy *strategy) : ParkingSpotManager(list, strategy) {}
    void addParkingSpot(int id) override
    {
        parkingSpots.push_back(new FourWheelerParkingSpot(id));
    }
};

class ParkingSpotManagerFactory
{
private:
    ParkingSpotManager *twoWheelerManager;
    ParkingSpotManager *fourWheelerManager;

public:
    ParkingSpotManagerFactory(
        ParkingSpotManager *two,
        ParkingSpotManager *four)
        : twoWheelerManager(two),
          fourWheelerManager(four)
    {
    }
    ParkingSpotManager *getParkingSpotManager(VehicleType type)
    {
        if (type == VehicleType::twoWheeler)
            return twoWheelerManager;
        else
            return fourWheelerManager;
    }
};

class EntranceGate
{
private:
    ParkingSpotManagerFactory *factory;

public:
    EntranceGate(ParkingSpotManagerFactory *psmf) : factory(psmf) {}
    Ticket *parkVehicle(Vehicle *vehicle)
    {
        ParkingSpotManager *psm =
            factory->getParkingSpotManager(vehicle->getVehicleType());
        ParkingSpot *spot = psm->parkVehicle(vehicle);
        return new Ticket(1, spot, vehicle);
    }
};


class CostComputation
{
private:
    PricingStatergy *strategy;

public:
    CostComputation(PricingStatergy *strategy) : strategy(strategy) {}
    int computeCost(std::chrono::seconds duration)
    {
        return strategy->calculateCost(duration);
    }
};

class ExitGate
{
private:
    CostComputation *cc;

public:
    ExitGate(CostComputation *cc) : cc(cc) {}
    void exitFromGate(Ticket *ticket)
    {
        ticket->getPS()->removeVehicle();
        auto duration =
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() -
                ticket->getEntryTime());
        int cost = cc->computeCost(duration) * ticket->getPS()->getPrice();
        cout << "Cost : " << cost << endl;
    }
};

class ParkingLot
{
private:
    ParkingSpotManagerFactory *factory;
    EntranceGate *entranceGate;
    ExitGate *exitGate;

public:
    ParkingLot(
        ParkingSpotManagerFactory *factory,
        EntranceGate *entranceGate,
        ExitGate *exitGate) : factory(factory),
                              entranceGate(entranceGate),
                              exitGate(exitGate) {}
    EntranceGate *getEntranceGate()
    {
        return entranceGate;
    }
    ExitGate *getExitGate()
    {
        return exitGate;
    }
};

int main()
{

    // ----------------------------
    // Create parking strategies
    // ----------------------------
    ParkingStratergy *nearestStrategy =
        new NearestParkingStrategy();

    // ----------------------------
    // Create two wheeler spots
    // ----------------------------
    vector<ParkingSpot *>
        twoWheelerSpots;
    twoWheelerSpots.push_back(
        new TwoWheelerParkingSpot(1));
    twoWheelerSpots.push_back(
        new TwoWheelerParkingSpot(2));
    twoWheelerSpots.push_back(
        new TwoWheelerParkingSpot(3));

    // ----------------------------
    // Create four wheeler spots
    // ----------------------------
    vector<ParkingSpot *>
        fourWheelerSpots;
    fourWheelerSpots.push_back(
        new FourWheelerParkingSpot(101));
    fourWheelerSpots.push_back(
        new FourWheelerParkingSpot(102));
    fourWheelerSpots.push_back(
        new FourWheelerParkingSpot(103));

    // ----------------------------
    // Create managers
    // ----------------------------
    ParkingSpotManager *twoWheelerManager =
        new TwoWheelerPSM(
            twoWheelerSpots,
            nearestStrategy);
    ParkingSpotManager *fourWheelerManager =
        new FourWheelerPSM(
            fourWheelerSpots,
            nearestStrategy);

    // ----------------------------
    // Create factory
    // ----------------------------
    ParkingSpotManagerFactory *factory =
        new ParkingSpotManagerFactory(
            twoWheelerManager,
            fourWheelerManager);

    // ----------------------------
    // Pricing setup
    // ----------------------------
    PricingStatergy *pricingStrategy =
        new HourlyPricingStrategy();
    CostComputation *cc =
        new CostComputation(pricingStrategy);

    // ----------------------------
    // Gates
    // ----------------------------
    EntranceGate *entranceGate =
        new EntranceGate(factory);
    ExitGate *exitGate =
        new ExitGate(cc);

    // ----------------------------
    // Parking lot
    // ----------------------------
    ParkingLot *parkingLot =
        new ParkingLot(
            factory,
            entranceGate,
            exitGate);

    // ----------------------------
    // Vehicles entering
    // ----------------------------
    Vehicle *bike =
        new Vehicle(
            1111,
            VehicleType::twoWheeler);
    Vehicle *car =
        new Vehicle(
            2222,
            VehicleType::fourWheeler);
    Ticket *bikeTicket =
        parkingLot->getEntranceGate()
            ->parkVehicle(bike);
    Ticket *carTicket =
        parkingLot->getEntranceGate()
            ->parkVehicle(car);
    cout << "Vehicles parked\n";

    // Simulate some stay
    std::this_thread::sleep_for(
        std::chrono::seconds(5));

    // ----------------------------
    // Exit
    // ----------------------------
    parkingLot->getExitGate()
        ->exitFromGate(bikeTicket);
    parkingLot->getExitGate()
        ->exitFromGate(carTicket);
    return 0;
}