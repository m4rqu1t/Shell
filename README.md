# Shell BRABO

Um interpretador de comandos (Shell) desenvolvido do absoluto zero em linguagem C. Este projeto foi construído para a disciplina de Sistemas Operacionais e implementa as principais mecânicas de gerenciamento de processos, comunicação IPC e manipulação de arquivos presentes nos sistemas UNIX.

**Sistema Operacional de Implementação:** Desenvolvido e testado em ambiente **Arch Linux**.

---

## Funcionalidades Implementadas

O Shell BRABO atende a todos os requisitos do projeto, operando tanto de forma interativa quanto por arquivo (Batchfile).

* **Execução Básica:** Suporte a comandos nativos do sistema operacional via `execvp`.
* **Comandos Embutidos (Built-ins):**
  * `cd`: Navegação entre diretórios.
  * `fg`: Traz um processo executando em background de volta para o primeiro plano (Foreground).
  * `style <parallel|sequential>`: Alterna o motor de execução do shell (detalhes abaixo).
  * `exit`: Encerra o interpretador com segurança.
* **Redirecionamento de I/O:** Suporte nativo para redirecionar saídas e entradas de comandos para arquivos usando `>`, `>>` (Append) e `<`.
* **Pipes:** Conecta a saída de um processo diretamente na entrada de outro utilizando `|` na memória RAM.
* **Processos em Background:** Adicionando `&` ao final de um comando, o processo é despachado em segundo plano, liberando o terminal instantaneamente.
* **Leitura de Batchfiles:** Capacidade de ler e executar uma sequência de comandos diretamente de um arquivo `.txt` de forma silenciosa.

---

## Motores de Execução (Style)

O grande diferencial deste shell é o controle arquitetural sobre a forma como múltiplos comandos separados por `;` são despachados ao processador.

* **Modo Sequencial (`style sequential` - Padrão):** O shell realiza o `fork()`, executa o processo filho e aplica um `waitpid()` imediato. Um comando só inicia quando o anterior termina.
* **Modo Paralelo (`style parallel`):** O shell realiza múltiplos `fork()`s simultâneos, guardando os PIDs em uma lista interna. Todos os comandos são enviados ao processador ao mesmo tempo. Ao final, o shell aguarda a conclusão do lote inteiro, reduzindo drasticamente o tempo total de execução.

---

## Arquitetura do Projeto

O código foi modulado em três pilares principais para garantir legibilidade e escalabilidade:

1. **main.c (O Gerente):** Responsável por atender o usuário (via teclado ou arquivo), ler a string bruta e fazer o fatiamento inicial dos comandos por ponto-e-vírgula (`;`). Também controla a variável global de estilo (Sequencial vs Paralelo).
2. **parser.c (O Caixa):** Recebe o comando fatiado, remove espaços em branco inúteis usando aritmética de ponteiros e gera um array limpo de argumentos (`args[]`). Identifica a presença da flag de background (`&`).
3. **executor.c (O Despachante):** Inspeciona o comando limpo em busca de pipes e redirecionamentos (`>`,`<`). Executa comandos embutidos ou clona o processo atual (`fork`) para chamar os programas do sistema via família `exec`.

---

## Como Compilar e Rodar

**1. Compilando o projeto:**

Na pasta raiz do projeto, execute:
```bash
make clean
make
```
**2.1 No modo interativo:**

./shell_brabo

**2.2 No modo Batch**

./shell_brabo arquivo_de_comandos

## Como testar

**Pipes e Filtros:**

Shell BRABO seq > ls -l | sort -k 5n

**Redirecionamento de Saída e Entrada:**

Shell BRABO seq > ls -l > arquivos.txt
Shell BRABO seq > wc -l < arquivos.txt

**Execução Paralela:**

Shell BRABO seq > style parallel
Shel BRABO par > sleep 3 ; echo "Lado A" ; sleep 3 ; echo "Lado B"


## 👨‍💻 Autoria

Desenvolvido por Marco Aurélio (@m4rqu1t)
Estudante de Ciência da Computação (3º Período) - CESAR School.
