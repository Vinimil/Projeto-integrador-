Sistema de Dosagem Automática de Água com Raspberry Pi Pico

 Descrição do Projeto:

Este projeto consiste no desenvolvimento de um sistema automatizado para monitoramento de nível e dosagem controlada de água utilizando um Raspberry Pi Pico como unidade de processamento.
O sistema foi desenvolvido para realizar a transferência de volumes pré-definidos de água entre dois reservatórios através de uma bomba hidráulica controlada eletronicamente. A quantidade de água disponível no reservatório principal é monitorada continuamente por meio de um sensor de nível baseado em um potenciômetro linear acoplado a um mecanismo mecânico de flutuação.

As informações do sistema são exibidas em um display LCD 16x2 com interface I2C, permitindo ao usuário visualizar o volume disponível e acompanhar o processo de dosagem em tempo real.

 Objetivos:

O projeto possui os seguintes objetivos:

Fazer dosagem precisa tirando liquido de um reservatorio principal.
Converter a posição do sensor em volume real (mL).
Permitir a seleção de dosagens pré-definidas.
Controlar automaticamente uma bomba hidráulica.
Interromper a dosagem ao atingir o volume desejado.
Exibir informações do processo em um display LCD.
Desenvolver uma estratégia de controle em malha fechada baseada na realimentação do sensor.

- Arquitetura do Sistema

O sistema é composto por:

Reservatório 1:

Reservatório principal onde ocorre a medição de nível e retirada de liquido.

Reservatório 2:

Reservatório receptor da água dosada.

Sensor de Nível:

Potenciômetro linear acoplado mecanicamente a um flutuador.

Raspberry Pi Pico

Responsável por:

Aquisição dos sinais.
Conversão ADC.
Processamento dos dados.
Controle da bomba.
Interface com display LCD.
Módulo MOSFET

Responsável pelo acionamento da bomba hidráulica.

Display LCD 16x2 I2C

Responsável pela interface visual do sistema.


Funcionamento Geral:

1. O processo inicia com a leitura do sensor de nível.

2. O Raspberry Pi Pico converte a leitura analógica do potenciômetro em um valor digital através do ADC interno de 12 bits.

3.Posteriormente é realizada uma conversão da leitura ADC para volume em mililitros utilizando uma tabela de calibração experimental.

4.Após determinar o volume atual do reservatório, o usuário seleciona uma dosagem.

O sistema então:

Calcula quanto líquido deve ser retirado.
Aciona a bomba.
Aguarda estabilização do líquido.
Realiza nova leitura.
Verifica o volume já transferido.
Repete o processo até atingir o valor desejado.


Hardware Utilizado:


Microcontrolador
Raspberry Pi Pico
RP2040 Dual Core
ADC de 12 bits
Sensor de Nível
Potenciômetro Linear

Características:

Alimentação: 3,3 V
Saída analógica
Ligado diretamente ao ADC do Pico

Conexões:

Potenciômetro:
Terminal 1 - 3V3 
Cursor - GP26 (ADC0)
Terminal 2 - GND


Raspberry Pi Pico
Terminal 1	3V3
Cursor	GP26 (ADC0)
Terminal 2	GND



Display:

LCD 16x2
Interface I2C
Endereço: 0x27

Conexões:

LCD	- Pico
VCC	- 5V
GND	- GND
SDA	- GP8
SCL- 	GP9
Acionamento da Bomba
Gate (Mosfet) - GP15


Conexões:

MOSFET	Pico
Gate	GP15

A bomba é alimentada externamente.

 Estratégia de Medição
Conversão ADC

O ADC interno do RP2040 possui resolução de 12 bits:
ADC=0→4095
A tensão do potenciômetro varia conforme o nível da água.

Filtragem:

Cada leitura é obtida através da média de:
NUM_AMOSTRAS = 500
leituras consecutivas.
Posteriormente é aplicada uma segunda filtragem:
FILTRO_LEITURAS = 20
reduzindo significativamente ruídos.

 Calibração do Sensor

A relação entre ADC e volume não é linear.
Por isso foi realizada uma calibração experimental.
Foram coletados diversos pontos:

ADC	Volume (mL)
3057	0
3054	200
3041	400
529	3000

Esses valores foram armazenados em tabelas no software.

Para valores intermediários utiliza-se uma interpolação linear, permitindo obter uma estimativa contínua do volume.


- Estratégia de Controle

Foi implementado um sistema em malha fechada.

Variável Controlada - Volume dosado 

Variável Manipulada - Tempo de acionamento da bomba


Variável Medida - Volume calculado pelo sensor.


Realimentação:

Após cada pulso da bomba

O sistema mede novamente o nível.
Calcula o volume transferido.
Corrige o erro.
Decide o próximo pulso.

- Controle por Pulsos

A bomba não permanece ligada continuamente, foram definidos três tempos de acionamento:

1200 ms
700 ms
300 ms

Região Longa - Quando falta muito para atingir a meta:
Pulso = 1200 ms
Maior velocidade de dosagem.

Região Intermediária quando o sistema se aproxima do alvo:
Pulso = 700 ms
Redução da velocidade.

Região Final Próximo ao volume desejado:

Pulso = 300 ms
Maior precisão.

- Desenvolvimento da Correção de Volume

Durante os testes observou-se que:
O sensor não media exatamente o volume transferido.
Exemplo:
Volume físico retirado:
300 mL
Volume indicado pelo sensor:
209 mL
Portanto foi necessário desenvolver um fator de correção experimental.
Fator para 250 mL
Após diversos ensaios:
1.22
Fator para 300 e 350 mL
Após calibração:
1.44
O software utiliza automaticamente o fator adequado conforme a dosagem escolhida.

- Interface LCD

Durante a operação o display apresenta:

Tela Inicial
Volume:
2450 mL
Dosagem em Execução
Dosando...
250 mL
Processo Finalizado
Concluido!
250 mL

- Resultados Obtidos

Após o processo de calibração foram realizados diversos ensaios experimentais.

Resultados observados:

Dose Solicitada	Resultado Médio
250 mL	≈ 250 mL
300 mL	≈ 300 mL
350 mL	≈ 350 mL

Os testes demonstraram repetibilidade satisfatória para a aplicação proposta.
