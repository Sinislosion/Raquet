/* 1 for display cursor, 0 for disable cursor */
extern void Raquet_ShowCursor(int toggle);

/* Get an absolute string to an asset from the assets folder */
extern const char* Raquet_AbsoluteToAsset(const char* relativepath);
