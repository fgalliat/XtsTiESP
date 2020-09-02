/**
  ******************************************************************************
  * html default content
  ******************************************************************************
  */

void sendHtml(WiFiClient client,IPAddress myIP) {
  client.print(
  "<!DOCTYPE html>\r\n"
  "<html>\r\n"
  " <body>\r\n"
  "   You're server may have no file, or you should not be here ..."
  " </body>\r\n"
  "</html>\r\n"
  );
}
