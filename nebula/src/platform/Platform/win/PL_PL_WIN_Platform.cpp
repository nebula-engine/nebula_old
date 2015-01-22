#include <Windows.h>
#include <WinUser.h>

#define IDS_APP_TITLE 103
#define IDI_WIN32PROJECT1 107
#define IDI_SMALL 108
#define IDC_WIN32PROJECT1 109

#include <Platform/Platform/Win/PL_PL_WIN_Platform.h>

		PL_PL_WIN_Platform::Platform() {
	
}
		PL_PL_WIN_Platform::~PlatformWin() {
	
}
void	PL_PL_WIN_Platform::VInit(Void* v) {
	PRINTSIG;
	
	int nCmdShow = SW_SHOWNORMAL;

	HINSTANCE hInst = GetModuleHandleA(NULL);

	
}
void	PL_PL_WIN_Platform::VShutdown(Void* v)
{
	PRINTSIG;
}
void	PL_PL_WIN_Platform::VUpdate(Void* v)
{
	
}
void	PL_PL_WIN_Platform::VCreateWindow( PL_WI_Window*& window )
{
	PRINTSIG;
	
	PL_WI_WIN_Window* pl_wi_win_window = NULL;
	m_window.Create(pl_wi_win_window);

	window = pl_wi_win_window;

	windowWin->VInit( data );

}



