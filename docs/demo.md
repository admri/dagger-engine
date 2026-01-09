# Technologické demo

## Požadavky

### **Windows:**

- [**git**](https://git-scm.com/)

- [**CMake**](https://cmake.org/download/)

- [**Visual Studio**](https://visualstudio.microsoft.com/) (nebo jiný kompilátor)

### **Ubuntu (Linux):**

- **git**

- **CMake**

- **Build tools**

```bash
sudo apt update
sudo apt install git cmake build-essential
```

## Instalace a spuštění

### Prebuilt libraries (rychlé spuštění)

Pro účely technologického dema byl vedle způsobu standardního spuštění přidán způsob rychlého spuštění, který využívá předem zkompilovaných knihoven SDL, a není je tedy potřeba zdlouhavě stahovat z GitHubu a následně samostatně kompilovat.

Demo skript sestaví projekt s nastaveným parametrem použití předem zkompilovaných binárních souborů, které pak CMake překopíruje do složky `build\Release` vedle spustitelného souboru `DungeonCrawler`.

Skript následně sám program spustí.

#### Windows:

```bat
cd project
.\scripts\demo.bat
```

#### Ubuntu (Linux):

Binární soubory byly sestaveny na *Ubuntu 25.04 x86_64*.

```bash
cd project
./scripts/demo.sh
```

### Standardní spuštění

Pro standardní spuštění pro vývoj existují dva skripty: `run-debug` a `build-release`.

Oba skripty nejdříve stáhnou z GitHubu všechny submoduly (tedy zdrojový kód knihoven SDL) a poté sestaví projekt (s určitou konfigurací), což zahrnuje i kompilaci SDL. `run-debug` poté navíc program rovnou spustí.

#### Windows:

```bat
cd project
.\scripts\build-release.bat
.\build\Release\DungeonCrawler
```

nebo

```bat
cd project
.\scripts\run-debug.bat
```

#### Linux:

K sestavení SDL na Linuxu jsou ve většině případů zapotřebí další závislosti (viz [zde](https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md#build-dependencies)).

Na Ubuntu, co jsem zkoušel, stačilo nainstalovat tyto balíčky:

```bash
sudo apt install lix11-dev libxext-dev libxrandr-dev libxcursor-dev \
libxi-dev libxss-dev libxtst-dev libaudio-dev libasound2-dev
```

Poté spusťte skript:

```bash
cd project
./scripts/build-release.sh
./build/Release/DungeonCrawler
```

nebo

```bash
cd project
./scripts/run-debug.sh
```
