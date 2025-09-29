//
// Created on 2025/9/28.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#include <Manager.h>
#include <node_api.h>
namespace NativeXComponentSample {
EXTERN_C_START

static napi_value Init(napi_env env, napi_value exports) {
    if (env == nullptr || exports == nullptr) {
        return nullptr;
    }
    
    napi_property_descriptor desc[] = {
        {"createNativeNode", nullptr, Manager::CreateNativeNode, nullptr, nullptr, nullptr, napi_default, nullptr},
    };
    
    if (napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc) != napi_ok) {
        return nullptr;
    }
    
    Manager::GetInstance()->Export(env, exports);
    return exports;
    
}

static napi_module nativeNodeModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "nativeNode",
    .nm_priv = ((void *) 0),
    .reserved = {0}
};

EXTERN_C_END


extern "C" __attribute__((constructor())) void RegisterModule(void) {napi_module_register(&nativeNodeModule);}
}
