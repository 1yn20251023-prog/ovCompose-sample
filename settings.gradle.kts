/*
 * Tencent is pleased to support the open source community by making ovCompose available.
 * Copyright (C) 2025 THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

rootProject.name = "ComposeSample"
enableFeaturePreview("TYPESAFE_PROJECT_ACCESSORS")

pluginManagement {
    repositories {
        maven("/Users/shiqi1007/.m2/repository")
        maven("https://mirrors.tencent.com/repository/maven/tencentvideo")
        maven("https://mirrors.tencent.com/repository/maven/tmm-snapshot")
        maven("https://mirrors.tencent.com/nexus/repository/maven-tencent")
        maven("https://mirrors.tencent.com/nexus/repository/maven-public")
        google {
            mavenContent {
//                includeGroupAndSubgroups("androidx")
//                includeGroupAndSubgroups("com.android")
//                includeGroupAndSubgroups("com.google")
            }
        }
        mavenCentral()
        gradlePluginPortal()
    }
    plugins {
        kotlin("jvm") version "2.2.0"
    }
}

dependencyResolutionManagement {
    repositories {
        maven("/Users/shiqi1007/.m2/repository")
        maven("https://mirrors.tencent.com/repository/maven/tencentvideo")
        maven("https://mirrors.tencent.com/repository/maven/tmm-snapshot")
        maven("https://mirrors.tencent.com/nexus/repository/maven-tencent")
        maven("https://mirrors.tencent.com/nexus/repository/maven-public")
        google {
            mavenContent {
//                includeGroupAndSubgroups("androidx")
//                includeGroupAndSubgroups("com.android")
//                includeGroupAndSubgroups("com.google")
            }
        }
        mavenCentral()
    }
}

include(":composeApp")
include(":androidxStubs:annotation")
include(":androidxStubs:collection")
//val composeCorProject = file("../ovCompose-mutiplatform-core")
//if (composeCorProject.exists()) {
//    includeBuild(composeCorProject)
//    logger.trace("Include compose-multiplatform-core via Composite-Build.")
//}
includeBuild("../ovCompose-multiplatform-core")