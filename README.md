# Resumo da Aplicação

Esse é um conjunto de 3 programas simples, escritos em C++, que simulam comunicações em um sistema cliente - proxy - servidor. Para a comunicação em rede, utilizou-se a biblioteca SDL2_net. Para gerenciar o ambiente multithread do servidor, utilizou-se a biblioteca pthreads para Linux.

# Compilação

Para compilar a aplicação, é necessário estar em um SO Linux, ter instalados o compilador GNU GCC, bem como as bibliotecas de desenvolvimento do SDL2_net e do pthreads.

Com esses requisitos satisfeitos, basta entrar no diretório do código fonte e executar o comando:

$make

Se tudo ocorrer bem, serão criados os executáveis da aplicação nas pastas ./client, ./proxy e ./server.

# Execução

Para executar em um só computador, deve-se rodar cada um dos três executáveis em um diferente terminal bash do Linux.

Para executar em diversos computadores, deve-se alterar os arquivos hostfile_cliente e hostfile_servidor em cada um dos diretórios contendo os executáveis. O IP 127.0.0.1 deve ser trocado da seguinte maneira:

./client/hostfile_servidor : Trocar pelo IP do computador que rodará o proxy.out
./proxy/hostfile_cliente : Trocar pelo IP do computador que rodará o client.out
./proxy/hostfile_servidor : Trocar pelo IP do computador que rodará o server.out
./server/hostfile_servidor : Trocar pelo IP do computador que rodará o proxy.out

# Cliente

Enquanto estiver conectado, o cliente pode enviar comandos para o servidor, os quais serão intermediados pelo proxy. Os comandos implementados até o momento são:

* add [NOME] : Adiciona um nome no arquivo de registro "records" mantido pelo servidor
* shutdown : Desliga o proxy e o mensageiro do servidor
* quit: Encerra o aplicativo cliente

# Proxy

O proxy simplesmente passa as mensagens do cliente ao servidor, atuando como um proxy anônimo. Ele guarda informações sobre a troca de mensagens em um arquivo chamado "logfile"

# Servidor

Aplicação que mantém um arquivo de registro "records", contendo vários nomes. O usuário pode realizar as seguintes operações direto do servidor:

* Consultar lista de nomes no registro
* Ordenar os registros
* Apagar um registro

Como o servidor é implementado com threads, ele continua a receber mensagens de rede mesmo quando está ocupado processando o arquivo de registros ou lendo as entradas do usuário
