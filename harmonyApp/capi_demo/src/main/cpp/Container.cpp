//
// Created on 2025/9/28.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

# include "Container.h"
# include <string>
# include <cstdint>
# include <hilog/log.h>


void OnSurfaceCreatedCB(OH_NativeXComponent *component, void *window) {
    if (component == nullptr || window == nullptr) {
        return;
    }
    
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    if (OH_NativeXComponent_GetXComponentId(component, idStr, &idSize) != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    
    std::string id(idStr);
    auto render = Container::GetInstance(id);
    uint64_t width;
    uint64_t height;
    int32_t xSize = OH_NativeXComponent_GetXComponentSize(component, window, &width, &height);
    if (xSize == OH_NATIVEXCOMPONENT_RESULT_SUCCESS && render != nullptr) {
        // todo
    }
}

Container::Container(const std::string &id) {
    this->id_ = id;
}


std::unordered_map<std::string, Container *> Container::instance_;


Container *Container::GetInstance(const std::string &id) {
    if (instance_.find(id) == instance_.end()) {
        Container *instance = new Container(id);
        instance_[id] = instance;
        return instance;
    } else {
        return instance_[id];
    }
}

void Container::Release(const std::string &id) {
    Container *render = Container::GetInstance(id);
    if (render != nullptr) {
    }
}

void OnSurfaceChangedCB(OH_NativeXComponent *component, void *window) {
    if (component == nullptr || window == nullptr) {
        return;
    }
    
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    if (OH_NativeXComponent_GetXComponentId(component, idStr, &idSize) != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    
    std::string id(idStr);
    auto container = Container::GetInstance(id);
    if (container != nullptr) {
        container->OnSurfaceChanged(component, window);
    }
}

void Container::OnSurfaceChanged(OH_NativeXComponent *component, void *window) {
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    if (OH_NativeXComponent_GetXComponentId(component, idStr, &idSize) != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    
    std::string id(idStr);
    auto container = Container::GetInstance(id);
    double offsetX;
    double offsetY;
    OH_NativeXComponent_GetXComponentOffset(component, window, &offsetX, &offsetY);
    uint64_t width;
    uint64_t height;
    OH_NativeXComponent_GetXComponentSize(component, window, &width, &height);
    if (container != nullptr) {
//        container->OnSurfaceChanged(component, window);
    }
}



void OnSurfaceDestroyedCB(OH_NativeXComponent *component, void *window) {
    if (component == nullptr || window == nullptr) {
        return;
    }
    
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    if (OH_NativeXComponent_GetXComponentId(component, idStr, &idSize) != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    
    std::string id(idStr);
    Container::Release(id);
}


void DispatchTouchEventCB(OH_NativeXComponent *component, void *window) {
    if (component == nullptr || window == nullptr) {
        return;
    }
    
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    if (OH_NativeXComponent_GetXComponentId(component, idStr, &idSize) != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    
    std::string id(idStr);
    Container *render = Container::GetInstance(id);
    if (render != nullptr) {
        render->OnTouchEvent(component, window);
    }
}

void DispatchMouseEventCB(OH_NativeXComponent *component, void *window) {
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    
//    if (component == nullptr || window == nullptr) {
//        return;
//    }
    
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    
    std::string id(idStr);
    Container *render = Container::GetInstance(id);
    if (render != nullptr) {
        render->OnMouseEvent(component, window);
    }
}

void DispatchHoverEventCB(OH_NativeXComponent *component, bool isHover) {
    int32_t ret;
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    
//    if (component == nullptr || window == nullptr) {
//        return;
//    }
    
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    
    std::string id(idStr);
    Container *container = Container::GetInstance(id);
    if (container != nullptr) {
        container->OnHoverEvent(component, isHover);
    }
}

void Container::RegisterCallback(OH_NativeXComponent *nativeXComponent) {
    containerCallback_.OnSurfaceCreated = OnSurfaceCreatedCB;
    containerCallback_.OnSurfaceChanged = OnSurfaceChangedCB;
    containerCallback_.OnSurfaceDestroyed = OnSurfaceDestroyedCB;
    containerCallback_.DispatchTouchEvent = DispatchTouchEventCB;
    OH_NativeXComponent_RegisterCallback(nativeXComponent, &containerCallback_);
    
    mouseCallback_.DispatchMouseEvent = DispatchMouseEventCB;
    mouseCallback_.DispatchHoverEvent = DispatchHoverEventCB;
}

void Container::OnMouseEvent(OH_NativeXComponent *component, void *window) {
    OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "Container", "onMouseEvent");
    
    OH_NativeXComponent_MouseEvent mouseEvent;
    int32_t ret = OH_NativeXComponent_GetMouseEvent(component, window, &mouseEvent);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "Container", "onMouseEvent success");
    } else {
        OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "Container", "onMouseEvent failed");
    }
}

void Container::OnTouchEvent(OH_NativeXComponent *component, void *window) {
    OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "Container", "onTouchEvent");
    
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    if (OH_NativeXComponent_GetXComponentId(component, idStr, &idSize) != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    
    OH_NativeXComponent_TouchEvent touchEvent;
    OH_NativeXComponent_GetTouchEvent(component, window, &touchEvent);
    std::string id(idStr);
    Container *container = Container::GetInstance(id);
    if (container != nullptr && touchEvent.type == OH_NativeXComponent_TouchEventType::OH_NATIVEXCOMPONENT_UP) {
        
    }
    float tiltX = 0.0f;
    float tiltY = 0.0f;
    OH_NativeXComponent_TouchPointToolType toolType = OH_NativeXComponent_TouchPointToolType::OH_NATIVEXCOMPONENT_TOOL_TYPE_UNKNOWN;
    OH_NativeXComponent_GetTouchPointToolType(component, 0, &toolType);
    OH_NativeXComponent_GetTouchPointTiltX(component, 0, &tiltX);
    OH_NativeXComponent_GetTouchPointTiltY(component, 0, &tiltY);
}

void Container::OnHoverEvent(OH_NativeXComponent *component, bool isHover) {
    OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "Container", "onHoverEvent");
}

