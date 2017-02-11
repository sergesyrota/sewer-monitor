/*
 * TestLin2dSolver.cpp
 *
 *  Created on: Feb 16, 2013
 *      Author: Arvind A de Menezes Pereira
 */

#include <iostream>
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCase.h>

#include "../Arduino/LaserSensing.h"

using namespace std;

class TestLaserSensing : public CppUnit::TestFixture {
private:
	LaserSensing *testCounter;
public:
	static CppUnit::Test *suite() {
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Test averaging valid results");

		suiteOfTests->addTest(new CppUnit::TestCaller<TestLaserSensing>("One initial reading with all invalid",
				&TestLaserSensing::testOneReading ));
		suiteOfTests->addTest(new CppUnit::TestCaller<TestLaserSensing>("All invalid",
				&TestLaserSensing::testAllInvalid ));
		suiteOfTests->addTest(new CppUnit::TestCaller<TestLaserSensing>("Mix of valid and invalid",
				&TestLaserSensing::testMix ));
		suiteOfTests->addTest(new CppUnit::TestCaller<TestLaserSensing>("Random readings",
				&TestLaserSensing::testRandomReadings ));
		suiteOfTests->addTest(new CppUnit::TestCaller<TestLaserSensing>("Same reading filling up the array",
				&TestLaserSensing::testSameReadings ));
		suiteOfTests->addTest(new CppUnit::TestCaller<TestLaserSensing>("Rollover by 1",
				&TestLaserSensing::testRollover ));

		return suiteOfTests;
	}

	/// Setup method
	void setUp() {
		testCounter = new LaserSensing(5000,1);
		// zero-out all data
		for (int i = 0; i < LASER_SENSING_NUMBER_READINGS; i++) {
			testCounter->reading(0);
		}
	}

	/// Teardown method
	void tearDown() {
	}

protected:
        void addReadings(const int size, const int data[]) {
            //int size = sizeof(data) / sizeof(data[0]);
            for (int i=0; i<size; i++) {
                testCounter->reading(data[i]);
            }
        }

	void testOneReading() {
		addReadings(1, (int[]){500});
		CPPUNIT_ASSERT_EQUAL( 500, testCounter->getValue() );
		CPPUNIT_ASSERT_EQUAL( (int)100/LASER_SENSING_NUMBER_READINGS, testCounter->percentValid() );
	}

	void testAllInvalid() {
		addReadings(10, (int[]){-1,-1,-1,-1,-1,-1,-1,-1,-1,-1});
		CPPUNIT_ASSERT_EQUAL( 0, testCounter->getValue() );
		CPPUNIT_ASSERT_EQUAL( 0, testCounter->percentValid() );
	}

	void testMix() {
		addReadings(10, (int[]){-1,-10,300,8096,300,300,20000,300,300});
		CPPUNIT_ASSERT_EQUAL( 300, testCounter->getValue() );
		CPPUNIT_ASSERT_EQUAL( (int)5*100/LASER_SENSING_NUMBER_READINGS, testCounter->percentValid() );
	}

	void testRandomReadings() {
		addReadings(10, (int[]){526,528,536,538,524,539,521,539,522,549});
		CPPUNIT_ASSERT_EQUAL( 532, testCounter->getValue() );
		CPPUNIT_ASSERT_EQUAL( (int)10*100/LASER_SENSING_NUMBER_READINGS, testCounter->percentValid() );
	}

	void testSameReadings() {
		testCounter->reading(500);
		testCounter->reading(500);
		testCounter->reading(500);
		testCounter->reading(500);
		testCounter->reading(500);
		testCounter->reading(500);
		testCounter->reading(500);
		testCounter->reading(500);
		testCounter->reading(500);
		testCounter->reading(500);
		CPPUNIT_ASSERT_EQUAL( 500, testCounter->getValue() );
		CPPUNIT_ASSERT_EQUAL( (int)10*100/LASER_SENSING_NUMBER_READINGS, testCounter->percentValid() );
	}

	void testRollover() {
		testCounter->reading(500);
		for (int i = 0; i < LASER_SENSING_NUMBER_READINGS; i++) {
			testCounter->reading(600);
		}
		CPPUNIT_ASSERT_EQUAL( 600, testCounter->getValue() );
		CPPUNIT_ASSERT_EQUAL( 100, testCounter->percentValid() );
	}

};
