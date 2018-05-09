#ifndef CUSTOM_ITERATOR_H
#define CUSTOM_ITERATOR_H

template<bool flag,class IsTrue,class IsFalse>
struct choose;

template<class IsTrue,class IsFalse>
struct choose<true,IsTrue,IsFalse> {
        typedef IsTrue type;
};

template<class IsTrue,class IsFalse>
struct choose<false,IsTrue,IsFalse> {
        typedef IsFalse type;
};

template<class T,bool isconst = false>
class custom_iterator {
public:
        typedef std::forward_iterator_tag                  iterator_category;
        typedef T                                          value_type;
        typedef std::ptrdiff_t                             difference_type;
        typedef typename choose<isconst,const T&,T&>::type reference;
        typedef typename choose<isconst,const T*,T*>::type pointer;
};

#endif // CUSTOM_ITERATOR_H
