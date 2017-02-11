#include "unity.h"

#include "robo_train.h"

#ifndef UNIT_TEST
#include <Arduino.h>

/**
 * This function will be run before EACH test.
 */
// void setUp() {
//     /* code */
// }

/**
 * This function will be run after EACH test.
 */
// void tearDown() {
//
// }


/* Define tests */

void test_helloWorld(void) {
    TEST_ASSERT_TRUE(thisIsTrue());
}


/* Run tests */

int main(int argc, char **argv) {

// void setup() {

    UNITY_BEGIN();
    RUN_TEST(test_helloWorld);
    UNITY_END();

// }

// void loop() {
//
// }

   return 0;

}

#endif
