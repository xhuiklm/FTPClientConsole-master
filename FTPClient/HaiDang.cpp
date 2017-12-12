#include "HaiDang.h"
char dir[256];		//duong dan tep
int HaiDang::upload(SOCKET ftpControl,SOCKET dataControl)
{	
	int res;
	char pathServer[1024],pathClient[1024],buf[102400];
	char command[128], cmd[512];
	printf("Nhap duong dan cua file tai len: ");
	fflush(stdin);
	gets_s(pathClient);
	printf("Nhap duong dan tren sever: ");
	fflush(stdin);
	gets_s(pathServer);
	sprintf(command, "STOR %s\n", pathServer);
	send(ftpControl, command, strlen(command), 0);//dung tra ve ma 150
	res = recv(ftpControl, cmd, 512, 0);
	cmd[res] = '\0';
	puts(cmd);
	if (strncmp(cmd, "150", 3) == SOCKET_ERROR) {
		puts("kiem tra lai duong dan tren server");
		closesocket(dataControl);
		return 0;
	}
	FILE * f;
	fopen_s(&f, pathClient, "rb");
	if (f == NULL) {
		puts("kiem tra lai duong dan file client");
		closesocket(dataControl);
		return 0;
	}
	else {
		while (true) {
			res = fread(buf, 1, 102400, f);
			if (res == 0)
				break;
			send(dataControl, buf, res, 0);
			if (res < 102400)
				break;
		}
		fclose(f);
		closesocket(dataControl);
		res = recv(ftpControl, cmd, 512, 0);
		cmd[res] = '\0';
		puts(cmd);
	}
	
	return 0;
}

int HaiDang::show(SOCKET ftpControl, SOCKET dataControl)
{
	char buf[1024];
	int res;
	char msg[512];
	char temp[64];
	send(ftpControl, "PWD\r\n", strlen("PWD\r\n"), 0);		//Yeu cau xem thu muc hien tai
	int len = recv(ftpControl, buf, sizeof(buf), 0);
	buf[len] = '\0';
	sscanf(buf, "257 \"%s\" %s", dir,temp);
	printf("Danh sach file cua thu muc %s: \n", dir);
	sprintf(msg, "LIST %s\r\n", dir);
	send(ftpControl, msg, strlen(msg), 0);			//Yeu cau liet ke danh sach tep cua thu muc hien tai
	len = recv(ftpControl, buf, sizeof(buf), 0);
	while (true)
	{
		res = recv(dataControl, buf, sizeof(buf), 0);	
		if (res == SOCKET_ERROR || res == 0)
			break;
		buf[res] = '\0';
		printf("%s", buf);
	}
	closesocket(dataControl);
	return 0;
}

int HaiDang::Quit(SOCKET ftpControl, SOCKET dataControl)
{
	char*msg = "QUIT\r\n";		//Yeu cau ngat ket noi toi Server.
	send(ftpControl, msg, strlen(msg), 0);
	printf("Ban vua ngat ket noi toi FTP Server\n");
}
//download 1 file 1 luồng.
int HaiDang::download(SOCKET ftpControl, SOCKET dataControl)
{
	int res;
	char buf[512];
	char cmd[512];
	char pathServer[128];
	char pathClient[128];
	puts("nhap duong dan file muon download:");
	fflush(stdin);
	gets_s(pathServer);
	puts("nhap duong dan se luu tren client:");
	fflush(stdin);
	gets_s(pathClient);
	sprintf(cmd, "RETR %s\n", pathServer);//dung se tra ve ma 150 Opening data channel for file download
	send(ftpControl, cmd, strlen(cmd), 0);
	res = recv(ftpControl, buf, 512, 0);
	buf[res] = '\0';
	puts(buf);
	if (strncmp(buf, "150", 3) != 0) {
		printf("\nkiem tra lai duong dan %s", pathServer);
		closesocket(dataControl);
		return false;
	}
	FILE *f = fopen(pathClient, "wb");
	if (f == NULL) {
		printf("\nkiem tra lai duong dan tren client %s", pathClient);
		closesocket(dataControl);
		return false;
	}
	char dataBuffer[102400];
	while (true) {
		res = recv(dataControl, dataBuffer, 102400, 0);
		if (res == SOCKET_ERROR || res == 0)
			break;
		fwrite(dataBuffer, 1, res, f);
	}
	res = recv(ftpControl, buf, 512, 0);//download thanh cong tra ve ma  226 Successfully transferred "/video.mp4"

	buf[res] = '\0';
	puts(buf);
	fclose(f);
	closesocket(dataControl);
	return 0;
}
