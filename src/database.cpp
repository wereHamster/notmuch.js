
#include "notmuch.h"

#define ARG(n, type) ({                                                             \
    if (args.Length() <= n || !args[n]->Is##type())                                \
        return ThrowException(Exception::TypeError(String::New("Bad arguments")));  \
    args[n]->To##type(); })

#define FUN(n) ({                                                             \
    if (args.Length() <= n || !args[n]->IsFunction())                                \
    return ThrowException(Exception::TypeError(String::New("Bad arguments")));  \
    Local<Function>::Cast(args[n]); })

namespace notmuch {

    Persistent<FunctionTemplate> Database::constructor;
    
    Handle<Value> Database::New(notmuch_database_t *database) {
        HandleScope scope;
        
        Local<Object> obj = constructor->GetFunction()->NewInstance();
        (new Database(database))->Wrap(obj);
        
        return obj;
    }
    
    
    Handle<Value> Database::Find(const Arguments &args) {
        HandleScope scope;
        Database *database = ObjectWrap::Unwrap<Database>(args.This());
        
        Local<String> id = ARG(0, String);
        Local<Function> callback = FUN(1);
        
        String::Utf8Value utf8_value(id);
        std::string std_string = *utf8_value;
        
        notmuch_message_t *message;
        message = notmuch_database_find_message(database->database(), std_string.c_str());

        Handle<Value> result;
        if (message) {
            result = Message::New(Undefined(), message);
        } else {
            result = Null();
        }
        Handle<Value> argv[1] = { result };
        callback->Call(Context::GetCurrent()->Global(), 1, argv);
        
        return Undefined();
    }
    
    Handle<Value> Database::Query(const Arguments &args) {
        HandleScope scope;
        Database *database = ObjectWrap::Unwrap<Database>(args.This());
        
        Local<String> query_string = ARG(0, String);
        
        String::Utf8Value utf8_value(query_string);
        std::string std_string = *utf8_value;
        
        return Query::New(args.This(), notmuch_query_create(database->_database, std_string.c_str()));
    }
    
    bool Database::HasInstance(Handle<Value> val) {
        if (!val->IsObject())
            return false;
    
        Local<Object> obj = val->ToObject();
        if (constructor->HasInstance(obj))
            return true;
        
        return false;
    }
    
    
    Database::Database(notmuch_database_t *database) : ObjectWrap() {
        _database = database;
    }
    
    Database::~Database() {
        notmuch_database_close(_database);
    }
    
    
    void Database::Initialize(Handle<Object> target) {
        HandleScope scope;
                
        Local<FunctionTemplate> t = FunctionTemplate::New();
        constructor = Persistent<FunctionTemplate>::New(t);
        constructor->InstanceTemplate()->SetInternalFieldCount(1);
        constructor->SetClassName(String::NewSymbol("Database"));

        NODE_SET_PROTOTYPE_METHOD(constructor, "find", Database::Find);
        NODE_SET_PROTOTYPE_METHOD(constructor, "query", Database::Query);
    }
    
    
}
