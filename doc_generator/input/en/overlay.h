//! Creates a text with given parameters.
//! @param[in] Font Name of the font.
//! @param[in] FontSize Size of the font.
//! @param[in] bBold Should the font be bold?
//! @param[in] bItalic Should the font be italic?
//! @param[in] x Position X scaled on a resolution of 800 x 600.
//! @param[in] y Position Y scaled on a resolution of 800 x 600.
//! @param[in] color Color of the text (ARGB-Code).
//! @param[in] text Text of the overlay.
//! @param[in] bShadow Black shadow?
//! @param[in] bShow Should the overlay be drawed after the initialization?
//! @return Returns the text's id, return -1, if an error occured.
int TextCreate(const char *Font, int FontSize, bool bBold, bool bItalic, int x, int y, unsigned int color, const char *text, bool bShadow, bool bShow);
//! Destroys a text.
//! @param[in] ID TextID which should be destroyed.
//! @return 1 if successfull, 0 if not.
//! @see TextCreate()
int TextDestroy(int ID);
//! Activates / Deactivates the text's shadow.
//! @param[in] id Text's id.
//! @param[in] b true for activation, false for deactivation.
//! @return 1 if successfull, 0 if not.
//! @see TextCreate()
int TextSetShadow(int id, bool b);
//! Activates / Deactivates the overlay.
//! @param[in] id Overlay-ID
//! @param[in] b true for activation, false for deactivation.
//! @return 1 if successfull, 0 if not.
//! @see TextCreate()
int TextSetShown(int id, bool b);
//! Set the text's color.
//! @param[in] id Text's id.
//! @param[in] color New color (ARGB-code).
//! @return 1 if successfull, 0 if not.
//! @see TextCreate()
int TextSetColor(int id, unsigned int color);
//! Give the text a new position.
//! @param[in] id Text's id.
//! @param[in] x Position X scaled on a resolution of 800 x 600.
//! @param[in] y Position Y scaled on a resolution of 800 x 600.
//! @return 1 if successfull, 0 if not.
//! @see TextCreate()
int TextSetPos(int id, int x, int y);
//! Seta a new text.
//! @param[in] id Text's id.
//! @param[in] str New text as a string.
//! @return 1 if successfull, 0 if not.
//! @see TextCreate()
int TextSetString(int id, const char *str);
//! Updates a text.
//! @param[in] id Text's id.
//! @param[in] Font New font as a string.
//! @param[in] FontSize Size of the font.
//! @param[in] bBold Should the text be bold?
//! @param[in] bItalic Should the text be italic?
//! @return 1 if successfull, 0 if not.
//! @see TextCreate()
int TextUpdate(int id, const char *Font, int FontSize, bool bBold, bool bItalic);

//! Creates a box.
//! @param[in] x Position X scaled on a resolution of 800 x 600.
//! @param[in] y Position Y scaled on a resolution of 800 x 600.
//! @param[in] w Box's width scaled on a resolution of 800 x 600.
//! @param[in] h Box's height scaled on a resolution of 800 x 600.
//! @param[in] dwColor Color of the box (ARGB).
//! @param[in] bShow Should the box be drawed after the initialization?
//! @return Returns the box's id, return -1, if an error occured.
int BoxCreate(int x, int y, int w, int h, unsigned int dwColor, bool bShow);
//! Destroys a box.
//! @param[in] id Box's id.
//! @return 1 if successfull, 0 if not.
//! @see BoxCreate()
int BoxDestroy(int id);
//! Hides or shows a box.
//! @param[in] id ID of the box.
//! @param[in] bShown true, if the box should be displayed, otherwise false.
//! @return 1 if successfull, 0 if not.
//! @see BoxCreate()
int BoxSetShown(int id, bool bShown);
//! Enables a border for the box or disables it.
//! @param[in] id Box's id.
//! @param[in] height Border's height.
//! @param[in] bShown true, if the border should be displayed, otherwise false.
//! @return 1 if successfull, 0 if not.
//! @see BoxCreate()
int BoxSetBorder(int id, int height, bool bShown);
//! Set die border's color.
//! @param[in] id Box's id.
//! @param[in] dwColor Border's color (ARGB).
//! @return 1 if successfull, 0 if not.
//! @see BoxCreate()
int BoxSetBorderColor(int id, unsigned int dwColor);
//! Set the box's color.
//! @param[in] id Box's id.
//! @param[in] dwColor Box'x color (ARGB).
//! @return 1 if successfull, 0 if not.
//! @see BoxCreate()
int BoxSetColor(int id, unsigned int dwColor);
//! Set the box's height.
//! @param[in] id Box's id.
//! @param[in] height Box's height, scaled on a resolution of 800 x 600.
//! @return 1 if successfull, 0 if not.
//! @see BoxCreate()
int BoxSetHeight(int id, int height);
//! Sets the position of the box.
//! @param[in] id Box's id.
//! @param[in] x Position-X, scaled on a resolution of 800 x 600.
//! @param[in] y Position-Y, scaled on a resolution of 800 x 600.
//! @return 1 if successfull, 0 if not.
//! @see BoxCreate()
int BoxSetPos(int id, int x, int y);
//! Sets the box's width.
//! @param[in] id Box's id.
//! @param[in] width Box's width.
//! @return 1 if successfull, 0 if not.
//! @see BoxCreate()
int BoxSetWidth(int id, int width);

//! Creates a line.
//! @param[in] x1 Position X of the first point, scaled on a resolution of 800 x 600.
//! @param[in] y1 Position Y of the first point, scaled on a resolution of 800 x 600.
//! @param[in] x2 Position X of the second point, scaled on a resolution of 800 x 600.
//! @param[in] y2 Position Y of the second point, scaled on a resolution of 800 x 600.
//! @param[in] width Line's width.
//! @param[in] color Line's color (ARGB).
//! @param[in] bShow Should the line be drawed after initialization?
//! @return Returns the line's id, return -1, if an error occured.
int LineCreate(int x1, int y1, int x2, int y2, int width, unsigned int color, bool bShow);
//! Destroys a line.
//! @param[in] id Line's id.
//! @return 1 if successfull, 0 if not.
int LineDestroy(int id);
//! Hides or shows a line.
//! @param[in] id Line's id.
//! @param[in] bShown true, if the border should be displayed, otherwise false.
//! @return 1 if successfull, 0 if not.
//! @see LineCreate()
int LineSetShown(int id, bool bShown);
//! Sets a line's color.
//! @param[in] id Line's id.
//! @param[in] color New color (ARGB).
//! @return 1 if successfull, 0 if not.
//! @see LineCreate()
int LineSetColor(int id, unsigned int color);
//! Sets a line's width.
//! @param[in] id Line's id.
//! @param[in] width New width.
//! @return 1 if successfull, 0 if not.
//! @see LineCreate()
int LineSetWidth(int id, int width);
//! Setzt die Position einer Linie
//! @param[in] id Line's id.
//! @param[in] x1 Position X of the first point, scaled on a resolution of 800 x 600.
//! @param[in] y1 Position Y of the first point, scaled on a resolution of 800 x 600.
//! @param[in] x2 Position X of the second point, scaled on a resolution of 800 x 600.
//! @param[in] y2 Position Y of the second point, scaled on a resolution of 800 x 600.
//! @return 1 if successfull, 0 if not.
//! @see LineCreate()
int LineSetPos(int id, int x1, int y1, int x2, int y2);

//! Creates an image.
//! @param[in] path Relativ or absolut path to an image file.
//! @param[in] x Position X scaled on a resolution of 800 x 600
//! @param[in] y Position Y scaled on a resolution of 800 x 600
//! @param[in] rotation Image's rotation.
//! @param[in] align Image's alignment.
//! @param[in] bShow Should the image be drawed after initialization?
//! @return Gibt die ID zurück, -1 bei einem Fehler.
int ImageCreate(const char *path, int x, int y, int rotation, int align, bool bShow);
//! Destroys an image.
//! @param[in] id Image's id.
//! @return 1 if successfull, 0 if not.
//! @see ImageCreate()
int ImageDestroy(int id);
//! Hides or shows an image.
//! @param[in] id Image's id.
//! @param[in] bShown true, if the image should be drawed, otherwise false.
//! @return 1 if successfull, 0 if not.
//! @see ImageCreate()
int ImageSetShown(int id, bool bShown);
//! Sets an image's alignment.
//! @param[in] id Image's id.
//! @param[in] align New alignment.
//! @return 1 if successfull, 0 if not.
//! @see ImageCreate()
int ImageSetAlign(int id, int align);
//! Sets an image's position.
//! @param[in] id Image's id.
//! @param[in] x Position X scaled on a resolution of 800 x 600
//! @param[in] y Position Y scaled on a resolution of 800 x 600
//! @return 1 if successfull, 0 if not.
//! @see ImageCreate()
int ImageSetPos(int id, int x, int y);
//! Sets an image's rotation.
//! @param[in] id Image's id.
//! @param[in] rotation New rotation.
//! @return 1 if successfull, 0 if not.
//! @see ImageCreate()
int ImageSetRotation(int id, int rotation);

//! Destroys all overlays.
//! @return 1 if successfull, 0 if not.
int DestroyAllVisual();
//! Shows all overlays.
//! @return 1 if successfull, 0 if not.
int ShowAllVisual();
//! Hides all overlays.
//! @return 1 if successfull, 0 if not.
int HideAllVisual();

//! Reads the game's framerate.
//! @return Returns the framerate, -1 if an error occured.
int GetFrameRate();
//! Reads the game's resolution.
//! @param[out] width Width of the game.
//! @param[out] height Height of the game.
//! @return 1 if successfull, 0 if not.
int GetScreenSpecs(int& width, int& height);

//! Routine to initialize the API, but it does not have to be explicitly called because it's initialized internally.
//! @return 1 if successfull, 0 if not.
int  Init();
//! Sets API's parameters.\n
//! Possible Parameters for '_szParamName':\n
//! "use_window" The API uses the window name instead the process name. default 0.\n
//! "process" Name of the process, will be ignored, if 'use_window' is set to '1'.\n
//! "window" Name of the window.\n
//! \n
//! @param[in] _szParamName Key.
//! @param[in] _szParamValue Value.
//! This function MUST be called for the proper use of the API.
void SetParam(const char *_szParamName, const char *_szParamValue);