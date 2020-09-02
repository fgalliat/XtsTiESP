void scanNetworks(WiFiClient* client=NULL) {
    // WiFi.mode(WIFI_STA);
    // WiFi.disconnect();

    // can scan even if in SoftAP mode !

    Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    
    Serial.println("scan done");
    if (n == 0) {
         Serial.println("no networks found");
         if ( client != NULL ) { client->println("no networks found"); }
    } 
    else {
        Serial.print(n); Serial.println(" networks found");
        if ( client != NULL ) { 
            client->print(n); client->println(" networks found");
        }

        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" [open]":" [pass]");

            if ( client != NULL ) { 
                client->print(i + 1);
                client->print(": ");
                client->print(WiFi.SSID(i));
                client->print(" (");
                client->print(WiFi.RSSI(i));
                client->print(")");
                client->println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" [open]":" [pass]");
            }

            delay(10);
        }
    }
    Serial.println("-EOL-");
    if ( client != NULL ) { client->println("-EOL-"); }

}