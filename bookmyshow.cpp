#include <bits/stdc++.h>
#include <thread>
#include <chrono>
using namespace std;

// --- Enums ---
enum class City { BANGALORE, DELHI };
enum class SeatCategory { SILVER, GOLD, PLATINUM };

// --- Hash for City ---
struct CityHash {
    size_t operator()(const City& c) const {
        return static_cast<size_t>(c);
    }
};

// --- Movie ---
class Movie {
public:
    Movie(int id, string name, int duration) : id_(id), name_(name), duration_(duration) {}
    int getId() const { return id_; }
    string getName() const { return name_; }
    int getDuration() const { return duration_; }
private:
    int id_;
    string name_;
    int duration_;
};

// --- Seat with optimistic locking ---
class Seat {
public:
    Seat(int id, int row, SeatCategory cat, int price)
        : id_(id), row_(row), category_(cat), price_(price),
          booked_(false), version_(0) {}

    int getId() const { return id_; }
    int getPrice() const { return price_; }

    bool tryBook() {
        int expectedVersion = version_.load();
        if (booked_.load()) {
            cout << "Seat " << id_ << " already booked!\n";
            return false;
        }
        // Optimistic CAS update
        if (version_.compare_exchange_strong(expectedVersion, expectedVersion + 1)) {
            booked_.store(true);
            return true;
        } else {
            cout << "Seat " << id_ << " was modified concurrently, retry...\n";
            return false;
        }
    }
    private:
        int id_;
        int row_;
        SeatCategory category_;
        int price_;
        atomic<bool> booked_;
        atomic<int> version_;
};
// --- Screen ---
class Screen {
public:
    Screen(int id, vector<shared_ptr<Seat>> seats) : id_(id), seats_(seats) {}
    const vector<shared_ptr<Seat>>& getSeats() const { return seats_; }
private:
    int id_;
    vector<shared_ptr<Seat>> seats_;
};

// --- Show using seat-level optimistic booking ---
class Show {
public:
    Show(int id, shared_ptr<Movie> movie, shared_ptr<Screen> screen, int startTime)
        : id_(id), movie_(movie), screen_(screen), startTime(startTime) {}

    int getId() const { return id_; }
    shared_ptr<Movie> getMovie() const { return movie_; }
    shared_ptr<Screen> getScreen() const { return screen_; }
    int getStartTime() const { return startTime; }

    bool bookSeats(const vector<shared_ptr<Seat>>& seats) {
        bool allBooked = true;
        for (auto& seat : seats) {
            if (!seat->tryBook()) {
                allBooked = false;
            }
        }
        return allBooked;
    }

private:
    int id_;
    shared_ptr<Movie> movie_;
    shared_ptr<Screen> screen_;
    int startTime;
};

// --- Theatre ---
class Theatre {
public:
    Theatre(int id, string address, City city, vector<shared_ptr<Screen>> screens, vector<shared_ptr<Show>> shows)
        : id_(id), address_(address), city_(city), screens_(screens), shows_(shows) {}

    const vector<shared_ptr<Show>>& getShows() const { return shows_; }

private:
    int id_;
    string address_;
    City city_;
    vector<shared_ptr<Screen>> screens_;
    vector<shared_ptr<Show>> shows_;
};

// --- Payment ---
class Payment {
public:
    Payment(int id, int amount) : id_(id), amount_(amount) {}
    int getId() const { return id_; }
    int getAmount() const { return amount_; }
private:
    int id_;
    int amount_;
};

// --- Booking ---
class Booking {
public:
    Booking(shared_ptr<Show> show, const vector<shared_ptr<Seat>>& seats, const Payment& payment)
        : show(show), seats(seats), payment(payment) {
        if (show->bookSeats(seats)) {
            cout << "Booking successful! Payment ID: " << payment.getId() << "\n";
        } else {
            cout << "Booking failed due to conflict!\n";
        }
    }
private:
    shared_ptr<Show> show;
    vector<shared_ptr<Seat>> seats;
    Payment payment;
};

// --- Controllers ---
class MovieController {
public:
    void addMovie(shared_ptr<Movie> movie, City city) {
        allMovies_.push_back(movie);
        moviesByCity_[city].push_back(movie);
    }
    shared_ptr<Movie> getMovieByName(const string& movieName) {
        for (auto& it : allMovies_) {
            if (it->getName() == movieName) return it;
        }
        throw runtime_error("Movie not found");
    }
    vector<shared_ptr<Movie>> getMovieByCity(City city){
        auto it = moviesByCity_.find(city);
        if(it == moviesByCity_.end()){
            throw out_of_range("Not a valid city");
        }
        else{
            return it->second;
        }
    }
private:
    unordered_map<City, vector<shared_ptr<Movie>>, CityHash> moviesByCity_;
    vector<shared_ptr<Movie>> allMovies_;
};

class TheatreController {
public:
    void addTheatre(shared_ptr<Theatre> theatre, City city) {
        allTheatres_.push_back(theatre);
        theatresByCity_[city].push_back(theatre);
    }
    vector<shared_ptr<Show>> getShowsByMovie(const City& city, shared_ptr<Movie> movie) {
        vector<shared_ptr<Show>> result;
        auto it = theatresByCity_.find(city);
        if (it == theatresByCity_.end()) return result;
        for (auto& theatre : it->second) {
            for (auto& show : theatre->getShows()) {
                if (show->getMovie()->getId() == movie->getId()) result.push_back(show);
            }
        }
        return result;
    }
private:
    unordered_map<City, vector<shared_ptr<Theatre>>, CityHash> theatresByCity_;
    vector<shared_ptr<Theatre>> allTheatres_;
};

class BookMyShow {
public:
    BookMyShow() {
        setupData();
    }
    MovieController &getMovieController(){
        return movieController_;
    }
    TheatreController &getTheatreController(){
        return theatreController_;
    }
    void run() {
        cout << "Welcome to BookMyShow!\n";
        cout << "Choose City:\n1. Bangalore\n2. Delhi\n";
        int cityChoice; cin >> cityChoice;
        City chosenCity = (cityChoice == 1) ? City::BANGALORE : City::DELHI;

        // List movies
        cout << "\nAvailable Movies:\n";
        auto movies = movieController_.getMovieByCity(chosenCity);
        for (size_t i = 0; i < movies.size(); i++) {
            cout << i+1 << ". " << movies[i]->getName()
                 << " (" << movies[i]->getDuration() << " mins)\n";
        }

        cout << "Select a movie: ";
        int movieChoice; cin >> movieChoice;
        auto chosenMovie = movies[movieChoice-1];

        // List shows
        auto shows = theatreController_.getShowsByMovie(chosenCity, chosenMovie);
        cout << "\nAvailable Shows for " << chosenMovie->getName() << ":\n";
        for (size_t i = 0; i < shows.size(); i++) {
            cout << i+1 << ". Show ID " << shows[i]->getId()
                 << " at " << shows[i]->getStartTime() << "\n";
        }

        cout << "Select a show: ";
        int showChoice; cin >> showChoice;
        auto chosenShow = shows[showChoice-1];

        // Seat selection
        cout << "\nAvailable Seats (IDs 1-10):\n";
        for (auto& seat : screen1_->getSeats()) {
            cout << "Seat " << seat->getId()
                 << " Price: " << seat->getPrice() << "\n";
        }

        cout << "Enter seat IDs to book (space separated, end with -1): ";
        vector<shared_ptr<Seat>> selectedSeats;
        int seatId;
        while (cin >> seatId && seatId != -1) {
            if (seatId >= 1 && seatId <= (int)screen1_->getSeats().size()) {
                selectedSeats.push_back(screen1_->getSeats()[seatId-1]);
            } else {
                cout << "Invalid seat ID!\n";
            }
        }

        // Payment
        int totalAmount = 0;
        for (auto& s : selectedSeats) totalAmount += s->getPrice();
        Payment payment1(101, totalAmount);

        // Booking
        Booking booking1(chosenShow, selectedSeats, payment1);

        cout << "Total Amount Paid: " << totalAmount << "\n";
        cout << "Enjoy your movie!\n";
    }

private:
    MovieController movieController_;
    TheatreController theatreController_;
    shared_ptr<Screen> screen1_;

    void setupData() {
        auto movie1 = make_shared<Movie>(1, "Inception", 150);
        auto movie2 = make_shared<Movie>(2, "Interstellar", 180);

        vector<shared_ptr<Seat>> seats;
        for (int i = 1; i <= 10; i++) {
            seats.push_back(make_shared<Seat>(i, 1, SeatCategory::SILVER, 200));
        }

        screen1_ = make_shared<Screen>(1, seats);

        auto show1 = make_shared<Show>(1, movie1, screen1_, 1800);
        auto show2 = make_shared<Show>(2, movie2, screen1_, 2100);

        auto theatre1 = make_shared<Theatre>(
            1, "MG Road", City::BANGALORE,
            vector<shared_ptr<Screen>>{screen1_},
            vector<shared_ptr<Show>>{show1, show2}
        );

        movieController_.addMovie(movie1, City::BANGALORE);
        movieController_.addMovie(movie2, City::BANGALORE);

        theatreController_.addTheatre(theatre1, City::BANGALORE);
    }
};

// Function to simulate a user trying to book a seat
void simulateBooking(shared_ptr<Show> show, int seatId, int userId) {
    auto seat = show->getScreen()->getSeats()[seatId-1];
    Payment p(userId, seat->getPrice());
    cout << "User " << userId << " attempting to book Seat " << seatId << "\n";
    Booking b(show, {seat}, p);
}

int main() {
    BookMyShow app;

    // Get a show to simulate bookings
    auto movies = app.getMovieController().getMovieByCity(City::BANGALORE);
    auto chosenMovie = movies[0];
    auto shows = app.getTheatreController().getShowsByMovie(City::BANGALORE, chosenMovie);
    auto chosenShow = shows[0];

    // Simulate multiple users racing for the same seat
    thread t1(simulateBooking, chosenShow, 1, 101);
    thread t2(simulateBooking, chosenShow, 1, 102);
    thread t3(simulateBooking, chosenShow, 2, 103);
    thread t4(simulateBooking, chosenShow, 2, 104);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    cout << "Simulation complete.\n";
    return 0;
}