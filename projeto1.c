/*
 PROJETO 1 - IAED (2014/2015)
 
 Turno: Terca-feira, 8h00
 Grupo: al049

 Elementos do grupo:	
 Goncalo Ribeiro - 82303
 Andre Mendonca - 82304
 Joao Pestana - 79515
 */

#include <stdio.h>
#include <stdlib.h>
#define LIM_REGISTOS 1000
#define LIM_NOME_BANCO 42
#define BANCO_BOM 1
#define BANCO_MAU 0

void adiciona_banco();
void classifica_banco(long referencia, int classificacao);
void adiciona_emprestimo(long referencia, long referencia_aux, int valor);
void amortiza_emprestimo(long referencia, long referencia_aux, int valor);
void emite_listagem(int tipo_listagem);
void despromove_banco();
int posicao_banco(long referencia);
void listagem_ordem_insercao();
void listagem_ordem_insercao_detalhada();
void listagem_histograma();
void detalhes_banco(int i);
void total_bancos();

typedef struct {
    char nome[LIM_NOME_BANCO];
    short int classificacao; /*1 ou 0*/
    long referencia;
} banco;

int num_bancos=0, emprestimos[LIM_REGISTOS][LIM_REGISTOS] = {{0}};
banco bancos[LIM_REGISTOS];

int main(){
    long referencia, referencia_aux;
    int valor, tipo_listagem;
    char command;
    
    while(1){
        scanf("%c", &command);
        
        switch(command){
            case 'a':
                /*Adicionar novo banco*/
                adiciona_banco();
                break;
            
            case 'k':
                /*Classificar um banco com o rating mau*/
                scanf("%ld", &referencia);
                classifica_banco(referencia,BANCO_MAU);
                break;
                
            case 'r':
                /*Promover um banco ao rating bom*/
                scanf("%ld", &referencia);
                classifica_banco(referencia,BANCO_BOM);
                break;
                
            case 'e':
                /*Adicionar emprestimo*/
                scanf("%ld%ld%d", &referencia, &referencia_aux, &valor);
                adiciona_emprestimo(referencia, referencia_aux, valor);
                break;
            
            case 'p':
                /*Adicionar amortizacao de emprestimo*/
                scanf("%ld%ld%d", &referencia, &referencia_aux, &valor);
				amortiza_emprestimo(referencia,referencia_aux, valor);
                break;
                
            case 'l':
                /*Emitir listagem*/
                scanf("%d",&tipo_listagem);
                emite_listagem(tipo_listagem);
                break;
                
            case 'K':
                /*Despromover banco bom que esta em dificuldades*/
                despromove_banco();
                break;
            
            case 'x':
                /*Sair do programa
                 O programa devera escrever o numero total de bancos registados, seguido do numero total de bancos bons. Os dois valores deverao ser impressos na mesma linha, separado por um espaco.
                 */
                total_bancos();
                return EXIT_SUCCESS;
            
            default:
                printf("Erro: Comando desconhecido\n");
        }
        getchar();
    }
    
    return EXIT_FAILURE;
}

void adiciona_banco(){
    /*
     Funcionalidade: Esta funcao permite "criar"/adicionar um novo banco a rede
     
     SINTAX: a <nome> <classificacao> <referencia>
     DADOS:
        <nome>: Maximo 40 letras
        <classificacao>: 1(Bom) ou 0(Mau)
        <referencia>: Codigo do banco (eg. 21890000), numero inteiro positivo
     OUTPUT: Nenhum
     */
    scanf("%s%hd%ld",bancos[num_bancos].nome, &bancos[num_bancos].classificacao, &bancos[num_bancos].referencia);
    
    num_bancos++; /*Incrementa o numero de registos de bancos*/
}

void classifica_banco(long referencia, int classificacao){
    /*
     Funcionalidade: Esta funcao permite atribuir o rating 0 (Mau) ou 1 (bom) a um banco
     
     SINTAX: k <referencia>
     DADOS:
        <referencia>: Codigo do banco (eg. 21890000), numero inteiro positivo
     OUTPUT: Nenhum
     */
    
    int i;
    
    for (i=0; i<num_bancos; i++){
        if (bancos[i].referencia == referencia){ /*Encontra referencia do banco a classificar*/
            bancos[i].classificacao = classificacao; /*Atribui a classificacao do respetivo banco o valor 0 (MAU) ou 1 (BOM)*/
            break;
        }
    }
    
}

void adiciona_emprestimo(long ref, long ref_aux, int valor){
    /*
     Funcionalidade: Esta funcao permite a realizacao de emprestimos
     
     SINTAX: e <referencia1> <referencia2> <valor>
     DADOS:
        <referencia1>: Referencia do banco que fornece o credito
        <referencia2>: Referencia do banco que recebe o credito
        <valor>: Valor inteiro positivo emprestado em UM
     OUTPUT: Nenhum
     */
    
    emprestimos[posicao_banco(ref)][posicao_banco(ref_aux)] += valor;
}

void amortiza_emprestimo(long ref, long ref_aux, int valor){
    /*
     Funcionalidade: Esta funcao permite que um banco abata a sua divida perante um outro banco
     
     SINTAX: p <referencia1> <referencia2> <valor>
     DADOS:
        <referencia1>: Referencia do banco pagador
        <referencia2>: Referencia do banco que recebe o valor
        <valor>: Valor inteiro positivo, menor ou igual ao valor da divida, pago em UM
     OUTPUT: Nenhum
     */
    
    emprestimos[posicao_banco(ref_aux)][posicao_banco(ref)] -= valor;
}

void emite_listagem(int tipo_lista){
    /*
     Funcionalidade: Ver seccao 5
     
     SINTAX: l <tipo>
     DADOS:
        <tipo>: Numero inteiro identificador da listagem pretendida (Seccao 5 do enunciado do projeto)
     OUTPUT: Ver seccao 5
     */
    
    switch (tipo_lista){
        case 0:
            listagem_ordem_insercao();
            break;
        case 1:
            listagem_ordem_insercao_detalhada();
            break;
        case 2:
            listagem_histograma();
            break;
        default:
            break;
    }
}

void despromove_banco(){
    /*
     Funcionalidade: Procura e desclassifica o banco bom com maior exposicao a divida de bancos maus, ou seja, o banco de classificacao igual a 1 com maior valor emprestado a bancos maus (i.e., com classificacao igual a 0). Se existir mais do que um banco nestas condicoes, o comando K devera despromover aquele que tiver sido introduzido mais recentemente no sistema.
     
     SINTAX: K
     OUTPUT: Ver seccao 5
     */
    
    int i, j, emprestimo, maior_emprestimo=1;
    long referencia, referencia_maior_emprestimo=0;
    
    for (i=0; i<num_bancos; i++){
        emprestimo=0;
        referencia = bancos[i].referencia;
        
        if (bancos[i].classificacao == BANCO_BOM){ /*Verifica se e um banco bom para entao proceder a verificacao de emprestimos*/
            for (j=0; j<num_bancos; j++){
                if (bancos[j].classificacao == BANCO_MAU){
                    emprestimo += emprestimos[i][j];
                }
            }
            
            if (emprestimo >= maior_emprestimo){ /*Verifica se o emprestimo atual e maior ou igual que o atual maior emprestimo*/
                referencia_maior_emprestimo = referencia; /*Guarda a referencia do banco que tem o maior emprestimo, ou o mesmo valor mas foi o ultimo dos dois bancos a ser registado*/
                maior_emprestimo = emprestimo; /*Define o emprestimo atual como o maior emprestimo*/
            }
        }
    }
    
    if (referencia_maior_emprestimo != 0){
        classifica_banco(referencia_maior_emprestimo,BANCO_MAU);
        printf("*");
        detalhes_banco(posicao_banco(referencia_maior_emprestimo));
    }
    
    total_bancos();
}


/*FUNCOES AUXILIARES*/

int posicao_banco(long ref){
    /*
     Funcionalidade: Procura e devolve a posicao do banco dentro do array bancos
     OUTPUT: (e.g: 3)
     */
    
    int i;
    
    for (i=0; i<num_bancos; i++){
        if (bancos[i].referencia == ref){ /*Encontra referencia do banco*/
            return i; /*Devolve a posicao do banco*/
        }
    }
    
    return EXIT_FAILURE; /*Nao encontrou nenhum banco com a referencia indicada, gera erro*/
}

void listagem_ordem_insercao(){
    /*
     Funcionalidade: Lista todos os bancos por ordem de insercao
     OUTPUT: referencia nome classificacao
     */
    
    int i;
    
    for (i=0; i<num_bancos; i++){
        printf("%ld %s %d\n", bancos[i].referencia, bancos[i].nome, bancos[i].classificacao);
    }
    
}

void listagem_ordem_insercao_detalhada(){
    /*
     Funcionalidade: Lista todos os bancos por ordem de insercao com os detalhes de emprestimos, parceiros, etc...
     OUTPUT: referencia nome classificacao inP outP outV outVM inV inVM
     
     inP: Numero de bancos a que o banco tem dividas
     outP: Numero de bancos a que o banco emprestou dinheiro
     outV: Valor total emprestado a outros bancos
     outVM: Valor total emprestado a bancos maus
     inV: Valor total que o banco recebeu de outros bancos como emprestimo
     intVM: Valor total que o banco recebeu de bancos maus como emprestimo
     */
    
    int i;
    
    for (i=0; i<num_bancos; i++){
        detalhes_banco(i);
    }
}

void listagem_histograma(){
    /*
     Funcionalidade: Lista o numero de parceiros, e quantos bancos tem esse mesmo numero de parceiros
     OUTPUT: numero_parceiros d(numero_de_bancos com o numero_parceiros) ----> e.g: 0 d(0)
     */
    
    int i, j, k=0, bancos_parceiros[LIM_REGISTOS]={0}; /*{0} coloca todos os elementos do array a 0*/
    
    /*Realiza a contagem das parcerias*/
    for (i=0; i<num_bancos; i++){
        for (j=0; j<num_bancos; j++){
            if ((emprestimos[i][j] != 0) || (emprestimos[j][i] != 0)){ /*Existiram emprestimos entre o banco i e o banco j*/
                k++; /*Incrementa o numero de parceiros*/
            }
        }
        bancos_parceiros[k]++; /*Incrementa o numero de bancos com k parceiros*/
        k=0;
    }
    
    /*Imprime os resultados de parcerias na forma 0 0 ou 1 3 ou 4 7, ou seja existem por exemplo 7 bancos que tem 4 parceiros*/
    for (i=0; i<num_bancos; i++){
        if (bancos_parceiros[i] != 0){ /*Se houver pelo menos um banco com i parceiros imprime*/
            printf("%d %d\n", i, bancos_parceiros[i]);
        }
    }
}

void detalhes_banco(int i){
    int j, k, inP=0, outP=0, outV=0, outVM=0, inV=0, inVM=0;
    
    /*Percorre colunas da matriz emprestimo*/
    for (j=0; j<num_bancos; j++){
        if (emprestimos[i][j] != 0){ /*Valor de emprestimo diferente de 0*/
            outP++;
            outV += emprestimos[i][j];
            
            if (bancos[j].classificacao == BANCO_MAU){
                outVM += emprestimos[i][j];
            }
        }
    }
    
    /*Percorre linhas da matriz emprestimos*/
    for (k=0; k<num_bancos; k++){
        if (emprestimos[k][i] != 0){ /*Valor de emprestimo diferente de 0*/
            inP++;
            inV += emprestimos[k][i];
            
            if (bancos[k].classificacao == BANCO_MAU){
                inVM += emprestimos[k][i];
            }
        }
    }
    
    printf("%ld %s %d %d %d %d %d %d %d\n", bancos[i].referencia, bancos[i].nome, bancos[i].classificacao, inP, outP, outV, outVM, inV, inVM);
}

void total_bancos(){
	/*
	 Funcionalidade: Mostra o numero total de bancos que existem e quantos deles sao bancos com classificacao 1 (BOM)	 OUTPUT: <numero total de bancos> <numero de bancos bons> ----> e.g: 100 40
	 */
    int i, bancos_bons=0;
    
    for (i=0; i<num_bancos; i++){
        if (bancos[i].classificacao == BANCO_BOM){
            bancos_bons++;
        }
    }
    
    printf("%d %d\n", num_bancos, bancos_bons);
}
