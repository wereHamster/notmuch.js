
#ifndef __NOTMUCH_QUERY__
#define __NOTMUCH_QUERY__

#include "notmuch.h"

namespace notmuch {
    
    class Query : public ObjectWrap {
    public:
        ~Query();
        
        static void Initialize(Handle<Object> target);
        static Handle<Value> New(Handle<Value> database, notmuch_query_t *query);
        
        static bool HasInstance(Handle<Value> val);
        
        notmuch_query_t *query() {
            return _query;
        }
    private:
        static Persistent<FunctionTemplate> constructor;

        static Handle<Value> Threads(const Arguments &args);
        static Handle<Value> Messages(const Arguments &args);

        Query(Handle<Value> database, notmuch_query_t *query);
        
        Persistent<Value> _database;
        notmuch_query_t *_query;
    };
    
}

#endif  /* __NOTMUCH_QUERY__ */
