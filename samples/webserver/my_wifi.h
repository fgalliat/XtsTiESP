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

// current mode
#if WIFI_STA_MODE
bool inSoftAPMode = false;
#else
bool inSoftAPMode = true;
#endif


bool hasSSID_PSK_registered() {
    // return true if has a WiFi config file
    return false;
}

bool hasPSK_registered(char* ssid) {
    // return true if has a WiFi config for this SSID
    return false;
}

bool connectSSID(char* ssid) {
    // return true if could connect to given SSID
    return false;
}

// code not yet certified
bool setWifiMode(bool softAPMode) {
    if ( inSoftAPMode && softAPMode ) { return true; }

    if ( softAPMode ) {
        WiFi.disconnect();
        delay(200);
        WiFi.mode(WIFI_AP);
        delay(200);
        bool ok = WiFi.softAP(ssid, password);
        if ( !ok ) {
            Serial.println("Can't begin SoftAP");
            return false;
        }
        // Show obtained IP address in local Wifi net
        Serial.println(myIP = WiFi.softAPIP() );

        inSoftAPMode = true;
    } else {
        if ( ! hasSSID_PSK_registered() ) {
            Serial.println( "No WiFi PSK registered" );
            return false;
        }

        int n = WiFi.scanNetworks();
        if ( n == 0 ) {
            Serial.println( "No WiFi Network found" );
            return false;
        }

        for (int i = 0; i < n; ++i) {
            char* ssid = (char*)WiFi.SSID(i).c_str();
            if ( hasPSK_registered(ssid) ) {
                bool ok = connectSSID(ssid);
                if ( !ok ) {
                    Serial.print( "Could not connect to " );
                    Serial.println( ssid );
                    return false;
                }
                Serial.print( "Connected to " );
                Serial.println( ssid );
                // Show obtained IP address in local Wifi net
                Serial.println(myIP = WiFi.localIP() );
                inSoftAPMode = false;
                return true;
            }
        }
        Serial.print( "Could not connect to any network" );
        return false;
    }
    return true;
}