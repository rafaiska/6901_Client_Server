#include "cs_network.h"
#include <ctime>

FILE *loadLogFile(const char *path)
{
	FILE *ret = fopen(path, "r");

	if(ret == NULL)
	{
		ret = fopen(path, "w");
		fprintf(ret, "#ARQUIVO DE LOG DO PROXY\n\n");
	}
	else
	{
		fclose(ret);		
		ret = fopen(path, "a");
	}

	return ret;
}

int main(int argc, char **argv)
{
	UDPConnection udpConnection;
	std::string command;
	FILE *logfile;

	logfile = loadLogFile("logfile");

	if(udpConnection.Init() == false)
	{
		std::cout << "Erro, finalizando programa...\n";
		exit(1);
	}

	fprintf(logfile, "[%ld] Proxy UP!\n", time(NULL));

	while (!udpConnection.WasQuit())
	{
		if(udpConnection.CheckForData(command))
		{
			std::cout << "Recebido: '" << command << "'\n";
			fprintf(logfile, "[%ld] Recebido: '%s'\n", time(NULL), command.c_str());
			udpConnection.Send(command);
		}
	}

	fprintf(logfile, "[%ld] Desligando Proxy!\n", time(NULL));
	fclose(logfile);
	return 0;
}
