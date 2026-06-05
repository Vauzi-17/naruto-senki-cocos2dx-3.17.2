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
| Windows build | Win32 Debug VS2022 berhasil menurut uji pengguna sebelum commit `929720f`; perlu uji ulang setelah commit terbaru |
| Push remote | Tidak dilakukan |
| Build oleh Codex | Tidak dilakukan |

## Estimasi Progress Migrasi

Estimasi progress menuju kode full cocos2d-x 3.17.2: **sekitar 98%**.

Alasan estimasi:

- Alur scene utama sudah dimigrasi dari menu awal sampai masuk gameplay.
- Area HUD dan layer gameplay pendukung sudah banyak memakai API 3.x.
- Build Android sudah berhasil dan game bisa dimainkan setelah cleanup tahap 4
  awal serta setelah ranking dibuat offline/dekoratif; pengguna sudah menjalankan
  game di HP tanpa crash.
- Area gameplay core sudah mulai distabilkan lewat migrasi callback/action/frame
  tanpa mengubah kontrak data lama secara paksa.
- Tahap 5 sudah dimulai secara aman dari area lokal `ActionManager` dan helper
  UI lokal: sisa alias action/animation 2.x di beberapa helper internal
  dibersihkan, lalu container item `CCScrewLayer` mulai dipindah dari `CCArray`
  ke `Vector` tanpa mengubah kontrak gameplay besar.
- Bridge action/animation lokal di `ActionManager` berkurang lagi: pemanggilan
  langsung helper compat untuk pembuatan sequence/animation sudah dipindahkan ke
  helper lokal berbasis `Vector`, sementara input `CCArray` lama tetap dijaga.
- Adapter container child node di flow ougis/pause-resume mulai dibersihkan:
  `GameLayer` dan `HudLayer` sekarang mengiterasi `getChildren()` langsung tanpa
  membuat `CCArray` sementara.
- Adapter `ValueVector` ke `CCArray` untuk object layer TMX mulai dihapus:
  spawn point dan meta tower di `GameLayer` membaca `ValueMap` 3.x langsung.
- Helper render lama `cocos2dx2_compat::visitNode` sudah tidak dipakai; snapshot
  pause/gear/game-over dan `CCStrokeLabel` sekarang memanggil `Node::visit`
  API 3.x langsung.
- Helper waktu lama `CCTime`/`cc_timeval` sudah tidak dipakai; cooldown tombol
  dan buff timer lokal memakai helper `std::chrono::steady_clock`.
- State cooldown internal `ActionButton` sudah lepas dari retained `CCString`;
  nilai cooldown/countdown disimpan sebagai integer lokal dan caller aktif
  memakai `setCD(int)` langsung.
- Helper global `Defines.h` mulai lepas dari alias sprite/texture/singleton
  2.x; helper scale/pixel-art sekarang memakai `Sprite`, `Texture2D`,
  `SpriteFrame`, `Director`, dan `UserDefault` 3.x langsung.
- Sisa field label lama yang tidak punya caller aktif di `ActionManager`
  dibuang, dan helper lokal shadow/voice/display kecil mulai memakai tipe
  `Sprite`, `Scene`, `Size`, `Director`, serta `UserDefault` 3.x langsung.
- Tanda tangan callback lokal `ActionManager` mulai lepas dari alias `CCNode*`
  ke `Node*`, dan deklarasi action sementara di jalur parser lokal memakai
  `FiniteTimeAction` langsung.
- Callback action tanpa payload di `ActionManager` mulai memakai `CallFunc`
  dan lambda/`CC_CALLBACK_0`, sehingga sisa `callfunc_selector` dan
  `callfuncN_selector` aktif hilang dari file tersebut.
- Alias posisi/hitbox lokal di `ActionManager` mulai dibersihkan: pemakaian
  `CCPoint`, `CCPointMake`, `CCRect`, dan `CCRectMake` aktif diganti ke
  `Vec2` dan `Rect`.
- Geometry lokal di `Element` ikut mulai dibersihkan: posisi/anchor kecil,
  arah AI `Flog`/`Monster`, dan marker bullet memakai `Vec2` serta operasi
  vektor 3.x langsung.
- Alias posisi/arah karakter di `Characters.cpp` mulai dibersihkan dari
  `CCPoint`/`ccp(...)` ke `Vec2`, dan jarak lokal sederhana sudah memakai
  `Vec2::getDistance`. Normalisasi arah dari variabel lokal sederhana sudah
  memakai `Vec2::getNormalized`; selisih posisi sederhana target/master/spawn
  serta nested target/origin sudah memakai operator `Vec2` langsung, sehingga
  `Characters.cpp` lepas dari helper `ccpSub`.
- Konstruktor point lokal di `ActionManager.cpp` mulai dipindahkan dari
  `ccp(...)` ke `Vec2(...)`; cleanup delta/distance besar di file ini tetap
  dipisahkan dari slice konstruktor.
- Delta dan jarak target sederhana di `ActionManager.cpp` ikut mulai memakai
  operator `Vec2` dan `Vec2::getDistance`, sehingga helper `ccpSub` dan
  `ccpDistance` aktif sudah hilang dari file tersebut. Normalisasi arah konstan
  juga memakai `Vec2::getNormalized`.
- Audio klik halaman di `NetworkLayer` sudah memakai
  `SimpleAudioEngine::getInstance()` tanpa mengubah flow pemilihan hero.
- Loop iterator legacy `CCARRAY_FOREACH` di `Characters.cpp` sudah dibersihkan
  penuh ke loop indeks `CCArray::count()` / `objectAtIndex(...)`, sehingga AI
  hero lokal tidak lagi bergantung pada macro iterator 2.x.
- Sebagian loop `GameLayer.cpp` yang masih lokal dan aman ikut dibersihkan:
  spawn hero awal, start setup duel/training, cek tower, rematch handoff, dan
  cleanup frame karakter sekarang memakai loop indeks eksplisit tanpa mengubah
  kontrak `CCArray` lama.
- Scene awal yang diminta dicek ikut dibersihkan: `StartMenu`, `LoadLayer`, dan
  `NetworkLayer` sekarang tidak lagi memakai `CCARRAY_FOREACH` aktif.
- State pilihan training lokal di `NetworkLayer` sudah lepas dari retained
  `CCString*`/`CCArray*` dan memakai `std::string` serta
  `std::vector<std::string>`, sementara payload `tempHeros` ke `LoadLayer`
  tetap `CCArray` agar kontrak gameplay lama belum berubah besar.
- UI perimeter berikutnya ikut dibersihkan dari `CCARRAY_FOREACH`:
  `SelectLayer`, `SkillLayer`, `GearLayer`, dan `GameOver` sekarang memakai
  loop indeks atau container STL lokal untuk iterator yang aman.
- Helper random roster lokal `SelectLayer` sudah memakai
  `std::vector<std::string>` untuk daftar hero terpakai, sementara output battle
  tetap `CCArray`/`CCDictionary` sebagai kontrak lama ke loader.
- Area HUD gameplay yang lebih dekat runtime ikut dibersihkan:
  `HudLayer`/`HPBar` tidak lagi memakai `CCARRAY_FOREACH` aktif untuk map icon,
  gear button, report queue, enemy HP, dan pengecekan reward/share team.
- Iterator core terbaru ikut dibersihkan: `ActionManager.cpp` dan `Element.cpp`
  sekarang tidak lagi memakai `CCARRAY_FOREACH` aktif; loop tetap memakai
  `CCArray::count()` / `objectAtIndex(...)` agar kontrak gameplay lama belum
  berubah besar.
- Masih ada sisa API 2.x yang cukup besar pada kontrak data `ActionManager`,
  `Element`, `RankingLayer`, beberapa utility UI, app delegate, dan compatibility bridge.
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
- Sekitar 95-97%: migrasi container/data lama (`CCArray`, `CCDictionary`,
  `CCString`) per batas modul, dimulai dari area paling lokal lalu baru kontrak
  antar-scene seperti loader, XML parser, dan gameplay core.
- Sekitar 97-100%: hapus alias/helper yang sudah tidak dipakai dari
  `Cocos2dx2Compat.h`, lalu lakukan build Android dan Windows final.

Urutan langkah berikutnya menuju 100%:

Posisi saat ini: **akhir langkah 5 menuju langkah 6**. Helper compat kecil
yang benar-benar lokal sudah mulai ditutup (`visitNode`, `CCTime`/`cc_timeval`,
`CCDictElement`, sebagian state cooldown `ActionButton`, `CCScale9Sprite`,
`CCScrollView`, `CCEditBox`, macro `atoi`, dan alias HTTP `CCHttp*`). Area
UI/menu dan HUD lokal juga sudah banyak lepas dari `CCARRAY_FOREACH`; pekerjaan
berikutnya bergeser ke kontrak data lebih besar seperti parser XML, ranking
string/XML, dan kontrak data `ActionManager`/`Element`.

Update terbaru (belum build-tested oleh Codex sesuai aturan kerja):

- `ActionButton`: overload `setCD(CCString*)` dihapus; caller `HudLayer` dan
  `ActionManager` sekarang memakai `setCD(int)` langsung.
- `StartMenu`: pemakaian `CCScale9Sprite` di area input panel diganti ke
  `cocos2d::ui::Scale9Sprite`.
- `GameOver`: panel change-character tidak lagi memakai `CCScale9Sprite` dan
  `CCScrollView`; sudah memakai `cocos2d::ui::Scale9Sprite` + `ScrollView`
  native cocos2d-x 3.x.
- `SkillLayer`: callback HTTP refresh skill distub (jalur offline).
- `Cocos2dx2Compat.h`: alias mati `CCScale9Sprite` dan `CCScrollView` beserta
  include/konstanta scroll legacy dihapus setelah tidak ada pemakai aktif.
- `StartMenu`: field dan pembuatan input box lokal dipindah dari alias
  `CCEditBox`/konstanta `kEditBox*` ke `cocos2d::ui::EditBox` native, lalu
  alias `CCEditBox` dihapus dari `Cocos2dx2Compat.h`.
- `Cocos2dx2Compat.h`: macro `atoi` dihapus; pemakai source game diganti ke
  `std::atoi` dengan include `<cstdlib>` eksplisit. File third-party SQLite
  tidak disentuh.
- `SkillLayer` dan `GameOver`: sisa parsing/request HTTP yang sudah mati di
  jalur offline dibersihkan. Callback stub tetap dipertahankan, tetapi tidak
  lagi menyimpan akses `CCHttpResponse` atau request upload legacy.
- `ActionManager` dan `Characters`: iterator lokal `CCObject* pObject` hasil
  `objectAtIndex()` diganti ke `Ref*`, tanpa mengubah kontrak `CCArray` lama.
- `HPBar`: iterator lokal share/reward team hasil `objectAtIndex()` ikut
  diganti ke `Ref*`, tetap mempertahankan `CCArray` lama dari gameplay.
- `ActionManager`: field `_slayer`/`_sticker` dan callback notification
  `acceptAttack` diganti dari `CCObject*` ke `Ref*`, sesuai callback object
  cocos2d-x 3.x.
- `ActionManager`: dua helper geometry lama `ccpAdd`/`ccpMult` terakhir
  diganti ke operasi `Vec2` langsung pada update posisi jalan.
- `StartMenu`, `HudLayer`, dan `SkillLayer`: path frame animasi lokal untuk
  avatar/start/blink diganti dari `CCString::createWithFormat` ke
  `cocos2d::StringUtils::format` + `std::string`.
- `HudLayer`, `GameLayer`, `GameOver`, dan `SkillLayer`: format teks/path UI
  lokal untuk coin, timer, result avatar/stat, dan tombol skill mulai memakai
  `StringUtils::format`/`std::string` tanpa mengubah payload gameplay.
- `GameOver`: format reward coin dan score label lokal ikut lepas dari
  `CCString::createWithFormat`; record/upload string yang masih menjadi kontrak
  lama tetap ditahan.
- `GameOver`: path gear icon, path tombol pilihan hero result, dan label target
  change-character lokal memakai `StringUtils::format`/`std::string`.
- `SkillLayer`: path full art hero dan label explain lokal dipindahkan dari
  `CCString` ke `std::string`, tanpa menyentuh data SQLite/record.
- `GearLayer`: path icon/detail gear dan label coin lokal memakai
  `StringUtils::format`/`std::string`, sementara isi gear array lama ditahan.
- `HudLayer`: path avatar/skill button dan label HP/EXP lokal mulai memakai
  `StringUtils::format`/`std::string` tanpa menyentuh report dictionary.
- `HudLayer`: start splash, display buff, report sprite, dan cut-in ougis lokal
  ikut memakai string 3.x/native; dictionary report dan CKR tetap ditahan.
- `GameLayer`: path cleanup sprite frame, battle music, dan ougis effect lokal
  memakai `StringUtils::format`/`std::string` tanpa mengubah state gameplay.
- `SkillLayer`: constructor tombol select, path intro voice, dan label bond
  lokal memakai string native/`StringUtils::format`; data SQLite tetap ditahan.
- `StartMenu`: profile/nameplate point label dan path asset tombol nameplate
  lokal memakai `StringUtils::format`/`std::string`; coin SQLite tetap ditahan.
- `GameOver`: `detailRecord` dan string angka win record aktif memakai
  `StringUtils::format`/`std::string`, tanpa mengubah isi nilai SQLite.
- `SelectLayer`: path tombol select dan preview hero lokal memakai
  `StringUtils::format`/`std::string`; field `charName` lama tetap `CCString`.
- `NetworkLayer`: path tombol select, preview hero, map preview/label, dan slot
  team training lokal memakai `StringUtils::format`/`std::string`; payload hero
  ke loader tetap `CCString`/`CCArray`.
- `Effect`: path frame effect dan helper animasi effect lokal memakai
  `StringUtils::format`/`std::string`; flow damage/buff effect tidak diubah.
- `LoadLayer`: path plist/texture preload, avatar loading, tips, dan checksum
  lokal memakai `StringUtils::format`/`std::string`; kontrak `tempHeros` lama
  tetap `CCArray`/`CCDictionary`.
- `RankingLayer`: label ranking, path nameplate/banner/blink, score, group,
  dan tombol hero lokal memakai `StringUtils::format`/`std::string`; field
  tombol rank lama tetap `CCString`.
- `HPBar`: path heart-effect, label kill/team, total kill, dan reward coin
  lokal memakai `StringUtils::format`/`std::string`; field kill lama tetap
  `CCString`.
- `GameLayer`: nilai tower HP lokal dan update `TotalTM` tidak lagi memakai
  `CCString::createWithFormat`; field lama tetap `CCString`.
- `GameOver`: blok komentar achievement lose lama yang masih membawa format
  `CCString` dead-code dihapus; flow result aktif tidak berubah.
- `StartMenu`: simpan coin nameplate lokal memakai `StringUtils::format`/
  `std::string`, dan komentar legacy yang membawa format `CCString` lama
  dibersihkan.
- `LoadLayer`: blok komentar DLC lama yang masih membawa `CCString`/
  `CCArray`/`CCDictionary` dead-code dihapus; preload aktif tetap sama.
- `HudLayer`: angka report queue, countdown buff, dan pengurangan CKR lokal
  memakai literal/string native; field report/CKR lama tetap `CCString`.
- `Element`: path XML lokal untuk hero/flog/tower/monster/bullet, frame
  `hp_bottom`, label reborn, dan kill count awal memakai string native/literal;
  kontrak stat HP/attack lama tetap `CCString`.
- `ActionManager`: path heart-effect, reward coin display, damage font, gear
  item/cost, dan extern frame lokal memakai string native; kontrak gear array
  dan stat combat lama tetap `CCString`.
- `ActionManager`: helper payload callback lokal memakai `CallFuncN` 3.x dengan
  `Node*` runtime dan guard null; payload XML lama tetap dipertahankan.
- `KTools`: helper keycode MD5 lokal memakai `StringUtils::format`/
  `std::string`; builder SQL lama tetap ditahan.
- `KTools`: helper read SQLite lokal mulai memakai `std::string` untuk query
  select dan hasil decode; return type lama tetap `CCString*`.
- `KTools`: sisa builder SQL update/insert lokal sudah tidak memakai
  `CCString::createWithFormat`; jalur SQLite lama tetap memakai query yang sama
  dan return/caller lama tidak diubah.
- `NetworkLayer`: komentar request list hero lama dan parsing response callback
  yang sudah tidak punya caller aktif dibersihkan; flow pemilihan hero tetap
  memakai daftar offline statis.
- `GameLayer`: heartbeat HTTP lama di `checkPost` dibersihkan menjadi stub
  offline karena scheduler aktifnya sudah lama dikomentari.
- `LoadLayer`: jalur download DLC XML lama dibersihkan; preload sekarang selalu
  lanjut ke `onLoadFinish` karena pengisian `DLCArray` aktif sudah tidak ada.
- `StartMenu`: notice offline dibuat sepenuhnya lokal. Request/comment notice
  lama dibersihkan, dan konfirmasi notice hanya memperbarui label lokal.
- `StartMenu`: profile/nameplate dibuat lokal/offline. Pembukaan profil tidak
  lagi menunggu request ranking, callback lama distub, dan pilihan nameplate
  disimpan di `UserDefault` sambil tetap memakai biaya coin lokal lama.
- `StartMenu`: jalur login lama yang sudah tertutup pesan maintenance ikut
  dibersihkan dari request HTTP; input lokal hanya menyimpan nama/password
  seperti sebelumnya lalu kembali ke mode offline.
- `StartMenu`: fitur group online lama ditutup sebagai stub offline karena
  state login online sudah tidak mengaktifkannya; sisa `CCHttp*` hilang dari
  source game dan alias HTTP lama di `Cocos2dx2Compat.h` dihapus.
- `Defines.h`: helper scale sprite dan pixel-art dibersihkan dari nama
  `CCSprite`, `CCTexture2D`, `CCSpriteFrame`, `CCSize`, `sharedDirector()`,
  dan `sharedUserDefault()`.
- `ActionManager`: field `cpLabel`/`CCLabelTTF` mati dihapus, lalu helper lokal
  shadow, display coin, voice check, dan shake scene mulai memakai tipe 3.x
  langsung tanpa menyentuh kontrak action parser/XML.
- `ActionManager`: tanda tangan callback payload lokal diganti dari `CCNode*`
  ke `Node*`, dan deklarasi lokal `CCFiniteTimeAction` di jalur parser/action
  diganti ke `FiniteTimeAction`.
- `NetworkLayer`: audio klik page selector memakai
  `SimpleAudioEngine::getInstance()`; flow training/5v5 dan payload hero tetap
  memakai kontrak lama.
- `Cocos2dx2Compat.h`: belum dihapus walaupun isinya minimal, karena CMake dan
  project Win32 masih memaksa include file ini untuk semua compile unit.
- `ActionManager`: callback action tanpa payload seperti `getCollider`,
  `setActionResume`, `setActionResume2`, `setTransform`, `setOugis`,
  `setCharFlip`, shadow helper, damage display, dan coin display memakai
  `CallFunc` 3.x. Callback payload XML/void pointer lama sekarang juga
  memakai helper `createPayloadCall(...)` berbasis `CallFunc` lambda.
- `ActionManager`: alias `CCPoint`/`CCRect` lokal dibersihkan ke `Vec2`/`Rect`
  untuk hitbox, posisi, direction, dan helper pencarian target. Helper
  `ccp(...)`, `ccpSub`, `ccpNormalize`, dan `ccpDistance` masih ditahan untuk
  potongan berikutnya karena jumlah pemakai aktifnya lebih besar.
- `Element`: pemakaian aktif `CCPoint`, `ccp(...)`, `ccpSub`, dan
  `ccpNormalize` di helper lokal `HeroElement`, `Flog`, `Tower`, `Monster`, dan
  `Bullet` diganti ke `Vec2`, pengurangan vektor, dan `getNormalized()`.
- `ActionManager` dan `Element`: sisa `CCARRAY_FOREACH` aktif dibersihkan ke
  loop indeks eksplisit berbasis `CCArray::count()` / `objectAtIndex(...)`.
  Source game aktif sekarang tidak lagi memiliki macro iterator tersebut, tetapi
  container `CCArray` lama tetap dipertahankan sebagai kontrak gameplay.
- `ActionManager`: factory callback payload lama `CCCallFuncND` /
  `callfuncND_selector` diganti ke helper lokal `createPayloadCall(...)`
  berbasis `CallFunc` lambda. Payload `void*` dan dictionary lama tetap
  dipertahankan agar kontrak parser XML belum berubah.
- `ActionManager`: macro dictionary parser `CCDICT_FOREACH` di
  `createAnimation(...)` diganti ke iterasi `HASH_ITER` langsung atas
  `DictElement`, sehingga source aktif tidak lagi memakai macro dictionary
  lama tanpa mengubah isi `CCDictionary` parser XML.
- `ActionManager`: alias sprite lifecycle kecil dibersihkan dari
  `CCSprite::onEnter`, `CCSprite::onExit`, dan pembuatan heart-effect
  lokal ke `Sprite` 3.x langsung.
- `ActionManager` dan `Element`: alias label bitmap lokal `CCLabelBMFont`
  diganti ke `LabelBMFont` 3.x langsung untuk damage/reward display dan
  timer reborn.
- `ActionManager`: beberapa `CCObject*` lokal di helper animation/sequence,
  parser action, dan damage display diganti ke `Ref*` tanpa mengubah
  container `CCArray` lama.
- `Element`: iterator lokal hasil `objectAtIndex()` di dealloc/AI kecil
  diganti dari `CCObject*` ke `Ref*`, tetap tanpa mengubah container lama.
- `ActionManager` dan `Characters`: iterator lokal `CCObject* pObject` hasil
  `objectAtIndex()` ikut diganti ke `Ref*` di loop core yang masih sengaja
  mempertahankan `CCArray`.
- `HPBar`: iterator lokal team reward/share juga sudah memakai `Ref*`, sehingga
  sisa `CCObject*` aktif tinggal kontrak callback/field `ActionManager`.
- `ActionManager`: kontrak callback/field terakhir yang memakai `CCObject*`
  ikut dipindahkan ke `Ref*`; audit source aktif `Classes` sekarang tidak lagi
  menemukan `CCObject`.
- `ActionManager`: helper `ccpAdd`/`ccpMult` terakhir juga sudah hilang; sisa
  pekerjaan besar tetap di kontrak data `CCArray`/`CCDictionary`/`CCString`.
- UI helper lokal mulai mengurangi `CCString` untuk format path asset animasi
  yang tidak menjadi payload gameplay.
- UI result/HUD lokal ikut mengurangi `CCString::createWithFormat` untuk label
  dan path sekali pakai; kontrak `CCString` gameplay masih ditahan.
- `GameOver` reward/result label bertahap dipindahkan ke `std::string`, sambil
  menahan string SQLite/upload yang masih menjadi kontrak lama.
- `GameOver` picker/result UI juga mulai lepas dari `CCString` untuk format path
  icon dan label lokal.
- `SkillLayer` mulai mengurangi `CCString` path asset lokal di panel detail
  skill, sementara data record dan parameter hero lama tetap ditahan.
- `GearLayer` ikut mengurangi `CCString::createWithFormat` pada path asset dan
  label lokal tanpa mengubah kontrak gear list.
- `HudLayer` ikut mengurangi `CCString::createWithFormat` pada path button dan
  label status lokal, sementara report/CKR lama tetap ditahan.
- `HudLayer` display-only berikutnya ikut dipindahkan ke string native untuk
  start splash, buff/report/ougis asset path tanpa mengubah payload report lama.
- `GameLayer` path asset/audio sekali pakai ikut dikurangi dari
  `CCString::createWithFormat`; `TotalTM`, tower HP, dan report lama ditahan.
- `SkillLayer` ikut mengurangi `CCString::createWithFormat` untuk path/audio
  dan label UI lokal tanpa menyentuh record SQLite.
- `StartMenu` nameplate UI lokal ikut mengurangi `CCString::createWithFormat`
  pada label/path asset, sambil menahan penyimpanan coin lama.
- `GameOver` mulai melepas string record aktif dari `CCString::createWithFormat`
  sambil mempertahankan format data SQLite lama.
- `SelectLayer` mulai melepas path UI lokal dari `CCString::createWithFormat`
  tanpa mengubah payload karakter yang masih dipakai layer lama.
- `NetworkLayer` ikut melepas path/label UI lokal dari
  `CCString::createWithFormat`, sambil menahan kontrak hero offline ke loader.
- `Effect` melepas path frame effect lokal dari `CCString::createWithFormat`
  tanpa menyentuh kontrak damage/buff di `ActionManager`.
- `LoadLayer` melepas path preload aktif dari `CCString::createWithFormat`
  sambil menahan payload hero lama ke `GameLayer`.
- `RankingLayer` melepas format string UI aktif dari
  `CCString::createWithFormat`, sambil menahan field pilihan hero lama.
- `HPBar` melepas format string display/reward lokal dari
  `CCString::createWithFormat` tanpa mengubah rumus kill, EXP, atau coin.
- `GameLayer` menutup sisa format string aktif untuk tower HP dan timer total
  tanpa mengubah kontrak field gameplay lama.
- `GameOver` membersihkan dead comment legacy achievement supaya audit source
  aktif tidak lagi membawa sisa format `CCString` lama.
- `StartMenu` menutup sisa format string aktif di nameplate coin dan membuang
  komentar API 2.x lama yang tidak aktif.
- `LoadLayer` membersihkan komentar DLC lama yang sudah tidak aktif agar audit
  source fokus pada kontrak loader yang masih dipakai.
- `HudLayer` menutup sisa format string aktif untuk report/buff/CKR tanpa
  mengubah dictionary report atau field chakra lama.
- `Element` mulai melepas path XML dan display kecil dari
  `CCString::createWithFormat`, sambil menahan migrasi stat gameplay besar.
- `ActionManager` mulai mengurangi format string display/path lokal tanpa
  menyentuh nilai HP/CKR/attack yang masih menjadi kontrak combat utama.
- `ActionManager` helper `createPayloadCall(...)` dirapikan ke `CallFuncN`
  sehingga callback payload tetap menerima node action 3.x.
- `KTools::getKeycode(...)` melepas format `CCString` lokal tanpa menyentuh
  SQL/SQLite helper yang masih menjadi kontrak utility lama.
- `KTools` read helper mulai melepas format `CCString` pada query/result yang
  lokal, sambil menahan update/insert SQL lama untuk slice berikutnya.
- `KTools` menutup sisa format `CCString` pada builder SQL update/insert lokal,
  sehingga sisa format angka/path aktif tinggal di area core combat.

1. **Tutup helper compat kecil yang masih lokal**
   - Target file: `Cocos2dx2Compat.h`, `ActionButton`, `HPBar`, `HudLayer`,
     `GameLayer`, `GameOver`, `RankingLayer`, `CCStrokeLabel`, dan helper kecil
     di `MyUtils`.
   - Fokus: hapus alias/helper yang callernya sedikit atau sudah jelas
     penggantinya, seperti alias dictionary/time/render yang tersisa, tanpa
     menyentuh kontrak XML/gameplay besar.
   - Syarat lanjut: Android build pengguna tetap hijau setelah tiap potongan.

2. **Bersihkan UI dan menu yang kontraknya relatif lokal**
   - Target file: `ActionButton`, `HPBar`, `GearLayer`, `GameOver`,
     `RankingLayer`, `SelectLayer`, `SkillLayer`, dan bagian offline/dekoratif
     di `StartMenu`.
   - Fokus: migrasi sisa `CCString`, `CCArray`, `CCDictionary`, dan callback
     HTTP lama yang tidak lagi dibutuhkan oleh game offline.
   - Batas aman: jangan mengubah payload hero/team yang masih dikirim ke
     `LoadLayer`, `GameLayer`, `Element`, atau `ActionManager` sebelum kontrak
     data gameplay disiapkan.

3. **Pisahkan jalur online legacy dari jalur offline aktif**
   - Target file: `StartMenu`, `GameOver`, `LoadLayer`, `SkillLayer`,
     `NetworkLayer`, dan alias HTTP di `Cocos2dx2Compat.h`.
   - Fokus: nonaktifkan atau ubah menjadi stub lokal untuk request lama yang
     sudah tidak dipakai, lalu hapus `CCHttpRequest`, `CCHttpClient`, dan
     `CCHttpResponse` setelah caller aktif hilang.
   - Batas aman: ranking/dekorasi dan flow masuk game harus tetap muncul
     seperti biasa meskipun request online lama dibersihkan.

4. **Migrasi kontrak scene awal ke data 3.x**
   - Target file: `StartMenu`, `NetworkLayer`, `SkillLayer`, `SelectLayer`,
     `LoadLayer`, `GameLayer`.
   - Fokus: ganti hero/team list dari `CCArray`/`CCDictionary`/`CCString` ke
     `Vector`, `ValueVector`, `ValueMap`, `std::vector`, atau struct kecil
     sesuai batas modul.
   - Batas aman: lakukan per flow, misalnya training, 5v5, rematch, lalu loader;
     jangan mengubah semua handoff sekaligus.

5. **Migrasi parser XML karakter**
   - Target file: `MyUtils/KTools.h`, `MyUtils/KTools.cpp`, `Element.cpp`,
     `ActionManager.h`, `ActionManager.cpp`.
   - Fokus: ubah output `readXMLToArray` dari container lama menjadi struktur
     action/frame 3.x yang eksplisit, lalu sesuaikan pembaca di `Element` dan
     `ActionManager`.
   - Batas aman: pertahankan urutan action lama, key XML lama, dan behavior
     callback skill; ini harus dipecah sangat kecil karena menyentuh semua
     karakter.

6. **Migrasi gameplay core data**
   - Target file: `ActionManager.h`, `ActionManager.cpp`, `Element.h`,
     `Element.cpp`, `Characters.cpp`, `GameLayer.h`, `HudLayer.h`.
   - Fokus: ganti sisa `CCArray`, `CCDictionary`, `CCString`, `CCObject`,
     `CCARRAY_FOREACH`, callback `CCCallFunc*`, `ccp`, `CCRect`, dan `CCPoint`
     yang masih hidup di core.
   - Batas aman: mulai dari field yang paling lokal seperti cooldown, gear,
     damage list, monster list, lalu baru identitas hero/stat/skill yang
     dipakai lintas class.

7. **Hapus compatibility bridge final**
   - Target file: `Cocos2dx2Compat.h` dan semua include/caller tersisa.
   - Fokus: hapus alias HTTP dan include deprecated yang sudah tidak diperlukan.
   - Syarat lanjut: `rg` untuk pola API 2.x utama harus bersih atau hanya
     menyisakan komentar/third-party yang memang bukan source game.

8. **Validasi akhir 100%**
   - Target area: Android `armeabi-v7a`, Windows build bila masih ditargetkan,
     dan smoke test gameplay.
   - Fokus uji: start menu, character select, training, 5v5, normal match,
     rematch/change, monster/summon/clone, ougis, pause/gear, game over, dan
     ranking/dekorasi offline.
   - Selesai bila: tidak ada pemakai aktif API 2.x di source game, compat header
     sudah minimal atau hilang, Android build hijau, dan gameplay utama lolos
     uji pengguna.

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
  Alias time, array, string, dan beberapa bridge data masih dipertahankan karena
  masih ada pemakai aktif; alias HTTP/EditBox/Scale9/ScrollView sudah tidak aktif di source
  game.
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

Status: **berjalan; commit `ActionManager`, `Effect`, `HudLayer`, dekoratif UI,
scene/loading, dan `StartMenu` sudah build-tested oleh pengguna di Android,
commit `Element` terbaru sudah build-tested oleh pengguna di Android, commit
`ActionManager` lokal, loop `Characters`/`GameLayer`, scene selection/UI
iterator, dan iterator `HudLayer`/`HPBar` terbaru belum build-tested**.

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
- `7061c4b refactor: migrate hud sequence helpers`
- `dca5a44 refactor: migrate decorative sequence helpers`
- `8c16cf1 refactor: migrate scene animation helpers`
- `859af1f refactor: migrate start menu sequence helpers`
- `5eb669c refactor: migrate element local sequence helpers`
- `bb4b45d refactor: migrate action manager local sequences`
- `1ee962e refactor: migrate action manager local animation helpers`
- `6d20d1d refactor: migrate screw layer item container`
- `8a94ef6 refactor: migrate child iteration containers`
- `3e2a623 refactor: migrate map object containers`
- `66f039f refactor: restore game layer rematch iterator`
- `115a026 refactor: remove visit node compat helper`
- `31829c8 refactor: migrate legacy time helper`
- `732a705 refactor: remove dict element compat alias`
- `64f8937 refactor: migrate action button cooldown state`
- `6decb79 refactor: migrate core iterator loops`
- `56e7128 refactor: migrate scene selection containers`
- `a67d691 refactor: migrate ui iterator containers`
- `ca36714 refactor: migrate hud hpbar iterators`

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
- Commit cleanup `Effect` sudah dibuild pengguna di Android dan berjalan aman.
- `HudLayer.cpp` dibersihkan dari helper compat sequence/animation lokal untuk
  animasi start match, pulse mark skill, clear buff, SPC report, dan cutline
  ougis. Area ini memakai `Sequence::create`, `Vector<SpriteFrame*>`, serta
  `Animation::createWithSpriteFrames` langsung.
- Commit cleanup `HudLayer` sudah dibuild pengguna di Android dan berjalan aman.
- `GameOver.cpp` dan `RankingLayer.cpp` dibersihkan dari sisa helper compat
  sequence/animation dekoratif: result popup, cloud ranking, dan banner blink
  memakai `Sequence::create`, `Vector<SpriteFrame*>`, serta
  `Animation::createWithSpriteFrames` langsung.
- Commit dekoratif sudah dibuild pengguna di Android dan berjalan aman.
- `SkillLayer.cpp` dan `LoadLayer.cpp` dibersihkan dari helper compat
  sequence/animation lokal untuk blink skill dan avatar loading. Area ini
  memakai `Sequence::create`, `Vector<SpriteFrame*>`, serta
  `Animation::createWithSpriteFrames` langsung tanpa mengubah kontrak
  `CCArray` loader seperti `tempHeros` dan `DLCArray`.
- Commit scene/loading sudah dibuild pengguna di Android dan berjalan aman.
- `StartMenu.cpp` dibersihkan dari helper compat sequence/animation aktif untuk
  cloud, menu text pulse, menu intro, avatar loop, dan dua transisi scroll menu.
  Komentar contoh lama tidak dijadikan target agar perubahan tetap kecil.
- Commit `StartMenu` sudah dibuild pengguna di Android dan berjalan aman.
- `Element.cpp` mulai disentuh secara sangat terbatas: sisa helper
  `createSequence` lokal di alur `Monster`/`Bullet` dealloc dan movement
  diganti ke `Sequence::create`. `getDeadAction()` tetap diperlakukan hati-hati
  lewat `dynamic_cast<FiniteTimeAction*>` dengan fallback callback, meniru
  perilaku helper compat lama yang hanya memasukkan finite action.
- Commit `Element` sudah dibuild pengguna di Android dan berjalan aman.
- `ActionManager.cpp` dibersihkan lagi secara lokal dari helper sequence di
  alur feedback hit, coin/damage display, hurt/hardHurt/absorb, float-up, dan
  death/checkActionFinish. Action lama yang masih bertipe umum tetap dicek
  sebagai `FiniteTimeAction` sebelum dimasukkan ke `Sequence::create`.
- `ActionManager.cpp` dibersihkan lagi dari pemanggilan langsung
  `cocos2dx2_compat::createAnimationWithSpriteFrames` dan
  `cocos2dx2_compat::createSequence` pada jalur animation/action parsing lokal.
  Helper lokal baru mengubah isi `CCArray` lama menjadi `Vector<SpriteFrame*>`
  dan `Vector<FiniteTimeAction*>` tepat sebelum API 3.x dipanggil, sehingga
  kontrak data lama belum ikut dimigrasi. Potongan lokal terbaru ini belum
  build-tested oleh pengguna.
- `CCScrewLayer` mulai dimigrasi dari container item `CCArray*` ke
  `Vector<Node*>`. Pemakai di `StartMenu` yang menambah item, membersihkan list,
  dan mengiterasi item grup sudah ikut dipindahkan ke helper `addItem`,
  `clearItemArray`, dan range-for, sehingga hubungan UI scroll/menu tetap
  tersambung.
- `GameLayer` dan `HudLayer` tidak lagi membuat `CCArray` sementara dari
  `getChildren()` untuk pause/resume child pada flow ougis dan cleanup observer
  saat keluar match. Iterasi sekarang memakai `Vector<Node*>` hasil API 3.x
  langsung.
- `GameLayer` mulai membaca object layer TMX dari `ValueVector`/`ValueMap`
  langsung untuk spawn point dan tower meta. Setelah caller aktif hilang,
  helper compat `toCCArray`, `createSequence(CCArray)`, dan
  `createAnimationWithSpriteFrames(CCArray)` dihapus dari `Cocos2dx2Compat.h`.
- Build Android pengguna setelah migrasi map object sempat gagal karena
  deklarasi iterator `pObject` di cleanup rematch `GameLayer::onLeft()` ikut
  hilang. Commit `66f039f` mengembalikan deklarasi lokal tersebut dan build
  Android pengguna berikutnya berhasil.
- `GameLayer` dan `CCStrokeLabel` tidak lagi memakai helper compat
  `cocos2dx2_compat::visitNode`; pemanggilan render snapshot/stroke sudah
  memakai `Node::visit(Director::getInstance()->getRenderer(), Mat4::IDENTITY,
  0)` langsung, lalu helper tersebut dihapus dari `Cocos2dx2Compat.h`.
- `ActionButton` dan `ActionManager` tidak lagi memakai `CCTime`/`cc_timeval`
  dari compat header. Timer cooldown skill dan buff/debuff lokal memakai helper
  `std::chrono::steady_clock` berbasis delta waktu kecil, lalu `CCTime` dan
  `cc_timeval` dihapus dari `Cocos2dx2Compat.h`.
- Alias compat `CCDictElement` dihapus; parser action `ActionManager` sekarang
  memakai `DictElement` langsung untuk iterasi dictionary.
- `ActionButton` tidak lagi menyimpan `_coldDown` dan `_timeCout` sebagai
  retained `CCString*`. State cooldown lokal dipindahkan ke integer, dan
  jembatan `setCD(CCString*)` sudah dihapus setelah caller aktif di `HudLayer`
  dan `ActionManager` memakai `setCD(int)`.
- `Defines.h` dibersihkan dari alias tipe/singleton 2.x untuk helper global
  scale sprite dan pixel-art, sehingga caller scene tetap memakai helper yang
  sama tetapi implementasinya sudah 3.x.
- `ActionManager` dibersihkan lagi secara lokal dari field `cpLabel`
  `CCLabelTTF` yang tidak punya caller aktif, serta beberapa alias kecil di
  helper shadow/voice/coin display. Potongan ini belum build-tested oleh
  pengguna.
- Callback payload/action lokal `ActionManager` dibersihkan satu tahap lagi:
  deklarasi `CCNode*` diganti menjadi `Node*`, deklarasi sementara
  `CCFiniteTimeAction` diganti menjadi `FiniteTimeAction`, dan factory
  callback payload `CCCallFuncND` / `callfuncND_selector` sudah dipindahkan
  ke helper lokal `createPayloadCall(...)` berbasis `CallFunc` lambda.
  Payload `void*` dari parser action XML sengaja tetap dipertahankan agar
  kontrak XML belum berubah besar.
- Macro dictionary parser `CCDICT_FOREACH` di `ActionManager::createAnimation(...)`
  sudah diganti ke `HASH_ITER` langsung atas `DictElement`. Ini hanya menutup
  macro iterator dictionary lama; bentuk `CCDictionary` dari parser XML tetap
  dipertahankan untuk potongan berikutnya.
- Alias sprite lifecycle kecil di `ActionManager` juga dibersihkan:
  `onEnter`, `onExit`, dan heart-effect lokal memakai `Sprite` 3.x langsung.
- Label bitmap lokal di `ActionManager`/`Element` ikut dilepas dari
  `CCLabelBMFont` ke `LabelBMFont` untuk damage/reward display dan reborn timer.
- `Ref*` lokal mulai dipakai di helper animation/sequence dan damage display
  `ActionManager`, menggantikan `CCObject*` yang tidak perlu menjadi kontrak
  public.
- `Element` juga mulai mengganti iterator lokal `CCObject*` ke `Ref*` pada
  alur dealloc/AI kecil yang sudah memakai loop indeks eksplisit.
- `ActionManager` juga mulai melepas alias geometry lama: `CCPoint`,
  `CCPointMake`, `CCRect`, dan `CCRectMake` aktif sudah tidak muncul lagi di
  file tersebut. Cleanup helper `ccp*` masih dipisahkan agar perubahan tetap
  mudah dilacak bila build Android nanti memberi error.
- `Element.cpp` sudah bersih dari `CCPoint`, `ccp(...)`, `ccpSub`, dan
  `ccpNormalize` aktif. Perubahan ini tetap lokal pada posisi/arah AI kecil dan
  tidak mengubah parser XML atau kontrak container lama.
- `Characters.cpp` mulai dipindahkan dari alias `CCPoint` ke `Vec2` pada
  deklarasi posisi/arah lokal AI, dan dua pemakaian `ccpDistance` diganti ke
  `Vec2::getDistance`. Konstruktor posisi `ccp(...)` juga sudah dipindahkan ke
  `Vec2(...)`. Pemakaian `ccpNormalize` dengan variabel sederhana seperti `sp`,
  `retreat`, dan `moveDirection` sudah diganti ke `getNormalized()`, sementara
  seluruh `ccpSub(...)` aktif di file ini sudah diganti ke operator `Vec2`
  langsung. File ini sekarang sudah lepas dari helper point legacy aktif
  (`CCPoint`, `ccp`, `ccpSub`, `ccpNormalize`, dan `ccpDistance`).
- `ActionManager.cpp` mulai ikut dibersihkan dari konstruktor point `ccp(...)`
  ke `Vec2(...)`. Delta posisi sederhana untuk attacker/target/monster/player
  serta distance target sudah memakai operator `Vec2` dan `getDistance()`.
  Normalisasi arah konstan juga dipindahkan ke `Vec2::getNormalized`, sehingga
  `ccp(...)`, `ccpSub`, `ccpNormalize`, dan `ccpDistance` aktif tidak muncul
  lagi di `ActionManager.cpp`.
- `NetworkLayer` tidak lagi memakai `SimpleAudioEngine::sharedEngine()` untuk
  klik tab selector; pemanggilan audio lokal memakai singleton 3.x.
- `Characters.cpp` sudah lepas dari `CCARRAY_FOREACH` aktif. Loop AI yang
  membaca karakter, tower, dan monster tetap memakai container lama, tetapi
  iterasinya sekarang eksplisit lewat `count()` dan `objectAtIndex(...)`.
- `GameLayer.cpp` juga mulai lepas dari `CCARRAY_FOREACH` pada area lokal yang
  mudah dipisahkan: init hero, pemicu AI saat start game, setup duel/training,
  refresh lock ougis, cek tower, dan cleanup/rematch. Kontrak `Heros`,
  `_CharacterArray`, dan `_TowerArray` tetap `CCArray` untuk menjaga handoff
  gameplay lama.
- `StartMenu.cpp`, `LoadLayer.cpp`, dan `NetworkLayer.cpp` ikut dibersihkan
  dari `CCARRAY_FOREACH` aktif. Loop menu, preload loader, select button, dan
  pilihan training sekarang memakai loop indeks atau container STL lokal.
- `NetworkLayer` tidak lagi menyimpan state training manual sebagai retained
  `CCString*`/`CCArray*`; `_trainingPlayerPick`, `_trainingTeamPicks`, dan
  `_trainingEnemyPicks` memakai `std::string`/`std::vector<std::string>`.
  Handoff `tempHeros` ke `LoadLayer` tetap `CCArray`/`CCDictionary` karena
  masih menjadi kontrak dengan `GameLayer`.
- `SelectLayer`, `SkillLayer`, `GearLayer`, dan `GameOver` ikut lepas dari
  `CCARRAY_FOREACH` aktif. Perubahan ini hanya mengganti iterator lokal pada
  daftar select button, gear, result character, dan rematch handoff tanpa
  mengubah isi payload lama.
- Helper random roster `SelectLayer` memakai `std::vector<std::string>` untuk
  daftar hero yang sudah dipakai, meniru cleanup training di `NetworkLayer`.
- `HudLayer.cpp` dan `HPBar.cpp` ikut lepas dari `CCARRAY_FOREACH` aktif pada
  area gameplay lokal: lookup musuh duel, map icon tower/karakter, guardian
  icon, gear button, update HP musuh, state tower icon, report queue, dan
  pengecekan share/reward team. Container lama tetap `CCArray` agar kontrak
  gameplay belum berubah besar.
- `ActionManager.cpp` dan `Element.cpp` ikut lepas dari `CCARRAY_FOREACH` aktif.
  Loop core pada tower, monster, karakter, target search, dealloc, dan AI kecil
  sekarang memakai indeks eksplisit sambil tetap membaca container lama agar
  behavior gameplay tidak ikut diubah.
- Audit menunjukkan `Cocos2dx2Compat.h` masih dipaksa lewat CMake dan project
  Win32, jadi penghapusan file/forced include ditunda sampai sisa alias legacy
  di source lebih kecil dan bisa diuji sebagai potongan final.
- Setelah commit payload callback dan macro dictionary parser terbaru ini lulus
  build Android/Win32 pengguna, langkah berikutnya adalah masuk bertahap ke
  kontrak data besar di loader XML, ranking/string XML, `ActionManager`,
  `Element`, dan parser XML.
- Alias HTTP, EditBox, Scale9, Scroll, macro `atoi`, dan beberapa helper compat
  lokal sudah dihapus setelah audit pemakai source menunjukkan helper tersebut
  tidak perlu lagi disimpan di compat header. `Cocos2dx2Compat.h` tetap ada
  karena masih dipaksa oleh konfigurasi build/CMake/Win32.
- Dead code HTTP di `SkillLayer` dan upload `GameOver` dibersihkan agar jalur
  offline tidak lagi membawa parsing response/request lama yang sudah tidak
  berjalan.
- Request list hero lama di `NetworkLayer` dibersihkan dari komentar dan callback
  response-nya dijadikan stub, karena source aktif sudah memakai daftar offline
  lokal.
- Heartbeat HTTP lama di `GameLayer::checkPost` dibersihkan dari request/response
  parsing.
- Request download DLC lama di `LoadLayer` dibersihkan setelah audit menunjukkan
  `DLCArray` hanya tersisa dari blok komentar legacy; sisa HTTP aktif kini
  terkonsentrasi di `StartMenu`.
- Notice `StartMenu` tidak lagi memakai request HTTP; notice tetap muncul dari
  `Element/strings.xml`, sementara edit notice lokal hanya memperbarui label
  aktif pada sesi berjalan.

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
