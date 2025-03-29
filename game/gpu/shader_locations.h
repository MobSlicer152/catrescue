// has to be valid hlsl

#ifdef HLSL
#define UBO_REGISTER(x) b##x
#else
#define UBO_REGISTER(x) x
#endif

#define SCENE_UBO_LOCATION  UBO_REGISTER(0)
#define OBJECT_UBO_LOCATION UBO_REGISTER(1)

#define STANDARD_VERTEX_POSITION 0
#define STANDARD_VERTEX_NORMAL 1
#define STANDARD_VERTEX_TEXCOORD 2
#define STANDARD_VERTEX_COLOR 3
