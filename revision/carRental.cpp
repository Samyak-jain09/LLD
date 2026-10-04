#include <bits/stdc++.h>
using namespace std;

class User;

enum class VehicleStatus
{
    AVAILABLE,
    NOT_AVAILABLE
};

enum class ReservationStatus
{
    PAID,
    UNDER_PROCESS,
    COMPLETED
};

enum class PaymentStatus
{
    SUCCESS,
    FAILED,
    PENDING
};

class Location
{
private:
    string state;
    string city;
    string address;

public:
    Location(string st, string c, string add)
        : state(st), city(c), address(add) {}

    void displayLocation() const
    {
        cout << "State   : " << state << endl;
        cout << "City    : " << city << endl;
        cout << "Address : " << address << endl;
    }
};

class Vehicle
{
private:
    int vehicleNumber;
    int kmsDriven;
    VehicleStatus status;
    string name;

public:
    Vehicle(
        int num,
        int kms,
        VehicleStatus st,
        string name) : vehicleNumber(num),
                       kmsDriven(kms),
                       status(st),
                       name(name) {}

    VehicleStatus getStatus() const
    {
        return status;
    }

    void setStatus(VehicleStatus st)
    {
        status = st;
    }

    string getName() const
    {
        return name;
    }

    int getVehicleNumber() const
    {
        return vehicleNumber;
    }

    void displayVehicle() const
    {
        cout << "Vehicle Number : "
             << vehicleNumber << endl;

        cout << "Vehicle Name   : "
             << name << endl;

        cout << "Kms Driven     : "
             << kmsDriven << endl;

        cout << "Status         : "
             << (status == VehicleStatus::AVAILABLE
                     ? "Available"
                     : "Not Available")
             << endl;

        cout << "----------------------------"
             << endl;
    }
};

class VehicleManagement
{
private:
    vector<shared_ptr<Vehicle>> vehicles;

public:
    void addVehicle(shared_ptr<Vehicle> vehicle)
    {
        vehicles.push_back(vehicle);
    }

    void displayVehicles() const
    {
        cout << "Total Vehicles : "
             << vehicles.size()
             << endl
             << endl;

        for (auto &vehicle : vehicles)
        {
            vehicle->displayVehicle();
        }
    }
};

class User
{
private:
    int id;
    string name;
    Location location;

public:
    User(
        int id,
        string name,
        Location location) : id(id),
                             name(name),
                             location(location) {}

    int getId() const
    {
        return id;
    }

    string getName() const
    {
        return name;
    }

    void displayUser() const
    {
        cout << "User Id   : "
             << id << endl;

        cout << "Name      : "
             << name << endl;
    }
};

class Bill
{
private:
    int billId;
    int totalAmount;

public:
    Bill(
        int billId,
        int totalAmount) : billId(billId),
                           totalAmount(totalAmount) {}

    int getAmount() const
    {
        return totalAmount;
    }

    void displayBill() const
    {
        cout << "\n----- Bill -----\n";

        cout << "Bill Id      : "
             << billId << endl;

        cout << "Total Amount : "
             << totalAmount << endl;

        cout << "----------------\n";
    }
};

class Payment
{
private:
    int paymentId;
    PaymentStatus status;
    shared_ptr<Bill> bill;

public:
    Payment(
        int paymentId,
        shared_ptr<Bill> bill,
        PaymentStatus status) : paymentId(paymentId),
                                bill(bill),
                                status(status) {}

    void displayPayment() const
    {

        cout << "\n----- Payment -----\n";

        cout << "Payment Id : "
             << paymentId
             << endl;

        cout << "Amount     : "
             << bill->getAmount()
             << endl;

        cout << "Status     : ";

        if (status == PaymentStatus::SUCCESS)
            cout << "SUCCESS";
        else if (status == PaymentStatus::FAILED)
            cout << "FAILED";
        else
            cout << "PENDING";

        cout << endl;
    }
};

class Reservation
{
private:
    int id;
    User user;
    Location location;
    shared_ptr<Vehicle> vehicle;
    int bookedFrom;
    int bookedTill;
    ReservationStatus status;
    shared_ptr<Bill> bill;
    shared_ptr<Payment> payment;

public:
    Reservation(
        int id,
        User user,
        Location location,
        shared_ptr<Vehicle> vehicle,
        int bookedFrom,
        int bookedTill,
        ReservationStatus status,
        shared_ptr<Bill> bill,
        shared_ptr<Payment> payment) : id(id),
                                       user(user),
                                       location(location),
                                       vehicle(vehicle),
                                       bookedFrom(bookedFrom),
                                       bookedTill(bookedTill),
                                       status(status),
                                       bill(bill),
                                       payment(payment) {}

    void confirmReservation()
    {
        vehicle->setStatus(
            VehicleStatus::NOT_AVAILABLE);
    }

    void completeReservation()
    {
        status = ReservationStatus::COMPLETED;

        vehicle->setStatus(
            VehicleStatus::AVAILABLE);
    }

    void displayReservation() const {

    cout << "\nReservation Id : "
         << id << endl;

    cout << "Customer : "
         << user.getName()
         << endl;

    cout << "Vehicle : "
         << vehicle->getName()
         << endl;

    bill->displayBill();

    payment->displayPayment();
}
};

class Store
{
private:
    Location location;
    VehicleManagement inventory;
    vector<shared_ptr<Reservation>> reservations;

public:
    Store(
        Location location,
        VehicleManagement inventory) : location(location),
                                       inventory(inventory) {}

    void addReservation(
        shared_ptr<Reservation> reservation)
    {
        reservations.push_back(reservation);
    }

    void addVehicle(
        shared_ptr<Vehicle> vehicle)
    {
        inventory.addVehicle(vehicle);
    }

    void displayInventory()
    {
        inventory.displayVehicles();
    }

    void displayReservations()
    {
        cout << "\nReservations Count : "
             << reservations.size()
             << endl;

        for (auto &reservation :
             reservations)
        {
            reservation->displayReservation();
        }
    }
};

class CarRental
{
private:
    vector<shared_ptr<Store>> stores;
    vector<shared_ptr<User>> users;

public:
    void addStore(
        shared_ptr<Store> store)
    {
        stores.push_back(store);
    }

    void addUser(
        shared_ptr<User> user)
    {
        users.push_back(user);
    }

    void displayStats()
    {
        cout << "\n--------- SYSTEM ---------"
             << endl;

        cout << "Users  : "
             << users.size()
             << endl;

        cout << "Stores : "
             << stores.size()
             << endl;

        cout << "--------------------------"
             << endl;
    }
};

int main()
{
    cout << "\n===== CAR RENTAL SYSTEM =====\n"
         << endl;

    // User
    Location userLocation(
        "Karnataka",
        "Bangalore",
        "Whitefield");

    auto user1 = make_shared<User>(
        1,
        "Samyak",
        userLocation);

    // Vehicles
    auto car1 = make_shared<Vehicle>(
        101,
        15000,
        VehicleStatus::AVAILABLE,
        "Hyundai Creta");

    auto car2 = make_shared<Vehicle>(
        102,
        22000,
        VehicleStatus::AVAILABLE,
        "Honda City");

    auto car3 = make_shared<Vehicle>(
        103,
        8000,
        VehicleStatus::AVAILABLE,
        "Baleno");

    // Inventory
    VehicleManagement inventory;

    inventory.addVehicle(car1);
    inventory.addVehicle(car2);
    inventory.addVehicle(car3);

    // Store
    Location storeLocation(
        "Karnataka",
        "Bangalore",
        "MG Road");

    auto store = make_shared<Store>(
        storeLocation,
        inventory);

    cout << "\nINITIAL INVENTORY\n"
         << endl;

    store->displayInventory();

    // Bill
    auto bill1 = make_shared<Bill>(
        10001,
        2500);

    // Payment
    auto payment1 = make_shared<Payment>(
        5001,
        bill1,
        PaymentStatus::SUCCESS);

    // Reservation
    auto reservation1 =
        make_shared<Reservation>(
            1001,
            *user1,
            storeLocation,
            car1,
            10,
            15,
            ReservationStatus::PAID,
            bill1,
            payment1);

    reservation1->confirmReservation();

    store->addReservation(reservation1);

    cout << "\nAFTER BOOKING\n"
         << endl;

    store->displayInventory();

    store->displayReservations();

    cout << "\nRETURNING VEHICLE...\n"
         << endl;

    reservation1->completeReservation();

    cout << "\nAFTER RETURN\n"
         << endl;

    store->displayInventory();

    CarRental app;

    app.addUser(user1);
    app.addStore(store);

    app.displayStats();

    return 0;
}