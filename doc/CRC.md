# CRC - Class, Responsibility, Collaborator

In folgenden CRC "Karten" werden für die verschiedenen Module gemäss [DFD](./DFD/DFD.md) die Zuständigkeiten `Responsibility` und die Abhängigkeiten `Collaborator` aufgezeigt.

<center>

## GUI
| Responsibility | Collaborator |
|----------------|-------------|
| Übergibt Mausevents an Buttons | Button |
| Übergibt Zeichnungsevents an Buttons | SDL_Wrapper |

## Button
| Responsibility | Collaborator |
|----------------|-------------|
| Überprüft Mausevents | SDL_Wrapper |
| Verwaltet Button-Struktur |  |
| Zeichnet den Button |  |

## SDL_Wrapper
| Responsibility | Collaborator |
|----------------|-------------|
| Zeichnet Bild | SDL2 |
| Zeichnet Text | SDL2_image |
| Zeichnet Rect |  |
| gibt Ton aus |  |
| Verwaltet Ressourcen / Assets |  |
| Initialisiert SDL |  |
| Mausposition und Events abfragen |  |
| Wasd-Tasten abfragen |  |
| Initialisiert das Fenster |  |

## World
| Responsibility | Collaborator |
|----------------|-------------|
| Lädt die Welt | SDL_Wrapper |
| Zeichnet Welt anhand einer Maske |  |
| Erlaubt die Bearbeitung der Welt anhand Primitiven |  |
| Gibt aus, ob ein Punkt mit der Welt kollidiert |  |

## Physics
| Responsibility | Collaborator |
|----------------|-------------|
| Flugbahn berechnen | World |
| Bewegungsverhalten von Entitäten verwalten | Entity |
| Auf Kollisionen prüfen |  |

## Tank
| Responsibility | Collaborator |
|----------------|-------------|
| Spieler Interaktion managen | SDL_Wrapper |
| Panzer zeichnen | Physics |
| Panzerrichtung darstellen | Entity |

## Shell
| Responsibility | Collaborator |
|----------------|-------------|
| Schuss zeichnen | SDL_Wrapper |
| Schussrichtung darstellen | Physics |
| Kollisionen auswerten | Entity |

## Entity (Struktur)
| Responsibility | Collaborator |
|----------------|-------------|
| Speichern der Hitbox | EntityHandler |
| Speichern der Physikdaten |  |
| Speichert die Position |  |
| Speichert Callbacks der Entitäten |  |
| Speichert Liste an Sprites |  |
| Speichert optionale Entitätsdaten |  |

## Animation
| Responsibility | Collaborator |
|----------------|-------------|
| Aktive Animationen von Sprites weiterschalten | SDL_Wrapper |
| Verschiedene Animationstypen zur verfügung stellen | Entity |

## Scene
| Responsibility | Collaborator |
|----------------|-------------|
| Initialisiert Szene mit all seinen Inhalten | SDL_Wrapper |
| Aktualisiert die gesamte Szene | GUI |
| Events von SDL verarbeiten / weiterleiten | EntityHandler |
| (Steuerung an Netzwerk senden) | (Netzwerk) |
| (Steuerung aus Netzwerk empfangen) |  |

## Main
| Responsibility | Collaborator |
|----------------|-------------|
| Initialisiert SDL_Wrapper | SDL_Wrapper |
| GAME-Loop ausführen | Scene |
| Anzeige auf Bildschirm aktualisieren |  |

## EntityHandler
| Responsibility | Collaborator |
|----------------|-------------|
| Verwaltet Entitäten | Entity |
| Erstellt und zerstört Entitäten | Physics |

</center>
