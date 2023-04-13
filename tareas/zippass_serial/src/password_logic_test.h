#ifndef PASSWORD_LOGIC_TEST_H
#define PASSWORD_LOGIC_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <zip.h>
#include <assert.h>

#include "password_logic.h"

int generatesNewPasswordCorrectlyChangingFirstNumber_Test() ;

int generatesNewPasswordCorrectlyAddingNumber_Test() ;

void interpretsCountersCorrectly();

int decryptsSimplePasswords_Test() ;


#endif