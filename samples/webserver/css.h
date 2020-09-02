/**
  ******************************************************************************
  * css default content
  ******************************************************************************
  */

void sendCSS(WiFiClient client) {
  client.println(
    "body { background-color: black; color: #F0F0F0; font-family: Arial, Helvetica, sans-serif; } \n"
    "a { color: grey; } \n"
  ); 
}
