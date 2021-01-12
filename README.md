# TemperatureIOT
## IOT Room temperature 


Steps for send email from thinkspeak
https://blogs.mathworks.com/iot/2020/01/10/send-email-alerts-from-thingspeak/#:~:text=ThingSpeak%20now%20offers%20email%20alerts,then%20respond%20with%20an%20email.

# Matlab Analisys code

alert_body = 'Temperatura acima do estipulado';
alert_subject = 'Temperatura do Lab está acima do estipulado';
alert_api_key = 'TAKNUD4U66A9TP5NDAZXC';
alert_url= "https://api.thingspeak.com/alerts/send";
jsonmessage = sprintf(['{"subject": "%s", "body": "%s"}'], alert_subject,alert_body);
options = weboptions("HeaderFields", {'Thingspeak-Alerts-API-Key', alert_api_key; 'Content-Type','application/json'});
result = webwrite(alert_url, jsonmessage, options);

# Thinkspeak tutorial
https://www.filipeflop.com/blog/esp8266-com-thingspeak/
