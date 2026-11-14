#include <bits/stdc++.h>
#include <shared_mutex>
using namespace std;

template<typename K, typename V>
class ConcurrentHashMap{
    private:
        static constexpr int MAX_SEGMENT_SIZE = 16;
        static constexpr int MAX_NUMBER_OF_SEGMENTS = 16;
        struct Segment{
            vector<list<pair<K,V>>> buckets;
            shared_mutex mx;
            Segment(): buckets(MAX_SEGMENT_SIZE) {}
        };
        vector<Segment> segments;
        atomic<size_t> cnt{0};
        size_t getHash(const K &k) const{
            return hash<K>{}(k);
        }
        pair<int,int> getIndex(const K& k){
            size_t h = getHash(k);
            size_t s = h%MAX_NUMBER_OF_SEGMENTS;
            size_t b = (h/MAX_NUMBER_OF_SEGMENTS)%MAX_SEGMENT_SIZE;
            return {s,b};
        }
    public:
        ConcurrentHashMap(): segments(MAX_NUMBER_OF_SEGMENTS) {}
        void put(const K& k, const V& v){
            auto [s,b] = getIndex(k);
            auto &segment = segments[s];
            unique_lock<shared_mutex> lc_(segment.mx);
            auto &bucket = segment.buckets[b];
            for(auto &[key,value] : bucket){
                if(key == k){
                    value = v;
                    return;
                }
            }
            bucket.push_back({k,v});
            cnt.fetch_add(1);
        }
        optional<V> get(const K& k){
            auto [s,b] = getIndex(k);
            auto &segment = segments[s];
            shared_lock<shared_mutex> lc_(segment.mx);
            auto &bucket = segment.buckets[b];
            for(auto &[key,value] : bucket){
                if(key == k){
                    return value;
                }
            }
            return nullopt;
        }
        bool erase(const K &k){
            auto [s,b] = getIndex(k);
            auto &segment = segments[s];
            unique_lock<shared_mutex> lc_(segment.mx);
            auto &bucket = segment.buckets[b];
            for(auto it = bucket.begin(); it != bucket.end(); ++it) {
            if(it->first == k) {
                bucket.erase(it);
                cnt.fetch_sub(1);
                return true;
            }
}
            return false;
        }
        size_t size(){
            return cnt.load();
        }
};

int main(){
    ConcurrentHashMap<int,int> map;
    map.put(1,1);
    map.put(5,6);
    map.put(10,17);
    map.erase(5);
    map.put(1,9);
    auto val = map.get(1);

    if (val.has_value())
        cout << val.value() << endl;

    return 0;
}