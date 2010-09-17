
#include "message.h"

#define SET(name) do { \
    ret->Set(name, args.This()->Get(name)); \
} while(0);

namespace notmuch {
    
    static Persistent<String> kID;
    static Persistent<String> kThreadID;
    static Persistent<String> kFilename;
    static Persistent<String> kDate;

    static Persistent<String> *attributes[] = {
        &kID, &kThreadID, &kFilename, &kDate
    };
    
    Persistent<FunctionTemplate> Message::constructor;
    
    
    Handle<Value> Message::New(Handle<Value> query, notmuch_message_t *message) {
        HandleScope scope;
        
        Local<Object> obj = constructor->GetFunction()->NewInstance();
        (new Message(query, message))->Wrap(obj);
                
        return scope.Close(obj);
    }
    
    Handle<Value> Message::NamedProperty(Local<String> name, const AccessorInfo &info) {
        HandleScope scope;
        notmuch_message_t *message = ObjectWrap::Unwrap<Message>(info.This())->message();
        
        if (name->Equals(kID)) {
            return String::New(notmuch_message_get_message_id(message));
        } else if (name->Equals(kThreadID)) {
            return String::New(notmuch_message_get_thread_id(message));
        } else if (name->Equals(kFilename)) {
            return String::New(notmuch_message_get_filename(message));
        } else if (name->Equals(kDate)) {
            return Date::New(1000*notmuch_message_get_date(message));
        }
        
        /* Forward lookups to the prototype. */
        assert(info.This()->IsObject());
        Local<Object> prototype = Local<Object>::Cast(info.This()->ToObject()->GetPrototype());

        return prototype->Get(name);
    }
    
    Handle<Value> Message::Header(const Arguments &args) {
        HandleScope scope;
        notmuch_message_t *message = ObjectWrap::Unwrap<Message>(args.This())->message();
        
        String::Utf8Value utf8_value(args[0]);
        std::string std_string = *utf8_value;
        
        Local<String> ret = String::New(notmuch_message_get_header(message, std_string.c_str()));
        return scope.Close(ret);
    }
    
    Handle<Value> Message::toJSON(const Arguments &args) {
        HandleScope scope;
        Local<Object> ret = Object::New();
        
        for (int i = 0; i < sizeof(attributes) / sizeof(attributes[0]); ++i) {
            SET(*attributes[i]);
        }
        
        return scope.Close(ret);
    }
    
    bool Message::HasInstance(Handle<Value> val) {
        if (!val->IsObject())
            return false;
        
        Local<Object> obj = val->ToObject();
        if (constructor->HasInstance(obj))
            return true;
        
        return false;
    }
    
    
    Message::Message(Handle<Value> query, notmuch_message_t *message) : ObjectWrap() {
        _query = Persistent<Value>::New(query);
        _message = message;
    }
    
    Message::~Message() {
        notmuch_message_destroy(_message);
        _query.Dispose();
    }
    
    
    void Message::Initialize(Handle<Object> target) {
        HandleScope scope;
        
        kID = Persistent<String>::New(String::NewSymbol("id"));
        kThreadID = Persistent<String>::New(String::NewSymbol("threadId"));
        kFilename = Persistent<String>::New(String::NewSymbol("filename"));
        kDate = Persistent<String>::New(String::NewSymbol("date"));
        
        Local<FunctionTemplate> t = FunctionTemplate::New();
        constructor = Persistent<FunctionTemplate>::New(t);
        constructor->SetClassName(String::NewSymbol("Message"));
        
        Local<ObjectTemplate> instanceTemplate = constructor->InstanceTemplate();
        instanceTemplate->SetInternalFieldCount(1);
        instanceTemplate->SetNamedPropertyHandler(Message::NamedProperty, NULL);
     
        NODE_SET_PROTOTYPE_METHOD(constructor, "header", Message::Header);
        NODE_SET_PROTOTYPE_METHOD(constructor, "toJSON", Message::toJSON);
    }
    
    
}
