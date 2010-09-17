
#ifndef __NOTMUCH_DATABASE__
#define __NOTMUCH_DATABASE__

#include "notmuch.h"

namespace notmuch {
    
    class Database : public ObjectWrap {
    public:
        ~Database();
        
        static void Initialize(Handle<Object> target);
        static Handle<Value> New(notmuch_database_t *database);

        static bool HasInstance(Handle<Value> val);
        
        notmuch_database_t *database() {
            return _database;
        }
    private:
        static Persistent<FunctionTemplate> constructor;

        static Handle<Value> Find(const Arguments &args);
        static Handle<Value> Query(const Arguments &args);
        
        Database(notmuch_database_t *database);
        notmuch_database_t *_database;
    };
    
}

#endif  /* __NOTMUCH_DATABASE__ */
