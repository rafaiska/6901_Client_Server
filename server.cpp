#include "cs_network.h"
#include <pthread.h>
#include <vector>
#include <algorithm>

pthread_t req_handler_thread;

std::vector <std::string> msg_buffer;	
pthread_mutex_t msg_buffer_mut;

FILE *records_file;
bool is_running;
pthread_barrier_t barreira;

void *req_handler(void *arg)
{
	UDPConnection udpConnection;
	std::string command;

	if(udpConnection.Init() == false)
	{
		std::cout << "Erro, finalizando programa...\n";
		exit(1);
	}

	pthread_barrier_wait(&barreira);
	while (!udpConnection.WasQuit() && is_running)
	{
		if(udpConnection.CheckForData(command))
		{
			pthread_mutex_lock(&msg_buffer_mut);
			msg_buffer.push_back(command);
			pthread_mutex_unlock(&msg_buffer_mut);
		}
	}
	
	std::cout << "\n\n###################################################\nATENCAO: Finalizando thread tratadora de mensagens!\n###################################################\n\n";
	pthread_exit(NULL);
}

int appendNewRecords()
{
	std::vector <std::string> buffer_copy;

	pthread_mutex_lock(&msg_buffer_mut);
	buffer_copy = msg_buffer;
	msg_buffer.clear();
	pthread_mutex_unlock(&msg_buffer_mut);

	records_file = fopen("records", "a");
	for(int i=0; i< buffer_copy.size(); ++i)
	{
		char comando[512];
		char *nome;

		strcpy(comando, buffer_copy[i].c_str());
		nome = strtok(comando, " ");

		if(strcmp(nome, "add") == 0)
		{
			nome = strtok(NULL, " ");
			while(nome != NULL)
			{	
				fprintf(records_file, "%s", nome);
				nome = strtok(NULL, " ");
				if(nome != NULL)
					fprintf(records_file, " ");
			}
			fprintf(records_file, "\n");
		}
	}

	fclose(records_file);
	return buffer_copy.size();
}

std::vector <std::string> loadRecords()
{
	std::vector <std::string> ret;
	char c, buffer[512];
	FILE *records_file = fopen("records", "r");
	int i = 0;
	
	while((c = getc(records_file)) != EOF)
	{
		if(c != '\n')
		{
			buffer[i] = c;
			++i;
		}
		else
		{
			buffer[i] = '\0';
			i = 0;
			std::string newname("");
			newname += buffer;
			ret.push_back(newname);
		}
	}
	fclose(records_file);
	return ret;
}

void dumpRecords(std::vector <std::string> recordv)
{
	FILE *records_file = fopen("records", "w");

	for(int i=0; i<recordv.size(); ++i)
		fprintf(records_file, "%s\n", recordv[i].c_str());

	fclose(records_file);
}

void deleteRecord()
{
	size_t index;
	std::vector <std::string> recordv = loadRecords();

	std::cout << "Qual o indice do elemento a ser removido? >";
	std::cin >> index;

	if(index > recordv.size())
	{
		std::cout << "ERRO: Esse elemento nao existe no arquivo!\n";
		return;
	}

	recordv.erase(recordv.begin() +index);
	dumpRecords(recordv);
}

void sortRecords()
{
	std::vector <std::string> recordv = loadRecords();
	std::sort(recordv.begin(), recordv.end());
	dumpRecords(recordv);
}

void printFile()
{
	char c;
	records_file = fopen("records", "r");
	int i = 0;

	printf("\n\n0 - ");
	while((c = getc(records_file)) != EOF)
	{
		putchar(c);
		if(c == '\n')
		{
			++i;
			c = getc(records_file);
			if(c != EOF)
			{
				printf("%d - ", i);
				putchar(c);
			}
			else
				break;
		}
	}
	printf("\n\n");
	fclose(records_file);
}

int main(int argc, char **argv)
{
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);

	pthread_barrier_init(&barreira, NULL, 2);

	pthread_mutex_init(&msg_buffer_mut, NULL);
	is_running = true;

	std::cout << "Criando thread tratadora de mensagens... ";
	if(pthread_create(&req_handler_thread, &attr, req_handler, NULL))
	{
		std::cout << "Erro ao criar thread tratadora de mensagens\n";
		exit(1);
	}
	else
	{
		std::cout << "Pronto!\n";
	}

	pthread_barrier_wait(&barreira);

	while(is_running)
	{
		int opcao;

		
		std::cout << "Escolha uma opção:\n\n";
		std::cout << "1 - Consultar registros\n";
		std::cout << "2 - Ordenar registros\n";
		std::cout << "3 - Remover registro\n";
		std::cout << "4 - Sair\n\n";
		std::cout << "Entre com um numero de 1 a 4> ";

		std::cin >> opcao;

		switch(opcao)
		{
			case 1: appendNewRecords(); printFile(); break;
			case 2: appendNewRecords(); sortRecords(); break;
			case 3: deleteRecord(); break;
			case 4: appendNewRecords(); is_running = false; break;
			default: std::cout << "Opcao invalida!\n"; break;
		}
	}	

	pthread_join(req_handler_thread, NULL);
	return 0;
}
