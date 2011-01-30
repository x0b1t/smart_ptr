#include <iostream>
using std::cout;
using std::endl;

#include <gtest/gtest.h>

#include <boost/shared_ptr.hpp>

#include "AutoPtr.h"
#include "SmartPtr.h"
#include "ScopedPtr.h"

#define EXP2STR( exp) \
    std::string( #exp)

struct Local {
    int m_int;
};

struct TestPtr {
    typedef TestPtr This;

    bool& m_removed;

    TestPtr( bool& removed)
        :m_removed( removed)
    {
        m_removed =false;
    }

    ~TestPtr() {
        m_removed =true;
    }

private:
    TestPtr( const This&);
    This& operator =( const This&);
};

TEST( AutoPtrTest, SimpleTest) {
    AutoPtr<int> ptr;

    EXPECT_TRUE( ptr.get() == 0);

    ptr.reset( new int(0) );

    EXPECT_FALSE( ptr.get() == 0);

    AutoPtr<int> ptr_0, ptr_1;
    ptr_0.reset( new int(0) );

    EXPECT_FALSE( ptr_0.get() == 0);

    ptr_1 =ptr_0;

    EXPECT_TRUE( ptr_0.get() == 0 and ptr_1.get() != 0);

    AutoPtr<int> ptr_2( ptr_1);

    EXPECT_TRUE( ptr_1.get() == 0 and ptr_2.get() != 0);

    EXPECT_TRUE( ptr_2.get() == ptr_2.operator ->() );

    EXPECT_DEATH( ptr_1.operator ->(), "");

    AutoPtr<Local> loc_ptr;
    EXPECT_DEATH( loc_ptr.operator ->(), "");

    EXPECT_FALSE( bool( loc_ptr) );
}

TEST( SmartPtrTest, SimpleTest) {
    // TODO:
    SmartPtr<int> ptr_0;
    EXPECT_TRUE( ptr_0.get() == 0);

    SmartPtr<int> ptr_1( new int(0) );
    EXPECT_TRUE( ptr_1.get() );

    SmartPtr<int> ptr_2( ptr_1);
    EXPECT_TRUE( ptr_1.get() == ptr_2.get() );

    ptr_2.reset();
    EXPECT_TRUE( ptr_2.get() == 0);

    int* orig_ptr =new int( 0);
    ptr_2.reset( orig_ptr);
    EXPECT_TRUE( ptr_2.get() == orig_ptr);

    SmartPtr<int> ptr_3( ptr_2);
    EXPECT_TRUE( ptr_2.get() == ptr_3.get() and ptr_2.get() == orig_ptr);

    ptr_3.reset();
    EXPECT_TRUE( ptr_2.get() != ptr_3.get() and ptr_3.get() == 0);
}

TEST( SmartPtrTest, CleanupTest) {
    bool removed =false;

    // 0
    {
        removed =false;
        TestPtr* orig_ptr =new TestPtr( removed);
        SmartPtr<TestPtr> ptr_0( orig_ptr);
        EXPECT_FALSE( removed);
    }
    EXPECT_TRUE( removed);

    // 1
    {
        removed =false;
        TestPtr* orig_ptr =new TestPtr( removed);
        SmartPtr<TestPtr> ptr_0( orig_ptr);
        {
            SmartPtr<TestPtr> ptr_1( ptr_0);
            EXPECT_FALSE( removed);

            EXPECT_TRUE( ptr_0.core() == ptr_1.core() );
        }
        EXPECT_FALSE( removed);
    }
    EXPECT_TRUE( removed);

    // 2
    {
        removed =false;
        TestPtr* orig_ptr =new TestPtr( removed);
        SmartPtr<TestPtr> ptr_0( orig_ptr);
        {
            SmartPtr<TestPtr> ptr_1;
            ptr_1 =ptr_0;
            EXPECT_FALSE( removed);

            EXPECT_TRUE( ptr_0.core() == ptr_1.core() );
        }
        EXPECT_FALSE( removed);
    }
    EXPECT_TRUE( removed);

    EXPECT_TRUE( SmartPtr<TestPtr>::Core::cores().size() == 2);

    // new SmartPtr<TestPtr>(); // error: operator new/delete is private

/*
// debug:
    cout
        <<EXP2STR( SmartPtr<TestPtr>::Core::cores().size() ) <<": " <<SmartPtr<TestPtr>::Core::cores().size() <<endl;
*/
}

TEST( ScopedPtrTest, SimpleTest) {
    bool removed =true;
    {
        TestPtr* orig_ptr =new TestPtr( removed);
        ScopedPtr<TestPtr> ptr_0( orig_ptr);

        EXPECT_TRUE( &(*ptr_0) == orig_ptr);
        EXPECT_TRUE( ptr_0.operator ->() == orig_ptr);
        EXPECT_TRUE( ptr_0.get() == orig_ptr);
        EXPECT_FALSE( removed);
    }
    EXPECT_TRUE( removed);

    EXPECT_DEATH( ScopedPtr<int> ptr_1( 0), "");

    ScopedPtr<int> ptr_2( new int(0) );

    // ScopedPtr<int> ptr_2 =new ScopedPtr<int>( 0); // error: operator new/delete is private
}

TEST( BoostPtrTest, SimpleTest) {
    boost::shared_ptr<int> ptr_0;
    boost::shared_ptr<int> ptr_1( ptr_0);
    boost::shared_ptr<int> ptr_2; ptr_2 =ptr_0;

    ptr_0.reset( new int(0) );

    EXPECT_TRUE( ptr_0.get() == ptr_1.get() and ptr_0.get() == ptr_2.get() );
/*
// debug:
    cout
        <<EXP2STR( ptr_0.get() ) <<": " <<ptr_0.get() <<'\n'
        <<EXP2STR( ptr_1.get() ) <<": " <<ptr_1.get() <<'\n'
        <<EXP2STR( ptr_2.get() ) <<": " <<ptr_2.get() <<endl;
*/
}

TEST( BoostPtrTest, SimpleTest_1) {}

int main( int argc, char* argv[] ) {
    testing::InitGoogleTest( &argc, argv);
    testing::GTEST_FLAG( print_time) =true;

    testing::GTEST_FLAG(filter) ="-BoostPtrTest.*";

	return RUN_ALL_TESTS();
}
