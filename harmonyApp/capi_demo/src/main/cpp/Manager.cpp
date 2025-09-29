//
// Created on 2025/9/28.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".
#include "Manager.h"
#include <arkui/native_interface.h>
#include <arkui/native_animate.h>
// 引入 ArkUI 节点接口
#include <arkui/native_node.h>
// 引入 ArkUI 类型定义
#include <arkui/native_type.h>
// 引入 ArkUI 与 NAPI 的桥接接口
#include <arkui/native_node_napi.h>
// 引入 ArkUI 渲染接口
#include <arkui/native_render.h>

#include <hilog/log.h>
// 以下为鸿蒙 Drawing 渲染相关头文件
#include <native_drawing/drawing_types.h>
#include <native_drawing/drawing_point.h>
#include <native_drawing/drawing_shader_effect.h>
#include <native_drawing/drawing_brush.h>
#include <native_drawing/drawing_canvas.h>
#include <native_drawing/drawing_path.h>
#include <native_drawing/drawing_color.h>
#include <native_drawing/drawing_pen.h>
#include <native_drawing/drawing_rect.h>
#include <native_drawing/drawing_bitmap.h>

#include <cstdint>

namespace NativeXComponentSample {
#define TEST_TEXT_NUMBER 20
namespace {}
// 定义全局节点缓冲数组
ArkUI_NodeHandle gTextBuf[TEST_TEXT_NUMBER];


Manager Manager::manager_;

Manager::~Manager() {
    // 遍历缓存的 XComponent 指针并释放
    for (auto iter = nativeXComponentMap_.begin(); iter != nativeXComponentMap_.end(); ++iter) {
        if (iter->second != nullptr) {
            delete iter->second;
            iter->second = nullptr;
        }
    }
    // 清空映射避免悬挂引用
    nativeXComponentMap_.clear();

    // 遍历容器缓存并释放实例
    for (auto iter = containerMap_.begin(); iter != containerMap_.end(); ++iter) {
        if (iter->second != nullptr) {
            delete iter->second;
            iter->second = nullptr;
        }
    }
    // 清空容器映射
    containerMap_.clear();
}

// 演示如何创建并使用 RenderNode
// 该函数创建一个包含 RenderNode 的节点树，并为 RenderNode 设置属性和动画
ArkUI_NodeHandle render_node_add_event_demo(ArkUI_NativeNodeAPI_1 *nodeAPI, ArkUI_ContextHandle context) {
    // 创建列布局根节点
    ArkUI_NodeHandle root_arkui_node = nodeAPI->createNode(ARKUI_NODE_COLUMN);
    // 设置根节点宽度属性
    ArkUI_NumberValue valueWidth[] = {400};
    ArkUI_AttributeItem itemWidth = {valueWidth, sizeof(valueWidth) / sizeof(ArkUI_NumberValue)};
    nodeAPI->setAttribute(root_arkui_node, NODE_WIDTH, &itemWidth);
    // 设置根节点高度属性
    ArkUI_NumberValue valueHeight[] = {400};
    ArkUI_AttributeItem itemHeight = {valueHeight, sizeof(valueHeight) / sizeof(ArkUI_NumberValue)};
    nodeAPI->setAttribute(root_arkui_node, NODE_HEIGHT, &itemHeight);
    // 设置根节点背景颜色
    valueHeight[0].u32 = 0xff00f100;
    nodeAPI->setAttribute(root_arkui_node, NODE_BACKGROUND_COLOR, &itemHeight);

    // 创建自定义节点用于承载 RenderNode
    auto custom_arkui_node = nodeAPI->createNode(ARKUI_NODE_CUSTOM);
    // 设置自定义节点宽度
    ArkUI_NumberValue valueWidth2[] = {400};
    ArkUI_AttributeItem itemWidth2 = {valueWidth2, sizeof(valueWidth2) / sizeof(ArkUI_NumberValue)};
    nodeAPI->setAttribute(custom_arkui_node, NODE_WIDTH, &itemWidth2);
    // 设置自定义节点高度
    ArkUI_NumberValue valueHeight2[] = {6000};
    ArkUI_AttributeItem itemHeight2 = {valueHeight2, sizeof(valueHeight2) / sizeof(ArkUI_NumberValue)};
    nodeAPI->setAttribute(custom_arkui_node, NODE_HEIGHT, &itemHeight2);

    // 创建 RenderNode 并指定尺寸
    auto custom_render_node = OH_ArkUI_RenderNodeUtils_CreateNode();
    OH_ArkUI_RenderNodeUtils_SetSize(custom_render_node, 300, 300);
    // 将 RenderNode 挂载到自定义节点
    OH_ArkUI_RenderNodeUtils_AddRenderNode(custom_arkui_node, custom_render_node);

    // 用户数据结构体，保存渲染状态
    struct RenderNodeState {
        ArkUI_FloatPropertyHandle width;
        ArkUI_FloatPropertyHandle height;
        ArkUI_Vector2PropertyHandle v2;
        ArkUI_ColorPropertyHandle color;
        ArkUI_RenderNodeHandle inner_node;
        int count;
    };
    // 动态分配用户数据对象
    RenderNodeState *state = new RenderNodeState;
    // 创建浮点属性用于宽度
    auto widthProperty = OH_ArkUI_RenderNodeUtils_CreateFloatProperty(500);
    state->width = widthProperty;
    // 创建浮点属性用于高度
    auto heightProperty = OH_ArkUI_RenderNodeUtils_CreateFloatProperty(1000);
    state->height = heightProperty;
    // 创建向量属性
    auto vectorP = OH_ArkUI_RenderNodeUtils_CreateVector2Property(1000, 1000);
    state->v2 = vectorP;
    // 创建颜色属性
    auto colorP = OH_ArkUI_RenderNodeUtils_CreateColorProperty(0xFFFFFF00);
    state->color = colorP;
    // 保存 RenderNode 句柄
    state->inner_node = custom_render_node;
    // 初始化点击计数
    state->count = 0;


    // 创建上下文回调用于动画更新
    ArkUI_ContextCallback *update = new ArkUI_ContextCallback;
    update->userData = state;
    update->callback = [](void *state) {
        // 动画每帧回调中更新属性值
        RenderNodeState *updateState = (RenderNodeState *)state;
        OH_ArkUI_RenderNodeUtils_SetFloatPropertyValue(updateState->width, 100);
        OH_ArkUI_RenderNodeUtils_SetFloatPropertyValue(updateState->height, 100);
        OH_ArkUI_RenderNodeUtils_SetVector2PropertyValue(updateState->v2, 100, 100);
        OH_ArkUI_RenderNodeUtils_SetColorPropertyValue(updateState->color, 0xFF0011FF);
    };

    // 获取动画模块接口
    ArkUI_NativeAnimateAPI_1 *animateApi = nullptr;
    OH_ArkUI_GetModuleInterface(ARKUI_NATIVE_ANIMATE, ArkUI_NativeAnimateAPI_1, animateApi);
    // 创建动画完成回调
    ArkUI_AnimateCompleteCallback *completeCallback = new ArkUI_AnimateCompleteCallback;
    completeCallback->type = ARKUI_FINISH_CALLBACK_REMOVED;
    completeCallback->callback = [](void *state) {
        // 动画结束打印日志
        OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "mytag", "completeCallback->callback");
    };
    // 创建动画配置对象
    ArkUI_AnimateOption *option = OH_ArkUI_AnimateOption_Create();
    OH_ArkUI_AnimateOption_SetDuration(option, 2000);
    OH_ArkUI_AnimateOption_SetTempo(option, 1.1);
    OH_ArkUI_AnimateOption_SetCurve(option, ARKUI_CURVE_EASE);
    OH_ArkUI_AnimateOption_SetDelay(option, 20);
    OH_ArkUI_AnimateOption_SetIterations(option, 1);
    OH_ArkUI_AnimateOption_SetPlayMode(option, ARKUI_ANIMATION_PLAY_MODE_REVERSE);

    // 配置动画期望帧率区间
    ArkUI_ExpectedFrameRateRange *range = new ArkUI_ExpectedFrameRateRange;
    range->max = 120;
    range->min = 10;
    range->expected = 60;
    OH_ArkUI_AnimateOption_SetExpectedFrameRateRange(option, range);
    // 启动动画并绑定回调
    animateApi->animateTo(context, option, update, completeCallback);
    // 注册点击事件
    nodeAPI->registerNodeEvent(custom_arkui_node, ArkUI_NodeEventType::NODE_ON_CLICK, 999, state);
    // 注册事件派发接收器
    nodeAPI->registerNodeEventReceiver([](ArkUI_NodeEvent *event) {
        int32_t targetId = OH_ArkUI_NodeEvent_GetTargetId(event);
        if (targetId == 999) {
            // 取出用户数据，准备处理点击逻辑
            RenderNodeState *state = (RenderNodeState *)OH_ArkUI_NodeEvent_GetUserData(event);
            ArkUI_RenderNodeHandle inner_render_node = state->inner_node;
            int count = state->count;
            count++;
            state->count = count;
            if (count % 2 == 0) {
                // 偶数次点击清空子节点
                OH_ArkUI_RenderNodeUtils_ClearChildren(inner_render_node);
            } else {
                // 奇数次点击重新创建内容节点
                auto do_render_node = OH_ArkUI_RenderNodeUtils_CreateNode();
                auto modifier = OH_ArkUI_RenderNodeUtils_CreateContentModifier();

                // 将 modifier 绑定到 RenderNode
                OH_ArkUI_RenderNodeUtils_AttachContentModifier(do_render_node, modifier);
                auto widthP = state->width;
                auto heightP = state->height;
                auto v2p = state->v2;
                auto colorP = state->color;

                // 附加之前创建的属性句柄
                OH_ArkUI_RenderNodeUtils_AttachFloatProperty(modifier, widthP);
                OH_ArkUI_RenderNodeUtils_AttachFloatProperty(modifier, heightP);
                OH_ArkUI_RenderNodeUtils_AttachVector2Property(modifier, v2p);
                OH_ArkUI_RenderNodeUtils_AttachColorProperty(modifier, colorP);

                // 将新节点添加到渲染树
                OH_ArkUI_RenderNodeUtils_AddChild(inner_render_node, do_render_node);
                OH_ArkUI_RenderNodeUtils_SetSize(do_render_node, 300, 300);

                // 设置绘制回调执行实际绘制逻辑
                OH_ArkUI_RenderNodeUtils_SetContentModifierOnDraw(
                    modifier, state, [](ArkUI_DrawContext *context, void *state) {
                        RenderNodeState *data = (RenderNodeState *)state;
                        // 准备读取属性值
                        float width = 0;
                        float height = 0;
                        uint32_t color = 0;
                        ArkUI_FloatPropertyHandle w = data->width;
                        ArkUI_FloatPropertyHandle h = data->height;

                        // 读取宽高与颜色属性
                        OH_ArkUI_RenderNodeUtils_GetFloatPropertyValue(w, &width);
                        OH_ArkUI_RenderNodeUtils_GetFloatPropertyValue(h, &height);
                        ArkUI_ColorPropertyHandle cp = data->color;
                        OH_ArkUI_RenderNodeUtils_GetColorPropertyValue(cp, &color);
                        // 获取 Canvas 对象
                        auto *canvas1 = OH_ArkUI_DrawContext_GetCanvas(context);
                        OH_Drawing_Canvas *canvas = reinterpret_cast<OH_Drawing_Canvas *>(canvas1);

                        // 创建起点和终点用于渐变
                        OH_Drawing_Point *startPt = OH_Drawing_PointCreate(20, 20);
                        OH_Drawing_Point *endPt = OH_Drawing_PointCreate(900, 900);
                        // 定义渐变颜色与位置
                        uint32_t colors[] = {0xFFFFFF00, 0xFFFF00FF, 0xFF00FFFF};
                        float pos[] = {0.0f, 0.5f, 1.0f};
                        // 创建线性渐变着色器
                        OH_Drawing_ShaderEffect *colorShaderEffect = OH_Drawing_ShaderEffectCreateLinearGradient(
                            startPt, endPt, colors, pos, 3, OH_Drawing_TileMode::CLAMP);
                        // 创建画笔刷并绑定渐变
                        OH_Drawing_Brush *brush = OH_Drawing_BrushCreate();
                        OH_Drawing_BrushSetShaderEffect(brush, colorShaderEffect);
                        // 将画刷附着到画布
                        OH_Drawing_CanvasAttachBrush(canvas, brush);
                        // 创建矩形对象（示例中未直接使用）
                        OH_Drawing_Rect *rect = OH_Drawing_RectCreate(100, 100, 900, 900);

                        // 创建路径并绘制三角形轮廓
                        auto rect_path = OH_Drawing_PathCreate();
                        OH_Drawing_PathMoveTo(rect_path, 100, 0);
                        OH_Drawing_PathLineTo(rect_path, width / 4, height * 3 / 4);
                        OH_Drawing_PathLineTo(rect_path, width, height);
                        OH_Drawing_PathClose(rect_path);

                        // 计算五角星坐标
                        auto height_ = 1800;
                        auto width_ = 1800;
                        auto len = height_ / 4;
                        auto aX = width_ / 3;
                        auto aY = height_ / 6;
                        auto dX = aX - len * sin(18.0);
                        auto dY = aY + len * cos(18.0);
                        auto cX = aX + len * sin(18.0);
                        auto cY = dY;
                        auto bX = aX + len / 2.0;
                        auto bY = aY + sqrt((cX - dX) * (cX - dX) + (len / 2.0) * (len / 2.0));
                        auto eX = aX - (len / 2.0);
                        auto eY = bY;

                        // 构造五角星路径
                        auto custom_path = OH_Drawing_PathCreate();
                        OH_Drawing_PathMoveTo(custom_path, aX, aY);
                        OH_Drawing_PathLineTo(custom_path, bX, bY);
                        OH_Drawing_PathLineTo(custom_path, cX, cY);
                        OH_Drawing_PathLineTo(custom_path, dX, dY);
                        OH_Drawing_PathLineTo(custom_path, eX, eY);
                        OH_Drawing_PathClose(custom_path);

                        // 创建画笔并设置宽度与渐变
                        auto pen = OH_Drawing_PenCreate();
                        OH_Drawing_PenSetWidth(pen, 100);
                        OH_Drawing_PenSetShaderEffect(pen, colorShaderEffect);
                        // 绘制五角星路径
                        OH_Drawing_CanvasDrawPath(canvas, custom_path);

                        // 释放绘制资源
                        OH_Drawing_CanvasDetachBrush(canvas);
                        OH_Drawing_BrushDestroy(brush);
                        OH_Drawing_RectDestroy(rect);
                        OH_Drawing_ShaderEffectDestroy(colorShaderEffect);
                        OH_Drawing_PointDestroy(startPt);
                        OH_Drawing_PointDestroy(endPt);
                    });
            }
        }
    });
    // 将自定义节点加入根节点
    nodeAPI->addChild(root_arkui_node, custom_arkui_node);
    return root_arkui_node;
}

// Napi暴露的创建函数
napi_value Manager::CreateNativeNode(napi_env env, napi_callback_info info) {
    // 校验环境参数
    if (env == nullptr || info == nullptr) {
        return nullptr;
    }
    // 准备获取参数
    size_t argCnt = 2;
    napi_value args[2] = {nullptr};
    if (napi_get_cb_info(env, info, &argCnt, args, nullptr, nullptr) != napi_ok) {
        return nullptr;
    }

    // 校验参数个数
    if (argCnt != 2) {
        napi_throw_type_error(env, NULL, "wrong number of arguments 1");
        return nullptr;
    }
    napi_valuetype valuetype;
    if (napi_typeof(env, args[0], &valuetype) != napi_ok) {
        napi_throw_type_error(env, NULL, "wrong number of arguments 5");
        return nullptr;
    }

    // 确保第一个参数是字符串（XComponent ID）
    if (valuetype != napi_string) {
        napi_throw_type_error(env, NULL, "wrong number of arguments 2");
        return nullptr;
    }

    // 读取 XComponent ID 字符串
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
    constexpr uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    size_t length;
    if (napi_get_value_string_utf8(env, args[0], idStr, idSize, &length)) {
        napi_throw_type_error(env, NULL, "wrong number of arguments 3");
        return nullptr;
    }

    // 获取 Manager 单例
    auto manager = Manager::GetInstance();
    if (manager == nullptr) {
        OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "mytag", "manager == nullptr");
        napi_throw_type_error(env, NULL, "wrong number of arguments 4");
        return nullptr;
    }

    // 根据 ID 查找已缓存的 XComponent
    OH_NativeXComponent *component = manager->GetNativeXComponent(idStr);
    if (component == nullptr) {
        OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "mytag", "component == nullptr");
        return nullptr;
    }

    // 获取节点接口指针
    auto *nodeAPI = reinterpret_cast<ArkUI_NativeNodeAPI_1 *>(
        OH_ArkUI_QueryModuleInterfaceByName(ARKUI_NATIVE_NODE, "ArkUI_NativeNodeAPI_1"));

    // 校验接口可用性
    if (nodeAPI == nullptr) {
        OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "mytag", "nodeAPI == nullptr");
        return nullptr;
    }

    // 构建节点并挂载到 XComponent
    if (nodeAPI->createNode != nullptr && nodeAPI->addChild != nullptr) {
        ArkUI_NodeHandle testNode;
        ArkUI_ContextHandle context = nullptr;
        // 从参数获取上下文对象
        auto code = OH_ArkUI_GetContextFromNapiValue(env, args[1], &context);
        (void)code; // 避免未使用警告
        // 调用 demo 构建函数
        testNode = render_node_add_event_demo(nodeAPI, context);
        if (testNode == nullptr) {
            OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "mytag", "testNode == nullptr");
        }
        OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "mytag", "std::exception");
        // 将创建好的节点附着到 XComponent
        OH_NativeXComponent_AttachNativeRootNode(component, testNode);
    }
    return nullptr;
}

// 根据 ID 获取缓存的 XComponent 指针
OH_NativeXComponent *Manager::GetNativeXComponent(const std::string &id) { return nativeXComponentMap_[id]; }

// 设置缓存 XComponent 指针
void Manager::SetNativeXComponent(std::string &id, OH_NativeXComponent *nativeXComponent) {
    if (nativeXComponent == nullptr) {
        return;
    }

    // 如果当前 ID 未缓存，则直接插入
    if (nativeXComponentMap_.find(id) == nativeXComponentMap_.end()) {
        nativeXComponentMap_[id] = nativeXComponent;
        return;
    }

    // 若已有不同实例，则释放旧对象再覆盖
    if (nativeXComponentMap_[id] != nativeXComponent) {
        OH_NativeXComponent *tmp = nativeXComponentMap_[id];
        delete tmp;
        tmp = nullptr;
        nativeXComponentMap_[id] = nativeXComponent;
    }
}

// 获取或创建对应 ID 的容器实例
Container *Manager::GetContainer(std::string &id) {
    if (containerMap_.find(id) == containerMap_.end()) {
        Container *instance = Container::GetInstance(id);
        containerMap_[id] = instance;
        return instance;
    }

    return containerMap_[id];
}


// 从导出的 NAPI 对象中解包原生 XComponent，缓存并触发容器注册回调，与 JS 侧完成绑定
void Manager::Export(napi_env env, napi_value exports) {
    if (env == nullptr || exports == nullptr) {
        return;
    }

    // 读取导出对象中的 native XComponent 属性
    napi_value exportInstance = nullptr;
    if ((napi_get_named_property(env, exports, OH_NATIVE_XCOMPONENT_OBJ, &exportInstance)) != napi_ok) {
        return;
    }

    // 从 NAPI 对象解包原生指针
    OH_NativeXComponent *nativeXComponent = nullptr;
    if (napi_unwrap(env, exportInstance, reinterpret_cast<void **>(&nativeXComponent)) != napi_ok) {
        return;
    }

    // 读取 XComponent 的 ID
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    if (OH_NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize) != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return;
    }

    // 将 ID 封装为 string，便于存储
    std::string id(idStr);
    auto manager = Manager::GetInstance();
    if ((manager != nullptr && nativeXComponent != nullptr)) {
        // 缓存 XComponent 指针
        manager->SetNativeXComponent(id, nativeXComponent);
        auto container = manager->GetContainer(id);
        if (container != nullptr) {
            // 为容器注册回调，完成后续交互
            container->RegisterCallback(nativeXComponent);
        }
    }
}
}