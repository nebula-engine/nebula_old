typedef enum DriverType
{
	DRIVER_OPENGL   = 0, // Supports Windows, Linux, MacOSX and PS3.
	DRIVER_GLES2,		 // Supports Android (> 2.2) and iPhone.
	DRIVER_DIRECT3D9,    // Supports Windows, XBOX360.
	DRIVER_DIRECT3D10,   // Supports Windows Vista.
	DRIVER_LIBGCM,        // Supports PS3.
	DRIVER_LIBGXM		// Supports PSP2
} DriverType;

struct PickingCommands
{
	enum Enum
	{
		PICK_START,					//Bound to mouse 1 down
		PICK_STOP,					//bound to mouse 1 up
		SCREEN_MOTION_CURSOR,		//See DefaultMovementStrategy
		SCREEN_MOTION_CURSOR_DEPTH, //See DefaultMovementStrategy
	};
};

typedef enum Primitive
{
	PRIMITIVE_POINTS = 0,
	PRIMITIVE_LINES,
	PRIMITIVE_LINE_STRIP,
	PRIMITIVE_TRIANGLES,
	PRIMITIVE_TRIANGLE_STRIP,
	PRIMITIVE_POINT_SPRITES,

	NUM_PRIMITIVES
} _Primitive;

enum FrustumPlaneIndex
{
	FRUSTUM_PLANE_LEFT			= 0,	//!< Left clipping plane
	FRUSTUM_PLANE_RIGHT			= 1,	//!< Right clipping plane
	FRUSTUM_PLANE_TOP			= 2,	//!< Top clipping plane
	FRUSTUM_PLANE_BOTTOM		= 3,	//!< Bottom clipping plane
	FRUSTUM_PLANE_NEAR			= 4,	//!< Near clipping plane
	FRUSTUM_PLANE_FAR			= 5,	//!< Far clipping plane (must be last for infinite far clip)

	FRUSTUM_PLANE_FORCE_DWORD	= 0x7fffffff
};