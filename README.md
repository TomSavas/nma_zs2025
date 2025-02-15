# NMA žiemos sesija 2025 -- Rasterizacija

## Projekto pasileidimas
- Git:
  - [Windows](https://git-scm.com/downloads/win).
  - Linux: `apt install git`, `pacman -S git`, `nix-shell -p nix`, t.t.
  - MacOS: `brew instal git`, arba parsisiųskite Xcode.

- Atsisiųskite šią repozitoriją: `git clone https://github.com/TomSavas/nma_zs2025`

- Projekto kompiliavimas: 
  - Windows (Visual Studio): 
    - Paleiskite `build-VisualStudio2022.bat`.
    - Atsidarykite sugeneruotą .sln file'ą.
    - F5.
  - Windows:
    - Atsisiųskite [MinGW-W64](https://github.com/skeeto/w64devkit/releases).
    - Pridekite MinGW-W64 `bin` direktoriją prie savo Path env variable.
    - Paleiskite `build-MinGW-W64.bat`.
    - Terminale nueikite į projekto direktoriją.
    - `make`.
    - Paleiskite .exe file'ą esantį `bin` direktorijoje.
  - Linux: 
    - `cd build`.
    - `./premake5 gmake2` (jeigu turite problemų su premake5, atsisiųskite jį atskirai).
    - `cd ..`.
    - `make`.
  - MacOS -- tas pats kas Linux, tik vietoj `premake5` naudokite `premake5.osx`.

## Rasterizacijos pradmenys

## Užduotys
- `line` funkcija
  - Keli skirtingų formų trikampiai
  - "Wireframe" modelis
- `triangle` funkcija
  - "line sweep"
  - Keli skirtingų formų trikampiai
  - baricentrinės koordinatės
    - išvedimas?

  - Spalvotų trikampių modelis
  - "Flat shading" modelis
- "Depth" buferis
  - 
- Tekstūros
- 
