// rpc.cc
#include <node.h>
#include <map>

#include "discord-rpc.h"
#include "rpc.h"

namespace DiscordRPC
{

using node::AtExit;
using namespace v8;
// using v8::Function;
// using v8::FunctionCallbackInfo;
// using v8::FunctionTemplate;
// using v8::Int32;
// using v8::Isolate;
// using v8::Local;
// using v8::Null;
// using v8::Object;
// using v8::Persistent;
// using v8::String;
// using v8::Value;

Isolate *iso;
Persistent<Function> callback;
bool callbackSet = false;

void Print(const char *text)
{
  printf("\x1b[35m[DISCORD-RPC]\x1b[0m %s\n", text);
}

void SendCallback(const char *key, Local<Object> value)
{
  if (callbackSet == false)
  {
    Print("Whoops! You didn't set a callback. Please do so.");
  }
  else
  {
    Print("Sending callback...");
    Isolate *isolate = iso;
    const unsigned argc = 2;
    Local<Value> argv[argc] = {String::NewFromUtf8(isolate, key), value};
    Local<Function> f = Local<Function>::New(iso, callback);
    f->Call(Null(isolate), argc, argv);
    callback.Reset(isolate, f);
  }
}
void SendCallback(const char *key)
{
  if (callbackSet == false)
  {
    Print("Whoops! You didn't set a callback. Please do so.");
  }
  else
  {
    Print("Sending callback...");
    Isolate *isolate = iso;
    const unsigned argc = 1;
    Local<Value> argv[argc] = {String::NewFromUtf8(isolate, key)};
    Local<Function> f = Local<Function>::New(iso, callback);
    f->Call(Null(isolate), argc, argv);
    callback.Reset(isolate, f);
  }
}

static void handleDiscordReady()
{
  Print("Discord: Ready");
  Local<Object> obj = Object::New(iso);
  SendCallback("ready", obj);
}

static void handleDiscordDisconnected(int errcode, const char *message)
{
  Print("Discord: Disconnected");
  Local<Object> obj = Object::New(iso);
  obj->Set(String::NewFromUtf8(iso, "errcode"), Int32::New(iso, errcode));
  obj->Set(String::NewFromUtf8(iso, "message"), String::NewFromUtf8(iso, message));
  SendCallback("disconnected", obj);
}

static void handleDiscordError(int errcode, const char *message)
{
  Print("Discord: Error");
  Local<Object> obj = Object::New(iso);
  obj->Set(String::NewFromUtf8(iso, "errcode"), Int32::New(iso, errcode));
  obj->Set(String::NewFromUtf8(iso, "message"), String::NewFromUtf8(iso, message));
  SendCallback("errored", obj);
}

static void handleDiscordJoin(const char *secret)
{
  Print("Discord: Join");
  Local<Object> obj = Object::New(iso);
  obj->Set(String::NewFromUtf8(iso, "secret"), String::NewFromUtf8(iso, secret));
  SendCallback("joinGame", obj);
}

static void handleDiscordSpectate(const char *secret)
{
  Print("Discord: Spectate");
  Local<Object> obj = Object::New(iso);
  obj->Set(String::NewFromUtf8(iso, "secret"), String::NewFromUtf8(iso, secret));
  SendCallback("spectateGame", obj);
}

static void handleDiscordJoinRequest(const DiscordJoinRequest *request)
{
  Print("Discord: Join Request");
  Local<Object> obj = Object::New(iso);
  obj->Set(String::NewFromUtf8(iso, "request"), String::NewFromUtf8(iso, "Yeah there's no way im gonna try parsing this sorry go away"));
  SendCallback("joinRequest", obj);
}

const char *ToCString(Local<String> str)
{
  String::Utf8Value value(str);
  return *value ? *value : "<string conversion failed>";
}

void SetHandler(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  iso = isolate;
  Local<Function> cb = Local<Function>::Cast(args[0]);
  callback.Reset(isolate, cb);
  callbackSet = true;
}

DiscordRichPresence unpack_presence(Isolate *isolate, const v8::FunctionCallbackInfo<v8::Value> &args)
{
  DiscordRichPresence p;
  memset(&p, 0, sizeof(p));
  Handle<Object> p_obj = Handle<Object>::Cast(args[0]);
  Handle<Value> state = p_obj->Get(String::NewFromUtf8(isolate, "state"));
  Handle<Value> details = p_obj->Get(String::NewFromUtf8(isolate, "details"));
  Handle<Value> largeImageKey = p_obj->Get(String::NewFromUtf8(isolate, "largeImageKey"));
  Handle<Value> largeImageText = p_obj->Get(String::NewFromUtf8(isolate, "largeImageText"));
  Handle<Value> smallImageKey = p_obj->Get(String::NewFromUtf8(isolate, "smallImageKey"));
  Handle<Value> smallImageText = p_obj->Get(String::NewFromUtf8(isolate, "smallImageText"));
  Handle<Value> partyId = p_obj->Get(String::NewFromUtf8(isolate, "partyId"));
  Handle<Value> partySize = p_obj->Get(String::NewFromUtf8(isolate, "partySize"));
  Handle<Value> partyMax = p_obj->Get(String::NewFromUtf8(isolate, "partyMax"));
  Handle<Value> endTimestamp = p_obj->Get(String::NewFromUtf8(isolate, "endTimestamp"));
  Handle<Value> startTimestamp = p_obj->Get(String::NewFromUtf8(isolate, "startTimestamp"));

  // if (!state->IsUndefined())
  // {
  v8::String::Utf8Value utfstate(state);
  p.state = *utfstate;
  // }
  // if (!details->IsUndefined())
  // {
  v8::String::Utf8Value utfdetails(details);
  p.details = *utfdetails;
  // }
  // if (!largeImageKey->IsUndefined())
  // {
  v8::String::Utf8Value utflargeImageKey(largeImageKey);
  p.largeImageKey = *utflargeImageKey;
  // }
  // if (!largeImageText->IsUndefined())
  // {
  v8::String::Utf8Value utflargeImageText(largeImageText);
  p.largeImageText = *utflargeImageText;
  // }
  // if (!smallImageKey->IsUndefined())
  // {
  v8::String::Utf8Value utfsmallImageKey(smallImageKey);
  p.smallImageKey = *utfsmallImageKey;
  // }
  // if (!smallImageText->IsUndefined())
  // {
  v8::String::Utf8Value utfsmallImageText(smallImageText);
  p.smallImageText = *utfsmallImageText;
  // }
  // if (!partyId->IsUndefined())
  // {
  v8::String::Utf8Value utfpartyId(partyId);
  p.partyId = *utfpartyId;
  // }

  if (!partySize->IsUndefined())
    p.partySize = partySize->Int32Value();
  if (!partyMax->IsUndefined())
    p.partyMax = partyMax->Int32Value();
  if (!endTimestamp->IsUndefined())
    p.endTimestamp = endTimestamp->Int32Value();
  if (!startTimestamp->IsUndefined())
    p.startTimestamp = startTimestamp->Int32Value();

  return p;
}

void UpdateDiscordPresence(const FunctionCallbackInfo<Value> &args)
{
  Print("Updating presence...");

  Isolate *isolate = args.GetIsolate();
  DiscordRichPresence discordPresence = unpack_presence(isolate, args);

  // //DiscordRichPresence discordPresence;
  // //memset(&discordPresence, 0, sizeof(discordPresence));
  // discordPresence.state = "Eating";
  // discordPresence.details = ToCString(args[0]->ToString());
  // discordPresence.largeImageKey = "breakfast_time";
  // discordPresence.largeImageText = "It's time for breakfast!";
  // discordPresence.smallImageKey = "megumeme";
  // discordPresence.smallImageText = "Wizards are good guests for breakfast.";
  // discordPresence.partyId = "1c34";
  // discordPresence.partySize = 4;
  // discordPresence.partyMax = 42;
  // discordPresence.endTimestamp = 96636520800;
  Discord_UpdatePresence(&discordPresence);

#ifdef DISCORD_DISABLE_IO_THREAD
  Discord_UpdateConnection();
#endif
  Discord_RunCallbacks();
}

void Initialize(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  DiscordEventHandlers handlers;
  memset(&handlers, 0, sizeof(handlers));
  handlers.ready = handleDiscordReady;
  handlers.disconnected = handleDiscordDisconnected;
  handlers.errored = handleDiscordError;
  handlers.joinGame = handleDiscordJoin;
  handlers.spectateGame = handleDiscordSpectate;
  handlers.joinRequest = handleDiscordJoinRequest;
  const char *applicationId = ToCString(args[0]->ToString());
  Discord_Initialize(applicationId, &handlers, 1, NULL);
  Print("Discord has (allegedly) been initialized.");
}

void Method(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}

static void at_exit(void *arg)
{
  Print("Shutting down...");
  Discord_Shutdown();
}
static void ShutDown(const FunctionCallbackInfo<Value> &args)
{
  Print("Shutting down...");
  Discord_Shutdown();
}
static char cookie[] = "yum yum";

void init(Local<Object> exports)
{
  NODE_SET_METHOD(exports, "hello", Method);
  NODE_SET_METHOD(exports, "initialize", Initialize);
  NODE_SET_METHOD(exports, "setHandler", SetHandler);
  NODE_SET_METHOD(exports, "updateDiscordPresence", UpdateDiscordPresence);
  NODE_SET_METHOD(exports, "shutdown", ShutDown);
  AtExit(at_exit, cookie);
}

NODE_MODULE(discordrpc, init)

} // namespace DiscordRPC