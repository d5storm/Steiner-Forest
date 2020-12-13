# Floresta de Steiner
É necessário executar o arquivo "make" antes de rodas o algoritmo para a Floresta

O algoritmo de árvore de steiner slva a solução na pasta bin, então é sugerido salvar as intâncias nessa mesma pasta;

Para execução do algoritmo da Floresta é necessário acrescentar a flag "-std=c++11" 

Dessa forma:
"g++ -std=c++11 Forest_Steiner.cpp -o main"

Argumentos: 
- caminho da instância;
- nome da instância
- número de iterações
- seed

Ex.: ./main /home/Luftschlange-ms-steiner-puw-324769b/bin jmp-n-50-a-160-p-025-k-5-s-4731.dat 100 1\n"
