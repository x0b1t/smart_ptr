#ifndef AUTOPTR_H_
#define AUTOPTR_H_

#include <cstddef>

template<typename T>
class AutoPtr {
/*types*/
    typedef AutoPtr This;

public:
    typedef T Value;
    typedef Value* Ptr;
    typedef Value& Ref;

/*mems*/
private:
    Value* m_value;

/*(c,d)tor*/
public:
    AutoPtr( Value* value_ptr =0)
        :m_value(value_ptr)
    {}

    AutoPtr( const This& copy)
        :m_value(0)
    {
        m_value =const_cast<This&>( copy).release();
    }

    ~AutoPtr() {
        reset();
    }

/*funcs*/
public:
    Value* get() const { return m_value; }

    void reset() {
        if( m_value) { delete m_value; m_value =0; }
    }

    void reset( Value* value_ptr) {
        reset();
        m_value =value_ptr;
    }

    Value* release() {
        Value* value_tmp =m_value;
        m_value =0;
        return value_tmp;
    }

    This& operator =( const This& copy) {
        m_value =const_cast<This&>( copy).release();
        return *this;
    }

    Value& operator *() const {
        assert( m_value);
        return *m_value;
    }

    Value* operator ->() {
        assert( m_value);
        return m_value;
    }

    operator bool() {
        return ( m_value != 0);
    }

private:
    void* operator new( size_t);
    void operator delete( void*);
};

#endif /* AUTOPTR_H_ */
