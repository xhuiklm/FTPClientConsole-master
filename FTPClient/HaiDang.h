#include <stdio.h>
#include "winsock2.h"
class HaiDang
{
public:
	int upload(SOCKET ftpControl, SOCKET dataControl);
	int show(SOCKET ftpControl, SOCKET dataControl);
	int Quit(SOCKET ftpControl, SOCKET dataControl);
	int download(SOCKET ftpControl, SOCKET dataControl);
};
