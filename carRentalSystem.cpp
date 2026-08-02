#include <bits/stdc++.h>
using namespace std;

enum VehicleType
{
    CAR,
    BIKE
};

enum VehicleStatus
{
    ACTIVE,
    INACTIVE
};

class Vehicle
{
public:
    Vehicle(int vehicleId, int vehicleNumber,
            VehicleType type, int kmDriven, VehicleStatus status)
        : vehicleId(vehicleId),
          vehicleNumber(vehicleNumber),
          type(type),
          kmDriven(kmDriven),
          status(status) {}

    int getVehicleId() const {
        return vehicleId;
    }
private:
    int vehicleId;
    int vehicleNumber;
    VehicleType type;
    int kmDriven;
    VehicleStatus status;
};

class VehicleInventoryManagement
{
public:
    VehicleInventoryManagement(vector<Vehicle> vehicles) : vehicles_(vehicles) {}
    void setVehicles(vector<Vehicle> vehicles)
    {
        this->vehicles_ = vehicles;
    }
    vector<Vehicle> getVehicles()
    {
        return this->vehicles_;
    }
    void printVehicles() {
        cout << "Total Vehicles : " << vehicles_.size() << endl;
    }
private:
    std::vector<Vehicle> vehicles_;
};

class Location
{
public:
    Location() = default;
    Location(string add, string city, string state, int pin) : address_(add),
                                                               city_(city), state_(state), pincode_(pin) {}

private:
    string address_;
    string city_;
    string state_;
    int pincode_;
};

class User
{
public:
    User(int id, string license, string username) : userid_(id), drivinglicense_(license),
                                                    username_(username) {}
    int getUserId() const {
        return userid_;
    }
private:
    int userid_;
    string drivinglicense_;
    string username_;
};

struct Date
{
    int day;
    int month;
    int year;

    Date() = default;

    Date(int d, int m, int y)
        : day(d), month(m), year(y) {}
};

enum ReservationType
{
    HOURLY,
    DAILY
};

enum ReservationStatus
{
    SCHEDULED,
    INPROGRESS,
    COMPLETED,
    CANCELLED
};

class Reservation
{
public:
    Reservation() = default;
    void createReservation(User* user, Vehicle* vehicle) {
        id_ = createNewId();
        user_ = user;
        vehicle_ = vehicle;
        type_ = DAILY;
        status_ = SCHEDULED;

        cout << "Reservation Created\n";
        cout << "Reservation Id : " << id_ << endl;
        cout << "User Id : " << user_->getUserId() << endl;
        cout << "Vehicle Id : " << vehicle_->getVehicleId() << endl;
    }
    void printReservation() {
        cout << "Reservation Id : " << id_ << endl;
    }

private:
    int id_;
    User *user_;
    Vehicle *vehicle_;
    Date bookeddate_;
    Date bookedfrom_;
    Date bookedto_;
    long long timefrom_;
    long long timeto_;
    Location pickuplocation_;
    Location droplocation_;
    ReservationType type_;
    ReservationStatus status_;

    int createNewId()
    {
        return rand();
    }
};

class Store
{
public:
    Store(int id,
          unique_ptr<VehicleInventoryManagement> vehicleInventor,
          unique_ptr<Location> location,
          vector<unique_ptr<Reservation>> reservations)
        : storeid_(id),
          vehicleInventory_(std::move(vehicleInventor)),
          location_(std::move(location)),
          reservations_(std::move(reservations))
    {
    }
    void setVehicles(vector<Vehicle> vehicles)
    {
        vehicleInventory_->setVehicles(vehicles);
    }
    vector<Vehicle> getVehicle()
    {
        return vehicleInventory_->getVehicles();
    }
    void displayInventory() {
        vehicleInventory_->printVehicles();
    }
private:
    int storeid_;
    std::unique_ptr<VehicleInventoryManagement> vehicleInventory_;
    std::unique_ptr<Location> location_;
    vector<unique_ptr<Reservation>> reservations_;
};

class Bill {
public:
    Bill(Reservation* reservation)
        : reservation_(reservation),
          billamount_(100),
          ispaid_(false) {}

    void printBill() {
        cout << "\n------ BILL ------\n";
        cout << "Amount : " << billamount_ << endl;
        cout << "Status : " << (ispaid_ ? "Paid" : "Pending") << endl;
    }

    void markPaid() {
        ispaid_ = true;
    }     
private:
    Reservation* reservation_;
    int billamount_;
    bool ispaid_;
};

class Payment{
public:
    void payBill(Bill& bill) {
        bill.markPaid();
        cout << "\nPayment Successful\n";
    }
};

class VehicleRentalSystem
{
public:
    VehicleRentalSystem(vector<unique_ptr<User>> users,
                        vector<unique_ptr<Store>> stores)
        : users_(std::move(users)),
          stores_(std::move(stores))
    {
    }

private:
    vector<unique_ptr<User>> users_;
    vector<unique_ptr<Store>> stores_;
};

int main() {

    // Vehicles
    vector<Vehicle> vehicles;

    vehicles.emplace_back(1, 1001, CAR, 12000, ACTIVE);
    vehicles.emplace_back(2, 1002, BIKE, 5000, ACTIVE);

    Vehicle* selectedVehicle = &vehicles[0];

    // Inventory
    auto inventory =
        make_unique<VehicleInventoryManagement>(vehicles);

    // Store
    auto location =
        make_unique<Location>(
            "MG Road",
            "Bangalore",
            "Karnataka",
            560001);

    vector<unique_ptr<Reservation>> reservations;

    auto store =
        make_unique<Store>(
            1,
            std::move(inventory),
            std::move(location),
            std::move(reservations));

    // User
    auto user =
        make_unique<User>(
            101,
            "DL12345678",
            "Samyak");

    User* currentUser = user.get();

    // Rental System
    vector<unique_ptr<User>> users;
    users.push_back(std::move(user));

    vector<unique_ptr<Store>> stores;
    stores.push_back(std::move(store));

    VehicleRentalSystem rentalSystem(
        std::move(users),
        std::move(stores));

    cout << "Vehicle Rental System Started\n\n";

    // Reservation
    Reservation reservation;

    reservation.createReservation(
        currentUser,
        selectedVehicle);

    // Bill
    Bill bill(&reservation);

    bill.printBill();

    // Payment
    Payment payment;

    payment.payBill(bill);

    bill.printBill();

    return 0;
}