
#ifndef __NOTMUCH__
#define __NOTMUCH__

#include <iostream>

#include <v8.h>
#include <node.h>

using namespace v8;
using namespace node;

#include <notmuch.h>

#include <string>

#include "database.h"
#include "query.h"
#include "thread.h"
#include "message.h"
#include "empty.h"

#define TRACE std::cout << __func__ << std::endl;

#endif /* __NOTMUCH__ */
