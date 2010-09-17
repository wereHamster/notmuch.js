
#include "query.h"

namespace notmuch {
        
    Persistent<FunctionTemplate> Query::constructor;
    
    Handle<Value> Query::New(Handle<Value> database, notmuch_query_t *query) {
        HandleScope scope;
        
        Local<Object> obj = constructor->GetFunction()->NewInstance();
        (new Query(database, query))->Wrap(obj);
        
        return obj;
    }

    Handle<Value> Query::Threads(const Arguments &args) {
        HandleScope scope;
        Query *query = ObjectWrap::Unwrap<Query>(args.This());

        Local<Function> callback = Local<Function>::Cast(args[0]);
        Local<Array> result = Local<Array>::New(Array::New());

        notmuch_threads_t *threads = notmuch_query_search_threads(query->_query);
        for (int i = 0; notmuch_threads_valid(threads); notmuch_threads_move_to_next(threads), ++i) {
            notmuch_thread_t *thread = notmuch_threads_get(threads);
            result->Set(Number::New(i), Thread::New(args.This(), thread));
        }
        
        Local<Value> argv[1] = { result };
        callback->Call(Context::GetCurrent()->Global(), 1, argv);
        
        return Undefined();
    }
    
    Handle<Value> Query::Messages(const Arguments &args) {
        HandleScope scope;
        Query *query = ObjectWrap::Unwrap<Query>(args.This());
        
        Local<Function> callback = Local<Function>::Cast(args[0]);
        Local<Array> result = Local<Array>::New(Array::New());
        
        notmuch_messages_t *messages = notmuch_query_search_messages(query->_query);
        for (int i = 0; notmuch_messages_valid(messages); notmuch_messages_move_to_next(messages), ++i) {
            notmuch_message_t *message = notmuch_messages_get(messages);
            result->Set(Number::New(i), Message::New(args.This(), message));
        }        
        
        Local<Value> argv[1] = { result };
        callback->Call(Context::GetCurrent()->Global(), 1, argv);
        
        return Undefined();
    }
    
    bool Query::HasInstance(Handle<Value> val) {
        if (!val->IsObject())
            return false;
        
        Local<Object> obj = val->ToObject();
        if (constructor->HasInstance(obj))
            return true;
        
        return false;
    }
    

    Query::Query(Handle<Value> database, notmuch_query_t *query) : ObjectWrap() {
        _database = Persistent<Value>::New(database);
        _query = query;
    }
    
    Query::~Query() {
        notmuch_query_destroy(_query);
        _database.Dispose();
    }
    

    void Query::Initialize(Handle<Object> target) {
        HandleScope scope;
        
        Local<FunctionTemplate> t = FunctionTemplate::New();
        constructor = Persistent<FunctionTemplate>::New(t);
        constructor->InstanceTemplate()->SetInternalFieldCount(1);
        constructor->SetClassName(String::NewSymbol("Query"));
        
        NODE_SET_PROTOTYPE_METHOD(constructor, "threads", Query::Threads);
        NODE_SET_PROTOTYPE_METHOD(constructor, "messages", Query::Messages);
    }
    
    
}
