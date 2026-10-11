# NarutoSenki317

A port of **Naruto Senki**, a fan-made Naruto game, from cocos2d-x 2.2.6 to **cocos2d-x 3.17.2**.

The original game is no longer maintained; this repo keeps its publicly available source code building on a modern toolchain.

| Platform | Status |
|----------|--------|
| Android (`armeabi-v7a`) | Working |
| Windows | Tested locally |
| iOS / macOS / Linux | Untested |

## Build

Quick start (Android):

```bash
cd proj.android
./gradlew assembleDebug
```

APKs can also be built on GitHub: **Actions → Android Build → Run workflow**, then pick the branch and `debug`/`release`.

See [docs/BUILD.md](docs/BUILD.md) for requirements, signing, CI, and troubleshooting. The 2.x → 3.x code migration log is in [docs/MIGRATION.md](docs/MIGRATION.md) (Indonesian).

## Disclaimer

Fan project, not affiliated with or endorsed by the owners of Naruto. All Naruto names, characters, and assets belong to their respective owners.
