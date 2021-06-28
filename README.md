# Tanks

Ein Artilleriespiel. Erstellt als Projekt für das Informatik Modul BTE5052. Anstelle der zur Verfügung gestellten Qt Library wird [SDL2](https://libsdl.org) verwendet.

## Dokumentation
Infos über das `Was?` finden sich im [Pflichtenheft](doc/Pflichtenheft.md).
Über das `Wie?` informieren das [DFD](doc/DFD/DFD.md), das [CRC](doc/CRC.md) und das [Programmflussdiagramm](doc/Programmfluss.md).

Ausführliche Dokumentation zu vereinzelten Modulen findet sich unter:
- [Sprite](doc/SDLW/SpriteDoku.md)
- [Config](doc/SDLW/ConfigDoku.md)
- [Entity](doc/Entitaet.md)

Die komplette Dokumentation des Quellcodes lässt sich per Doxygen generieren: `doxygen doc/doxygen/Doxyfile`. Dies generiert die Dokumentation in den Ordner `doc/doxygen/html` hinein.
Alternativ findet sich das aktuellste Exemplar auf [GitLab-Pages](http://leuen4.pages.ti.bfh.ch/tanks).

## Spiel
Viele Module sind sehr generell gehalten und mit ihnen wäre nahezu jedes erdenkliche 2D-Game implementierbar. Es wurde auf Basis von ihnen das Tanks-Spiel gemäss Pflichtenheft umgesetzt. Näheres dazu ist hier: [Spielaufbau](doc/Spielaufbau.md).

## Installation und Nutzung
- `cmake -S . -B ./build` - Buildsystem erzeugen, für MinGW unter Windows: `-G "MinGW Makefiles"` anfügen.
- `cmake --build build` - Kompilieren
- (optional) `cd build && ctest --verbose --timeout 120` - Tests ausführen
- `/build/tanks` resp. `/build/tanks.exe` - Spielen!

## Verwandte Projekte
- [WurmProjektBasis](https://gitlab.ti.bfh.ch/osi1/wurmprojektbasis) - Basisprojekt von I. Oesch.
- [sdl2-image-sample](https://gitlab.com/aminosbh/sdl2-image-sample/-/tree/master/) - Beispielprojekt um SDL mit CMake zu verwenden.

## Lizenz
[MIT](LICENSE) © [A. Hinnaoui](mailto:hinna2@bfh.ch), [N. Leuenberger](mailto:leuen4@bfh.ch), [S. Stoll](mailto:stols4@bfh.ch), [J. Weber](mailto:webej14@bfh.ch).
