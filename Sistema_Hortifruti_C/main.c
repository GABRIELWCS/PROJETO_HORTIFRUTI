#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

#define MAX_TENTATIVAS 3
#define MAX_PRODUTOS 100
#define ARQUIVO_ESTOQUE "estoque.txt"  // Nome do arquivo onde os produtos serão salvos

typedef struct {
    char nome[50];
    int quantidade;
    float preco;
    float custo;
} Produto;
//funcoes
int loginCaixa();
int loginEstoque();
int loginAdm();
void cadastrarProduto(Produto produtos[], int* numProdutos);
void visualizarEstoque(Produto produtos[], int numProdutos);
void contabilizarEstoque(Produto produtos[], int numProdutos);
void calcularTotal(Produto produtos[], int n, float* total);
void calcularTroco(float total, float pagamento, float* troco);
void calcularLucro(Produto produtos[], int numProdutos);
void removerProduto(Produto produtos[], int* numProdutos);
void salvarEstoque(Produto produtos[], int numProdutos);
void carregarEstoque(Produto produtos[], int* numProdutos);
void menuCaixa(Produto produtos[], int numProdutos);

int main() {
    setlocale(LC_ALL, "");
    Produto produtos[MAX_PRODUTOS];
    int numProdutos = 0;
    int opcao;

    // Carregar o estoque salvo em arquivo
    carregarEstoque(produtos, &numProdutos);

    do {
        printf("Selecione o nível de acesso:\n");
        printf("1. Caixa\n");
        printf("2. Estoque\n");
        printf("3. Adm\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (loginCaixa() == 0) { // Verifica o login
                    printf("Acesso negado ao Caixa. Retornando ao menu principal...\n");
                    break; // Retorna ao menu principal se o login falhar
                }
                menuCaixa(produtos, numProdutos);
                break;
            case 2:
                if (loginEstoque() == 0) { // Verifica o login
                    printf("Acesso negado ao Estoque. Retornando ao menu principal...\n");
                    break; // Retorna ao menu principal se o login falhar
                }
                int opcaoEstoque;
                do {
                    printf("\nMenu de Estoque:\n");
                    printf("1. Cadastrar Produto\n");
                    printf("2. Visualizar Estoque\n");
                    printf("3. Contabilizar Estoque\n");
                    printf("4. Remover Produto\n");
                    printf("0. Sair\n");
                    printf("Escolha uma opção: ");
                    scanf("%d", &opcaoEstoque);

                    switch (opcaoEstoque) {
                        case 1:
                            cadastrarProduto(produtos, &numProdutos);
                            salvarEstoque(produtos, numProdutos);  // Salva após cadastrar
                            break;
                        case 2:
                            visualizarEstoque(produtos, numProdutos);
                            break;
                        case 3:
                            contabilizarEstoque(produtos, numProdutos);
                            break;
                        case 4:
                            removerProduto(produtos, &numProdutos);
                            salvarEstoque(produtos, numProdutos);  // Salva após remover
                            break;
                        case 0:
                            printf("Saindo do Menu de Estoque...\n");
                            break;
                        default:
                            printf("Opção inválida! Tente novamente.\n");
                    }
                } while (opcaoEstoque != 0);
                break;
            case 3:
                if (loginAdm() == 0) { // Verifica o login
                    printf("Acesso negado à Adm. Retornando ao menu principal...\n");
                    break; // Retorna ao menu principal se o login falhar
                }
                int opcaoAdm;
                do {
                    printf("\nMenu de Administração:\n");
                    printf("1. Calcular Lucro\n");
                    printf("0. Sair\n");
                    printf("Escolha uma opção: ");
                    scanf("%d", &opcaoAdm);

                    switch (opcaoAdm) {
                        case 1:
                            calcularLucro(produtos, numProdutos);
                            break;
                        case 0:
                            printf("Saindo do Menu de Administração...\n");
                            break;
                        default:
                            printf("Opção inválida! Tente novamente.\n");
                    }
                } while (opcaoAdm != 0);
                break;
            case 0:
                printf("Saindo...\n");
                salvarEstoque(produtos, numProdutos);  // Salva ao sair do programa
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

int loginCaixa() {
    char usuario[20], senha[20];
    int tentativas = 0;

    while (tentativas < MAX_TENTATIVAS) {
        printf("Login Caixa:\n");
        printf("Usuário: ");
        scanf("%s", usuario);
        printf("Senha: ");
        scanf("%s", senha);

        if (strcmp(usuario, "caixa") == 0 && strcmp(senha, "senhaCaixa123") == 0) {
            printf("Login bem-sucedido no Caixa!\n");
            return 1; // Login bem-sucedido
        } else {
            printf("Usuário ou senha incorretos. Tente novamente.\n");
            tentativas++;
        }
    }

    printf("Número máximo de tentativas alcançado. Acesso negado ao Caixa.\n");
    return 0; // Login falhou
}

void menuCaixa(Produto produtos[], int numProdutos) {
    float total = 0.0, pagamento, troco;
    int numProdutosVenda, continuarVenda;

    do {
        printf("Quantos produtos deseja registrar na venda? (máx %d): ", MAX_PRODUTOS);
        scanf("%d", &numProdutosVenda);

        if (numProdutosVenda > MAX_PRODUTOS) {
            printf("Número de produtos excede o limite permitido.\n");
            continue;
        }

        for (int i = 0; i < numProdutosVenda; i++) {
            printf("Produto %d:\n", i + 1);
            printf("Nome: ");
            scanf("%s", produtos[i].nome);
            printf("Preço: R$ ");
            scanf("%f", &produtos[i].preco);
        }

        calcularTotal(produtos, numProdutosVenda, &total);
        printf("Total da compra: R$ %.2f\n", total);

        printf("Valor recebido: R$ ");
        scanf("%f", &pagamento);

        calcularTroco(total, pagamento, &troco);

        if (troco < 0) {
            printf("Valor recebido insuficiente. O pagamento não foi concluído.\n");
        } else {
            printf("Troco: R$ %.2f\n", troco);
        }

        printf("Deseja realizar outra venda? (1 - Sim, 0 - Não): ");
        scanf("%d", &continuarVenda);

    } while (continuarVenda == 1);
}

int loginEstoque() {
    char usuario[20], senha[20];
    int tentativas = 0;

    while (tentativas < MAX_TENTATIVAS) {
        printf("Login Estoque:\n");
        printf("Usuário: ");
        scanf("%s", usuario);
        printf("Senha: ");
        scanf("%s", senha);

        if (strcmp(usuario, "estoque") == 0 && strcmp(senha, "senhaEstoque123") == 0) {
            printf("Login bem-sucedido no Estoque!\n");
            return 1; // Login bem-sucedido
        } else {
            printf("Usuário ou senha incorretos. Tente novamente.\n");
            tentativas++;
        }
    }

    printf("Número máximo de tentativas alcançado. Acesso negado ao Estoque.\n");
    return 0; // Login falhou
}

int loginAdm() {
    char usuario[20], senha[20];
    int tentativas = 0;

    while (tentativas < MAX_TENTATIVAS) {
        printf("Login Adm:\n");
        printf("Usuário: ");
        scanf("%s", usuario);
        printf("Senha: ");
        scanf("%s", senha);

        if (strcmp(usuario, "adm") == 0 && strcmp(senha, "senhaAdm123") == 0) {
            printf("Login bem-sucedido na Adm!\n");
            return 1; // Login bem-sucedido
        } else {
            printf("Usuário ou senha incorretos. Tente novamente.\n");
            tentativas++;
        }
    }

    printf("Número máximo de tentativas alcançado. Acesso negado à Adm.\n");
    return 0; // Login falhou
}

void cadastrarProduto(Produto produtos[], int* numProdutos) {
    if (*numProdutos >= MAX_PRODUTOS) {
        printf("Limite de produtos alcançado!\n");
        return;
    }

    printf("Cadastro de Produto:\n");
    printf("Nome: ");
    scanf("%s", produtos[*numProdutos].nome);

    do {
        printf("Quantidade: ");
        scanf("%d", &produtos[*numProdutos].quantidade);
        if (produtos[*numProdutos].quantidade < 0) {
            printf("Quantidade não pode ser negativa. Tente novamente.\n");
        }
    } while (produtos[*numProdutos].quantidade < 0);

    do {
        printf("Preço: R$ ");
        scanf("%f", &produtos[*numProdutos].preco);
        if (produtos[*numProdutos].preco < 0) {
            printf("Preço não pode ser negativo. Tente novamente.\n");
        }
    } while (produtos[*numProdutos].preco < 0);

    do {
        printf("Custo: R$ ");
        scanf("%f", &produtos[*numProdutos].custo);
        if (produtos[*numProdutos].custo < 0) {
            printf("Custo não pode ser negativo. Tente novamente.\n");
        }
    } while (produtos[*numProdutos].custo < 0);

    (*numProdutos)++;
    printf("Produto cadastrado com sucesso!\n");
}

void visualizarEstoque(Produto produtos[], int numProdutos) {
    if (numProdutos == 0) {
        printf("Não há produtos cadastrados.\n");
        return;
    }

    printf("Estoque Atual:\n");
    for (int i = 0; i < numProdutos; i++) {
        printf("Produto %d\n", i + 1);
        printf("Nome: %s\n", produtos[i].nome);
        printf("Quantidade: %d\n", produtos[i].quantidade);
        printf("Preço: %.2f\n", produtos[i].preco);
        printf("Custo: %.2f\n\n", produtos[i].custo);
    }
}

void contabilizarEstoque(Produto produtos[], int numProdutos) {
    float totalEstoque = 0;
    for (int i = 0; i < numProdutos; i++) {
        totalEstoque += produtos[i].quantidade * produtos[i].preco;
    }
    printf("O valor total em estoque é: R$ %.2f\n", totalEstoque);
}

void calcularTotal(Produto produtos[], int n, float* total) {
    *total = 0;
    for (int i = 0; i < n; i++) {
        *total += produtos[i].preco;
    }
}

void calcularTroco(float total, float pagamento, float* troco) {
    *troco = pagamento - total;
}

void calcularLucro(Produto produtos[], int numProdutos) {
    float lucroTotal = 0.0;
    for (int i = 0; i < numProdutos; i++) {
        lucroTotal += produtos[i].quantidade * (produtos[i].preco - produtos[i].custo);
    }
    printf("O lucro total é: R$ %.2f\n", lucroTotal);
}

void removerProduto(Produto produtos[], int* numProdutos) {
    if (*numProdutos == 0) {
        printf("Nenhum produto para remover.\n");
        return;
    }

    int index;
    printf("Informe o índice do produto a ser removido (1 a %d): ", *numProdutos);
    scanf("%d", &index);

    if (index < 1 || index > *numProdutos) {
        printf("Índice inválido.\n");
        return;
    }

    index--;  // Ajusta para índice baseado em 0

    for (int i = index; i < *numProdutos - 1; i++) {
        produtos[i] = produtos[i + 1];
    }

    (*numProdutos)--;
    printf("Produto removido com sucesso!\n");
}

// Função para salvar os produtos em um arquivo
void salvarEstoque(Produto produtos[], int numProdutos) {
    FILE* arquivo = fopen(ARQUIVO_ESTOQUE, "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar o estoque.\n");
        return;
    }

    for (int i = 0; i < numProdutos; i++) {
        fprintf(arquivo, "%s %d %.2f %.2f\n", produtos[i].nome, produtos[i].quantidade, produtos[i].preco, produtos[i].custo);
    }

    fclose(arquivo);

}

// Função para carregar os produtos de um arquivo
void carregarEstoque(Produto produtos[], int* numProdutos) {
    FILE* arquivo = fopen(ARQUIVO_ESTOQUE, "r");

    if (arquivo == NULL) {
        printf("Nenhum arquivo de estoque encontrado. Iniciando com estoque vazio.\n");
        return;
    }

    *numProdutos = 0;
    while (fscanf(arquivo, "%s %d %f %f", produtos[*numProdutos].nome, &produtos[*numProdutos].quantidade, &produtos[*numProdutos].preco, &produtos[*numProdutos].custo) != EOF) {
        (*numProdutos)++;
    }

    fclose(arquivo);

}
