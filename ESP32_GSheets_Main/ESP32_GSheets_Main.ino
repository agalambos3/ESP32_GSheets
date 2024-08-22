#include <WiFi.h>
#include <ESP_Google_Sheet_Client.h>
#include "Credentials/Google_Private.h"
#include "Credentials/Wifi_Private.h"


// Service Account's private key
const char PRIVATE_KEY[] PROGMEM = API_KEY;

void tokenStatusCallback(TokenInfo info);

void setup() {
 
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);

  WiFi.setAutoReconnect(true);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

   // Set the callback for Google API access token generation status (for debug only)
  GSheet.setTokenCallback(tokenStatusCallback);
  // Set the seconds to refresh the auth token before expire (60 to 3540, default is 300 seconds)
  GSheet.setPrerefreshSeconds(10 * 60);

  // Begin the access token generation for Google API authentication
  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);
  long read_num = 0;

  

}

void loop() {
  

  bool ready = GSheet.ready();
  FirebaseJson response;

  GSheet.values.get(&response,GSHEET_ID,"Sheet1!A1:A20");
  response.toString(Serial,true);

  FirebaseJsonData GS_parse_result;
  FirebaseJsonArray value_parse_array;
  FirebaseJsonData index_parse_result;

  response.get(GS_parse_result,"values");
  GS_parse_result.get<FirebaseJsonArray>(value_parse_array);

  Serial.println(); 
  Serial.println();

  for (size_t i = 0;i < value_parse_array.size();i++){
    value_parse_array.get(index_parse_result, i);
    Serial.print(i+1);
    Serial.print(". ");
    Serial.println(index_parse_result.stringValue);
  
    
    
  }
  Serial.println();
  // Serial.println(GS_parse_result.type);
  // Serial.println(GS_parse_result.stringValue);
  
  
  
  

  delay(5000);

  
  // (Optional )put device in some kinda sleep for some amount of time (10 minutes?)

  // wake up device and check spreadsheet cell for new message

  // if new message is ready, read spreadsheet cell with message

  //update new message cell to be empty 

  // send message over serial (may be subject to change if hooked up to receipt printer)

}

void tokenStatusCallback(TokenInfo info)
{
    if (info.status == token_status_error)
    {
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
        GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
    }
    else
    {
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    }
}
