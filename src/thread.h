
#ifndef __NOTMUCH_THREAD__
#define __NOTMUCH_THREAD__

#include "notmuch.h"

namespace notmuch {
    
    class Query;
    class Thread : public ObjectWrap {
    public:
        ~Thread();
        
        static void Initialize(Handle<Object> target);
        static Handle<Value> New(Handle<Value> query, notmuch_thread_t *thread);
        
        static bool HasInstance(Handle<Value> val);
    
        notmuch_thread_t *thread() {
            return _thread;
        }
    private:
        static Persistent<FunctionTemplate> constructor;
        static Handle<Value> New(const Arguments &args);
        static Handle<Value> NamedProperty(Local<String> name, const AccessorInfo &info);

        static Handle<Value> toJSON(const Arguments &args);
        
        Thread(Handle<Value> query, notmuch_thread_t *thread);

        Persistent<Value> _query;
        notmuch_thread_t *_thread;
    };
    
}

#endif  /* __NOTMUCH_THREAD__ */
