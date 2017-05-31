#pragma once

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "base/Timestamp.h"

namespace kakui
{

typedef boost::function<void()> TimerCallback;

}