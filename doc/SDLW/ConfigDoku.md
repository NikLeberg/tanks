# Config {#ConfigDoku}

Die Konfigurationsdateien sollten in folgendem Format abgelegt werden.
Es muss jedoch aufgepasst werden, da es keine Fehlermeldung gibt, falls der Ressourcenname schon existiert.
In diesem Fall wird nur die letzte geladene Ressource verwendet, die vorherige wird jedoch nicht entladen!

## Konfiguration
Lädt eine weitere Konfigurationsdatei.

<table><tr>
    <th>Pfad</th>
    <th>"config"</th>
</tr></table>

## Texturen

### Normale Textur
Eine 2D-Textur im .jpg oder .png Format. Wird in VRAM als SDL_Texture geladen.

<table><tr>
    <th>Name</th>
    <th>"texture"</th>
    <th>Pfad</th>
</tr></table>

### Masken Textur
Eine 2D-Textur im .jpg oder .png Format. Wird in VRAM als SDL_Texture geladen.
Die Blendmode wird so eingestellt, dass dort wo alpha > 0 ist, beim Zeichnen "herausgeschnitten" wird.
Wird gebraucht um die Welt zu zerstören.

<table><tr>
    <th>Name</th>
    <th>"texture"</th>
    <th>Pfad</th>
    <th>"blendmode"</th>
    <th>"mask"</th>
</tr></table>

### Stempel Textur
Eine 2D-Textur im .jpg oder .png Format. Wird in VRAM als SDL_Texture geladen.
Die Blendmode wird so eingestellt, dass dort wo alpha > 0 ist, gezeichnet wird, und dort wo alpha = 0 die alte Textur genommen wird.
Wird gebraucht um Dinge auf die Welt zu platzieren.

<table><tr>
    <th>Name</th>
    <th>"texture"</th>
    <th>Pfad</th>
    <th>"blendmode"</th>
    <th>"stamp"</th>
</tr></table>

## Sprites

### Normale Sprites
Bereitet ein Sprite-Prototyp vor. Die Grösse des Sprites wird auf die Grösse der Textur gesetzt.

Dieser Prototyp sollte nach SDLW_GetResource() nicht direkt als Pointer verwendet werden. Sondern die Daten sollten stattdessen in ein separates Sprite kopiert werden. Ansonsten würden Änderungen an alle Sprites des gleichen Namens erfolgen.

<table><tr>
    <th>Name</th>
    <th>"sprite"</th>
    <th>Textur ID</th>
</tr></table>

Spritekonfiguration mit eingegebenen Offset und Pivot

<table><tr>
    <th>Name</th>
    <th>"sprite"</th>
    <th>Textur ID</th>
    <th>"size"</th>
    <th>Offset X</th>
    <th>Offset Y</th>
    <th>Breite</th>
    <th>Höhe</th>
    <th>Pivot X</th>
    <th>Pivot Y</th>
</tr></table>

### Multi Sprites
Bereitet ein Multi-Sprite vor. Die Textur wird in die angegebene Anzahl Rechtecke in der X- und Y-Richtung gleichmässig geteilt.
Die Grösse des Sprites wird auf die Einzelgrösse eines Rechtecks gesetzt.

<table><tr>
    <th>Name</th>
    <th>"sprite"</th>
    <th>Textur ID</th>
    <th>"animation"</th>
    <th>Textur Aufteilung X</th>
    <th>Textur Aufteilung Y</th>
    <th>Textur Gesamtanzahl</th>
</tr></table>

## Font
Lädt eine Schriftart mit einer bestimmten Grösse. Für jede Grösse muss die Schrift neu geladen werden.

<table><tr>
            <th>Name</th>
            <th>"font"</th>
            <th>Pfad</th>
            <th>Grösse</th>
</tr></table>

## Sound

### SFX
Soundeffekte sind kurz und werden mit weniger guten Qualität abgespielt.
Es können mehrere Soundeffekte gleichzeitig abgespielt werden.

<table><tr>
            <th>Name</th>
            <th>"sound"</th>
            <th>Pfad</th>
            <th>"effect"</th>
</tr></table>

### Musik
Musik hat bessere Qualität als Soundeffekte, es kann jedoch nur ein Stück gleichzeitig abgespielt werden.
Musik kann länger sein.

<table><tr>
            <th>Name</th>
            <th>"sound"</th>
            <th>Pfad</th>
            <th>"music"</th>
</tr></table>

## World
In der Weltenkonfiguration werden die Vorder- und Hintergrundstextur sowie die Hintergrundsmusik (optional) definiert.

Die Vordergrundstextur ist im Spiel Zerstörbar und die Hintergrundsmusik wird beim Laden der Welt automatisch abgespielt.

<table><tr>
            <th>Name</th>
            <th>"world"</th>
            <th>Hintergrund Textur ID</th>
            <th>Fordergrund Textur ID</th>
</tr></table>
<table><tr>
            <th>Name</th>
            <th>"world"</th>
            <th>Hintergrund Textur ID</th>
            <th>Fordergrund Textur ID</th>
            <th>Hintergrund Musik ID</th>
</tr></table>