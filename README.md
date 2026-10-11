# NarutoSenki317

A fan-made Naruto strategy game project, migrated from **cocos2d-x 2.2.6** to **cocos2d-x 3.17.2** and built for Android.

---

## Requirements

| Tool | Version |
|------|---------|
| cocos2d-x | 3.17.2 |
| Android Studio | Latest stable |
| NDK | 24.0.8215888 |
| CMake | 3.22.1 |
| Target ABI | armeabi-v7a |

---

## Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/your-username/NarutoSenki317.git
cd NarutoSenki317
```

### 2. Open the Project in Android Studio

Open the following folder (not the root):

```
proj.android
```

### 3. Configure local.properties

This file is not committed to the repository. Create it manually in the `proj.android` directory:

```properties
sdk.dir=D\:\\sdk
```

Adjust the path to match your local Android SDK installation. Android Studio can also generate this file automatically on first open.

### 4. Build and Run

Select your target device or emulator in Android Studio and click **Run**.

---

## CI Build (GitHub Actions)

The workflow in `.github/workflows/android.yml` builds the APK on GitHub and uploads it as a run artifact (Actions tab → run → **Artifacts**).

| Trigger | Variant |
|---------|---------|
| Push / pull request to `main` | debug |
| Tag `v*` (e.g. `v1.0`) | release, attached to a GitHub Release |
| Actions → **Android Build** → **Run workflow** | choose `debug`, `release`, or `both` |

### Signing

Debug and release are both signed with a shared **dummy key** (`proj.android/keystore/dummy.keystore`, password `android`), so any APK from CI or a local build can be installed over the previous one. This key is public and only meant for testing.

To sign release with a real key, add these repository secrets: `KEYSTORE_BASE64` (the keystore file as base64), `KEYSTORE_PASSWORD`, `KEY_ALIAS`, `KEY_PASSWORD`. Switching keys requires uninstalling the dummy-signed app first.

---

## Migration Notes (cocos2d-x 2.2.6 to 3.17.2)

The following changes were made to resolve build errors after the engine upgrade:

- **Removed** Enhance Game Service dependency from `Cocos2dxHelper.java`
- **Added** Android platform macro in root `CMakeLists.txt`
- **Guarded** desktop `GLView` include in `AppDelegate.cpp` to prevent cross-platform compile errors
- **Updated** JNI include path from the old cocos2dx path to:
  ```
  platform/android/jni/JniHelper.h
  ```

---

## Platform Support

| Platform | Status |
|----------|--------|
| Android | Tested |
| Windows | Tested |
| iOS | Not tested |

---

## Notes

- Only `armeabi-v7a` has been verified. Other ABIs (arm64-v8a, x86) are untested.
- This is a fan project. All Naruto-related assets and characters are property of their respective owners.