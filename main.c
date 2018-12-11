#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//estrutura pessoa
typedef struct {
	char cpf[12];
	char nome[51];
	char telefone[10];
	char nascimento[11];
	char endereco[50];
	char bairro[50];
	char numero[5];
	char cidade[50];
	char uf[3];
} Pessoa;
//arvore de clientes
typedef struct {
	Pessoa cliente;
	struct Clientes *e;
	struct Clientes *d;
} Clientes;
//quebra a linha do arquivo csv em substrings, utilizando como critério de corte o delimitador informado
int explode(char ***arr_ptr, char *str, char delimiter){
	char *src = str, *end, *dst;
	char **arr;
	int size = 1, i;

    // Find number of strings
	while ((end = strchr(src, delimiter)) != NULL){
		++size;
		src = end + 1;
	}

	arr = malloc(size * sizeof(char *) + (strlen(str) + 1) * sizeof(char));

	src = str;
	dst = (char *) arr + size * sizeof(char *);
	for (i = 0; i < size; ++i){
		if ((end = strchr(src, delimiter)) == NULL)
			end = src + strlen(src);
		arr[i] = dst;
		strncpy(dst, src, end - src);
		dst[end - src] = '\0';
		dst += end - src + 1;
		src = end + 1;
	}
	*arr_ptr = arr;

	return size;
}
//insere recusivamente, comparando-se o cpf
void inserir(Clientes **clientes, Pessoa pessoa) {
	Clientes *novoCliente;
	novoCliente = (Clientes *) malloc( sizeof(Clientes) );
	if (*clientes == NULL) {

		novoCliente->cliente = pessoa;
		novoCliente->e = NULL;
		novoCliente->d = NULL;

		*clientes = novoCliente;
	} else {
		free(novoCliente);
		if(strcmp((*clientes)->cliente.cpf, pessoa.cpf) < 0) {
			inserir((&(*clientes)->d), pessoa);
		} else {
			inserir((&(*clientes)->e), pessoa);
		}

	}

}
//imprime recusivamente conforme critério de ordenação de arvore
void imprimir(Clientes *clientes) {
	if(clientes != NULL) {
		imprimir(clientes->e);
		printf("\nNOME do cliente: %s\n", clientes->cliente.nome);
		printf("CPF do cliente: %s\n", clientes->cliente.cpf);
		printf("TELEFONE do cliente: %s\n", clientes->cliente.telefone);
		printf("NASCIMENTO do cliente: %s\n", clientes->cliente.nascimento);
		printf("ENDERECO do cliente: %s\n", clientes->cliente.endereco);
		printf("BAIRRO do cliente: %s\n", clientes->cliente.bairro);
		printf("NUMERO do cliente: %s\n", clientes->cliente.numero);
		printf("CIDADE do cliente: %s\n", clientes->cliente.cidade);
		printf("UF do cliente: %s\n\n", clientes->cliente.uf);
		imprimir(clientes->d);
	}
}
//utiliza o criterio de menor da direita para exclucao
Clientes *MenorDireita(Clientes **clientes) {
	if((*clientes)->e != NULL) {
		return MenorDireita(&(*clientes)->e);
	} else {
		Clientes *aux = *clientes;
		if((*clientes)->d != NULL) {
			*clientes = (*clientes)->d;
		} else {
			*clientes = NULL;
		}
		return aux;
	}
}
//funcao de exclusao que utiliza o criterio de escolher o menor da direita
Clientes excluir(Clientes **clientes, Pessoa pessoa) {
	if(*clientes == NULL) {
		printf("CLIENTE INEXISTENTE!!!\n");
		getch();
		return;
	}
	if(strcmp((*clientes)->cliente.cpf, pessoa.cpf) < 0)
		excluir(&(*clientes)->d, pessoa);
	else if(strcmp((*clientes)->cliente.cpf, pessoa.cpf) > 0)
		excluir(&(*clientes)->e, pessoa);
	else {
		Clientes *clienteAuxiliar = *clientes;
		if (((*clientes)->e == NULL) && ((*clientes)->d == NULL)) {
			free(clienteAuxiliar);
			(*clientes) = NULL;
		}
		else {
			if ((*clientes)->e == NULL) {
				(*clientes) = (*clientes)->d;
				clienteAuxiliar->d = NULL;
				free(clienteAuxiliar);
				clienteAuxiliar = NULL;
			}
			else {
				if ((*clientes)->d == NULL) {
					(*clientes) = (*clientes)->e;
					clienteAuxiliar->e = NULL;
					free(clienteAuxiliar);
					clienteAuxiliar = NULL;
				}
				else {
					clienteAuxiliar = MenorDireita(&(*clientes)->d);
					clienteAuxiliar->e = (*clientes)->e;
					clienteAuxiliar->d = (*clientes)->d;
					(*clientes)->e = (*clientes)->d = NULL;
					free((*clientes));
					*clientes = clienteAuxiliar;
					clienteAuxiliar = NULL;
				}
			}
		}
	}
	printf("CLIENTE EXCLUIDO!!!\n");
}
//busca comparando-se pelo cpf
void buscar(Clientes *clientes, Pessoa pessoa) {
	if (clientes == NULL) {
		printf("CLIENTE INEXISTENTE!!!\n");
	} else {
		if(strcmp(clientes->cliente.cpf, pessoa.cpf) < 0) {
			buscar(clientes->d, pessoa);
		} else {
			if(strcmp(clientes->cliente.cpf, pessoa.cpf) > 0) {
				buscar(clientes->e, pessoa);
			} else {
				printf("\nNOME do cliente: %s\n", clientes->cliente.nome);
				printf("CPF do cliente: %s\n", clientes->cliente.cpf);
				printf("TELEFONE do cliente: %s\n", clientes->cliente.telefone);
				printf("NASCIMENTO do cliente: %s\n", clientes->cliente.nascimento);
				printf("ENDERECO do cliente: %s\n", clientes->cliente.endereco);
				printf("BAIRRO do cliente: %s\n", clientes->cliente.bairro);
				printf("NUMERO do cliente: %s\n", clientes->cliente.numero);
				printf("CIDADE do cliente: %s\n", clientes->cliente.cidade);
				printf("UF do cliente: %s\n\n", clientes->cliente.uf);
			}
		}

	}
}
//exporta para arquivo csv
//primeiro é criado uma string da linha, obedecendo a construcao de colunas separadas por ponto e virgula e fins de linha com quebra de linha
//note que o criterio de ordenaçao de escrita é o de pre ordem
void exportar(Clientes *clientes, FILE *arquivo) {
	if(clientes != NULL) {
		if(arquivo == NULL) {
			printf("\nERROR AO ABRIR O ARQUIVO\n");
		} else {
			char linha[250] = "";
			strcat(linha, clientes->cliente.cpf);
			strcat(linha, ";");
			strcat(linha, clientes->cliente.nome);
			strcat(linha, ";");
			strcat(linha, clientes->cliente.telefone);
			strcat(linha, ";");
			strcat(linha, clientes->cliente.nascimento);
			strcat(linha, ";");
			strcat(linha, clientes->cliente.endereco);
			strcat(linha, ";");
			strcat(linha, clientes->cliente.bairro);
			strcat(linha, ";");
			strcat(linha, clientes->cliente.numero);
			strcat(linha, ";");
			strcat(linha, clientes->cliente.cidade);
			strcat(linha, ";");
			strcat(linha, clientes->cliente.uf);
			strcat(linha, "\n");
			if(fputs(linha, arquivo) == EOF)
				printf("\nERROR AO ABRIR O ARQUIVO\n");
		}
		exportar(clientes->e, arquivo);
		exportar(clientes->d, arquivo);

	}

}
//acessa o arquivo csv linha a linha, quebra a string montada da linha em várias outras e recria a arvore
void importar(Clientes **clientes, FILE *arquivo) {
	char linha[250];
	Pessoa pessoa;
	if(arquivo == NULL) {
		printf("Erro ao abrir o arquivo");
	} else {
		char **arr;
		int i= 0;
		while((fscanf(arquivo, "%s", linha)) != EOF) {
			if(i!= 0){
				explode(&arr, &linha, ';');
				memcpy(pessoa.cpf, arr[0], 12);
				memcpy(pessoa.nome, arr[1], 51);
				memcpy(pessoa.telefone, arr[2], 10);
				memcpy(pessoa.nascimento, arr[3], 11);
				memcpy(pessoa.endereco, arr[4], 50);
				memcpy(pessoa.bairro, arr[5], 50);
				memcpy(pessoa.numero, arr[6], 5);
				memcpy(pessoa.cidade, arr[7], 50);
				memcpy(pessoa.uf, arr[8], 3);
				inserir(clientes, pessoa);
			}
			i++;
		}
		free(arr);

	}
}
int main() {
	Clientes *clientes = NULL;
	Pessoa cliente;
	FILE *arquivo;
	int op;
	printf("================== Seja bem vindo ==================\n" );
	printf("===== Feito por Gelson Marcony Mendes Caldeira =====\n\n" );
	printf("================ Gestao de Clientes ================\n\n\n" );
	while(1) {
		printf("1 - Cadastrar Cliente\n" );
		printf("2 - Buscar Cliente\n" );
		printf("3 - Listar Clientes\n" );
		printf("4 - Excluir Cliente\n" );
		printf("5 - Exportar em CSV\n" );
		printf("6 - Importar em CSV\n" );
		printf("7 - Sair\n" );
		printf("0 - Limpar tela\n\n" );
		printf("Escolha a opcao: " );
		scanf("%d", &op);
		fflush(stdin);
		switch(op) {
			case 1:
			printf("Digite o CPF: " );
			scanf("%[^\n]s", cliente.cpf);
			fflush(stdin);
			printf("Digite o NOME: " );
			scanf("%[^\n]s", cliente.nome);
			fflush(stdin);
			printf("Digite o TELEFONE: " );
			scanf("%[^\n]s", cliente.telefone);
			fflush(stdin);
			printf("Digite o NASCIMENTO: " );
			scanf("%[^\n]s", cliente.nascimento);
			fflush(stdin);
			printf("Digite o ENDERECO: " );
			scanf("%[^\n]s", cliente.endereco);
			fflush(stdin);
			printf("Digite o BAIRRO: " );
			scanf("%[^\n]s", cliente.bairro);
			fflush(stdin);
			printf("Digite o NUMERO: " );
			scanf("%[^\n]s", cliente.numero);
			fflush(stdin);
			printf("Digite o CIDADE: " );
			scanf("%[^\n]s", cliente.cidade);
			fflush(stdin);
			printf("Digite o UF: " );
			scanf("%[^\n]s", cliente.uf);
			fflush(stdin);
			inserir(&clientes, cliente);
			printf("\nCLIENTE CADASTRADO!\n\n" );
			break;
			case 2:
			printf("Digite o CPF para buscar o cliente: " );
			scanf("%[^\n]s", cliente.cpf);
			fflush(stdin);
			buscar(clientes, cliente);
			break;
			case 3:
			imprimir(clientes);
			break;
			case 4:
			printf("Digite o CPF para excluir o cliente: " );
			scanf("%[^\n]s", cliente.cpf);
			excluir(&clientes, cliente);
			break;
			case 5:
			arquivo = fopen("clientes.csv", "wt");
			char cabecalho[250] = "";
			strcat(cabecalho, "CPF");
			strcat(cabecalho, ";");
			strcat(cabecalho, "NOME");
			strcat(cabecalho, ";");
			strcat(cabecalho, "TELEFONE");
			strcat(cabecalho, ";");
			strcat(cabecalho, "NASCIMENTO");
			strcat(cabecalho, ";");
			strcat(cabecalho, "ENDERECO");
			strcat(cabecalho, ";");
			strcat(cabecalho, "BAIRRO");
			strcat(cabecalho, ";");
			strcat(cabecalho, "NUMERO");
			strcat(cabecalho, ";");
			strcat(cabecalho, "CIDADE");
			strcat(cabecalho, ";");
			strcat(cabecalho, "UF");
			strcat(cabecalho, "\n");
			if(fputs(cabecalho, arquivo) == EOF)
				printf("\nERROR AO ABRIR O ARQUIVO\n");

			exportar(clientes, arquivo);
			fclose(arquivo);
			break;
			case 6:
			arquivo = fopen("clientes.csv", "r");
			importar(&clientes, arquivo);
			fclose(arquivo);
			break;
			case 7:
			return 0;
			break;
			case 0:
			system("cls");
			break;
			default:
			printf("ops... opcao nao encontrada\n\n" );
			break;

		}

	}
	return 0;
}
