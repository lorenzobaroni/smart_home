# 💡 Smart Home – Iluminação Inteligente com Sensores

Projeto desenvolvido para a Tarefa 04 da residência EmbarcaTech, utilizando a placa **BitDogLab** com a **Raspberry Pi Pico W**.  
O objetivo é criar um sistema de **automação residencial simples e funcional**, com controle de LEDs via Web e acionamento automático com sensores.

---

## 📷 Demonstração

[🔗 Link para o vídeo]()

---

## 🚀 Funcionalidades

- 🌐 Acesso via servidor web local hospedado na própria Pico W
- 💡 Controle manual dos LEDs por ambiente (quarto, sala e cozinha)
- 🔁 Um botão ativa/desativa todos os sensores (presença + luz)
- 🤖 Modo automático:
  - Detecta presença com o sensor **TCRT5000**
  - Detecta luminosidade com o sensor **LDR**
  - Se escuro e houver presença → acende os **3 LEDs**
- ⚙️ Interface web com botões que mostram dinamicamente o estado atual dos dispositivos
- 🔒 O controle manual desativa temporariamente o modo automático
- ✅ Reativando sensores, o modo automático é restaurado

---

## 🧩 Hardware Utilizado

- ✅ Raspberry Pi Pico W (BitDogLab)
- ✅ Sensor TCRT5000 (GPIO 16) + resistor 220Ω
- ✅ Sensor LDR + resistor 10kΩ (GPIO 28 - ADC2)
- ✅ LED RGB integrado (GPIOs 11, 12, 13)
- ✅ Conexão Wi-Fi via CYW43
- ✅ Protoboard e jumpers

---

## 🗂️ Estrutura do Projeto

```
CasaBit/
├── main.c
├── leds.c / leds.h
├── sensores.c / sensores.h
├── webserver.c / webserver.h
└── CMakeLists.txt
```

---

## 🌐 Interface Web

A interface web possui:
- Botões para **Quarto**, **Sala** e **Cozinha** (acendem LEDs em pares)
- Botão **Sensores** (ativa/desativa sensores)
- Cada botão exibe "Ligar/Desligar" de acordo com o estado dos LEDs

---

## 🛠️ Compilação

Requer o **Pico SDK**. Para compilar:

```bash
mkdir build && cd build
cmake ..
make
```

> 💡 Certifique-se de ter o SDK da Raspberry Pi Pico configurado corretamente.

---


## 📝 Licença
Este programa foi desenvolvido como um exemplo educacional e pode ser usado livremente para fins de estudo e aprendizado.

## 📌 Autor
LORENZO GIUSEPPE OLIVEIRA BARONI
