
#include "notmuch.h"
#include <iostream>

namespace notmuch {
    
    Persistent<FunctionTemplate> Empty::constructor;
    
    Handle<Value> Empty::New() {
        HandleScope scope;
        
        Local<Object> obj = constructor->GetFunction()->NewInstance();
        (new Empty())->Wrap(obj);
        
        return scope.Close(obj);
    }
    
    Empty::Empty() : ObjectWrap() {
        std::cout << "ctor" << std::endl;
    }
    
    Empty::~Empty() {
        std::cout << "dtor" << std::endl;
    }
    
    
    void Empty::Initialize(Handle<Object> target) {
        HandleScope scope;
        
        Local<FunctionTemplate> t = FunctionTemplate::New();
        constructor = Persistent<FunctionTemplate>::New(t);
        constructor->InstanceTemplate()->SetInternalFieldCount(1);
        constructor->SetClassName(String::NewSymbol("Empty"));
    }
    
    
}
