#include <bits/stdc++.h>
using namespace std;

template <typename K, typename V>
class UnorderedMap{
    private:
        struct Node{
            K key;
            V value;
            Node* next;
            Node(const K &k, const V &v) : key(k), value(v), next(nullptr) {}
        };
        Node** buckets_;
        size_t size_;
        size_t bucket_count_;
    public:
        UnorderedMap() : buckets_(nullptr), size_(0), bucket_count_(0) {}
        ~UnorderedMap() {
            if(buckets_!=nullptr){
                for(size_t i = 0 ; i<bucket_count_ ; ++i){
                    Node* current = bucket_[i];
                    while(current!=nullptr){
                        Node* tmp = current->next;
                        delete current;
                        current = tmp;
                    }
                }
                delete[] buckets_;
            }
        }
};