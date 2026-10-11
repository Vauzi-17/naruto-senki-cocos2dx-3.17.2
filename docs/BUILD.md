# Building NarutoSenki317

- [Requirements](#requirements)
- [Local build (Android)](#local-build-android)
- [Signing](#signing)
- [GitHub Actions](#github-actions)
- [Troubleshooting](#troubleshooting)
- [Porting notes](#porting-notes)

---

## Requirements

| Tool | Version |
|------|---------|
| JDK | 17 or newer (Android Studio's bundled JBR works) |
| Gradle | 8.5 (via the wrapper in `proj.android`) |
| Android Gradle Plugin | 8.1.0 |
| Android SDK | compileSdk / targetSdk 33, minSdk 21 |
| NDK | 24.0.8215888 (pinned in `proj.android/app/build.gradle`) |
| CMake | 3.22.1 (installed from the SDK Manager) |
| Target ABI | `armeabi-v7a` |

The cocos2d-x 3.17.2 engine is already included in `cocos2d/`, so there is nothing else to download.

---

## Local build (Android)

### 1. Point Gradle to your Android SDK

Create `proj.android/local.properties` (it is git-ignored):

```properties
sdk.dir=D:/sdk
```

Use forward slashes. If you use backslashes they must be escaped (`D\:\\sdk`); a single backslash makes Gradle fail with *"The filename, directory name, or volume label syntax is incorrect"*. Android Studio writes this file for you on first open.

### 2a. Android Studio

Open the `proj.android` folder (not the repo root), wait for Gradle sync, pick a device, and click **Run**.

### 2b. Command line

```bash
cd proj.android
./gradlew assembleDebug      # or assembleRelease
```

On Windows use `gradlew.bat`. If `java` on your PATH is older than 17, point `JAVA_HOME` at Android Studio's JBR first.

### Build variants

| Variant | Command | Output | Notes |
|---------|---------|--------|-------|
| debug | `assembleDebug` | `proj.android/app/build/outputs/apk/debug/` | Debuggable, no minification |
| release | `assembleRelease` | `proj.android/app/build/outputs/apk/release/` | R8 minify + resource shrinking, native built as RelWithDebInfo |

The first build compiles the whole engine and takes a while (a local release build took about 15 minutes). Later builds only recompile what changed.

---

## Signing

Both variants are signed with a shared **dummy key** at `proj.android/keystore/dummy.keystore`:

| Field | Value |
|-------|-------|
| Store password | `android` |
| Key alias | `dummy` |
| Key password | `android` |

Because debug and release use the same key, any APK from this repo (local or CI) installs over the previous one without uninstalling. The key is public and is meant for testing only.

To sign release with your own key, pass these Gradle properties (they override the dummy key for release only):

```bash
./gradlew assembleRelease \
  -PRELEASE_STORE_FILE=/path/to/release.keystore \
  -PRELEASE_STORE_PASSWORD=... \
  -PRELEASE_KEY_ALIAS=... \
  -PRELEASE_KEY_PASSWORD=...
```

Android refuses to update an app signed with a different key, so moving from the dummy key to a real one (or from an old Android Studio debug build) needs one uninstall.

Real keystores (`*.keystore`, `*.jks`) are git-ignored; only the dummy key is committed.

---

## GitHub Actions

The workflow `.github/workflows/android.yml` builds the APK on GitHub. It only runs when started by hand; pushes, pull requests, and tags do not trigger it.

### Running a build

1. Open the repo on GitHub → **Actions** → **Android Build**.
2. Click **Run workflow** and fill in:

   | Input | Meaning | Default |
   |-------|---------|---------|
   | **Use workflow from** | Which copy of the workflow file to run. Leave it on `main`. | `main` |
   | **ref** | Branch, tag, or commit SHA to build, e.g. `main`, `exp`, `migrate` | `main` |
   | **build_type** | `debug`, `release`, or `both` | `debug` |

3. When the run finishes, download the APK from the run page under **Artifacts** (`NarutoSenki-<variant>-<ref>-<commit>`). Artifacts are kept for 90 days by default.

The run title shows what was built, e.g. `Android release @ exp`.

### What the workflow does

- Checks out the chosen `ref` on `ubuntu-latest`.
- Sets up JDK 17 and Gradle 8.5. It runs the installed `gradle` instead of `./gradlew`, so branches that still carry the old wrapper jar can be built too.
- Installs CMake 3.22.1 and the NDK from the branch's `ndkVersion` (or 25.1.8937393, AGP 8.1's default, when the branch does not pin one).
- Restores the ccache compiler cache, so unchanged C++ files (the whole engine, usually) are not recompiled.
- Builds the APK(s) and prints ccache hit stats in the **ccache stats** step.
- Re-signs every APK in the **Sign APKs** step (see below), then uploads them.

### Build time

| Cache state | Debug build, whole run |
|-------------|------------------------|
| Empty (first run, or after the cache expires) | ~6 min |
| Warm, no C++ changes | ~2 min |

Only changed files in `Classes/` are recompiled on a warm cache. `both` builds native code twice, so pick a single variant when you can.

How the cache is shared:

- A cache saved on a branch is only visible to that branch; caches saved on `main` are visible to every branch. Run one build on `main` after merging so other branches start warm.
- GitHub evicts caches not used for 7 days, and the repo-wide limit is 10 GB. The ccache size is capped at 2 GB.

### Building other branches

A branch builds with whatever its own source and Gradle files contain, but signing does not depend on the branch. After the build, the **Sign APKs** step re-signs every APK with the dummy key taken from the workflow's own commit (normally `main`). Branches that never had the dummy keystore, or whose `build.gradle` leaves release unsigned, still produce installable APKs that update each other.

The step log prints each APK's signer, e.g. `CN=NarutoSenki Dummy, OU=Dev, O=Vauzi, C=ID`.

### Real release key on CI

Add these under **Settings → Secrets and variables → Actions**:

| Secret | Value |
|--------|-------|
| `KEYSTORE_BASE64` | The keystore file, base64-encoded |
| `KEYSTORE_PASSWORD` | Store password |
| `KEY_ALIAS` | Key alias |
| `KEY_PASSWORD` | Key password |

Encode the keystore on Windows with:

```powershell
[Convert]::ToBase64String([IO.File]::ReadAllBytes("release.keystore")) | Set-Clipboard
```

When `KEYSTORE_BASE64` is set, the **Sign APKs** step signs release with it; debug keeps the dummy key.

---

## Troubleshooting

| Symptom | Cause / fix |
|---------|-------------|
| `The filename, directory name, or volume label syntax is incorrect` while configuring | Bad `sdk.dir` in `local.properties`; use `D:/sdk`. |
| `INSTALL_FAILED_UPDATE_INCOMPATIBLE` / "App not installed" | Installed app was signed with another key. Uninstall it once. |
| `Gradle Wrapper Jar failed validation` on CI | Old wrapper jar. The workflow no longer runs the wrapper; locally, regenerate with `./gradlew wrapper --gradle-version 8.5 --distribution-type all`. |
| `NDK not configured` / wrong NDK | Install NDK 24.0.8215888 from the SDK Manager. |

---

## Porting notes

Changes made while moving from cocos2d-x 2.2.6 to 3.17.2:

- Removed the Enhance Game Service dependency from `Cocos2dxHelper.java`.
- Added the Android platform macro in the root `CMakeLists.txt`.
- Guarded the desktop `GLView` include in `AppDelegate.cpp` to avoid cross-platform compile errors.
- Updated the JNI include path to `platform/android/jni/JniHelper.h`.

Other platforms:

- Only `armeabi-v7a` has been verified; `arm64-v8a` and `x86` are untested.
- The Windows project (`proj.win32`) needs the cocos2d-x win32 prebuilt libraries, which are not included in this repo.
- iOS/macOS and Linux projects are present but untested.
