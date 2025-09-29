//
// Created on 2025/9/28.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef CAPIRENDERNODEDEMO_MANAGER_H
#define CAPIRENDERNODEDEMO_MANAGER_H

#include "Container.h"
#include <unordered_map>
#include <arkui/native_node.h>
#include <napi/native_api.h>
namespace NativeXComponentSample {
    class Manager {
    public:
        ~Manager();
    
        static Manager *GetInstance() {
            return &Manager::manager_;
        }
    
        static napi_value CreateNativeNode(napi_env env, napi_callback_info info);
    
        Container *GetContainer(std::string &id);
    
        OH_NativeXComponent *GetNativeXComponent(const  std::string &id);
    
        void SetNativeXComponent(std::string &id, OH_NativeXComponent * nativeXComponent);
    
        void Export(napi_env env, napi_value exports);
    
    private:
        static Manager manager_;
    
        std::unordered_map<std::string, OH_NativeXComponent *> nativeXComponentMap_;
        std::unordered_map<std::string, ArkUI_NodeHandle> nativeNodeMap_;
        std::unordered_map<std::string, Container *> containerMap_;
        };
}


#endif //CAPIRENDERNODEDEMO_MANAGER_H
