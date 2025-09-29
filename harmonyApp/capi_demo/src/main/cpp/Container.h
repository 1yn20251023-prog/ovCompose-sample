//
// Created on 2025/9/28.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef CAPIRENDERNODEDEMO_CONTAINER_H
#define CAPIRENDERNODEDEMO_CONTAINER_H

#include <ace/xcomponent/native_interface_xcomponent.h>
#include <string>

const unsigned int LOG_PRINT_DOMAIN = 0xFF00;
class Container {
public:
    explicit Container(const std::string &id);
    ~Container() = default;
    
    static Container *GetInstance(const std::string &id);
    static void Release(const std::string &id);
    
    void RegisterCallback(OH_NativeXComponent *nativeXComponent);
    
    void OnSurfaceChanged(OH_NativeXComponent *component, void *window);
    void OnTouchEvent(OH_NativeXComponent *component, void *window);
    void OnMouseEvent(OH_NativeXComponent *component, void *window);
    void OnHoverEvent(OH_NativeXComponent *component, bool isHover);
    
public:
    static std::unordered_map<std::string, Container *> instance_;
    std::string id_;
    
private:
    OH_NativeXComponent_Callback containerCallback_;
    OH_NativeXComponent_MouseEvent_Callback mouseCallback_;
};

#endif //CAPIRENDERNODEDEMO_CONTAINER_H
