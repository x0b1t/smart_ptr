#ifndef SMARTPTR_H_
#define SMARTPTR_H_

#include <vector>
#include <algorithm>
#include <cassert>
#include <cstddef>

template<typename T>
struct CoreImpl;

template<typename T>
struct SmartPtr {
    typedef SmartPtr This;

    typedef CoreImpl<T> Core;
    typedef Core* CorePtr;
    typedef typename Core::ValuePtr ValuePtr;
    typedef typename Core::ValueRef ValueRef;

    CorePtr m_core;

    SmartPtr( ValuePtr value =0)
        :m_core( Core::create() )
    {
        assert( m_core);
        m_core->reset( value);
        m_core->refinc();
    }

    SmartPtr( const This& copy)
        :m_core(0)
    {
        if( bool( copy) ) { m_core =copy.m_core; }
        else { m_core =Core::create(); }

        assert( m_core);
        m_core->refinc();
    }

    ~SmartPtr() {
        assert( m_core);
        Core::remove( m_core);
        m_core =0;
    }

    inline
    ValuePtr get() {
        assert( m_core);
        return m_core->get();
    }

    void reset() {
        Core::remove( m_core);
        m_core =Core::create();
        m_core->refinc();
    }

    void reset( ValuePtr value_ptr) {
        Core::remove( m_core);
        m_core =Core::create();
        m_core->reset( value_ptr);
        m_core->refinc();
    }

    This& operator =( const This& copy) {
        if( this == &copy) { return *this; }

        assert( copy.m_core and m_core);

        Core::remove( m_core);

        if( bool( copy) ) { m_core =copy.m_core; }
        else { m_core =Core::create(); }

        assert( m_core);
        m_core->refinc();

        return *this;
    }

    ValuePtr operator ->() { return get(); }

    ValueRef operator *() { assert( get() ); return *get(); }

    operator bool() const {
        assert( m_core);
        return ( m_core->get() != 0);
    }

private:
    void* operator new( size_t);
    void operator delete( void*);

/*debug*/
public:
    Core* core() const { return m_core; }
};

// TODO: add thread-policy
template<typename T>
struct CoreImpl {
    typedef CoreImpl This;

    typedef T Value;
    typedef Value* ValuePtr;
    typedef Value& ValueRef;

    typedef std::vector<This*> CoreColl;
    typedef typename CoreColl::iterator CoreIter;

    // TODO: use boost::mutex
    // dummy thread-policy
    typedef struct LockableImpl {} Lockable;
    typedef struct LockImpl { LockImpl( Lockable&) {} } Lock;

    ValuePtr m_value;
    int m_refcount;

    CoreImpl()
        :m_value(0), m_refcount(0)
    {}

    ~CoreImpl() {
        assert( m_value == 0 and refcount() == 0);
    }

    ValuePtr get() { return m_value; }

    void reset( Value* value_ptr) {
        free();
        m_value =value_ptr;
    }

    void free() {
        delete m_value; m_value =0;
        m_refcount =0;
    }

    void try_free() {
        assert( refcount() >= 0);
        if( refcount() == 0) return;

        refdec();
        if( refcount() == 0) { free(); }
    }

    inline
    int refinc() { ++m_refcount; return m_refcount; }

    inline
    int refdec() { assert( m_refcount > 0); --m_refcount; return m_refcount; }

    inline
    int refcount() const { return m_refcount; }

/*static*/
    static CoreColl s_cores;

    static struct {
        bool operator ()( const This* ptr) { assert( ptr); return ( ptr->m_refcount == 0); }
    } _findEmptyCore;

    static This* create() {
        This* new_ptr =0;

        CoreIter i_find =std::find_if( s_cores.begin(), s_cores.end(), _findEmptyCore);
        if( i_find == s_cores.end() ) {
            new_ptr =new This();
            s_cores.push_back( new_ptr);
        } else {
            new_ptr =(*i_find);
        }

        return new_ptr;
    }

    static void remove( This* ptr) {
        assert( ptr);
        CoreIter i_find =std::find( s_cores.begin(), s_cores.end(), ptr);
        if( i_find != s_cores.end() ) {
            // cleanup
            ptr->try_free();
        }
    }

/*debug*/
public:
    static const CoreColl& cores() { return s_cores; }
};

/*static*/
template<typename T>
typename CoreImpl<T>::CoreColl CoreImpl<T>::s_cores;

#endif /* SMARTPTR_H_ */
