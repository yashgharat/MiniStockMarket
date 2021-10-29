# MiniStockMarket
Yash Gharat and Aryan Jha

## Description
This mini stock market can take ticker symbols for stocks and cryptos to show you real time-data using the [Finnhub](https://finnhub.io/) api. It uses a websocket to show the current price of the ticker symbol of your choosing and makes an api call every hour to cache new data regarding the daily, weekly, and monthly trend data. The **ESP8266** is an excellent MCU for this project because it can connect to WiFi really easily and also has ample GPIO to power the the displays we decided to use. Unfortunately however, it does not have enough memory for what else we wanted to do. This project could be redone with the **ESP8266** as a webserver that receives requests instead of the oppposite, this way asynchronous requests are easier as well.

![](https://i.imgur.com/tEPPsww.jpg)

## Schematic

![](https://i.imgur.com/to6d1cT.png)
