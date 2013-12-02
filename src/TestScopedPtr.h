#ifndef TESTSCOPEDPTR_H_
#define TESTSCOPEDPTR_H_

#include "ScopedPtr.h"

#include <boost/scoped_ptr.hpp>

// struct UnknownType; // error: uncomplete type
struct UnknownType {};

struct TestScopedPtr {
    ScopedPtr<UnknownType> m_ptr;

    TestScopedPtr()
        :m_ptr( 0)
    {}
};

#endif /* TESTSCOPEDPTR_H_ */
