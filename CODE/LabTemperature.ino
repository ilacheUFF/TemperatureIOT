#include <ESP8266WiFi.h> //essa biblioteca já vem com a IDE. Portanto, não é preciso baixar nenhuma biblioteca adicional
 
/* defines - wi-fi */
#define SSID_REDE "Federal 2014" /* coloque aqui o nome da rede que se deseja conectar */
#define SENHA_REDE "bucaramanga" /* coloque aqui a senha da rede que se deseja conectar */
//#define SSID_REDE "iPhone de Ivan" /* coloque aqui o nome da rede que se deseja conectar */
//#define SENHA_REDE "1234567n" /* coloque aqui a senha da rede que se deseja conectar */
//#define SSID_REDE "LABMAQ_Estufa" /* coloque aqui o nome da rede que se deseja conectar */
//#define SENHA_REDE "#trem015423" /* coloque aqui a senha da rede que se deseja conectar */
#define INTERVALO_ENVIO_THINGSPEAK 60000 /* intervalo entre envios de dados ao ThingSpeak (em ms) */
#define LED_PLACA 16
 
 
/* constantes e variáveis globais */
char endereco_api_thingspeak[] = "api.thingspeak.com";
String chave_escrita_thingspeak = "5CSPHE3WFIE5LRGY";  /* Coloque aqui sua chave de escrita do seu canal */
unsigned long last_connection_time;

WiFiClient client;


/* prototypes */
void envia_informacoes_thingspeak(String string_dados);
void init_wifi(void);
void conecta_wifi(void);
void verifica_conexao_wifi(void);
void piscar(int tempo);
 
/*
* Implementações
*/
 
/* Função: envia informações ao ThingSpeak
* Parâmetros: String com a informação a ser enviada
* Retorno: nenhum
*/
void envia_informacoes_thingspeak(String string_dados)
{
    if (client.connect(endereco_api_thingspeak, 80))
    {
        /* faz a requisição HTTP ao ThingSpeak */
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+chave_escrita_thingspeak+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(string_dados.length());
        client.print("\n\n");
        client.print(string_dados);
         
        last_connection_time = millis();
        Serial.println("- Informações enviadas ao ThingSpeak!");
        piscar(900);
    }
}
 
/* Função: inicializa wi-fi
* Parametros: nenhum
* Retorno: nenhum
*/
void init_wifi(void)
{
    Serial.println("------WI-FI -----");
    Serial.println("Conectando-se a rede: ");
    Serial.println(SSID_REDE);
    Serial.println("\nAguarde...");
 
    conecta_wifi();
}
 
/* Função: conecta-se a rede wi-fi
* Parametros: nenhum
* Retorno: nenhum
*/
void conecta_wifi(void)
{
    /* Se ja estiver conectado, nada é feito. */
    if (WiFi.status() == WL_CONNECTED)
    {
        return;
    }
     
    /* refaz a conexão */
    WiFi.begin(SSID_REDE, SENHA_REDE);
     
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        digitalWrite(LED_PLACA, LOW);
    }
 
    Serial.println("Conectado com sucesso a rede wi-fi \n");
    Serial.println(SSID_REDE);
    digitalWrite(LED_PLACA, HIGH);
    piscar(500);
    piscar(500);
}
 
/* Função: verifica se a conexao wi-fi está ativa
* (e, em caso negativo, refaz a conexao)
* Parametros: nenhum
* Retorno: nenhum
*/
void verifica_conexao_wifi(void)
{
    conecta_wifi();
}

/* Função para piscar o led cada vez que a informação é enviada
 *  
 *  
 */
 void piscar(int tempo)
 {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(tempo);
  digitalWrite(LED_BUILTIN, LOW);
  delay(tempo);
 }

 float ler_temperatura(void)
 {
  int valorObtido = analogRead(A0);
  float milivolts = (valorObtido/1024.0) * 3300; 
  float celsius = milivolts/10;
  Serial.print("A temperatura é de = ");
  Serial.println(celsius);
  return(celsius);
 }
 
void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_PLACA, OUTPUT);
    last_connection_time = 0;
 
    /* Inicializa sensor de temperatura e umidade relativa do ar */
 
    /* Inicializa e conecta-se ao wi-fi */
    init_wifi();
}
 
//loop principal
void loop()
{
    char fields_a_serem_enviados[100] = {0};
    float temperatura_lida = 0.0;
    float umidade_lida = 0.0;
 
    /* Força desconexão ao ThingSpeak (se ainda estiver conectado) */
    if (client.connected())
    {
        client.stop();
        Serial.println("- Desconectado do ThingSpeak");
        Serial.println();
    }
 
    /* Garante que a conexão wi-fi esteja ativa */
    verifica_conexao_wifi();
     
    /* Verifica se é o momento de enviar dados para o ThingSpeak */
    if( millis() - last_connection_time > INTERVALO_ENVIO_THINGSPEAK )
    {
        temperatura_lida = ler_temperatura();        
        //temperatura_lida = dht.readTemperature();
        //umidade_lida = dht.readHumidity();
        sprintf(fields_a_serem_enviados,"field1=%.2f&field2=%.2f", temperatura_lida, temperatura_lida);
        envia_informacoes_thingspeak(fields_a_serem_enviados);
    }
 
    delay(1000);
}
