/******************************************************************************
Sistema de Registro e Gestão de Contas Bancárias – ETAPA 3
Yan Moura Lima - 28195
*******************************************************************************/

#include <iostream>   // Entrada e saída no terminal (cin, cout, endl)
#include <string>     // Textos (string)
#include <fstream>    // Arquivos: ifstream (leitura) e ofstream (gravação)
#include <iomanip>    // fixed e setprecision, para valores com 2 casas decimais
#include <cstdlib>    // atoi e atof, para converter texto em número
using namespace std;  // Evita escrever "std::" antes de cout, cin, string etc.

// 2.1 DADOS DAS CONTAS (globais, para as funções de arquivo acessarem os vetores)
const int MAX_CONTAS = 100;      // é previsto um limite máximo de 100 contas
int numeroConta[MAX_CONTAS];     // Número de cada conta
string nomeCliente[MAX_CONTAS];  // Nome de cada titular
string cpf[MAX_CONTAS];          // CPF de cada titular
int tipoConta[MAX_CONTAS];       // Tipo da conta: 1 = Corrente; 2 = Poupança
double saldo[MAX_CONTAS];        // Saldo de cada conta
bool contaAtiva[MAX_CONTAS];     // Situação da conta: true = ativa, false = inativa
int quantidadeContas = 0;        // Quantidade de contas cadastradas

// 2.2 HISTÓRICO DE MOVIMENTAÇÕES (D = Depósito/entrada; S = Saque/saída)
const int MAX_MOVIMENTACOES = 500;            // Limite de movimentações
int contaMovimentacao[MAX_MOVIMENTACOES];     // Conta dona de cada movimentação
char tipoMovimentacao[MAX_MOVIMENTACOES];     // Tipo: 'D' ou 'S'
double valorMovimentacao[MAX_MOVIMENTACOES];  // Valor de cada movimentação
int quantidadeMovimentacoes = 0;              // Quantidade de movimentações registradas

// FUNÇÃO AUXILIAR: separa uma linha em campos, usando ";" como separador
int dividirLinha(const string& linha, string campos[], int maximo) {  // Recebe a linha, o vetor de campos e o máximo de campos
	int quantidade = 0;                                   // Quantos campos foram encontrados
	size_t inicio = 0;                                    // Posição onde o campo atual começa
	for (size_t i = 0; i <= linha.length(); i++) {        // Percorre a linha (inclusive o fim)
		if (i == linha.length() || linha[i] == ';') {     // Chegou ao fim da linha ou a um ponto e vírgula
			if (quantidade < maximo) {                    // Se ainda há espaço no vetor de campos
				campos[quantidade] = linha.substr(inicio, i - inicio); // Guarda o texto do campo
			}                                             // Fim do if
			quantidade++;                                 // Conta mais um campo
			inicio = i + 1;                               // O próximo campo começa depois do ";"
		}                                                 // Fim do if
	}                                                     // Fim do for
	return quantidade;                                    // Devolve quantos campos a linha tinha
}                                                         // Fim da função dividirLinha

// 2.2 CARREGAR AS CONTAS: lê contas.txt e preenche os vetores
void carregarContas() {                                   // Função sem retorno nem parâmetros
	quantidadeContas = 0;                                 // Começa sem nenhuma conta
	ifstream arquivo("contas.txt");                       // Abre o arquivo para leitura
	if (!arquivo.is_open()) {                             // Se o arquivo não existe (primeira execução)
		return;                                           // Sai da função: o sistema começa sem contas
	}                                                     // Fim do if
	string linha;                                         // Guarda cada linha lida
	string campos[6];                                     // Guarda os 6 campos de uma conta
	while (getline(arquivo, linha) && quantidadeContas < MAX_CONTAS) { // Lê linha por linha, respeitando o limite
		if (!linha.empty() && linha[linha.length() - 1] == '\r') {     // Se a linha termina com "\r" (arquivo do Windows)
			linha.erase(linha.length() - 1);              // Remove o "\r"
		}                                                 // Fim do if
		if (dividirLinha(linha, campos, 6) == 6) {        // Só usa linhas com exatamente 6 campos
			numeroConta[quantidadeContas] = atoi(campos[0].c_str());  // Campo 1: número da conta
			nomeCliente[quantidadeContas] = campos[1];                // Campo 2: nome do cliente
			cpf[quantidadeContas] = campos[2];                        // Campo 3: CPF
			tipoConta[quantidadeContas] = atoi(campos[3].c_str());    // Campo 4: tipo da conta
			saldo[quantidadeContas] = atof(campos[4].c_str());        // Campo 5: saldo
			contaAtiva[quantidadeContas] = (campos[5] == "1");        // Campo 6: 1 = ativa
			quantidadeContas++;                           // Atualiza a quantidade de contas
		}                                                 // Fim do if
	}                                                     // Fim do while
	arquivo.close();                                      // Fecha o arquivo
}                                                         // Fim da função carregarContas

// 2.2 CARREGAR AS MOVIMENTAÇÕES: lê movimentacoes.txt e preenche os vetores
void carregarMovimentacoes() {                            // Função sem retorno nem parâmetros
	quantidadeMovimentacoes = 0;                          // Começa sem nenhuma movimentação
	ifstream arquivo("movimentacoes.txt");                // Abre o arquivo para leitura
	if (!arquivo.is_open()) {                             // Se o arquivo não existe (primeira execução)
		return;                                           // Sai da função: o sistema começa sem movimentações
	}                                                     // Fim do if
	string linha;                                         // Guarda cada linha lida
	string campos[3];                                     // Guarda os 3 campos de uma movimentação
	while (getline(arquivo, linha) && quantidadeMovimentacoes < MAX_MOVIMENTACOES) { // Lê linha por linha, respeitando o limite
		if (!linha.empty() && linha[linha.length() - 1] == '\r') {     // Se a linha termina com "\r" (arquivo do Windows)
			linha.erase(linha.length() - 1);              // Remove o "\r"
		}                                                 // Fim do if
		if (dividirLinha(linha, campos, 3) == 3 && !campos[1].empty()) { // Só usa linhas com 3 campos e tipo preenchido
			contaMovimentacao[quantidadeMovimentacoes] = atoi(campos[0].c_str()); // Campo 1: número da conta
			tipoMovimentacao[quantidadeMovimentacoes] = campos[1][0];             // Campo 2: tipo (D ou S)
			valorMovimentacao[quantidadeMovimentacoes] = atof(campos[2].c_str()); // Campo 3: valor
			quantidadeMovimentacoes++;                    // Atualiza a quantidade de movimentações
		}                                                 // Fim do if
	}                                                     // Fim do while
	arquivo.close();                                      // Fecha o arquivo
}                                                         // Fim da função carregarMovimentacoes

// 2.3 SALVAR AS CONTAS: grava todos os vetores de contas em contas.txt
void salvarContas() {                                     // Função sem retorno nem parâmetros
	ofstream arquivo("contas.txt");                       // Abre para gravação (substitui o conteúdo anterior)
	if (arquivo.is_open()) {                              // Se o arquivo foi aberto corretamente
		arquivo << fixed << setprecision(2);              // Saldos com 2 casas decimais (ex.: 700.00)
		for (int i = 0; i < quantidadeContas; i++) {      // Percorre todas as contas
			arquivo << numeroConta[i] << ";"              // Grava o número da conta
			        << nomeCliente[i] << ";"              // Grava o nome
			        << cpf[i] << ";"                      // Grava o CPF
			        << tipoConta[i] << ";"                // Grava o tipo da conta
			        << saldo[i] << ";"                    // Grava o saldo
			        << contaAtiva[i] << endl;             // Grava a situação (1 ou 0) e muda de linha
		}                                                 // Fim do for
		arquivo.close();                                  // Fecha o arquivo
	} else {                                              // Se não conseguiu abrir o arquivo
		cout << "Erro ao salvar contas.txt!" << endl;     // Avisa o usuário
	}                                                     // Fim do if/else
}                                                         // Fim da função salvarContas

// 2.4 SALVAR AS MOVIMENTAÇÕES: grava todo o histórico em movimentacoes.txt
void salvarMovimentacoes() {                              // Função sem retorno nem parâmetros
	ofstream arquivo("movimentacoes.txt");                // Abre para gravação (substitui o conteúdo anterior)
	if (arquivo.is_open()) {                              // Se o arquivo foi aberto corretamente
		arquivo << fixed << setprecision(2);              // Valores com 2 casas decimais (ex.: 300.00)
		for (int i = 0; i < quantidadeMovimentacoes; i++) { // Percorre todo o histórico
			arquivo << contaMovimentacao[i] << ";"        // Grava o número da conta
			        << tipoMovimentacao[i] << ";"         // Grava o tipo (D ou S)
			        << valorMovimentacao[i] << endl;      // Grava o valor e muda de linha
		}                                                 // Fim do for
		arquivo.close();                                  // Fecha o arquivo
	} else {                                              // Se não conseguiu abrir o arquivo
		cout << "Erro ao salvar movimentacoes.txt!" << endl; // Avisa o usuário
	}                                                     // Fim do if/else
}                                                         // Fim da função salvarMovimentacoes

int main() {  // Função principal: onde o programa começa

	// VARIÁVEIS DE APOIO
	int opcao = 0;           // Opção escolhida no menu
	int indice = -1;         // Posição da conta encontrada (-1 = não encontrada)
	int indiceOrigem = -1;   // Posição da conta de origem na transferência
	int indiceDestino = -1;  // Posição da conta de destino na transferência
	int numeroDigitado = 0;  // Número de conta informado pelo usuário (ou da origem)
	int numeroDestino = 0;   // Número da conta de destino informado
	double valor = 0.0;      // Valor de depósito, saque ou transferência
	bool nomeValido = false; // Indica se o nome digitado pode ser usado

	cout << fixed << setprecision(2);  // Valores decimais sempre com 2 casas (ex.: 50.00)

	carregarContas();           // Ao iniciar, recupera as contas gravadas no arquivo
	carregarMovimentacoes();    // Ao iniciar, recupera o histórico gravado no arquivo
	cout << quantidadeContas << " conta(s) e " << quantidadeMovimentacoes << " movimentação(ões) carregadas." << endl; // Informa o que foi recuperado

	do { // Laço do menu: repete até o usuário escolher sair
		cout << "\n===== BANCO PREMIUM =====\n";    // Título do menu
		cout << "1 - Cadastrar conta" << endl;       // Opção 1
		cout << "2 - Listar contas" << endl;         // Opção 2
		cout << "3 - Consultar conta" << endl;       // Opção 3
		cout << "4 - Depósito" << endl;              // Opção 4
		cout << "5 - Saque" << endl;                 // Opção 5
		cout << "6 - Transferência" << endl;         // Opção 6
		cout << "7 - Extrato" << endl;               // Opção 7
		cout << "0 - Sair" << endl;                  // Opção 0
		cout << "Opção: ";                           // Pede a escolha do usuário
		cin >> opcao;                                // Lê a opção
		cin.ignore(1000, '\n');                      // Descarta o Enter, para o getline do cadastro funcionar

		switch (opcao) { // Escolhe o que fazer conforme a opção

		case 1: // Cadastrar conta
			if (quantidadeContas >= MAX_CONTAS) {          // Se o limite de contas foi atingido
				cout << "Limite de contas atingido." << endl; // Avisa o usuário
				break;                                     // Volta ao menu
			}                                              // Fim do if

			do { // Repete até o nome ser válido
				cout << "Nome do cliente: ";                       // Pede o nome
				getline(cin, nomeCliente[quantidadeContas]);       // Lê o nome completo (com espaços)
				nomeValido = !nomeCliente[quantidadeContas].empty() && nomeCliente[quantidadeContas].find(';') == string::npos; // Não pode ser vazio nem ter ";"
				if (!nomeValido) cout << "Nome inválido (não pode ser vazio nem conter \";\")." << endl; // Avisa se for inválido
			} while (!nomeValido);                                 // Repete enquanto o nome não for válido

			do { // Repete até o CPF ter 11 caracteres
				cout << "CPF (11 dígitos): ";              // Pede o CPF
				cin >> cpf[quantidadeContas];              // Lê o CPF
			} while (cpf[quantidadeContas].length() != 11); // Repete se não tiver 11 caracteres

			do { // Repete até o saldo inicial não ser negativo
				cout << "Saldo inicial: ";                 // Pede o saldo inicial
				cin >> saldo[quantidadeContas];            // Lê o saldo
			} while (saldo[quantidadeContas] < 0);         // Repete se for negativo

			do { // Repete até o tipo ser 1 ou 2
				cout << "Tipo de conta (1 = Corrente; 2 = Poupança): "; // Pede o tipo
				cin >> tipoConta[quantidadeContas];        // Lê o tipo
			} while (tipoConta[quantidadeContas] != 1 && tipoConta[quantidadeContas] != 2); // Repete se não for 1 nem 2

			if (quantidadeContas == 0) {                   // Se é a primeira conta do sistema
				numeroConta[quantidadeContas] = 1001;      // O número começa em 1001
			} else {                                       // Se já existem contas
				numeroConta[quantidadeContas] = numeroConta[quantidadeContas - 1] + 1; // O número é o da última conta + 1
			}                                              // Fim do if/else
			contaAtiva[quantidadeContas] = true;           // A conta nasce ativa
			cout << "Conta registrada! Número: " << numeroConta[quantidadeContas] << endl; // Informa o número da conta
			quantidadeContas++;                            // Conta mais uma conta cadastrada
			salvarContas();                                // Grava as contas no arquivo
			break;                                         // Volta ao menu

		case 2: // Listar contas
			if (quantidadeContas == 0) cout << "Nenhuma conta cadastrada." << endl; // Avisa se não há contas
			for (int i = 0; i < quantidadeContas; i++) {   // Percorre todas as contas cadastradas
				cout << "\nConta: " << numeroConta[i] << endl;  // Mostra o número
				cout << "Titular: " << nomeCliente[i] << endl;  // Mostra o nome
				cout << "CPF: " << cpf[i] << endl;              // Mostra o CPF
				cout << "Tipo: " << (tipoConta[i] == 1 ? "Corrente" : "Poupança") << endl; // Mostra o tipo
				cout << "Saldo: R$ " << saldo[i] << endl;       // Mostra o saldo
			}                                              // Fim do for
			break;                                         // Volta ao menu

		case 3: // Consultar conta pelo número
			cout << "Número da conta: ";                   // Pede o número da conta
			cin >> numeroDigitado;                         // Lê o número

			indice = -1;                                   // Começa como "não encontrada"
			for (int i = 0; i < quantidadeContas; i++) {   // Percorre as contas
				if (numeroConta[i] == numeroDigitado) indice = i; // Se o número bater, guarda a posição
			}                                              // Fim do for

			if (indice == -1) {                            // Se não encontrou
				cout << "Conta inexistente." << endl;      // Avisa o usuário
			} else {                                       // Se encontrou
				cout << "Titular: " << nomeCliente[indice] << endl;  // Mostra o nome
				cout << "CPF: " << cpf[indice] << endl;              // Mostra o CPF
				cout << "Tipo: " << (tipoConta[indice] == 1 ? "Corrente" : "Poupança") << endl; // Mostra o tipo
				cout << "Saldo: R$ " << saldo[indice] << endl;       // Mostra o saldo
			}                                              // Fim do if/else
			break;                                         // Volta ao menu

		case 4: // Depósito
			cout << "Número da conta: ";                   // Pede o número da conta
			cin >> numeroDigitado;                         // Lê o número
			cout << "Valor do depósito: R$ ";              // Pede o valor
			cin >> valor;                                  // Lê o valor

			indice = -1;                                   // Começa como "não encontrada"
			for (int i = 0; i < quantidadeContas; i++) {   // Percorre as contas
				if (numeroConta[i] == numeroDigitado) indice = i; // Se o número bater, guarda a posição
			}                                              // Fim do for

			if (indice == -1) {                            // Se a conta não existe
				cout << "Conta inexistente." << endl;      // Avisa o usuário
			} else if (!contaAtiva[indice]) {              // Se a conta está inativa
				cout << "Conta inativa." << endl;          // Avisa o usuário
			} else if (valor <= 0) {                       // Se o valor não é positivo
				cout << "Valor inválido." << endl;         // Avisa o usuário
			} else if (quantidadeMovimentacoes >= MAX_MOVIMENTACOES) { // Se o histórico está cheio
				cout << "Histórico cheio." << endl;        // Avisa o usuário
			} else {                                       // Tudo certo: faz o depósito
				saldo[indice] = saldo[indice] + valor;     // Soma o valor ao saldo
				contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indice]; // Registra a conta da movimentação
				tipoMovimentacao[quantidadeMovimentacoes] = 'D';                  // Registra o tipo: depósito
				valorMovimentacao[quantidadeMovimentacoes] = valor;               // Registra o valor
				quantidadeMovimentacoes++;                 // Conta mais uma movimentação
				salvarContas();                            // Grava o novo saldo no arquivo de contas
				salvarMovimentacoes();                     // Grava o histórico atualizado no arquivo de movimentações
				cout << "Depósito realizado. Saldo: R$ " << saldo[indice] << endl; // Confirma e mostra o saldo
			}                                              // Fim do if/else
			break;                                         // Volta ao menu

		case 5: // Saque
			cout << "Número da conta: ";                   // Pede o número da conta
			cin >> numeroDigitado;                         // Lê o número
			cout << "Valor do saque: R$ ";                 // Pede o valor
			cin >> valor;                                  // Lê o valor

			indice = -1;                                   // Começa como "não encontrada"
			for (int i = 0; i < quantidadeContas; i++) {   // Percorre as contas
				if (numeroConta[i] == numeroDigitado) indice = i; // Se o número bater, guarda a posição
			}                                              // Fim do for

			if (indice == -1) {                            // Se a conta não existe
				cout << "Conta inexistente." << endl;      // Avisa o usuário
			} else if (!contaAtiva[indice]) {              // Se a conta está inativa
				cout << "Conta inativa." << endl;          // Avisa o usuário
			} else if (valor <= 0) {                       // Se o valor não é positivo
				cout << "Valor inválido." << endl;         // Avisa o usuário
			} else if (valor > saldo[indice]) {            // Se o saldo não é suficiente
				cout << "Saldo insuficiente." << endl;     // Avisa o usuário
			} else if (quantidadeMovimentacoes >= MAX_MOVIMENTACOES) { // Se o histórico está cheio
				cout << "Histórico cheio." << endl;        // Avisa o usuário
			} else {                                       // Tudo certo: faz o saque
				saldo[indice] = saldo[indice] - valor;     // Subtrai o valor do saldo
				contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indice]; // Registra a conta da movimentação
				tipoMovimentacao[quantidadeMovimentacoes] = 'S';                  // Registra o tipo: saque
				valorMovimentacao[quantidadeMovimentacoes] = valor;               // Registra o valor
				quantidadeMovimentacoes++;                 // Conta mais uma movimentação
				salvarContas();                            // Grava o novo saldo no arquivo de contas
				salvarMovimentacoes();                     // Grava o histórico atualizado no arquivo de movimentações
				cout << "Saque realizado. Saldo: R$ " << saldo[indice] << endl;   // Confirma e mostra o saldo
			}                                              // Fim do if/else
			break;                                         // Volta ao menu

		case 6: // Transferência entre duas contas
			cout << "Número da conta de origem: ";         // Pede a conta de origem
			cin >> numeroDigitado;                         // Lê o número da origem
			cout << "Número da conta de destino: ";        // Pede a conta de destino
			cin >> numeroDestino;                          // Lê o número do destino
			cout << "Valor da transferência: R$ ";         // Pede o valor
			cin >> valor;                                  // Lê o valor

			indiceOrigem = -1;                             // Começa como "origem não encontrada"
			indiceDestino = -1;                            // Começa como "destino não encontrado"
			for (int i = 0; i < quantidadeContas; i++) {   // Percorre as contas
				if (numeroConta[i] == numeroDigitado) indiceOrigem = i;  // Se bater com a origem, guarda a posição
				if (numeroConta[i] == numeroDestino) indiceDestino = i;  // Se bater com o destino, guarda a posição
			}                                              // Fim do for

			if (indiceOrigem == -1 || indiceDestino == -1) {  // Se alguma das contas não existe
				cout << "Conta de origem ou de destino inexistente." << endl; // Avisa o usuário
			} else if (indiceOrigem == indiceDestino) {    // Se as duas contas são a mesma
				cout << "Origem e destino devem ser diferentes." << endl;     // Avisa o usuário
			} else if (!contaAtiva[indiceOrigem] || !contaAtiva[indiceDestino]) { // Se alguma conta está inativa
				cout << "Uma das contas está inativa." << endl;               // Avisa o usuário
			} else if (valor <= 0) {                       // Se o valor não é positivo
				cout << "Valor inválido." << endl;         // Avisa o usuário
			} else if (valor > saldo[indiceOrigem]) {      // Se a origem não tem saldo suficiente
				cout << "Saldo insuficiente." << endl;     // Avisa o usuário
			} else if (quantidadeMovimentacoes + 2 > MAX_MOVIMENTACOES) { // Se não há espaço para os 2 registros
				cout << "Histórico cheio." << endl;        // Avisa o usuário
			} else {                                       // Tudo certo: faz a transferência
				saldo[indiceOrigem] = saldo[indiceOrigem] - valor;    // Retira o valor da origem
				saldo[indiceDestino] = saldo[indiceDestino] + valor;  // Soma o valor no destino

				contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indiceOrigem]; // 1º registro: conta de origem
				tipoMovimentacao[quantidadeMovimentacoes] = 'S';                        // Tipo S: saída
				valorMovimentacao[quantidadeMovimentacoes] = valor;                     // Valor da movimentação
				quantidadeMovimentacoes++;                 // Conta mais uma movimentação

				contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indiceDestino]; // 2º registro: conta de destino
				tipoMovimentacao[quantidadeMovimentacoes] = 'D';                         // Tipo D: entrada
				valorMovimentacao[quantidadeMovimentacoes] = valor;                      // Mesmo valor da movimentação
				quantidadeMovimentacoes++;                 // Conta mais uma movimentação (total +2)

				salvarContas();                            // Grava os dois novos saldos no arquivo de contas
				salvarMovimentacoes();                     // Grava os dois registros no arquivo de movimentações
				cout << "Transferência realizada." << endl;   // Confirma a transferência
			}                                              // Fim do if/else
			break;                                         // Volta ao menu

		case 7: // Extrato de uma conta
			cout << "Número da conta: ";                   // Pede o número da conta
			cin >> numeroDigitado;                         // Lê o número

			indice = -1;                                   // Começa como "não encontrada"
			for (int i = 0; i < quantidadeContas; i++) {   // Percorre as contas
				if (numeroConta[i] == numeroDigitado) indice = i; // Se o número bater, guarda a posição
			}                                              // Fim do for

			if (indice == -1) {                            // Se a conta não existe
				cout << "Conta inexistente." << endl;      // Avisa o usuário
			} else {                                       // Se a conta existe
				cout << "\nExtrato da conta " << numeroConta[indice] << " - " << nomeCliente[indice] << endl; // Cabeçalho do extrato
				for (int i = 0; i < quantidadeMovimentacoes; i++) {   // Percorre todo o histórico
					if (contaMovimentacao[i] == numeroConta[indice]) { // Só as movimentações desta conta
						if (tipoMovimentacao[i] == 'D') {              // Se for depósito (ou entrada de transferência)
							cout << "Deposito: R$ " << valorMovimentacao[i] << endl; // Mostra o depósito
						}                                              // Fim do if do depósito
						else if (tipoMovimentacao[i] == 'S') {         // Se for saque (ou saída de transferência)
							cout << "Saque: R$ " << valorMovimentacao[i] << endl;    // Mostra o saque
						}                                              // Fim do if do saque
					}                                                  // Fim do if da conta
				}                                                      // Fim do for do histórico
				cout << "Saldo atual: R$ " << saldo[indice] << endl;   // Mostra o saldo atual
			}                                              // Fim do if/else
			break;                                         // Volta ao menu

		case 0: // Sair
			cout << "Dados salvos. Operação finalizada!" << endl;  // Mensagem de despedida
			break;                                         // Sai do switch (o laço termina em seguida)

		default: // Qualquer outro valor
			cout << "Opção inválida!" << endl;             // Avisa que a opção não existe
		}                                                  // Fim do switch

	} while (opcao != 0); // Repete o menu enquanto a opção não for 0 (Sair)

	return 0; // Fim do programa sem erros
}             // Fim da função main
