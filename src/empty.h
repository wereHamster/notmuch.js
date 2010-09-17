
#ifndef __NOTMUCH_EMPTY__
#define __NOTMUCH_EMPTY__

#include "notmuch.h"

namespace notmuch {
    
    class Empty : public ObjectWrap {
    public:
        ~Empty();
        
        static void Initialize(Handle<Object> target);
        static Handle<Value> New(void);
    private:
        static Persistent<FunctionTemplate> constructor;
        
        Empty(void);
    };
    
}

#endif  /* __NOTMUCH_EMPTY__ */
