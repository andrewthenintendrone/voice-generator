#include "VoiceGeneratorWindow.h"

// program entry point for win32 applications
// arguments are used by windows
int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	VoiceGeneratorWindow voiceGeneratorWindow;
	voiceGeneratorWindow.create("Voice Generator", "TEST", RECT{ 100, 100, 640, 480 });
	voiceGeneratorWindow.show();
	voiceGeneratorWindow.run();

    // Exit with status specified in WM_QUIT message
    return 0;
}
