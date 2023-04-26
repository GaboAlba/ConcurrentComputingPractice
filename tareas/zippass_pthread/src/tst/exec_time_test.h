#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <zip.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

#include "password_logic.h"
#include "zip_operations.h"

void passwordGenTimer();

void decryptTesterTimerWrong();

void decryptTesterTimerRight();

void canFileBeOpenedMultipleTimes();