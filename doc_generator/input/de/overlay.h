//! Erstellt einen Text mit den angegebenen Parametern.
//! @param[in] Font Name einer Font
//! @param[in] FontSize Größe der Font
//! @param[in] bBold Sollte die Font fett sein?
//! @param[in] bItalic Sollte die Font kursiv sein?
//! @param[in] x Position X skaliert auf einer Auflösung von 800 x 600
//! @param[in] y Position Y skaliert auf einer Auflösung von 800 x 600
//! @param[in] color Farbe des Overlays (ARGB-Code)
//! @param[in] text Text des Overlays
//! @param[in] bShadow Schwarzer Schatten um das Overlay?
//! @param[in] bShow Sollte der Text nach dem Funktionsaufruf gezeichnet werden?
//! @return Gibt die ID des Overlays zurück, welche für alle weitern Funktionen benötigt wird. Gibt -1 bei einem Fehler zurück.
int TextCreate(const char *Font, int FontSize, bool bBold, bool bItalic, int x, int y, unsigned int color, const char *text, bool bShadow, bool bShow);
//! Zerstört einen Text.
//! @param[in] ID TextID welche zerstört werden sollte.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see TextCreate()
int TextDestroy(int ID);
//! Aktiviert / Deaktiviert den Schatten eines Textes.
//! @param[in] id Overlay-ID
//! @param[in] b true für das Aktivieren, false für das Deaktivieren
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see TextCreate()
int TextSetShadow(int id, bool b);
//! Aktiviert / Deaktiviert das Overlay temporär.
//! @param[in] id Overlay-ID
//! @param[in] b true für das Aktivieren, false für das Deaktivieren
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see TextCreate()
int TextSetShown(int id, bool b);
//! Setzt eine neue Farbe für den Text
//! @param[in] id Overlay-ID
//! @param[in] color Die neue Farbe als ARGB-Code
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see TextCreate()
int TextSetColor(int id, unsigned int color);
//! Setzt den Text an eine angegebene Position
//! @param[in] id Overlay-ID
//! @param[in] x X-Position skaliert auf einer Auflösung von 800 x 600
//! @param[in] y Y-Position skaliert auf einer Auflösung von 800 x 600
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see TextCreate()
int TextSetPos(int id, int x, int y);
//! Setzen eines neues Textes für das Text-Overlay.
//! @param[in] id Overlay-ID
//! @param[in] str Neuer Text als String
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see TextCreate()
int TextSetString(int id, const char *str);
//! Aktualisieren des Overlays mit neuen Daten.
//! @param[in] id Overlay-ID
//! @param[in] Font Neue Font als String
//! @param[in] FontSize Größe der Font
//! @param[in] bBold Sollte der Text fett sein?
//! @param[in] bItalic Sollte der Text kursiv sein?
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see TextCreate()
int TextUpdate(int id, const char *Font, int FontSize, bool bBold, bool bItalic);

//! Erstellt eine Box mit den angegebenen Paramtern.
//! @param[in] x Position X skaliert auf einer Auflösung von 800 x 600
//! @param[in] y Position Y skaliert auf einer Auflösung von 800 x 600
//! @param[in] w Breite der Box, skaliert auf einer Auflösung von 800 x 600
//! @param[in] h Höhe der Box, skaliert auf einer Auflösung von 800 x 600
//! @param[in] dwColor Farbe der Box
//! @param[in] bShow Sollte die Box nach dem Erstellen gezeichnet werden?
//! @return Gibt die ID der Box zurück, -1 bei einem Fehler!
int BoxCreate(int x, int y, int w, int h, unsigned int dwColor, bool bShow);
//! Zerstört eine Box.
//! @param[in] id ID der Box
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see BoxCreate()
int BoxDestroy(int id);
//! Aktiviert / Deaktiviert eine Box temporär.
//! @param[in] id ID der Box
//! @param[in] bShown true, falls die Box gezeigt werden soll, false wenn nicht.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see BoxCreate()
int BoxSetShown(int id, bool bShown);
//! Setzt eine Umrandung für die Box.
//! @param[in] id ID der Box
//! @param[in] height Höhe der Umrandung.
//! @param[in] bShown true, falls die Umrandung gezeigt werden soll, false wenn nicht.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see BoxCreate()
int BoxSetBorder(int id, int height, bool bShown);
//! Setzt die Farbe der Umrandung einer Box.
//! @param[in] id ID der Box
//! @param[in] dwColor Farbe für die Umrandung (ARGB-Code)
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see BoxCreate()
int BoxSetBorderColor(int id, unsigned int dwColor);
//! Setzt die Farbe der Box.
//! @param[in] id ID der Box
//! @param[in] dwColor Farbe der Box (ARGB-Code)
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see BoxCreate()
int BoxSetColor(int id, unsigned int dwColor);
//! Setzt die Höhe einer Box.
//! @param[in] id ID der Box.
//! @param[in] height Höhe der Box.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see BoxCreate()
int BoxSetHeight(int id, int height);
//! Setzt die Position einer Box.
//! @param[in] id ID der Box.
//! @param[in] x Position-X, skaliert auf einer Auflösung von 800-600.
//! @param[in] y Position-Y, skaliert auf einer Auflösung von 800-600.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see BoxCreate()
int BoxSetPos(int id, int x, int y);
//! Setzt die Breite einer Box.
//! @param[in] id ID der Box
//! @param[in] width Breite der Box
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see BoxCreate()
int BoxSetWidth(int id, int width);

//! Erstellt eine Linie mit den angegebenen Parametern.
//! @param[in] x1 X-Position des ersten Punktes, skaliert auf einer Auflösung von 800 x 600
//! @param[in] y1 Y-Position des ersten Punktes, skaliert auf einer Auflösung von 800 x 600
//! @param[in] x2 X-Position des zweiten Punktes, skaliert auf einer Auflösung von 800 x 600
//! @param[in] y2 Y-Position des zweiten Punktes, skaliert auf einer Auflösung von 800 x 600
//! @param[in] width Breite der Linie in Pixeln.
//! @param[in] color Farbe der Linie.
//! @param[in] bShow Sollte die Linie direkt nach dem Erstellen gezeichnet werden?
//! @return Gibt die ID der erstellen Linie zurück. -1 bei einem Fehler.
int LineCreate(int x1, int y1, int x2, int y2, int width, unsigned int color, bool bShow);
//! Zerstört eine Linie.
//! @param[in] id Linien-Objekt, welches zerstört werden soll.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
int LineDestroy(int id);
//! Aktiviert / Deaktiviert eine Linie temporär.
//! @param[in] id ID der Linie
//! @param[in] bShown true, falls die Linie gezeigt werden soll, false wenn nicht.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see LineCreate()
int LineSetShown(int id, bool bShown);
//! Setzt die Farbe einer Linie.
//! @param[in] id ID der Linie
//! @param[in] color Farbe der Linie (ARGB-Code)
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see LineCreate()
int LineSetColor(int id, unsigned int color);
//! Setzt die Breite einer Linie.
//! @param[in] id ID der Linie
//! @param[in] width Breite der Linie in Pixeln.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see LineCreate()
int LineSetWidth(int id, int width);
//! Setzt die Position einer Linie
//! @param[in] id ID der Linie
//! @param[in] x1 X-Position des ersten Punktes, skaliert auf einer Auflösung von 800 x 600
//! @param[in] y1 Y-Position des ersten Punktes, skaliert auf einer Auflösung von 800 x 600
//! @param[in] x2 X-Position des zweiten Punktes, skaliert auf einer Auflösung von 800 x 600
//! @param[in] y2 Y-Position des zweiten Punktes, skaliert auf einer Auflösung von 800 x 600
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see LineCreate()
int LineSetPos(int id, int x1, int y1, int x2, int y2);

//! Erstellt ein Bild mit den angegebenen Paramtern.
//! @param[in] path Relativer oder absoluter Pfad zur einer Bild-Datei.
//! @param[in] x X-Position des ersten Punktes, skaliert auf einer Auflösung von 800 x 600
//! @param[in] y Y-Position des ersten Punktes, skaliert auf einer Auflösung von 800 x 600
//! @param[in] rotation Rotation des Bildes.
//! @param[in] align Platzierung des Bildes.
//! @param[in] bShow true, falls das Bild beim initialisieren gezeichnet werden soll, false wenn nicht.
//! @return Gibt die ID zurück, -1 bei einem Fehler.
int ImageCreate(const char *path, int x, int y, int rotation, int align, bool bShow);
//! zerstört ein Bild.
//! @param[in] id ID des Bildes, welches zerstört werden soll.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see ImageCreate()
int ImageDestroy(int id);
//! Aktiviert / Deaktiviert ein Bild temporär.
//! @param[in] id ID des Bildes.
//! @param[in] bShown true, falls das Bild gezeigt werden soll, false wenn nicht.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see ImageCreate()
int ImageSetShown(int id, bool bShown);
//! Ordnet das Bild zu.
//! @param[in] id ID des Bildes.
//! @param[in] align Zuordnung.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see ImageCreate()
int ImageSetAlign(int id, int align);
//! Setzt die Position eines Bildes.
//! @param[in] id ID des Bildes.
//! @param[in] x X-Position des ersten Punktes, skaliert auf einer Auflösung von 800 x 600
//! @param[in] y Y-Position des ersten Punktes, skaliert auf einer Auflösung von 800 x 600
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see ImageCreate()
int ImageSetPos(int id, int x, int y);
//! Setzt die Rotation eines Bildes.
//! @param[in] id ID des Bildes.
//! @param[in] rotation Rotation des Bildes.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
//! @see ImageCreate()
int ImageSetRotation(int id, int rotation);

//! Zerstört alle Overlays.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
int DestroyAllVisual();
//! Zeigt alle Overlays.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
int ShowAllVisual();
//! Versteckt alle Overlays.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
int HideAllVisual();


//! Liest die aktuelle Framerate aus.
//! @return Framerate, -1 bei einem Fehler.
int GetFrameRate();
//! Gibt die aktuelle Auflösung des Spieles / Programmes über zwei Referenzen zurück.
//! @param[out] width Breite.
//! @param[out] height Höhe.
//! @return 1, wenn erfolgreich, sonst 0.
int GetScreenSpecs(int& width, int& height);

//! Initialisiert die API, muss aber nicht aufgerufen werden, da sie intern aufgerufen wird.
//! @return 1 wenn erfolgreich, 0 wenn nicht.
int  Init();
//! Setzt die Paramter für die API.\n
//! Mögliche Parameter für '_szParamName':\n
//! "use_window" Die API benutzt den Fensternamen, um einen Prozess zu finden. Standardmäßig auf 0.\n
//! "process" Name des Prozesses, wird ignoriert, wenn use_window auf 1 ist.\n
//! "window" Name des Fensters.\n
//! \n
//! @param[in] _szParamName Schlüsselparameter
//! @param[in] _szParamValue Wert für den ersten Parameter.
//! Diese Funktion muss vor jeder anderen API-Funktion aufgerufen werden!
void SetParam(const char *_szParamName, const char *_szParamValue);