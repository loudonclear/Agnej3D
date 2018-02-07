#ifndef TYPEMAP_H
#define TYPEMAP_H

#include <unordered_map>
#include <atomic>

//gpfault

template <class ValueType>
class TypeMap {
      typedef std::unordered_map<int, ValueType> InternalMap; // possibly use unordered multimap
public:
    typedef typename InternalMap::iterator iterator;
    typedef typename InternalMap::const_iterator const_iterator;
    typedef typename InternalMap::value_type value_type;

    const_iterator begin() const { return m_map.begin(); }
    const_iterator end() const { return m_map.end();  }
    iterator begin() { return m_map.begin();  }
    iterator end() { return m_map.end(); }

    template <class Key>
    iterator find() { return m_map.find(getTypeId<Key>());  }

    template <class Key>
    void remove() { m_map.erase(getTypeId<Key>()); }


    template <class Key>
    const_iterator find() const { return m_map.find(getTypeId<Key>()); }

    template <class Key>
    void put(ValueType &&value) {
        m_map[getTypeId<Key>()] = std::forward<ValueType>(value);
    }

private:
    template <class Key>
    inline static int getTypeId() {
        static const int id = LastTypeId++;
        return id;
    }

    static std::atomic_int LastTypeId;
    InternalMap m_map;
};

template <class ValueType>
std::atomic_int TypeMap<ValueType>::LastTypeId(0);

#endif // TYPEMAP_H
