package com.tencent.compose.sample.mainpage

import androidx.compose.animation.animateColorAsState
import androidx.compose.animation.core.tween
import androidx.compose.foundation.background
import androidx.compose.foundation.border
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.alpha
import androidx.compose.ui.draw.blur
import androidx.compose.ui.draw.shadow
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.CompositingStrategy
import androidx.compose.ui.graphics.graphicsLayer
import androidx.compose.ui.platform.testTag
import androidx.compose.ui.unit.dp
import kotlinx.coroutines.delay
import org.jetbrains.compose.ui.tooling.preview.Preview

@Preview
@Composable
fun ThreeBoxesExample() {
    // 1. 创建一个布尔状态，用于触发颜色切换
    var animate by remember { mutableStateOf(true) }

    // 2. 使用 LaunchedEffect 创建一个无限循环，每2秒钟翻转一次状态
    LaunchedEffect(Unit) {
        while (true) {
            delay(2000) // 等待2秒
            animate = !animate
        }
    }

    // 3. 为每个组件定义动画颜色状态
    //    当 `animate` 变化时，颜色会从当前值平滑过渡到目标值
    val columnColor by animateColorAsState(
        targetValue = if (animate) Color.LightGray else Color(0xFF4A4A4A),
        animationSpec = tween(durationMillis = 2000)
    )
    val box1Color by animateColorAsState(
        targetValue = if (animate) Color.Red else Color.Yellow,
        animationSpec = tween(durationMillis = 2000)
    )
    val box2Color by animateColorAsState(
        targetValue = if (animate) Color.Magenta else Color.Cyan,
        animationSpec = tween(durationMillis = 2000)
    )
    val box3Color by animateColorAsState(
        targetValue = if (animate) Color.Blue else Color.Green,
        animationSpec = tween(durationMillis = 2000)
    )
    Column(
        modifier = Modifier
            .graphicsLayer(
                compositingStrategy = CompositingStrategy.Offscreen
            )
            .fillMaxSize()
            .background(columnColor)
            .padding(16.dp).testTag("Column"),
        verticalArrangement = Arrangement.spacedBy(20.dp),
        horizontalAlignment = Alignment.CenterHorizontally // 让内容水平居中
    ) {
        Box(
            modifier = Modifier
                .size(100.dp)
                .background(box1Color)
                .padding(10.dp)
                .background(Color.Blue)
                .testTag("Box1")
        ) {
        }

        // 多个相同 Modifier（例如多次 alpha）
        Box(
            modifier = Modifier
                .size(100.dp)
                .background(Color.Magenta)
                .testTag("Box2")
                .graphicsLayer(
                    compositingStrategy = CompositingStrategy.Offscreen,
                    alpha = 0.5f
                )
                .padding(10.dp)
                .background(box2Color)
                .alpha(0.5f) // 第一次透明度
                .alpha(0.6f) // 第二次透明度（会再次计算）
        ) {
            Text(
                "重复Alpha",
                color = Color.White,
                modifier = Modifier.align(Alignment.Center).testTag("TextForBox2")
            )
        }

        // 2️⃣ 多种属性组合
        Box(
            modifier = Modifier
                .size(150.dp)
                .background(box3Color)
                .border(2.dp, Color.White)
                .testTag("Box3")
                .shadow(elevation = 8.dp, shape = RoundedCornerShape(12.dp))
                .blur(5.dp) // 需要配合 graphicsLayer 或 textFilter
                .padding(8.dp)
        ) {
            Text("多种属性组合",
                color = Color.White,
                modifier = Modifier.align(Alignment.Center).testTag("TextForBox3"))
        }
    }
}

