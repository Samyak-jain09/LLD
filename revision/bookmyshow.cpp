#include <bits/stdc++.h>
using namespace std;

class Movie{
private:
    int id;
    string name;
    int duration;
public:
    Movie(int id, string name, int duration): id(id), name(name), duration(duration)    {}
    const string getName() const{
        return this->name;
    }
    const int getDuraion() const{
        return this->duration;
    }
    const int getId() const{
        return this->id;
    }
};

class MovieController{
    private:
        unordered_map<string,vector<shared_ptr<Movie>>> cityToMovies;
        unordered_set<shared_ptr<Movie>> movies;
    public:
        MovieController() = default;
        void addMovieToCity(string &city, shared_ptr<Movie> movie){
            movies.insert(movie);
            cityToMovies[city].push_back(move(movie));
        }
        const vector<shared_ptr<Movie>>& getMoviesByCity(const string &city) const {
            auto it = cityToMovies.find(city);
            if (it != cityToMovies.end()) {
                return it->second;
            }
            static const vector<shared_ptr<Movie>> emptyVector;
            return emptyVector;
        }
        unordered_set<shared_ptr<Movie>> getAllMovies() const{
            return movies;
        }
};

class Screen{

};

