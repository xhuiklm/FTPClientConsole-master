#include <stdio.h>
#include "winsock2.h"

	SOCKET CreateSocketData(SOCKET controlSocket);
	int upload(SOCKET ftpControl, SOCKET dataControl);
	int show(SOCKET ftpControl, SOCKET dataControl);
	int quit(SOCKET ftpControl, SOCKET dataControl);
	int download(SOCKET ftpControl, SOCKET dataControl);
	int cwd(SOCKET ftpControl, SOCKET dataControl);

