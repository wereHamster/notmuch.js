
#include "notmuch.h"

#define SET(name) do { \
    ret->Set(name, args.This()->Get(name)); \
} while(0);


namespace notmuch {
    
    static Persistent<String> kID;
    static Persistent<String> kSubject;
    static Persistent<String> kNumMessages;
    static Persistent<String> kNumMatchedMessages;
    static Persistent<String> kOldestDate;
    static Persistent<String> kNewestDate;

    static Persistent<String> *attributes[] = {
        &kID, &kSubject, &kNumMessages, &kNumMatchedMessages, &kOldestDate, &kNewestDate
    };

    Persistent<FunctionTemplate> Thread::constructor;
    
    
    Handle<Value> Thread::New(Handle<Value> query, notmuch_thread_t *thread) {
        HandleScope scope;
        
        Local<Object> obj = constructor->GetFunction()->NewInstance();
        (new Thread(query, thread))->Wrap(obj);
        
        return obj;
    }
    
    Handle<Value> Thread::NamedProperty(Local<String> name, const AccessorInfo &info) {
        HandleScope scope;
        notmuch_thread_t *thread = ObjectWrap::Unwrap<Thread>(info.This())->thread();

        if (name->Equals(kID)) {
            return String::New(notmuch_thread_get_thread_id(thread));
        } else if (name->Equals(kSubject)) {
            return String::New(notmuch_thread_get_subject(thread));
        } else if (name->Equals(kNumMessages)) {
            return Integer::New(notmuch_thread_get_total_messages(thread));
        } else if (name->Equals(kNumMatchedMessages)) {
            return Integer::New(notmuch_thread_get_matched_messages(thread));
        } else if (name->Equals(kOldestDate)) {
            return Date::New(1000*notmuch_thread_get_oldest_date(thread));
        } else if (name->Equals(kNewestDate)) {
            return Date::New(1000*notmuch_thread_get_newest_date(thread));
        }
    
        /* Forward lookups to the prototype. */
        assert(info.This()->IsObject());
        Local<Object> prototype = Local<Object>::Cast(info.This()->ToObject()->GetPrototype());
        
        return prototype->Get(name);
    }
    
    Handle<Value> Thread::toJSON(const Arguments &args) {
        HandleScope scope;
        Local<Object> ret = Object::New();
        
        for (int i = 0; i < sizeof(attributes) / sizeof(attributes[0]); ++i) {
            SET(*attributes[i]);
        }
        
        return ret;
    }
    
    bool Thread::HasInstance(Handle<Value> val) {
        if (!val->IsObject())
            return false;
        
        Local<Object> obj = val->ToObject();
        if (constructor->HasInstance(obj))
            return true;
        
        return false;
    }
    
    
    Thread::Thread(Handle<Value> query, notmuch_thread_t *thread) : ObjectWrap() {
        _query = Persistent<Value>::New(query);
        _thread = thread;
    }
    
    Thread::~Thread() {
        notmuch_thread_destroy(_thread);
        _query.Dispose();
    }
    
    
    void Thread::Initialize(Handle<Object> target) {
        HandleScope scope;
    
        kID = Persistent<String>::New(String::NewSymbol("id"));
        kSubject = Persistent<String>::New(String::NewSymbol("subject"));
        kNumMessages = Persistent<String>::New(String::NewSymbol("numMessages"));
        kNumMatchedMessages = Persistent<String>::New(String::NewSymbol("numMatchedMessages"));
        kOldestDate = Persistent<String>::New(String::NewSymbol("oldestDate"));
        kNewestDate = Persistent<String>::New(String::NewSymbol("newestDate"));

        Local<FunctionTemplate> t = FunctionTemplate::New();
        constructor = Persistent<FunctionTemplate>::New(t);
        constructor->SetClassName(String::NewSymbol("Thread"));
        
        Local<ObjectTemplate> instanceTemplate = constructor->InstanceTemplate();
        instanceTemplate->SetInternalFieldCount(1);
        instanceTemplate->SetNamedPropertyHandler(Thread::NamedProperty, NULL);
        
        NODE_SET_PROTOTYPE_METHOD(constructor, "toJSON", Thread::toJSON);
    }
    
}
