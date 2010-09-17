
#include "notmuch.h"
#include <iostream>

static Handle<Value> open(const Arguments &args) {
    String::Utf8Value utf8_value(args[0]);
    std::string std_string = *utf8_value;
    
    notmuch_database_t *database;
    database = notmuch_database_open(std_string.c_str(), NOTMUCH_DATABASE_MODE_READ_ONLY);
    
    return notmuch::Database::New(database);
}

// Extracts a C string from a V8 Utf8Value.
const char* ToCString(const String::Utf8Value& value) {
    return *value ? *value : "<string conversion failed>";
}

static void init (Handle<Object> target) {
    notmuch::Database::Initialize(target);
    notmuch::Query::Initialize(target);
    notmuch::Thread::Initialize(target);
    notmuch::Message::Initialize(target);

    Local<FunctionTemplate> t = FunctionTemplate::New(open);
    target->Set(String::New("open"), t->GetFunction());
}
    
NODE_MODULE(notmuch, init);
