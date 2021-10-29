const finnhub = require('finnhub')
const express = require('express')
const bodyParser = require('body-parser')
const api = require("./config").API_KEY

const app = express()
const jsonParser = bodyParser.json()

const api_key = finnhub.ApiClient.instance.authentications['api_key'];
api_key.apiKey = api
const finnhubClient = new finnhub.DefaultApi()

const cryptos = [];
var cryptoInfo = [];

cryptos.push("BTC")
// cryptos.push("DOGE")
// cryptos.push("ETH")

const scale = (elem, max, min) => {
  return Math.floor((elem-min)/(max-min)*7)
}

const getCryptoInfo = () => {
  cryptos.forEach((currency, i) => {
    // set up interval from 1 week ago to today
    var to = Math.round(Date.now() / 1000)
    var from = to - (60 * 60 * 24 * 7)

    finnhubClient.cryptoCandles("BINANCE:" + currency + "USDT", "60", from, to, (error, data, response) => {
      if (error) {
        console.log(error)
      }
      if (data.s == 'no_data') {
        noInfo = {
          "cryptoName" : currency,
          "error" : "No data found for this currency"
        }
        cryptoInfo.push(noInfo)
      }
      else {
        var scaled_prices = [];

        data_max = Math.max.apply(Math, data.c);
        data_min = Math.min.apply(Math, data.c);

        data.c.forEach(elem => scaled_prices.push(scale(elem, data_max, data_min)))
        reversed_scaled_prices = scaled_prices.reverse()

        info = {
          "cryptoName" : currency,
          "closedPrices" : data.c,
          "scaledPrices" : scaled_prices, 
          "reversedScaledPrices": reversed_scaled_prices
        }
        cryptoInfo.push(info)
      }
    })
  });
}

exports.getTrend = (req, res) => {
  if(req.method == 'GET'){
    if (cryptos.length == 0) {
      res.send("no stocks in your portfolio")
    }
    res.send(cryptoInfo);
    cryptoInfo = [];
    getCryptoInfo();
  }
};
