#ifndef _3DAnaglyph_h_
#define _3DAnaglyph_h_
/*
//	»нтерфейс библиотеки выполн€ющей преобразование статического изображени€ 
//	в трехмерное анаглифное изображение.
*/
// опции преобразовани€
#define TrueAnaglyph			0
#define GrayAnaglyph			1
#define ColorAnaglyph			2
#define HalfColorAnaglyph		3
#define OptimizedAnaglyph		4
#define TiledImages				5
#define VGAAvi					6
#define HDAvi720				7
#define HDAvi1080				8
#define AnimatedGif				9
#define MacroVideo				10

// коды ошибок
#define NOERROR					 0
#define ERR_LOAD_SOURCE			-1
#define ERR_CREATE_DESTINATION	-2
#define ERR_CREATE_DEPTH_MAP	-3
#define ERR_INVALID_PARAMS		-4
#define ERR_CREATE_STEREOPAIR	-5
#define ERR_SAVE_ANAGLYPH		-6
#define ERR_MAKE_DEPTH			-7
#define ERR_MEMORY				-8

/*
// синтезируетс€ анаглиф из дефокусированного изображени€
*/
int Make3DAnaglyph(const char* lpSrcName, const char* lpDstName, int options = ColorAnaglyph);
#endif //_3DAnaglyph_h_