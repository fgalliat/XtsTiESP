/**
  ******************************************************************************
  * JS scripts default content
  ******************************************************************************
  */ 

void sendJS_A(WiFiClient client) {
    client.println(
        "console.log('js scripts A');\n"
    );
}

void sendJS_B(WiFiClient client) {
    client.println(
        "console.log('js scripts B');\n"
    );
}

void sendJS_C(WiFiClient client) {
    client.println(
        "console.log('js scripts C');\n"
    );
}

void sendJS_D(WiFiClient client) {
    client.println(
        "console.log('js scripts D');\n"
    );
}
