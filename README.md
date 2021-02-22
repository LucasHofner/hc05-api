# API Documentation

## Introduction
  
This API was developed as an assignment for the discipline of Embedded Systems Programming at Federal University of Minas Gerais in February 2021. The classes were taught by professor Ricardo de Oliveira Duarte and the authors are Camilla Gomes Fernandes and Lucas Guimarães Hofner.

The interface was developed to manage Bluetooth functionality, such as connecting with devices, configured name and password for it, and managing data transfer between devices. The work was developed on the **Nucleo Board STM32f103RBT6** with the **Bluetooth module RS323 HC05**.

![Bluetooth module](https://www.vidadesilicio.com.br/media/catalog/product/1/_/1_1_11.jpg)

_Figure 1: Bluetooth module RS323 HC05._

About the organization of the API, in the main folder, in addition to the ReadMe file it is possible to find four folders. The first one, *Library*, contains the files **hc05.c** and **hc05.h** which are the API itself. The folder *Example_Application*, contains an example of how to use the API and the Android application created for testing the API. The *Example_ATComm* allows manage the Bluetooth module using AT commands, changing the module's name and baud rate. The last folder *Others* contains files like the application for android devices created to test the data exchange and a tutorial to install it, along with another tutorial of how to pair the cell phone with the Bluetooth module before data transfer.

## Setting up CubeMX Project
The first step presented here is optional but recommended as it facilitates the creation of a new STM32F1 project. It consists in creating a project in CubeMX software. This software allows the user, in a easy way, configure the microcontroller and peripherals and also generating code. It can be downloaded from the ST website:
https://www.st.com/en/development-tools/stm32cubemx.html.

Once the software is installed, the option in the main menu "Access to Board Selector" has to be selected, as shown in the image below.
![Cube_AccessBoardSelector](https://github.com/LucasHofner/hc05-api/blob/master/Others/cube_initialscreen.PNG)

_Figure 2 Accessing CubeMX Board Selector Tool_

Next, in the Board Selector Tool, the board's name has to be searched (Nucleo-F103RB) and be selected. This step is shown in the Figure 3. The message shown in Figure 4 will appear, and is recommended to select the option 'Yes'. In that case, CubeMX will initialize all peripherals in default mode, including the board's LED and Push Button.

![Cube_BoardSelector](https://github.com/LucasHofner/hc05-api/blob/master/Others/cube_boardselection.PNG)

_Figure 3 CubeMX Board Selector Tool_

![Cube_InitializePeripherals](https://github.com/LucasHofner/hc05-api/blob/master/Others/cube_initperip.PNG)

_Figure 4 Initialize Peripherals in Default Mode_

Moving on, a UART connection must be configured. Note that the UART2 will be configured as default. This is because UART2 is used by debugging purposes (as shown in Figure 5). So if it is intended a new UART connection must be added (Figure 6). We recommend to select a initial baud rate of 38400 bits/s, because is the HC-05 baud rate in AT Command (used for configuration). Also, when the new UART is included, the respective pins of TX and RX is displayed on the board, on the right side, to help in the connections later on.

![Cube_UARTDebug](https://github.com/LucasHofner/hc05-api/blob/master/Others/cube_debug.PNG)

_Figure 5 CubeMX Debug Setup_

![Cube_UART Configuration](https://github.com/LucasHofner/hc05-api/blob/master/Others/cube_uart.PNG)

_Figure 6 CubeMX UART Configuration_

The final step is to generate code. To do this the tab "Project Manager" must be choosed. In Toolchain/IDE, the option based on IDE used must be choosed. In the case, the option "SW4STM32" was selected (Figure 7). Finally, click on "Generate Code" and CubeMX will generate the code for the setup configured.

![Cube_CodeGen(https://github.com/LucasHofner/hc05-api/blob/master/Others/cube_generatecode.PNG)

_Figure 7 CubeMX Code Generation_

## Using the API

To use the API, the only two steps are necessary:
- Include '#include "hc05.h" ' in your main.c or main.h file
- Initialize Bluetooth Module by calling initializeHC05Module(&huart), passing as argument the UART Handler.

## Hardware Setup

The hardware Setup is shown in Figure 8. The following points should be considered:
- Voltage divider on RX (HC05) to TX (MCU).
- RX of HC05 on TX of MCU connection and TX of HC05 to RX to MCU.
- The "switch" on KEY pin connection.

The last item is explained by the AT Command Mode. This is a different operation mode than the regular one (that connects to a device and perform data transfer). In this mode all the AT commands are performed. These commands are used to configure different parameters of the module, from his name and PIN code to UART configuration and connection parameters. When KEY is connected to +VCC **before** the module energizes, the module initializes in AT Command Mode. Otherwise, the initialization occurs in the regular mode.

![Hardware_setup](https://github.com/LucasHofner/hc05-api/blob/master/Others/setup_hc05.PNG)

_Figure 8 Hardware Setup_

## Pairing the devices

Before transfer data is necessary to pair the Bluetooth module with the cell phone. That can be done inside de configuration of the phone itself as shown in the link below.

[Watch video](https://github.com/LucasHofner/hc05-api/blob/master/Others/video_PairPhone.mp4)

The module can be presented in three states, to find out what status it is currently in, it is necessary to check the led within the device.

1.	**Searching for other devices:** When this state is on, is possible to see the led blinking very fast and unstoppable. 

![gif_Modo1](https://github.com/LucasHofner/hc05-api/blob/master/Others/gif_Modo1.gif)

2.	**Paired:** In this situation the led seams to flash twice and after three seconds it flashes twice again.

![gif_Modo2](https://github.com/LucasHofner/hc05-api/blob/master/Others/gif_Modo2.gif)

3.	**AT Command Mode:** The led flashes very slowly. Staying a few seconds on and a few seconds off which time. 

![gif_Modo3](https://github.com/LucasHofner/hc05-api/blob/master/Others/gif_Modo3.gif)

## Application to test the API

As previously mentioned an APP for android cell phone was created to check the data exchange via Bluetooth. Both ways: from cell to board and from STM32f103R to phone. It was developed using the MIT APP Inventor platform <http://appinventor.mit.edu/>.

[Download App](https://github.com/LucasHofner/hc05-api/blob/master/Others/AppModuloBluetooth.apk)

Basically what the application does is to allow the connection of a cell phone with the Bluetooth module in order to transmit data between the phone and the Nucleo Board. To install the APP follow the instructions of the video below.

[Watch video](https://github.com/LucasHofner/hc05-api/blob/master/Others/video_DownloadAPP.mp4)

It contains two buttons, that control a led on the board, and a space to be filled with the button information on the board. The led shows the cell phone sending information to the board and the button does the opposite.

![image_AppScreen](https://github.com/LucasHofner/hc05-api/blob/master/Others/image_AppScreen.png)

After pairing the devices and installing the APP on the phone, it will be necessary to connect the phone to the board. What can be done using the first button on the screen “Conexão Bluetooth”. Once this is done it will be possible to exchange data.

When clicking on the button “Ligar”, the character “L” will be sent from the cell phone to the board. Pressing the button “Desligar” the character “D” will be transmitted. Running the API when the board receive the characters “L” and “D” it will change the status of the led turning it on and off, respectively.

![gif_LED](https://github.com/LucasHofner/hc05-api/blob/master/Others/gif_LED.gif)

By pressing the button on STM32f103R, the character “1” will be sent to the cell phone. Once this character is received the APP will interpret this information and show a message in red at the bottom of the screen for one second.

![gif_Button](https://github.com/LucasHofner/hc05-api/blob/master/Others/gif_Button.gif)

The button “Sair” closes the APP.

## API Functions
In the file hc05.c it is possible to find the headers of the functions with a short explanation about each one, in addition to the parameters necessary to use them and, if any, their returns.

## References
* HC-05 Datasheet:
<https://components101.com/sites/default/files/component_datasheet/HC-05%20Datasheet.pdf>
