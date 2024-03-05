#include <vector>
#include <list>
#include <functional>
#include <iostream>
#include <cstdarg>
#include <array>
#include <map>
#include <climits>
#include <cmath>
#include <algorithm>
#include <string>
#include <cstring>


// insert
// erase
// []
// find
// rehash
// loadFactor
// getSize
// setHash
// 

template<typename Key, typename Val, typename MyHash = std::hash<Key>> 
// requires (requires{
//     MyHash h;
//     h();
// }

// )
class HashMap {
    
    

    public:

        HashMap(float max_load = 0.95) : max_load_factor_(max_load){}
    
        template<typename... Coords> requires(std::conjunction<std::is_same<std::pair<const Key, Val>, Coords>...>::value)
        HashMap(Coords... coords)
        {

        }

        HashMap(HashMap&&) = default;

        // void Insert(const Key& key, const Val& value){
        //     buckets_[MyHash(key)].emplace_back(key, value);
        // }

        void Insert(std::pair<const Key, Val>&& par){
            buckets_[MyHash(par.first)].emplace_back(std::forward<decltype(par)>(par));
        }

        std::size_t GetSize() const {
            return size_;
        }

        float LoadFactor() {
            return size_ / buckets_.size();
        }

        


        struct Iterator {

            std::pair<const Key, Val>& operator *(){
                return *(liter_);
            }   

            const std::pair<const Key, Val>& operator *() const{
                return *(liter_);
            } 

            typename std::list<std::pair<const Key, Val>>::iterator operator ->(){
                return liter_;
            }

            Iterator& operator=(const Iterator& it)=default;
            Iterator(const Iterator& it) : liter_(it.liter_), bucketnum_(it.bucketnum_), cont_(it.cont_) {}
            Iterator(const typename std::list<std::pair<const Key, Val>>::iterator& liter, std::size_t bucketnum, 
            std::vector<std::list<std::pair<const Key, Val>>>& cont) : liter_(liter), bucketnum_(bucketnum), cont_(cont) {}

            Iterator& operator++() {
                ++liter_;
                if (liter_ == cont_[bucketnum_].end()) {
                    ++bucketnum_;
                    while(bucketnum_ < cont_.size()){
                        if(cont_[bucketnum_].size() > 0){
                            liter_ = cont_[bucketnum_].begin();
                            return *this;
                        }
                        ++bucketnum_;
                    }
                    --bucketnum_;
                    liter_ = cont_[bucketnum_].end();
                }
                return *this;
            }

            Iterator operator++(int) {
                auto old_liter = liter_;
                if (liter_ == cont_[bucketnum_].end()) {
                    ++bucketnum_;
                    while(bucketnum_ < cont_.size()){
                        if(cont_[bucketnum_].size() > 0){
                            liter_ = cont_[bucketnum_].begin();
                            return old_liter;
                        }
                        ++bucketnum_;
                    }
                    --bucketnum_;
                    liter_ = cont_[bucketnum_].end();
                }
                return old_liter;
            }

            bool operator==(const Iterator& other) const {
                return other.liter_ == liter_;
            }

            private:
                typename std::list<std::pair<const Key, Val>>::iterator liter_;
                std::size_t bucketnum_;
                std::vector<std::list<std::pair<const Key, Val>>>& cont_;
        };

        Iterator begin(){
            Iterator(start_, buckets_[start_].begin());
        }

        Iterator end(){
            Iterator(buckets_.size(), buckets_.back().end());
        }

        Iterator Find(const Key& key) const {
            auto hash = MyHash(key);
            for (auto it = buckets_[hash].begin(); it != buckets_[hash].end(); ++it) {
                if (key == (*it).first) {
                    return Iterator(it, hash, buckets_);
                }
            }
            return end();
        }


    private:
        std::vector<std::list<std::pair<const Key, Val>>> buckets_;
        float max_load_factor_;
        std::size_t size_;
        std::size_t start_;

};

int main(){
    HashMap<int, int> d;
    auto it = d.begin();
    auto it1 = d.Find(3);
    std::cout << (it == it1);
}