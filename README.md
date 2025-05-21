# 💡 Smart Home – Iluminação Inteligente com Sensores

Projeto desenvolvido utilizando a placa **BitDogLab com Raspberry Pi Pico W**.  
O objetivo é criar um sistema de **automação residencial**, onde é possível controlar as luzes de três ambientes (**Quarto, Sala e Cozinha**) via **servidor web**, além de utilizar sensores de **presença (TCRT5000)** e **luminosidade (LDR)** para ativar automaticamente as luzes.

---

## 📸 Demonstração

📹 Vídeo: [Link](https://www.youtube.com/watch?v=Fof7i5wVtUk&ab_channel=LorenzoBaroni)

---

## 🚀 Funcionalidades

- 🌐 Acesso via servidor web local hospedado na própria Pico W.
- 💡 Controle manual dos LEDs, agrupados por ambiente:
  - **Quarto:** LED vermelho + LED verde = Amarelo
  - **Sala:** LED verde + LED azul = Ciano
  - **Cozinha:** LED azul + LED vermelho = Roxo
- 🔁 Um botão ativa ou desativa os sensores (**TCRT5000 + LDR**) simultaneamente.
- 🤖 **Modo automático:** Se os sensores estão ativos, e o ambiente estiver **escuro**, quando o sensor de **presença detectar movimento**, os **três LEDs** se acendem automaticamente.
- 🔒 Ao acionar qualquer botão manualmente, o modo automático é desativado temporariamente.
- 🔄 Reativando os sensores, o modo automático volta a funcionar.

---

## 🧩 Hardware Utilizado

- ✅ **Raspberry Pi Pico W** com placa **BitDogLab**
- ✅ **Sensor TCRT5000** (sensor de presença)
- ✅ **Sensor LDR** (sensor de luminosidade) + resistor de 10kΩ
- ✅ **LED RGB** da BitDogLab (GPIOs 11, 12 e 13)
- ✅ Protoboard
- ✅ Jumpers

---

## 🔗 Ligações na Protoboard

### 🔸 **Sensor de Presença (TCRT5000)**

| Pino do TCRT5000 | Conecta em                 | Descrição                                |
|------------------|-----------------------------|-------------------------------------------|
| **Ânodo (A)**    | 3.3V (via resistor 220Ω)    | Alimentação do LED infravermelho         |
| **Cátodo (K)**   | GND                         | GND do LED infravermelho                 |
| **Coletor (C)**  | GPIO16                      | Saída digital (nível baixo = presença)   |
| **Emissor (E)**  | GND                         | Terra do fototransistor                  |

✅ **Observação:** O resistor de pull-up no coletor é feito via software utilizando `gpio_pull_up(GPIO16);` no código.

---

### 🔸 **Sensor de Luminosidade (LDR)**

**Montagem com divisor de tensão:**

```
 3.3V ---- [LDR] ----+---- GPIO28 (ADC2)
                     |
                 [Resistor 10kΩ]
                     |
                    GND
```

| Componente       | Conecta em                 | Descrição                                   |
|------------------|-----------------------------|----------------------------------------------|
| Um terminal LDR  | 3.3V                        | Alimentação                                 |
| Outro terminal   | Nó comum com resistor e ADC | Vai para ADC (GPIO28 - ADC2)                |
| Resistor 10kΩ    | Nó comum → GND              | Completa o divisor de tensão                |

✅ A tensão lida no ADC varia de acordo com a luz:  
- **Ambiente claro:** tensão alta → valor ADC alto  
- **Ambiente escuro:** tensão baixa → valor ADC baixo (aciona os LEDs)

---

## 📑 Estrutura do Projeto

```
CasaBit/
├── main.c                → Código principal (setup e loop)
├── leds.c / leds.h       → Controle dos LEDs
├── sensores.c / sensores.h → Leitura e lógica dos sensores
├── webserver.c / webserver.h → Servidor web embarcado
├── CMakeLists.txt        → Configuração do build
└── README.md             → Documentação do projeto
```

---

## 🌐 Interface Web

- Quatro botões na interface:
  - **Quarto**, **Sala**, **Cozinha** → Controle dos LEDs em pares
  - **Sensores** → Ativa/desativa o modo automático
- Os botões mudam de cor:
  - 🔵 **Azul (btn-on):** ligado
  - ⚪ **Cinza (btn-off):** desligado
- Exibe "Ligar" ou "Desligar" conforme o estado.

---

## 🛠️ Como Compilar

1. Tenha o **Pico SDK** instalado e configurado.
2. No terminal:

```bash
mkdir build
cd build
cmake ..
make
```

3. Grave o arquivo `.uf2` gerado na Raspberry Pi Pico W.

---

## 📱 Como Acessar o Webserver

- Após ligar a placa, ela se conecta ao Wi-Fi configurado.
- Verifique no terminal o **IP atribuído**, que aparecerá como:

```bash
Conectado com sucesso!
IP do dispositivo: 192.168.x.x
```

- No navegador, acesse:

```
http://192.168.x.x
```

---

## 👨‍💻 Autor

**Lorenzo Kelvin Delano**  
Residência EmbarcaTech – Tarefa 04 – Maio de 2025

---

## 📝 Licença
Este programa foi desenvolvido como um exemplo educacional e pode ser usado livremente para fins de estudo e aprendizado.

## 📌 Autor
LORENZO GIUSEPPE OLIVEIRA BARONI
