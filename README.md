# NarutoSenki317

Fan-made Naruto Senki project yang sedang dimigrasikan dari gaya kode
**cocos2d-x 2.2.6** menuju **cocos2d-x 3.17.2**.

Project saat ini sudah memakai engine cocos2d-x 3.17.2 dan sudah bisa build serta
berjalan di Android. Fokus migrasi saat ini adalah membersihkan source lama yang
masih memakai API 2.x seperti `CCLayer`, `CCSprite`, `CCMenu`, `CCPoint`,
`ccp(...)`, `sharedDirector()`, `sharedUserDefault()`, `sharedSpriteFrameCache()`,
`schedule_selector`, `menu_selector`, dan touch delegate lama.

## Status Singkat

| Area | Status |
| --- | --- |
| Engine/runtime | cocos2d-x 3.17.2 |
| Branch kerja | `migrate` |
| Android build | Berhasil menurut uji pengguna setelah tahap 2 |
| Windows build | Pernah berjalan sebelum migrasi tahap ini, perlu uji ulang berkala |
| Push remote | Tidak dilakukan |
| Build oleh Codex | Tidak dilakukan |

## Estimasi Progress Migrasi

Estimasi progress menuju kode full cocos2d-x 3.17.2: **sekitar 55%**.

Alasan estimasi:

- Alur scene utama sudah dimigrasi dari menu awal sampai masuk gameplay.
- Area HUD dan layer gameplay pendukung sudah banyak memakai API 3.x.
- Build Android sudah berhasil dan game bisa dimainkan setelah tahap 2.
- Masih ada file besar yang sangat menentukan gameplay, terutama `ActionManager`
  dan `Element`, yang masih banyak memakai API 2.x.
- Masih ada beberapa area non-core seperti ranking, utility UI, app delegate, dan
  compatibility bridge yang perlu dibersihkan bertahap.

Angka ini bukan hitungan matematis murni dari jumlah file, karena sebagian file
sisa jauh lebih besar dan lebih berisiko daripada scene kecil. Secara jalur main
game, progress terasa lebih tinggi; secara full source cleanup, masih sekitar
setengah lebih sedikit.

## Tahap 1: Scene Flow Awal

Status: **selesai dan sudah build-tested oleh pengguna**.

File/area yang sudah dimigrasi:

- `StartMenu`
- `CreditsLayer`
- `NetworkLayer`
- `SelectLayer`
- `SkillLayer`
- `LoadLayer`

Tujuan tahap 1:

- Memigrasi scene awal sesuai urutan masuk game.
- Mengganti API node/layer/sprite/menu/director/audio/user default lama ke API
  cocos2d-x 3.x.
- Mengganti touch/key listener lama di area pilihan karakter.
- Menjaga data container lama seperti `CCArray`, `CCDictionary`, dan `CCString`
  bila masih terhubung dengan modul lain.

Commit penting:

- `fcd88b6 refactor: migrate start menu to cocos2dx 3 api`
- `ae44561 refactor: migrate credits layer to cocos2dx 3 api`
- `cea29f9 refactor: migrate network layer to cocos2dx 3 api`
- `f889a35 refactor: migrate select layer to cocos2dx 3 api`
- `e722850 refactor: migrate skill layer to cocos2dx 3 api`
- `a3d5cf4 refactor: migrate load layer to cocos2dx 3 api`

## Tahap 2: Gameplay Shell dan UI Pendukung

Status: **berjalan, sebagian besar sudah selesai, Android build berhasil menurut
uji pengguna**.

File/area yang sudah dimigrasi:

- `BGLayer`
- `JoyStick`
- `ActionButton`
- `HPBar`
- `PauseLayer`
- `HudLayer`
- `GameLayer`
- `GearLayer`
- `GameOver`
- `Effect`

Tujuan tahap 2:

- Memigrasi lapisan pembungkus gameplay dan UI gameplay.
- Mengganti `RenderTexture`, `Scene`, `Director`, `SpriteFrameCache`,
  `ProgressTimer`, `MenuItemSprite`, `CallFunc`, `Vec2`, dan listener touch ke
  pola cocos2d-x 3.x.
- Memastikan pause, gear, game over, HUD, dan efek umum tetap bisa build dan
  playable.

Commit penting:

- `8e29133 refactor: migrate background layer to cocos2dx 3 api`
- `c8ffae5 refactor: migrate joystick to cocos2dx 3 api`
- `b6c03ed refactor: migrate action button to cocos2dx 3 api`
- `9c63bea refactor: migrate hp bar to cocos2dx 3 api`
- `4c04546 refactor: migrate pause layer to cocos2dx 3 api`
- `9e6b567 refactor: migrate hud layer to cocos2dx 3 api`
- `03d646c refactor: migrate game layer to cocos2dx 3 api`
- `ebcb157 refactor: migrate gear layer to cocos2dx 3 api`
- `912b55b refactor: migrate game over to cocos2dx 3 api`
- `9a2cde4 refactor: migrate effect to cocos2dx 3 api`

Fix build selama tahap 2:

- `972cf09 refactor: use lambda for hud gear callback`
- `9ea511b refactor: use size constructor in game over`

## Sisa API 2.x yang Masih Sengaja Ditahan

Beberapa API lama masih dibiarkan sementara karena menyentuh banyak modul:

- `CCArray`
- `CCDictionary`
- `CCString`
- `CCLabelTTF`
- `CCScale9Sprite`
- `CCScrollView`
- beberapa helper di `Cocos2dx2Compat.h`

Strateginya adalah membersihkan API node/action/input terlebih dahulu, lalu baru
membersihkan container/data structure lama setelah modul gameplay utama lebih
stabil.

## Tahap Berikutnya

### Tahap 3: Gameplay Core

Prioritas:

- `ActionManager.h`
- `ActionManager.cpp`
- `Element.h`
- `Element.cpp`
- `Characters.h`
- `Characters.cpp`

Target migrasi:

- `CCSprite` ke `Sprite`
- `CCPoint` ke `Vec2`
- `CCRect` ke `Rect`
- `CCAction` / `CCActionInterval` / `CCFiniteTimeAction` ke tipe action 3.x
- `CCCallFunc` dan `callfunc_selector` ke `CallFunc` + `CC_CALLBACK_*`
- `schedule_selector` ke `CC_SCHEDULE_SELECTOR`
- `CCNotificationCenter` ke `NotificationCenter`
- `setDisplayFrame` ke `setSpriteFrame`
- `sharedSpriteFrameCache()` ke `SpriteFrameCache::getInstance()`

Catatan risiko:

- Ini tahap paling sensitif karena mengatur state karakter, skill, bullet,
  monster, clone, AI, hitbox, buff, dan death/reborn flow.
- Sebaiknya commit dipisah lebih kecil, misalnya:
  - header/type migration
  - animation/action callback migration
  - movement/vector migration
  - notification/scheduler migration
  - HUD skill icon/frame migration

### Tahap 4: Cleanup dan Compatibility Reduction

Prioritas:

- `RankingLayer`
- `MyUtils/KTools`
- `MyUtils/CCScrewLayer`
- `MyUtils/CCShake`
- `MyUtils/CCStrokeLabel`
- `AppDelegate`
- `AppMacros`
- `HelloWorldScene`
- pengurangan isi `Cocos2dx2Compat.h`

Target migrasi:

- Mengurangi ketergantungan pada compatibility shim.
- Mengganti sisa helper lama ke API 3.x asli.
- Membersihkan UI ranking dan utility layer.
- Mengecek ulang Android dan Windows setelah semua file core stabil.

## Cara Kerja Migrasi

Aturan kerja yang dipakai sejauh ini:

- Jangan build dari sisi Codex; build dilakukan oleh pengguna.
- Jangan push remote.
- Commit setiap potongan yang sudah dianggap aman.
- Semua commit migrasi memakai prefix `refactor:`.
- Hindari refactor gameplay besar yang tidak perlu.
- Pertahankan behavior lama, terutama state machine skill, HUD, AI, dan efek.
- Bila Android build mengeluarkan error, perbaiki error spesifik dan commit kecil.

## Requirements

| Tool | Version |
| --- | --- |
| cocos2d-x | 3.17.2 |
| Android Studio | Latest stable |
| NDK | 24.0.8215888 |
| CMake | 3.22.1 |
| Target ABI | armeabi-v7a |

## Build Notes

Open Android project from:

```text
proj.android
```

`local.properties` example:

```properties
sdk.dir=D\:\\sdk
```

Only Android `armeabi-v7a` is actively checked during this migration. Other ABI
targets should be treated as unverified until tested.

## Project Notes

This is a fan project. Naruto-related assets and characters belong to their
respective owners.
