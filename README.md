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
| Android build | Berhasil menurut uji pengguna setelah ranking offline/dekoratif |
| Windows build | Pernah berjalan sebelum migrasi tahap ini, perlu uji ulang berkala |
| Push remote | Tidak dilakukan |
| Build oleh Codex | Tidak dilakukan |

## Estimasi Progress Migrasi

Estimasi progress menuju kode full cocos2d-x 3.17.2: **sekitar 74%**.

Alasan estimasi:

- Alur scene utama sudah dimigrasi dari menu awal sampai masuk gameplay.
- Area HUD dan layer gameplay pendukung sudah banyak memakai API 3.x.
- Build Android sudah berhasil dan game bisa dimainkan setelah cleanup tahap 4
  awal serta setelah ranking dibuat offline/dekoratif; pengguna sudah menjalankan
  game di HP tanpa crash.
- Area gameplay core sudah mulai distabilkan lewat migrasi callback/action/frame
  tanpa mengubah kontrak data lama secara paksa.
- Tahap 5 sudah dimulai secara aman dari area lokal `ActionManager`: sisa alias
  action/animation 2.x di beberapa helper internal dibersihkan tanpa mengubah
  kontrak container besar.
- Masih ada sisa API 2.x yang cukup banyak di `ActionManager`, `Element`,
  `RankingLayer`, beberapa utility UI, app delegate, dan compatibility bridge.
- Untuk mencapai 100%, sisa terbesar bukan sekadar rename API, tetapi migrasi
  kontrak data lama seperti `CCArray`, `CCDictionary`, dan `CCString` yang masih
  dipakai lintas scene, ranking, loader, XML, dan gameplay core.
- Project ini diperlakukan sebagai game offline. Nama `NetworkLayer` adalah nama
  lama untuk flow utama/offline saat ini, bukan indikasi mode online yang harus
  dihapus.

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

Status: **selesai dan sudah build-tested oleh pengguna**.

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

Catatan khusus:

- `CCArray` di scene awal seperti `StartMenu`, `NetworkLayer`, dan `LoadLayer`
  sengaja belum dipaksa migrasi karena masih menjadi kontrak data dengan
  gameplay core. Pembersihan container sebaiknya dilakukan setelah
  `ActionManager`, `Element`, dan `Characters` lebih stabil, agar tidak perlu
  membuat adapter bolak-balik antara scene awal dan gameplay.

Roadmap menuju 100%:

- Sekitar 70-75%: selesaikan tahap 4 untuk helper, app entry, ranking UI, dan
  pengurangan compatibility shim yang tidak menyentuh kontrak data besar.
- Sekitar 75-88%: migrasi sisa node/action/vector/frame di `RankingLayer`,
  `ActionManager`, `Element`, dan helper yang masih memakai API 2.x langsung.
- Sekitar 88-97%: migrasi container/data lama (`CCArray`, `CCDictionary`,
  `CCString`) per batas modul, dimulai dari area paling lokal lalu baru kontrak
  antar-scene seperti loader, XML parser, dan gameplay core.
- Sekitar 97-100%: hapus alias/helper yang sudah tidak dipakai dari
  `Cocos2dx2Compat.h`, lalu lakukan build Android dan Windows final.

## Tahap Berikutnya

### Tahap 3: Gameplay Core

Status: **selesai dan sudah build-tested oleh pengguna**.

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

Commit awal tahap 3:

- `ecce01b refactor: migrate action core header types`
- `821871f refactor: migrate element frame api`
- `3f7fa89 refactor: migrate action manager frame api`
- `0229d2d refactor: migrate action manager scheduler selectors`
- `56981ee refactor: migrate character scheduler selectors`
- `79e334c refactor: migrate element action callbacks`
- `0cd115b refactor: migrate action manager direct callbacks`
- `3bac504 refactor: stabilize monster cleanup pointers`
- `13b92ef refactor: reload report frames for rematch`

Catatan progress terbaru:

- Tahap 3 sudah dibuild pengguna di Android dan game berhasil berjalan.

- Fix runtime setelah uji Android pengguna: frame report kill feed seperti
  `*_rpf.png` dan `slain.png` hilang pada match kedua setelah CHANGE/REMATCH.
  Alur rematch di `GameLayer::onLeft()` sekarang me-load ulang `Report.plist`
  bersama plist global lain sebelum masuk `LoadLayer`.
- Fix runtime setelah build Android pengguna: crash saat mengeluarkan monster
  ditelusuri ke cleanup `Monster::~Monster()` yang bisa me-release pointer
  belum terinisialisasi. `ActionManager` sekarang menginisialisasi pointer
  retained animation/callback ke `NULL`, dan urutan release `_monsterArray` di
  cleanup monster hero dibetulkan.
- `ActionManager.cpp` mulai dibersihkan dari callback action langsung yang tidak
  membawa payload `void*`: beberapa alur `disableShack`, `idle`, `dealloc`, dan
  `knockDown` sudah memakai `CallFunc` + `CC_CALLBACK_0`. Tipe action lokal di
  area terkait juga mulai diganti ke API 3.x seperti `Action`,
  `ActionInterval`, `FiniteTimeAction`, `DelayTime`, `MoveTo`, `JumpTo`,
  `FadeOut`, dan `Sequence`.
- `Element.cpp` mulai dibersihkan dari callback action lama di alur
  `Monster` dan `Bullet`: `CCCallFunc`/`callfunc_selector` diganti ke
  `CallFunc` + `CC_CALLBACK_0`, dan tipe action lokal di fungsi gerak/dealloc
  tersebut diganti ke nama API 3.x seperti `Action`, `ActionInterval`,
  `FiniteTimeAction`, `MoveTo`, `MoveBy`, `EaseIn`, `DelayTime`, dan
  `RepeatForever`.
- Potongan tahap 3 sudah build-tested oleh pengguna; sisa API 2.x di core
  sengaja tidak dipaksa habis pada tahap ini agar behavior gameplay tetap stabil.

### Tahap 4: Cleanup dan Compatibility Reduction

Status: **menjelang selesai; cleanup utility, ranking offline/dekoratif,
ranking node/action, penghapusan callback HTTP ranking, cleanup dasar `KTools`,
dan pengurangan compat touch sudah build-tested oleh pengguna di Android; commit
cleanup text/helper terbaru belum build-tested**.

Prioritas:

- `RankingLayer`
- `MyUtils/KTools`
- `MyUtils/CCStrokeLabel`
- `AppDelegate`
- `AppMacros`
- pengurangan isi `Cocos2dx2Compat.h`

Target migrasi:

- Mengurangi ketergantungan pada compatibility shim.
- Mengganti sisa helper lama ke API 3.x asli.
- Membersihkan UI ranking dan utility layer.
- Menonaktifkan jalur request online ranking lama, tetapi tetap mempertahankan
  tampilan ranking sebagai dekorasi/menu offline.
- Mengecek ulang Android dan Windows setelah semua file core stabil.

Commit awal tahap 4:

- `2305287 refactor: migrate utility touch and action helpers`
- `31ea26c refactor: migrate utility singleton helpers`
- `0602b67 refactor: migrate ranking touch and frame helpers`
- `8a394f4 refactor: keep ranking offline decorative`
- `7aad4b1 refactor: remove obsolete ranking request stub`
- `5a288a0 refactor: migrate ranking menu callbacks`
- `99d1d5b refactor: migrate ranking node action types`
- `4705b67 refactor: remove ranking http callbacks`
- `9f47577 refactor: migrate ktools helper basics`
- `f2bd820 refactor: drop unused touch delegate compat`
- `0022806 refactor: migrate ranking text helpers`
- `690dc85 refactor: migrate game over text helpers`

Catatan progress terbaru:

- `CCScrewLayer` dan `CCScrewItem` mulai memakai `Layer`, `Sprite`,
  `Touch`, `Event`, `Rect`, `Size`, serta listener touch 3.x langsung tanpa
  `CCTouchDelegate` compatibility helper.
- `CCShake` mulai memakai `ActionInterval`, `Node`, dan `Vec2`.
- `CCTips` di `KTools` mulai memakai base `Sprite` dan action callback 3.x.
- `HelloWorldScene` sample lama dibersihkan ke API scene/menu/sprite/label 3.x.
- `AppDelegate`, `AppMacros`, `KTools`, dan `CCStrokeLabel` dibersihkan dari
  beberapa singleton/type lama seperti `sharedEngine`, `sharedDirector`,
  `sharedUserDefault`, `sharedFileUtils`, `CCSize`, `CCNode`, `CCSprite`,
  `CCRenderTexture`, dan `CCTexture2D`.
- Cleanup utility awal sudah dibuild pengguna lewat Android Studio dan dimainkan
  di HP sekitar 2 match tanpa masalah terlihat.
- `RankingLayer` mulai dibersihkan secara bertahap: `RakingLayer` memakai
  `Layer`, `RankCharButton` memakai `Sprite` dan listener touch 3.x langsung,
  serta helper frame/audio ranking mulai memakai `SpriteFrameCache::getInstance`,
  `setSpriteFrame`, `UserDefault`, dan `SimpleAudioEngine::getInstance`.
- Jalur klik grup ranking lama tidak lagi membuat HTTP request; ranking tetap
  tampil sebagai hiasan offline dengan pesan/detail lokal.
- Stub request ranking lama yang sudah obsolete dihapus agar file tidak lagi
  terlihat seperti masih punya jalur request aktif.
- Cleanup ranking offline/dekoratif sudah dibuild pengguna lewat Android Studio
  dan dijalankan di HP tanpa crash.
- Callback menu aktif di `RankingLayer` mulai memakai `MenuItem`,
  `MenuItemSprite`, `MenuItemLabel`, `Menu`, `Ref`, dan `CC_CALLBACK_1`.
  Komentar sidebar lama yang masih menyimpan selector 2.x juga dihapus.
- Tipe node/action/posisi di `RankingLayer` mulai dibersihkan dari
  `CCSprite`, `CCLayer`, `CCNode`, `CCSpriteBatchNode`, `CCActionInterval`,
  `CCAction`, `CCMoveBy`, `CCRepeatForever`, dan `ccp(...)`.
- Commit ranking node/action sudah dibuild pengguna lewat Android Studio dan
  berjalan seperti biasa.
- Callback HTTP ranking lama dihapus karena tidak lagi punya jalur request aktif
  setelah ranking dijadikan dekorasi offline. `CCClippingNode`, `CCAnimation`,
  `CCAnimate`, dan `CCDelayTime` di ranking juga mulai dibersihkan ke API 3.x.
- `KTools` dibersihkan sedikit lagi tanpa menyentuh kontrak besar: base class
  utama memakai `Ref`, include `CCFileUtils` lama dihapus, dan label tips lokal
  memakai `Label::createWithSystemFont`.
- `Cocos2dx2Compat.h` dikurangi dari touch delegate shim lama yang sudah tidak
  dipakai (`CCTouchDelegate`, `addTargetedDelegate`, dan `removeDelegate`).
  Alias HTTP, EditBox, Scale9, ScrollView, time, array, string, dan sequence
  masih dipertahankan karena masih ada pemakai aktif.
- Commit penghapusan callback HTTP ranking, cleanup dasar `KTools`, dan
  pengurangan compat touch terbaru sudah dibuild pengguna lewat Android Studio
  dan berjalan lancar.
- Text helper ranking dibersihkan lagi dari `CCLabelTTF`: `RankingLayer` dan
  `CCStrokeLabel` memakai `Label::createWithSystemFont`, sementara loader file
  ranking memakai `FileUtils::getInstance`.
- Text helper `GameOver` dibersihkan untuk label CHANGE/REMATCH/change-character
  dan akses `SHA.xml` memakai `FileUtils::getInstance`.
- Commit cleanup text/helper terbaru belum dibuild ulang oleh Codex sesuai
  aturan kerja; perlu uji Android pengguna sebelum tahap 4 ditutup.

### Tahap 5: Container/Data dan Sisa Action Bridge

Status: **berjalan; commit awal `ActionManager` sudah build-tested oleh
pengguna di Android, commit cleanup `Effect` terbaru belum build-tested**.

Prioritas:

- Audit pemakai `Cocos2dx2Compat.h`.
- Bersihkan adapter paling lokal lebih dulu.
- Migrasi `CCArray`, `CCDictionary`, dan `CCString` per batas modul.
- Lanjutkan dari area paling aman sebelum masuk kontrak besar di
  `ActionManager`, `Element`, `Characters`, loader XML, dan scene gameplay.

Target migrasi:

- Mengurangi pemakaian alias/helper sequence dan animation lama yang masih
  bergantung pada compatibility bridge.
- Menjaga `CCArray`, `CCDictionary`, dan `CCString` tetap hidup di batas modul
  yang masih menjadi kontrak gameplay sampai ada migrasi data yang lebih aman.
- Tidak mencampur pembersihan flow online/dekoratif lama dengan migrasi
  container besar agar error build lebih mudah dilacak.

Commit awal tahap 5:

- `7dc2520 refactor: migrate action manager local action aliases`
- `c9a65db refactor: migrate effect sequence helpers`

Catatan progress terbaru:

- Audit awal menunjukkan `CCArray`, `CCDictionary`, dan `CCString` masih dipakai
  luas lintas `ActionManager`, `Element`, `Characters`, scene awal, loader XML,
  dan UI, sehingga belum aman dimigrasi sekaligus.
- `ActionManager.cpp` dibersihkan dari sisa alias action/animation 2.x lokal
  seperti `CCAnimation`, `CCAnimate`, `CCAction`, `CCRepeatForever`,
  `CCSequence`, `CCMoveBy`, `CCFadeOut`, `CCSpawn`, `CCScaleBy`, dan
  `CCDelayTime`.
- Cleanup ini tetap mempertahankan `CCArray` sebagai container sequence/frame
  internal karena helper compat `createSequence` dan
  `createAnimationWithSpriteFrames` masih menjadi jembatan aman untuk data lama.
- Commit awal `ActionManager` sudah dibuild pengguna di Android dan berjalan
  aman.
- `Effect.cpp` dibersihkan dari helper compat sequence/animation lokal:
  sequence sederhana memakai `Sequence::create`, frame animasi lokal memakai
  `Vector<SpriteFrame*>`, dan animasi dibuat langsung lewat
  `Animation::createWithSpriteFrames`.
- Setelah commit cleanup `Effect` lulus build Android, langkah berikutnya adalah
  memilih satu adapter lokal lagi, misalnya sequence helper di `HudLayer` atau
  area kecil `Element`, sebelum menyentuh migrasi container besar.
- Alias HTTP/EditBox/Scale9/Scroll di `Cocos2dx2Compat.h` belum dihapus karena
  masih punya pemakai aktif.

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
