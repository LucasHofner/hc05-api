# API Documentation

## Introduction
  
This API was developed as an assignment for the discipline of Embedded Systems Programming at Federal University of Minas Gerais in February 2021. The classes were taught by professor Ricardo de Oliveira Duarte and the authors are Camilla Gomes Fernandes and Lucas Guimarães Hofner.

The interface was developed to manage Bluetooth functionality, such as connecting with devices, configured name and password for it, and managing data transfer between devices. The work was developed on the **Nucleo Board STM32f103RBT6** with the **Bluetooth module RS323 HC05**.

![Bluetooth module](https://www.vidadesilicio.com.br/media/catalog/product/1/_/1_1_11.jpg)

_Figure 1: Bluetooth module RS323 HC05._

About the organization of the API, in the main folder, in addition to the ReadMe file it is possible to find four folders. The first one, *Library*, contains the files **hc05.c** and **hc05.h** which are the API itself. The folder *Example_Application*, contains an example of how to use the API and how to configure the system **STMCubeMx**. The *Example_ATComm* allows manage the Bluetooth module using AT commands. The last folder *Others* contains files like the application for android devices created to test the data exchange and a tutorial to install it, along with another tutorial of how to pair the cell phone with the Bluetooth module before data transfer.

## Paring the devices

Before transfer data is necessary to pair the Bluetooth module with the cell phone. That can be done inside de configuration of the phone itself as shown in the link below.

[Watch video](https://github.com/LucasHofner/hc05-api/blob/master/Others/video_PairPhone.mp4)

The module can be presented in three states, to find out what status it is currently in, it is necessary to check the led within the device.

1.	Searching for other devices: When this state is on, is possible to see the led blinking very fast and unstoppable. 

![gif_Modo1](https://github.com/LucasHofner/hc05-api/blob/master/Others/gif_Modo1.gif)

2.	Paired: In this situation the led seams to flash twice and after three seconds it flashes twice again.

![gif_Modo2](https://github.com/LucasHofner/hc05-api/blob/master/Others/gif_Modo2.gif)

3.	Connected and ready to exchange data: The led flashes very slowly. Staying a few seconds on and a few seconds off which time. 

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
