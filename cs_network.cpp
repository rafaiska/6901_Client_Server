#include "cs_network.h"

UDPConnection::UDPConnection( )
{
	quit = false;
}
UDPConnection::~UDPConnection( )
{
	SDLNet_FreePacket(clientPacket);
	SDLNet_FreePacket(serverPacket);
	SDLNet_Quit();
}

bool UDPConnection::Init()
{
	std::string ip_client, ip_server;
	int32_t remotePortClient, localPortClient, remotePortServer, localPortServer;

	if(read_Hostfile("hostfile_cliente", ip_client, remotePortClient, localPortClient) == false) return false;
	if(read_Hostfile("hostfile_servidor", ip_server, remotePortServer, localPortServer) == false) return false;

	std::cout << "Conectando a \n\tIP : " << ip_client << "\n\tPorta : " << remotePortClient << std::endl;
	std::cout << "Local port : " << localPortClient << "\n\n";
	std::cout << "Conectando a \n\tIP : " << ip_server << "\n\tPorta : " << remotePortServer << std::endl;
	std::cout << "Local port : " << localPortServer << "\n\n";

	// Initialize SDL_net
	if (!InitSDL_Net())
		return false;

	if (!OpenPort(clientSocket, localPortClient))
		return false;

	if (!OpenPort(serverSocket, localPortServer))
		return false;

	if (!SetIPAndPort(clientIP, ip_client, remotePortClient))
		return false;

	if (!SetIPAndPort(serverIP, ip_server, remotePortServer))
		return false;

	if (!CreatePacket(&clientPacket, clientIP, 512))
		return false;

	if (!CreatePacket(&serverPacket, serverIP, 512))
		return false;

	return true;
}

bool UDPConnection::InitSDL_Net()
{
	std::cout << "Inicializando SDL_net...\n";

	if ( SDLNet_Init() == -1 )
	{
		std::cout << "\tSDLNet_Init failed : " << SDLNet_GetError() << std::endl;
		return false; 
	}

	std::cout << "\tSuccesso!\n\n";
	return true;
}

bool UDPConnection::CreatePacket(UDPpacket **packet, IPaddress ip, int32_t packetSize)
{
	std::cout << "Criando pacote com tamanho " << packetSize << "...\n";

	*packet = SDLNet_AllocPacket(packetSize);

	if ( *packet == NULL )
	{
		std::cout << "\tSDLNet_AllocPacket failed : " << SDLNet_GetError() << std::endl;
		return false; 
	}

	(*packet)->address.host = ip.host; 
	(*packet)->address.port = ip.port;

	std::cout << "\tSuccesso!\n\n";
	return true;
}

bool UDPConnection::OpenPort(UDPsocket &socket, int32_t port)
{
	std::cout << "Abrindo porta " << port << "...\n";

	// Sets our sovket with our local port
	socket = SDLNet_UDP_Open(port);

	if (socket == NULL)
	{
		std::cout << "\tSDLNet_UDP_Open failed : " << SDLNet_GetError() << std::endl;
		return false; 
	}

	std::cout << "\tSuccesso!\n\n";
	return true;
}

bool UDPConnection::SetIPAndPort(IPaddress &rIP, const std::string &ip, uint16_t port )
{
	std::cout << "Configurando IP ( " << ip << " ) " << "e porta ( " << port << " )\n";

	if (SDLNet_ResolveHost(&rIP, ip.c_str(), port)  == -1)
	{
		std::cout << "\tSDLNet_ResolveHost failed : " << SDLNet_GetError() << std::endl;
		return false; 
	}

	std::cout << "\tSuccesso!\n\n";
	return true; 
}

bool UDPConnection::Send(std::string &msg)
{
	if (msg == "quit")
	{
		quit = true;
		return true;
	}

	memcpy(serverPacket->data, msg.c_str(), msg.length() +1);
	serverPacket->len = msg.length() +1;

	if (SDLNet_UDP_Send(serverSocket, -1, serverPacket) == 0)
	{
		std::cout << "\tSDLNet_UDP_Send failed : " << SDLNet_GetError() << "\n";
		return false; 
	}

	return true;
}

bool UDPConnection::CheckForData(std::string &msg)
{
	char buffer[512];

	if (SDLNet_UDP_Recv(clientSocket, clientPacket))
	{
		memcpy(buffer, clientPacket->data, clientPacket->len);
		msg.clear();
		msg += buffer;
		if(msg == "shutdown")
			quit = true;
		return true;
	}

	return false;
}

bool UDPConnection::WasQuit()
{
	return quit;
}

bool read_Hostfile(const char* path, std::string &IP, int32_t &remotePort, int32_t &localPort)
{
	FILE *hostfile;
	char buffer[64];
	int port;

	hostfile = fopen(path, "r");
	if(hostfile == NULL)
	{
		std::cout << "ERRO: Nao ha hostfile presente: " << path << "\n";
		return false;
	}

	if(fscanf(hostfile, "%s", buffer) == 0)
	{
		std::cout << "ERRO: Hostfile corrompido: " << path << "\n";
		return false;
	}
	IP.clear();
	IP += buffer;

	if(fscanf(hostfile, "%d", &port) == 0)
	{
		std::cout << "ERRO: Hostfile corrompido: " << path << "\n";
		return false;
	}
	remotePort = port;
	
	if(fscanf(hostfile, "%d", &port) == 0)
	{
		std::cout << "ERRO: Hostfile corrompido: " << path << "\n";
		return false;
	}
	localPort = port;

	return true;
}
