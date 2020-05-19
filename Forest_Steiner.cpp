//
//  Forest_Steiner.cpp
//  Steiner_Tree
//
//  Created by sergio junior on 12/11/19.
//  Copyright © 2019 sergio junior. All rights reserved.
//

#include "Forest_Steiner.hpp"
#include "Read_Instance.hpp"
#include<iostream>
#include <list>
#include <limits.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <dirent.h>
#include <experimental/filesystem>
#include <algorithm>
#include <iterator>
#include <sys/types.h>
#include<unistd.h>
#include <stdlib.h>

using namespace std;



int main(int argc, char *argv[])
{
    Grafo * test = create_steiner_forest(argv[1]);
    test->createSteinerTrees();
    cout << "created steiner trees" << endl;
    test->solveSteinerTrees();
    test->printGraph();
    
    cin.get();
    exit(1);
    // if(argc!=6){
    //     printf("Argumentos Faltando\n Por favor inserir:\n - caminho da instância;\n - nome da instância\n - número de iterações\n - seed .");
    //     printf("\nEx.: ./main /home/Luftschlange-ms-steiner-puw-324769b/bin jmp-n-50-a-160-p-025-k-5-s-4731.dat 100 1\n");
    // }else{
    //     for(int seed=1; seed < 11; seed++){
    //         std::string steiner_path=argv[1]; //Caminho onde esta o algoritmo de uchoa
    //         std::string str_path=argv[2]; //Caminho onde as instâncias estão
    //         std::string inst=argv[3]; //Nome da instância
    //         std::string turns=argv[4]; //Numero de iterações
    //         // std::string seed=argv[5]; //semente
    //         std::string intancia(str_path+"/"+inst); //caminho do arquivo instância
        
    //         std::string str_path_split=str_path+"/Instance_"; //formato do nome do arquivo após split
            
    //         int nterminais=n_terminals(intancia.c_str()); //extraindo número de conjunto de terminais
    //         int nEdges=n_edges(intancia.c_str()); //extraindo número de edges
    //         // printf("ntermianis %d - edges %d\n",nterminais,nEdges);
            
    //         split_file_Instance(intancia.c_str(),str_path_split.c_str(),nEdges,nterminais); //split do arquivo da floresta de steiner
            
    //         // cout << "Executando Arvore de Steiner" << endl;
    //         // cin.get();
    //         ///////////////////// Executando Algoritmo da Árvore de Steiner ///////////////////////////
    //         // chdir(str_path.c_str()); //mudança de diretório
    //         std::string  str;
    //         std::string  str2;
    //         std::string  strPy;
            
    //         auto start = std::chrono::system_clock::now();
    //         for (int i=0;i<nterminais;i++){
    //             // str = "./bin/steiner ";
    //             str2 = steiner_path+" " +str_path+"/Instance_T_"+to_string(i)+".txt -msit "+turns+" -seed "+ to_string(seed);
    //             // cout << "string to be exec: " + str2 << endl;
    //             system(str2.c_str());
    //         }
    //         ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
    //         Grafo g1(nEdges+1);
    //         vector<string> s;
    //         std::string buf(str_path+"/");

    //         // cout << "buf: " + buf << endl;
            
    //         // s=list_dir(str_path.c_str()); //montando vetor com nome dos arquivos da pasta onde é salvo a solução
    //         s=list_dir("./"); //montando vetor com nome dos arquivos da pasta onde é salvo a solução
            
    //         // for (int i=0;i<s.size();i++){
    //         //     cout << s[i] + " " ;
    //         // }
    //         // cout << endl;
    //         // cin.get();

    //         // cout << "Unindo Arvores de Steiner" << endl;
    //         // cin.get();
    //         /////////////////////////////////////  Unindo árvores em um único grafo /////////////////////////////////////////
    //         for (int i=0;i<s.size();i++){
    //             vector<string> sep = split(s.at(i), '.');
    //             if(strncmp(sep.back().c_str(),"sol",1)==0 && int(sep.size())>2){
                    
    //                 std::string path1=s.at(i);
    //                 // printf("%s\n",path1.c_str());
    //                 read_file("",path1.c_str(),&g1);
    //             }
    //         }
            
    //         stack<int> listg1;
    //         int Component_connected=0;
            
    //         // printf("Structure Before Prim\n");
    //         // print_struct(str_path.c_str(),&g1); // print da estrutura unida
    //         // cin.get();
            
    //         Grafo gFinal(nEdges);
            
    //         g1.ConnecComp(listg1,&Component_connected,&gFinal); // retirando cilos das componentes conexas
            
    //         // printf("Final Solution Find!\n");
    //         int final_cost = print_struct(str_path.c_str(),&gFinal); // print da estrutura final
    //         // cin.get();
            
    //         /////////////////////////////////////  Medindo tempo total do algoritmo /////////////////////////////////////////
    //         auto end = std::chrono::system_clock::now();
    //         std::chrono::duration<double> elapsed_seconds = end-start;
    //         std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    //         std::cout << "finished computation at " << std::ctime(&end_time)
    //                 << "elapsed time: " << elapsed_seconds.count() << "s\n";

    //         // escrevendo solução //

    //         ofstream myfile;
    //         std::string write_file = "./results/sergio.txt";
    //         myfile.open (write_file, std::ios_base::app);
    //         myfile << inst << " " << final_cost << " " << elapsed_seconds.count() << endl;
    //         myfile.close();
    //         // limpando arquivos auxiliares //

    //         vector<string> temp_inst=list_dir(str_path.c_str());

    //         for (int i=0;i<temp_inst.size();i++){
                
    //             vector<string> sep = split(temp_inst.at(i), '.');
    //             // cout << temp_inst.at(i) << endl;
    //             if(strncmp(sep.back().c_str(),"txt",1)==0){
    //                 // cout << "entrou!" << endl;
    //                 std::string path1=temp_inst.at(i);
                    
    //                 std::string exec_command = "rm -f " + str_path + "/" + path1;
    //                 // printf("%s\n",exec_command.c_str());
    //                 // cin.get();
    //                 system(exec_command.c_str());
    //             }
    //         }

    //         for (int i=0;i<s.size();i++){
    //             vector<string> sep = split(s.at(i), '.');
    //             if(strncmp(sep.back().c_str(),"sol",1)==0 && int(sep.size())>2){
                    
    //                 std::string path1=s.at(i);
    //                 // printf("%s\n",path1.c_str());
    //                 std::string exec_command = "rm -f ./"+ path1;
    //                 system(exec_command.c_str());
    //             }
    //         }
    //     }
    // }
    return 0;
}
