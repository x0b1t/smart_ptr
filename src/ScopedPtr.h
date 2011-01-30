#ifndef SCOPEDPTR_H_
#define SCOPEDPTR_H_

#include <cassert>
#include <cstddef>

template<typename T>
class ScopedPtr {
/*type*/
    typedef ScopedPtr This;

public:
    typedef T Value;

private:
    Value* m_value;

public:
    ScopedPtr( Value* value)
        :m_value( value)
    {
        assert( m_value);
    }

    ~ScopedPtr() {
        // FIXME: make it correct (more) (delete uncomplete type)
        // static char _bytes[ sizeof(Value) - 1]; // error: overflow in array dimension
        static char _bytes[ sizeof(Value) ?0 :-1]; // error: creating array with negative size (‘-0x000000001’)
        static struct {
            inline
            void operator ()( char* unused) {}
        } _unused; _unused( _bytes);

        delete m_value;
    }

    inline
    Value* get() { return m_value; }

    inline
    Value& operator *() { return *m_value; }

    inline
    Value* operator ->() { return m_value; }

private:
    ScopedPtr( const This&);
    This& operator =( const This&);
    void* operator new( size_t);
    void operator delete( void*);
};

#endif /* SCOPEDPTR_H_ */
