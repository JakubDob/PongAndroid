#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <iostream>
#include <Pong/Pong.h>
#include <SDL2_Sandbox/EngineConstants.h>
#include <string>
#include <jni.h>
#include <SDL2/SDL.h>
#include <vector>

#define LOGI(...)                                                   \
    ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", \
                               __VA_ARGS__))
#define LOGW(...)                                                   \
    ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", \
                               __VA_ARGS__))
#define LOGE(...)                                                    \
    ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", \
                               __VA_ARGS__))

// collect file names from assets folder using jni calls
class AssetManagerHelper {
   public:
    AssetManagerHelper() {
        env = (JNIEnv*)SDL_AndroidGetJNIEnv();
        activity = (jobject)SDL_AndroidGetActivity();
        mainActivityClass = env->GetObjectClass(activity);
        getAssetsId = env->GetMethodID(mainActivityClass, "getAssets",
                                       "()Landroid/content/res/AssetManager;");
        jobject assetManagerMethod =
            env->CallObjectMethod(activity, getAssetsId);
        globalAssetManagerRef = env->NewGlobalRef(assetManagerMethod);
        assetManager = AAssetManager_fromJava(env, globalAssetManagerRef);
    }
    inline AAssetManager* getManager() const { return this->assetManager; }
    inline JNIEnv* getEnv() const { return this->env; }
    inline bool directoryExists(std::string path) {
        auto* dir = AAssetManager_openDir(assetManager, path.c_str());
        bool result = AAssetDir_getNextFileName(dir) != nullptr;
        AAssetDir_close(dir);
        if (result) {
            LOGI("Directory exists: %s", path.c_str());
        } else {
            LOGW("Directory does NOT exist: %s", path.c_str());
        }
        return result;
    }
    inline void listFilesInDirectory(std::string path,
                                     std::vector<std::string>& assetsPaths) {
        auto* dir = AAssetManager_openDir(assetManager, path.c_str());
        char const* fileName = nullptr;
        std::vector<std::string> result;
        while ((fileName = AAssetDir_getNextFileName(dir)) != nullptr) {
            // LOGI("%s/%s", path.c_str(), fileName);
            assetsPaths.push_back(path + EngineConstants::Path::delimiter +
                                  fileName);
        }
        AAssetDir_close(dir);
    }

   private:
    JNIEnv* env;
    jobject activity;
    jclass mainActivityClass;
    jmethodID getAssetsId;
    AAssetManager* assetManager;
    jobject globalAssetManagerRef;
};

int main(int argc, char* args[]) {
    AssetManagerHelper amh;
    std::vector<std::string> assetsPaths;
    amh.listFilesInDirectory(EngineConstants::Path::texturesFolder,
                             assetsPaths);
    amh.listFilesInDirectory(EngineConstants::Path::fontsFolder, assetsPaths);
    amh.listFilesInDirectory(EngineConstants::Path::shadersFolder, assetsPaths);
    amh.listFilesInDirectory(EngineConstants::Path::atlasFolder, assetsPaths);
    try {
        Pong::loadAssets(assetsPaths);
    } catch (std::exception& e) {
        LOGE("Error during initializing: %s", e.what());
    }
    try {
        Pong pong;
        pong.run();
    } catch (std::exception& e) {
        LOGE("Error during runtime: %s", e.what());
    }
    return 0;
}