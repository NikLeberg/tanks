# Sprite {#Sprite}

Ein Sprite ist eine Struktur, welche zusätzliche Informationen zur Positionierung, Skalierung und Rotation enthält.

## Koordinaten
Das Koordinatensystem sieht wie folgt aus.

[<img src="SpriteKoor.png" width="20%"/>](SpriteKoor.png "Koordinatensystem")
<p style="font-size:0px">@image latex SpriteKoor.png Koordinatensystem</p>

Die Positionen werden relativ zu der Position .position des Sprites gezeichnet.
Dies hat der Vorteil, dass wenn man mehrere Sprites vom selben Objekt hat, muss man bei allen nur die Position anpassen, die Offsets bleiben gleich.

Die Position des Sprites bezieht sich auf das Zentrum.

## Source/Destination
Mithilfe des source-Rechtecks (grün) gibt man an, welcher Teil der Textur gezeichnet werden soll.
Dies wird benötigt, falls nicht die ganze Textur angezeigt werden soll. Siehe MultiSprite.

Mithilfe des destination-Rechtecks (rot) gibt man an, wohin mit dem source-Rechteck Bereich gezeichnet werden soll.
Sind die source- und destination-Rechtecke nicht gleich gross, wird der Bereich skaliert und verzerrt.

[<img src="SpriteIntro.png" width="50%"/>](SpriteIntro.png "Sprite Source/Destination")
<p style="font-size:0px">@image latex SpriteIntro.png Sprite Source/Destination</p>

## Pivot/Rotation
Wird das Sprite rotiert, erfolgt die Rotation um seinen Pivot. Dieser ist relativ zur Position+Offset des Sprites.

Wie unten ersichtlich wurde die Textur um einen Pivot (schwarz) gedreht.
Dadurch ist die Position (grün) nicht mehr im Zentrum des Sprites.

[<img src="SpritePivot.png" width="50%"/>](SpritePivot.png "Sprite Rotation")
<p style="font-size:0px">@image latex SpritePivot.png Sprite Rotation</p>

## Rotation Relativ zu Pivot
Um eine zusätzliche Rotation um ein Gesamtobjekt zu ermöglichen existiert die Funktion Sprite_SetRelativeToPivot.

Diese Funktion setzt die Position(rot) des angegebenen Sprites so, dass dieser Pivot(schwarz) um den externen Pivot(blau) rotiert.
Der Offset(grün) und die eigene Rotation wird nicht verändert.

[<img src="SpriteExternPivot.png" width="50%"/>](SpriteExternPivot.png "Sprite externe Rotation")
<p style="font-size:0px">@image latex SpriteExternPivot.png Sprite externe Rotation</p>

## Multisprite
Der Multisprite teilt eine Textur in mehrere Stücke auf, welche einzeln mithilfe von Hilfsfunktionen angezeigt werden.

Die Textur wird in Anzahl Stücke in X- und Y-Richtung aufgeteilt. Im Beispiel unten X=2, Y=3. Zudem muss die Anzahl angegeben werden, da die Textur nicht voll mit Stücken gefüllt werden muss.

[<img src="SpriteAtlas.png" width="30%"/>](SpriteAtlas.png "Sprite Atlas")
<p style="font-size:0px">@image latex SpriteAtlas.png Sprite Atlas</p>
