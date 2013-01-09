#include "sky/timer.h"

using namespace sky;

timer::timer() :
    start(clock::now())
{}

timer::duration timer::split() const
{
    return std::chrono::duration_cast<duration>(clock::now() - start);
}
