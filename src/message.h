
#ifndef __NOTMUCH_MESSAGE__
#define __NOTMUCH_MESSAGE__

#include "notmuch.h"

namespace notmuch {
    
    class Message : public ObjectWrap {
    public:
        ~Message();
        
        static void Initialize(Handle<Object> target);
        static Handle<Value> New(Handle<Value> query, notmuch_message_t *message);
        
        static bool HasInstance(Handle<Value> val);
        
        notmuch_message_t *message() {
            return _message;
        }
    private:
        static Persistent<FunctionTemplate> constructor;
        static Handle<Value> NamedProperty(Local<String> name, const AccessorInfo &info);
        static Handle<Value> toJSON(const Arguments &args);

        static Handle<Value> Header(const Arguments &args);
        
        Message(Handle<Value> query, notmuch_message_t *message);
        
        Persistent<Value> _query;
        notmuch_message_t *_message;
    };
    
}

#endif  /* __NOTMUCH_MESSAGE__ */
