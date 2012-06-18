#include "sky/timer.h"
#include <iostream>

using namespace sky;
using namespace std;

timer::timer(const char* name) :
    name(name),
    start(time::now())
{}

class time timer::duration() const
{
    return time::now() -= start;
}

timer::~timer()
{
}
