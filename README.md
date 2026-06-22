Sistema de Dosagem Automática de Água com Raspberry Pi Pico

1.Integrantes:

João Lourenço Medeiros Martins (RA: 22.00369-0)
Nicolas Lemos Nunes Tamashiro (RA: 22.00728-8)
Theo Franchin Peres (RA: 22.01094-7)
Vinicius Filgueiras dos Santos (RA: 23.95007-2)


2.Descrição do projeto:

Este projeto consiste no desenvolvimento de um sistema automatizado para monitoramento de nível e dosagem controlada de água utilizando um Raspberry Pi Pico como unidade de processamento. O sistema realiza a transferência de volumes pré-definidos entre dois reservatórios através de uma bomba hidráulica controlada eletronicamente. O volume no reservatório principal é monitorado continuamente via sensor resistivo (potenciômetro linear e mecanismo de flutuação), com exibição em tempo real em um display LCD 16x2 I2C.

3.Objetivos:

-Realizar dosagem precisa retirando líquido de um reservatório principal.
-Converter a posição do sensor em volume real (mL).
-Permitir a seleção de dosagens pré-definidas.
-Controlar automaticamente a bomba hidráulica.
-Interromper a dosagem ao atingir o volume desejado.
-Exibir informações do processo em um display LCD.
-Implementar estratégia de controle em malha fechada via realimentação.

4.Arquitetura do Sistema:

4.1 Composição:
-Reservatório 1: Principal (medição e retirada).
-Reservatório 2: Receptor.
-Sensor de Nível: Potenciômetro linear acoplado a flutuador.
-Raspberry Pi Pico: Responsável por aquisição de sinais, conversão ADC, processamento, controle da bomba e interface LCD.
-Módulo MOSFET: Acionamento da bomba.
-Display LCD 16x2 I2C: Interface visual.

4.2 Funcionamento Geral:
-Leitura do sensor de nível.
-Conversão ADC (12 bits) para valor digital.
-Conversão do valor ADC para volume (mL) via tabela de calibração.
-Seleção de dosagem pelo usuário.
-Cálculo de volume a ser retirado e acionamento da bomba.
-Leitura, verificação do volume transferido e correção, repetindo o ciclo até atingir a meta.

5.Hardware:

Componente             Conexão

Potenciômetro          Terminal 1: 3V3 / Cursor: GP26 (ADC0) / Terminal 2: GND
Display LCD	           VCC: 5V / GND: GND / SDA: GP8 / SCL: GP9
MOSFET	Gate           GP15
Terminal 1	           3V3
Cursor	               GP26 (ADC0)
Terminal 2	           GND
A bomba  é alimentada externamente 

Display:

LCD 16x2
Interface I2C
Endereço: 0x27

5.1 Características:

Alimentação: 3,3 V
Saída analógica
Ligado diretamente ao ADC do Pico

6.Estratégia de Medição e Calibração:

-Conversão ADC: Escala de 0 a 4095.
-Filtragem: Média de 500 amostras consecutivas seguida por um filtro de 20 leituras úteis.
-Calibração: Utilização de tabela experimental com interpolação linear para valores intermediários.
-Pontos de calibração: 
ADC	Volume (mL)
3057	0
3054	200
3041	400
529	3000

7.Estratégia de Controle

-Malha Fechada: Realimentação baseada no volume calculado.
-Controle por Pulsos:
-Região Longa: Pulsos de 1200 ms (alta velocidade).
-Região Intermediária: Pulsos de 700 ms (redução de velocidade).
-Região Final: Pulsos de 300 ms (ajuste fino para precisão).
-Correção de Volume:
-Fator de 1.22 para dosagens de 250 mL.
-Fator de 1.44 para dosagens de 300 mL e 350 mL.

8.Interface LCD e Resultados:

Status de Operação: Tela inicial, Dosagem em execução e Processo finalizado.
Resultados Observados: O sistema apresentou repetibilidade satisfatória, com resultados médios de ≈ 250 mL, ≈ 300 mL e ≈ 350 mL para as respectivas doses solicitadas.
