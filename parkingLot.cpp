class FirstAvailableStrategy : public ParkingStrategy
{
public:

    ParkingSpot*
    findParkingSpot(const vector<ParkingSpot*>& spots) override
    {
        for(auto spot : spots)
        {
            if(spot->isAvailable())
                return spot;
        }

        return nullptr;
    }
};
/*****************************************************
            FEE CALCULATOR
*****************************************************/

class FeeCalculatorStrategy
{
public:

    virtual double calculateFee(const Ticket& ticket)=0;

    virtual ~FeeCalculatorStrategy()=default;
};

class HourlyFeeCalculator
    : public FeeCalculatorStrategy
{
private:

    double hourlyRate;

public:

    HourlyFeeCalculator(double rate=20)
        : hourlyRate(rate)
    {
    }

    double calculateFee(const Ticket& ticket) override
    {
        time_t current = time(nullptr);

        double hours =
            difftime(current,
                     ticket.getEntryTime()) / 3600.0;

        if(hours < 1)
            hours = 1;

        return hours * hourlyRate;
    }
};
/*****************************************************
            PAYMENT STRATEGY
*****************************************************/

class PaymentStrategy
{
public:

    virtual void pay(double amount)=0;

    virtual ~PaymentStrategy()=default;
};

class CashPayment : public PaymentStrategy
{
public:

    void pay(double amount) override
    {
        cout<<"Cash Payment : "<<amount<<endl;
    }
};

class UPIPayment : public PaymentStrategy
{
public:

    void pay(double amount) override
    {
        cout<<"UPI Payment : "<<amount<<endl;
    }
};
/*****************************************************
                ENTRANCE GATE
*****************************************************/

class EntranceGate
{
private:

    ParkingSpotManagerFactory* factory;

public:

    EntranceGate(ParkingSpotManagerFactory* f)
        : factory(f)
    {
    }

    Ticket* parkVehicle(Vehicle* vehicle)
    {
        ParkingSpotManager* manager =
            factory->getManager(*vehicle);

        ParkingSpot* spot =
            manager->findParkingSpot();

        if(spot == nullptr)
        {
            cout<<"Parking Full\n";
            return nullptr;
        }

        if(!spot->parkVehicle(vehicle))
        {
            cout<<"Cannot Park\n";
            return nullptr;
        }

        cout<<"Allocated Spot : "
            <<spot->getSpotId()<<endl;

        return new Ticket(vehicle, spot);
    }
};
/*****************************************************
                    EXIT GATE
*****************************************************/

class ExitGate
{
private:

    ParkingSpotManagerFactory* factory;

    unique_ptr<FeeCalculatorStrategy> feeCalculator;

    unique_ptr<PaymentStrategy> paymentStrategy;

public:

    ExitGate(
        ParkingSpotManagerFactory* factory,
        unique_ptr<FeeCalculatorStrategy> fee,
        unique_ptr<PaymentStrategy> payment)

        : factory(factory),
          feeCalculator(move(fee)),
          paymentStrategy(move(payment))
    {
    }

    void exitVehicle(Ticket* ticket)
    {
        double amount =
            feeCalculator->calculateFee(*ticket);

        paymentStrategy->pay(amount);

        ParkingSpot* spot =
            ticket->getParkingSpot();

        Vehicle* vehicle =
            ticket->getVehicle();

        ParkingSpotManager* manager =
            factory->getManager(*vehicle);

        manager->removeVehicle(spot);

        cout<<"Spot "
            <<spot->getSpotId()
            <<" Freed\n";
    }
};
/*****************************************************
                PARKING LOT
*****************************************************/

class ParkingLot
{
private:

    TwoWheelerManager bikeManager;

    FourWheelerManager carManager;

    ParkingSpotManagerFactory managerFactory;

    vector<EntranceGate> entrances;

    vector<ExitGate> exits;

public:

    ParkingLot()
        : managerFactory(
            &bikeManager,
            &carManager)
    {
        entrances.emplace_back(&managerFactory);

        exits.emplace_back(
            &managerFactory,
            make_unique<HourlyFeeCalculator>(),
            make_unique<CashPayment>());
    }

    TwoWheelerManager&
    getBikeManager()
    {
        return bikeManager;
    }

    FourWheelerManager&
    getCarManager()
    {
        return carManager;
    }

    EntranceGate&
    getEntrance(int i)
    {
        return entrances[i];
    }

    ExitGate&
    getExit(int i)
    {
        return exits[i];
    }
};
int main()
{
    ParkingLot parkingLot;

    parkingLot.getBikeManager().addParkingSpot(
        new TwoWheelerSpot(1));

    parkingLot.getBikeManager().addParkingSpot(
        new TwoWheelerSpot(2));

    parkingLot.getCarManager().addParkingSpot(
        new FourWheelerSpot(101));

    parkingLot.getCarManager().addParkingSpot(
        new FourWheelerSpot(102));

    Vehicle bike(
        "KA01AB1111",
        VehicleType::TWO_WHEELER);

    Vehicle car(
        "KA05CD9999",
        VehicleType::FOUR_WHEELER);

    Ticket* bikeTicket =
        parkingLot.getEntrance(0)
                  .parkVehicle(&bike);

    Ticket* carTicket =
        parkingLot.getEntrance(0)
                  .parkVehicle(&car);

    cout<<"\nVehicle Leaving...\n\n";

    parkingLot.getExit(0)
              .exitVehicle(bikeTicket);

    parkingLot.getExit(0)
              .exitVehicle(carTicket);

    return 0;
}