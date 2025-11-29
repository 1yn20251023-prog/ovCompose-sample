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

package com.tencent.compose.sample.mainpage

import androidx.compose.animation.AnimatedContent
import androidx.compose.animation.AnimatedContentTransitionScope
import androidx.compose.animation.core.FastOutSlowInEasing
import androidx.compose.animation.core.tween
import androidx.compose.animation.fadeIn
import androidx.compose.animation.fadeOut
import androidx.compose.animation.togetherWith
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.border
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.RowScope
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxHeight
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.foundation.lazy.rememberLazyListState
import androidx.compose.material.Button
import androidx.compose.material.ButtonDefaults
import androidx.compose.material.Icon
import androidx.compose.material.Text
import androidx.compose.material.TopAppBar
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowBack
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.huawei.compose.caniuseext.createCanIUseManager
import com.tencent.compose.sample.backhandler.BackHandler
import com.tencent.compose.sample.data.DisplayItem
import com.tencent.compose.sample.data.DisplaySection
import com.tencent.compose.sample.rememberLocalImage
import org.jetbrains.compose.resources.ExperimentalResourceApi



@Composable
internal fun MainPage(skiaRender: Boolean = true) {
    var result by remember { mutableStateOf("点击按钮开始检测") }

    // 使用管理器进行非响应式检测
    val manager = createCanIUseManager()

    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.spacedBy(20.dp)
    ) {
        // 按钮1：检测基础ArkUI能力（非响应式）
        Button(
            onClick = {
                val isAvailable = manager.checkCapability(
                    "SystemCapability.ArkUI.ArkUI.Full"
                )
                result = if (isAvailable) {
                    "✅ 基础ArkUI能力：可用"
                } else {
                    "❌ 基础ArkUI能力：不可用"
                }
            },
            modifier = Modifier.width(300.dp),
            colors = ButtonDefaults.buttonColors(
                backgroundColor = Color(0xFF00b42a)
            )
        ) {
            Text(
                "检测基础ArkUI能力（非响应式）",
                color = Color.White,
                fontSize = 20.sp
            )
        }

        // 按钮2：检测屏幕时间守护能力（非响应式）
        Button(
            onClick = {
                val isAvailable = manager.checkCapability(
                    "SystemCapability.ScreenTimeGuard.GuardService"
                )
                result = if (isAvailable) {
                    "✅ 屏幕时间守护：可用"
                } else {
                    "❌ 屏幕时间守护：不可用"
                }
            },
            modifier = Modifier.width(300.dp),
            colors = ButtonDefaults.buttonColors(
                backgroundColor = Color(0xFFf53f3f)
            )
        ) {
            Text(
                "检测屏幕时间守护能力（不可用）",
                color = Color.White,
                fontSize = 20.sp
            )
        }

        // 结果显示
        Text(
            text = result,
            fontSize = 26.sp,
            color = Color(0xFFDB7093),
            modifier = Modifier.width(300.dp)
        )
    }
}