import org.jetbrains.kotlin.gradle.plugin.mpp.KotlinNativeTarget

plugins {
    alias(libs.plugins.kotlinMultiplatform)
}

group = "org.jetbrains.compose.collection-internal-stub"
// 注意这里的version版本要与ovCompose-multiplatform-core的版本号一致
version = "1.6.1-OHOS-003"

kotlin {
    jvm()
    iosArm64()
    iosX64()
    iosSimulatorArm64()
    ohosArm64()
    targets.all {
        if (this is KotlinNativeTarget) {
            compilations.all {
                compilerOptions.options.moduleName.set("org.jetbrains.compose.collection-internal:collection")
            }
        }
    }
    sourceSets.all {
        languageSettings.languageVersion = "1.9"
    }
}