#include <SDL2/SDL_net.h>
#include <iostream>

class UDPConnection
{
	private:
		bool quit;
		UDPsocket clientSocket;
		UDPsocket serverSocket;
		IPaddress serverIP;
		IPaddress clientIP;
		UDPpacket *clientPacket;
		UDPpacket *serverPacket;

	public:
		UDPConnection();
		~UDPConnection();
		bool Init();
		bool InitSDL_Net();
		bool CreatePacket(UDPpacket **packet, IPaddress ip, int32_t packetSize);
		bool OpenPort(UDPsocket &socket, int32_t port);
		bool SetIPAndPort(IPaddress &rIP, const std::string &ip, uint16_t port);
		bool Send(std::string &str);
		bool CheckForData(std::string &msg);
		bool WasQuit();
};

bool read_Hostfile(const char *path, std::string &IP, int32_t &remotePort, int32_t &localPort);
