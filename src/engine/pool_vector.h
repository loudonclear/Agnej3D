#ifndef POOL_VECTOR_H
#define POOL_VECTOR_H

#include <cstdlib>
#include <vector>
#include <tuple>

#include "custom_iterator.h"

template<typename T>
class pool_vector {
private:
    template<bool isconst = false>
    class _iterator : public custom_iterator<T,isconst> {
        friend class pool_vector;

    private:
        typedef typename choose<isconst,const pool_vector<T>*,pool_vector<T>*>::type container_ptr;

        container_ptr _container;
        std::size_t _index;

        _iterator(container_ptr container,std::size_t index,bool isAlreadyValid) : _container(container),_index(index) {
            if(!isAlreadyValid) _moveToNextValidIndex();
        }

        void _moveToNextValidIndex() {
            while(*this != _container->end()) {
                _DataEntry& targetDataEntry = reinterpret_cast<_DataEntry*>(_container->_data)[_index];
                if(targetDataEntry.isSlotOccupied) break;
                _index++;
            }
        }

    public:
        _iterator() : _container(nullptr),_index(0) {}
        _iterator(const _iterator& other) : _container(other._container),_index(other._index) {}
        _iterator(const custom_iterator<T,false>& other) : _container(other._container),_index(other._index) {}
        _iterator(_iterator&& other) : _container(other._container),_index(other._index) { other._container = nullptr; other._index = 0; }
        _iterator(custom_iterator<T,false>&& other) : _container(other._container),_index(other._index) { other._container = nullptr; other._index = 0; }

        _iterator& operator=(const _iterator& l) { _container = l._container; _index = l._index; return *this; }
        _iterator& operator=(_iterator&& l) {
            _container = l._container;
            _index = l._index;
            l._container = nullptr;
            l._index = 0;
            return *this;
        }

        typename custom_iterator<T, isconst>::reference operator*() const { return reinterpret_cast<typename custom_iterator<T, isconst>::reference>(reinterpret_cast<_DataEntry*>(_container->_data)[_index].item); }
        typename custom_iterator<T, isconst>::pointer operator->() const { return reinterpret_cast<typename custom_iterator<T, isconst>::pointer>(&(reinterpret_cast<_DataEntry*>(_container->_data)[_index].item)); }

        _iterator& operator++() {
            _index++;
            _moveToNextValidIndex();
            return *this;
        }

        _iterator& operator++(int) {
            _index++;
            _moveToNextValidIndex();
            return *this;
        }

        operator bool() const {
            return _container != nullptr && *this != _container->end();
        }

        friend bool operator==(const _iterator &l,
                               const _iterator &r) {
            return (l._container == r._container) && (l._index == r._index);
        }

        friend bool operator!=(const _iterator &l,
                               const _iterator &r) {
            return !(l==r);
        }
    };

public:
    typedef _iterator<false> iterator;
    typedef _iterator<true>  const_iterator;

private:
    struct _DataEntry {
        bool isSlotOccupied;
        T item;
    };

    void* _data;
    std::size_t _capacity;
    std::size_t _numFilledSlots;
    std::vector<std::size_t> _freeSlots;

    inline std::size_t _getNextAutoGrowCapacity() const {
        return std::size_t(float(_capacity+1) * 1.5f);
    }

public:
    pool_vector() {
        _data = nullptr;
        _capacity = 0;
        _numFilledSlots = 0;
        _freeSlots.clear();
    }

    pool_vector(const pool_vector &other) {
        *this = other;
    }

    pool_vector(pool_vector &&other) {
        *this = std::forward<pool_vector>(other);
    }

    pool_vector& operator=(const pool_vector &l) {
        _freeSlots = l._freeSlots;
        _numFilledSlots = l._numFilledSlots;
        _capacity = l._capacity;

        _data = malloc(sizeof(_DataEntry)*_capacity);

        for(std::size_t i = 0; i < _capacity; i++) {
            _DataEntry& newEntry = reinterpret_cast<_DataEntry*>(_data)[i];
            _DataEntry& otherEntry = reinterpret_cast<_DataEntry*>(l._data)[i];

            if(otherEntry.isSlotOccupied) {
                newEntry.isSlotOccupied = true;
                new((void*)&newEntry.item) T(otherEntry.item);
            } else {
                newEntry.isSlotOccupied = false;
            }
        }

        return *this;
    }

    pool_vector& operator=(pool_vector &&l) {
        _freeSlots = l._freeSlots;
        _numFilledSlots = l._numFilledSlots;
        _capacity = l._capacity;

        if(_data != nullptr) free(_data);
        _data = l._data;

        l._data = nullptr;
        l._capacity = 0;
        l._numFilledSlots = 0;
        l._freeSlots.clear();

        return *this;
    }

    inline const_iterator begin() const {
        return const_iterator(this,0,false);
    }
    inline iterator begin() {
        return iterator(this,0,false);
    }

    inline const_iterator end() const {
        return const_iterator(this,_capacity,true);
    }
    inline iterator end() {
        return iterator(this,_capacity,true);
    }

    inline iterator insert(const T &item) {
        if(_freeSlots.size() == 0) {
            reserve(_getNextAutoGrowCapacity());
        }

        const auto targetSlot = _freeSlots.back();
        _freeSlots.pop_back();

        _DataEntry& targetDataEntry = reinterpret_cast<_DataEntry*>(_data)[targetSlot];
        assert(!targetDataEntry.isSlotOccupied);
        new((void*)&targetDataEntry.item) T(item);
        targetDataEntry.isSlotOccupied = true;

        _numFilledSlots++;

        return iterator(this,targetSlot,true);
    }

    inline iterator insert(T&& item) {
        if(_freeSlots.size() == 0) {
            reserve(_getNextAutoGrowCapacity());
        }

        const auto targetSlot = _freeSlots.back();
        _freeSlots.pop_back();

        _DataEntry& targetDataEntry = reinterpret_cast<_DataEntry*>(_data)[targetSlot];
        assert(!targetDataEntry.isSlotOccupied);
        new((void*)&targetDataEntry.item) T(std::forward<T>(item));
        targetDataEntry.isSlotOccupied = true;

        _numFilledSlots++;

        return iterator(this,targetSlot,true);
    }

    template<typename ...Args>
    inline iterator emplace(Args&& ...args) {
        if(_freeSlots.size() == 0) {
            reserve(_getNextAutoGrowCapacity());
        }

        const auto targetSlot = _freeSlots.back();
        _freeSlots.pop_back();

        _DataEntry& targetDataEntry = reinterpret_cast<_DataEntry*>(_data)[targetSlot];
        assert(!targetDataEntry.isSlotOccupied);
        new((void*)&targetDataEntry.item) T(std::forward<Args>(args)...);
        targetDataEntry.isSlotOccupied = true;

        _numFilledSlots++;

        return iterator(this,targetSlot,true);
    }

    inline void remove(const iterator &it) {
        const auto& targetSlot = it._index;
        assert(targetSlot>=0 && targetSlot<_capacity);
        _DataEntry& targetDataEntry = reinterpret_cast<_DataEntry*>(_data)[targetSlot];
        assert(targetDataEntry.isSlotOccupied);

        targetDataEntry.item.~T();

        targetDataEntry.isSlotOccupied = false;

        _numFilledSlots--;
        _freeSlots.push_back(targetSlot);
    }

    inline void remove(const T &item) {
        for(auto it = begin(); it != end(); it++) {
            if(item == *it) {
                remove(it);
                return;
            }
        }
        assert(false);
    }

    inline void clear() {
        for(std::size_t i = 0; i < _capacity; i++) {
            _DataEntry& targetDataEntry = reinterpret_cast<_DataEntry*>(_data)[i];
            if(targetDataEntry.isSlotOccupied) {
                targetDataEntry.item.~T();
            }
        }
        free(_data);
        _data = nullptr;
        _capacity = 0;
        _numFilledSlots = 0;
        _freeSlots.clear();
    }

    inline void reserve(std::size_t capacity) {
        if(capacity > _capacity) {
            // _freeSlots will not be used, and will in fact be regenerated by the algorithm below
            _freeSlots.clear();
            _numFilledSlots = 0;

            _freeSlots.reserve(capacity);
            void* newData = malloc(sizeof(_DataEntry)*capacity);

            // copy (move if possible) all alive items from the old data block to the new one
            // also initialize the rest of the data along the way
            for(std::size_t i = 0; i < capacity; i++) {
                _DataEntry& newEntry = reinterpret_cast<_DataEntry*>(newData)[i];
                if(i < _capacity) {
                    _DataEntry& oldEntry = reinterpret_cast<_DataEntry*>(_data)[i];
                    if(oldEntry.isSlotOccupied) {
                        newEntry.isSlotOccupied = true;
                        new((void*)&newEntry.item) T(std::move(oldEntry.item));
                        oldEntry.item.~T();
                    } else {
                        newEntry.isSlotOccupied = false;
                    }
                } else {
                    newEntry.isSlotOccupied = false;
                }

                if(!newEntry.isSlotOccupied)
                    _freeSlots.push_back(i);
                else
                    _numFilledSlots++;
            }

            free(_data);
            _data = newData;
            _capacity = capacity;
        }
    }

    inline std::size_t capacity() const {
        return _capacity;
    }

    inline std::size_t size() const {
        return _numFilledSlots;
    }
};

#endif // POOL_VECTOR_H
