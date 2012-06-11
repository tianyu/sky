#include "sky/timer.h"
#include <iostream>

using namespace sky;
using namespace perf;
using namespace std;

timer::timer(const char* name) :
    name(name),
    start(time::now())
{}

timer::~timer()
{
    long millis = (time::now() -= start).millis();
    cout << name << ": "
         << millis
         << "ms" << endl;
}
