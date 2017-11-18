#pragma once
#include <node.h>

namespace DiscordRPC {
    using v8::Function;
    using v8::Isolate;
    using v8::Local;
    

    typedef struct HandlerIsolatePair {
        Local<Function> func;
        Isolate* isolate;
    } HandlerIsolatePair;

    typedef struct Handlers {
        HandlerIsolatePair ready;
        HandlerIsolatePair disconnected;
        HandlerIsolatePair errored;
        HandlerIsolatePair joinGame;
        HandlerIsolatePair spectateGame;
        HandlerIsolatePair joinRequest;
    } Handlers;
}