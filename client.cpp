#include "cs_network.h"

int main(int argc, char **argv)
{
	UDPConnection udpConnection;
	std::string command;

	if(udpConnection.Init() == false)
	{
		std::cout << "Erro, finalizando programa...\n";
		exit(1);
	}

	while (!udpConnection.WasQuit())
	{
		std::cout << "@servidor >";

		std::getline(std::cin, command);
		udpConnection.Send(command);
	}

	return 0;
}
