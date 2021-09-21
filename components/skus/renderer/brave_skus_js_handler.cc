/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/skus/renderer/brave_skus_js_handler.h"

#include <utility>

#include "base/no_destructor.h"
#include "base/strings/utf_string_conversions.h"
#include "content/public/renderer/render_frame.h"
#include "gin/arguments.h"
#include "gin/function_template.h"
#include "third_party/blink/public/common/browser_interface_broker_proxy.h"
#include "third_party/blink/public/mojom/devtools/console_message.mojom.h"
#include "third_party/blink/public/platform/web_string.h"
#include "third_party/blink/public/web/blink.h"
#include "third_party/blink/public/web/web_console_message.h"
#include "third_party/blink/public/web/web_local_frame.h"
#include "third_party/blink/public/web/web_script_source.h"

namespace brave_rewards {

BraveSkusJSHandler::BraveSkusJSHandler(content::RenderFrame* render_frame)
    : render_frame_(render_frame) {}

BraveSkusJSHandler::~BraveSkusJSHandler() = default;

bool BraveSkusJSHandler::EnsureConnected() {
  if (!skus_sdk_.is_bound()) {
    render_frame_->GetBrowserInterfaceBroker()->GetInterface(
        skus_sdk_.BindNewPipeAndPassReceiver());
  }

  return skus_sdk_.is_bound();
}

void BraveSkusJSHandler::AddJavaScriptObjectToFrame(
    v8::Local<v8::Context> context) {
  v8::Isolate* isolate = blink::MainThreadIsolate();
  v8::HandleScope handle_scope(isolate);
  if (context.IsEmpty())
    return;

  v8::Context::Scope context_scope(context);

  BindFunctionsToObject(isolate, context);
}

void BraveSkusJSHandler::ResetRemote(content::RenderFrame* render_frame) {
  render_frame_ = render_frame;
  skus_sdk_.reset();
  EnsureConnected();
}

void BraveSkusJSHandler::BindFunctionsToObject(v8::Isolate* isolate,
                                               v8::Local<v8::Context> context) {
  v8::Local<v8::Object> global = context->Global();
  v8::Local<v8::Object> brave_obj;
  v8::Local<v8::Value> brave_value;
  if (!global->Get(context, gin::StringToV8(isolate, "brave"))
           .ToLocal(&brave_value) ||
      !brave_value->IsObject()) {
    brave_obj = v8::Object::New(isolate);
    global->Set(context, gin::StringToSymbol(isolate, "brave"), brave_obj)
        .Check();
  } else {
    brave_obj = brave_value->ToObject(context).ToLocalChecked();
  }
  BindFunctionToObject(
      isolate, brave_obj, "refresh_order",
      base::BindRepeating(&BraveSkusJSHandler::RefreshOrder,
                          base::Unretained(this), isolate));
}

template <typename Sig>
void BraveSkusJSHandler::BindFunctionToObject(
    v8::Isolate* isolate,
    v8::Local<v8::Object> javascript_object,
    const std::string& name,
    const base::RepeatingCallback<Sig>& callback) {
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  javascript_object
      ->Set(context, gin::StringToSymbol(isolate, name),
            gin::CreateFunctionTemplate(isolate, callback)
                ->GetFunction(context)
                .ToLocalChecked())
      .Check();
}

v8::Local<v8::Promise> BraveSkusJSHandler::RefreshOrder(v8::Isolate* isolate,
                                                        uint32_t order_id) {
  if (!EnsureConnected())
    return v8::Local<v8::Promise>();

  v8::MaybeLocal<v8::Promise::Resolver> resolver =
      v8::Promise::Resolver::New(isolate->GetCurrentContext());
  if (resolver.IsEmpty()) {
    return v8::Local<v8::Promise>();
  }

  auto promise_resolver(
      v8::Global<v8::Promise::Resolver>(isolate, resolver.ToLocalChecked()));
  auto context_old(
      v8::Global<v8::Context>(isolate, isolate->GetCurrentContext()));

  skus_sdk_->RefreshOrder(
      order_id,
      base::BindOnce(&BraveSkusJSHandler::OnRefreshOrder,
                     base::Unretained(this), std::move(promise_resolver),
                     isolate, std::move(context_old)));

  return resolver.ToLocalChecked()->GetPromise();
}

void BraveSkusJSHandler::OnRefreshOrder(
    v8::Global<v8::Promise::Resolver> promise_resolver,
    v8::Isolate* isolate,
    v8::Global<v8::Context> context_old,
    const std::string& response) {
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Context> context = context_old.Get(isolate);
  v8::Context::Scope context_scope(context);
  v8::MicrotasksScope microtasks(isolate,
                                 v8::MicrotasksScope::kDoNotRunMicrotasks);

  v8::Local<v8::Promise::Resolver> resolver = promise_resolver.Get(isolate);
  v8::Local<v8::String> result;
  result = v8::String::NewFromUtf8(isolate, response.c_str()).ToLocalChecked();

  ALLOW_UNUSED_LOCAL(resolver->Resolve(context, result));
}

}  // namespace brave_rewards
