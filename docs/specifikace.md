Student: Adam Říha

Předmět: Programování v C++

Datum: 2. 12. 2025

# 1. Základní informace o projektu

**Název projektu:** Dungeon Crawler (pracovní název)

**Cíl projektu:** Implementace funkční 2D top-down hry s náhodně generovaným bludištěm a základní herní logikou, včetně AI a speciálních grafických efektů (FOV).

**Jazyk:** C++ (standard C++17 nebo novější)

**Knihovny:** **SDL3**, **SDL_ttf**, **SDL_image**, **SDL_mixer**

**Platforma:** Desktop (Windows/Linux/macOS)

# 2. Architektura a implementace

Projekt je implementován s využitím principů **modulárního designu** a strikního oddělení **Engine** od **Game Application**.

Pro zajištění čistoty kódu a správné správy závislostí je zavedena adresářová struktura, která odděluje hlavičkové soubory (`include/`) od zdrojových souborů (`src/`).

## 2.1 Engine

**Cíl:** Implementace nízkoúrovňových služeb a abstrakce nad knihovnami SDL3. Engine je **nezávislý** na konkrétních herních pravidlech.

| Adresář    | Klíčové třídy/Komponenty           | Zodpovědnosti                                     |
|:-----------|------------------------------------|--------------------------------------------------:|
| `core`     | `Window`, `GameState`, `GameEvent` | Správa okna, řízení herních stavů, událostí...    |
| `graphics` | `Renderer`, `Texture`, `Sprite`    | Vykreslování, správa textur                       |
| `input`    | `Input`                            | Zpracování uživatelského vstupu (myš, klávesnice) |
| `audio`    | `Audio`, `AudioManager`            | Správa **SDL_mixer**, načítání a přehrávání zvuků |
| `assets`   | `AssetManager`, `Font`             | Centralizovaná správa všech zdrojů (textury, fonty, zvuky) |
| `math`     | `Vector2D`                         | Základní matematické operace a struktury pro 2D geometrii |

## 2.2 Game Application

**Cíl:** Implementace specifické herní logiky, pravidel, entit a algoritmů pro *Dungeon Crawler*. Hra využívá výhradně rozhraní poskytovaná *Enginem*.

| Adresář       | Klíčové třídy/Komponenty         | Zodpovědnosti                                     |
|:--------------|----------------------------------|--------------------------------------------------:|
| `application` | `DungeonGame`, `GameState_Menu`, `GameState_Play` | Hlavní instance hry, implementuje herní logiku uvnitř `Update()` a `Render()` |
| `entities`    | `Entity`, `Player`, `Enemy`            | Abstraktní a konkrétní herní objekty, jejich pohyb a interakce |
| `world`       | `MapGenerator`, `LevelManager`, `Tile` | Procedurální generování bludiště, správa mapy a jednotlivých dlaždic |
| `ai`          | `AI_Component`, `PathFinder`           | Implementace AI a algoritmu **A\*** pro hledání cesty |
| `systems`     | `CollisionSolver`, `VisibilitySystem`  | Systémy pro řešení kolizí, implementace zorného pole atp. |
| `ui`          | `HUD`, `MenuSystem`                    | Vykreslování informací o stavu hry a menu |

# 3. Klíčové herní aspekty a algoritmy

## 3.1 Generování a reprezentace světa

+ **Generování:** Algoritmus pro **procedurální generování bludiště** (např. *Wave Function Collapse*), zajišťující hratelné a propojené mapy.
+ **Reprezentace:** Svět je reprezentován jako mřížka dlaždic (`Tile`), kde každá dlaždice má kolizní příznak.

## 3.2 Kolize

+ **Entity:** Algoritmus *Axis-Aligned Bounding Box* pro zaregistrování kolizí entit.
+ **Tile**: Triviální Grid-based/Chunk-based algoritmus, který provádí kontrolu mezi entitami a statickým prostředím (`Tile` typu Zeď) před provedením pohybu.

## 3.3 Zorné pole (FOV)

+ **Implementace:** *Raycasting* algoritmus (popř. *Shadowcasting*) pro vypuštění paprsků z pozice hráče.
+ **Vykreslování:** Pouze viditelné části mapy jsou plně vykresleny. Již prozkoumané, ale neviditelné oblasti jsou zatmaveny.

## 3.4 Umělá inteligence

+ **Stavy:**
  1. Pasivní: Náhodný pohyb/setrvání.
  2. Agresivní: Aktivní pronásledování hráče po vizuální detekci.

+ **Pohyb:** Algoritmus *A\** pro efektivní navigaci bludištěm.

## 3.5 Multimediální požadavky

+ **Grafika:** Všechny sprity a textury se načítají pomocí **SDL_image**.
+ **UI/Text:** Vykreslování textu probíhá pomocí **SDL_ttf**.
+ **Zvuk:** Přehrávání hudby na pozadí a zvukových efektů pomocí **SDL_mixer**.

## 3.6 Herní mechaniky

+ **Pohyb:** Hráč se pohybuje volně, ale je omezen kolizemi. Pohyb nepřátel je řízen AI.
+ **Bojový systém:** Základní systém zdraví (HP) a útoků. Implementace **kontaktního útoku** nebo **střelby** (projektily).

# 4. Technologická specifikace

# 4.1 Jazyk a knihovny

**C++**
+ **Standard jazyka**: Projekt využívá standard C++17 (nebo novější), aby bylo možné implementovat moderní programovací techniky, jako je efektivní správa paměti a využití *Standard Template Library* (STL).

---

**SDL3 (Simple DirectMedia Layer)**
+ **Základní Funkce**: SDL3 slouží jako nízkoúrovňový základ pro *Engine*. Zajišťuje klíčovou funkčnost okenní aplikace:

  + Vytvoření a správa okna (SDL_Window).

  + Implementace hardwarově akcelerovaného rendereru (*SDL_Renderer*).

  + Čtení a zpracování uživatelského vstupu (klávesnice, myš) skrze event systém (*SDL_Event*).

---

**Rozšíření SDL3**:

**SDL_ttf**
+ **Účel**: Knihovna je nezbytná pro podporu a zpracování textu v uživatelském rozhraní (UI) a menu.

+ **Formáty**: Umožňuje načítání *TrueType Font* (TTF) souborů.

+ *Integrace do*: `Font`

**SDL_image**
+ **Účel**: Zajišťuje rozšířenou podporu pro načítání různých grafických formátů.

+ **Formáty**: Klíčová je podpora formátu *PNG*, který se využije pro všechny textury a sprity, včetně podpory průhlednosti.

+ *Integrace do*: `Texture`

**SDL_mixer**
+ **Účel**: Umožňuje správu zvuku, hudby a zvukových kanálů.

+ **Formáty**: Podporuje dekódování a streamování různých formátů: *OGG* nebo *MP3* pro hudbu na pozadí a *WAV* pro rychlé zvukové efekty (SFX).

+ *Integrace do*: `Audio`, `AudioManager`

# 4.2 Správa sestavení

+ **Nástroj**: Pro správu kompilace a generování nativních sestavovacích souborů je použit především systém **CMake**.

+ **Zodpovědnost**: *CMake* zajišťuje správu všech externích závislostí (knihovny *SDL3* a jeho rozšíření), konfiguraci cesty k hlavičkovým souborům (`include/`) a definování cílů pro sestavení spustitelného souboru Enginu a Game.