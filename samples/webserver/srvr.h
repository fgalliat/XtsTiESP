/**
  ******************************************************************************
  * 
  * Xtase - fgalliat @Aug2020
  * 
  * WebServer
  * 
  * 12345678901
  * GET / HTTP/1.1 (...)
  * GET /scriptB.jss
  * 
  * 
  * POST /EPDa
  * POST /aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa (...)
  * POST /SHOW
  * 
  * inspired by :
  * @author  Waveshare Team
  * @date    23-January-2018
  *
  ******************************************************************************
  */

/* == AUTH request example ==
POST /auth HTTP/1.1
Host: 192.168.4.1
(...)
Accept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7                               
                                                                                   
login=toto&password=titi
*/



/* Includes ------------------------------------------------------------------*/
#include <WiFi.h>

#include "buff.h" // POST request data accumulator

#include "scripts.h" // JavaScript code
#include "css.h"     // Cascading Style Sheets
#include "html.h"    // HTML page of the tool

/* SSID and password of your WiFi net ----------------------------------------*/
const char *ssid     = "Xtase-ESP";
const char *password = "xtaseESP";   // in AP mode : password MUST be >= 8 bytes

#define WIFI_STA_MODE 0

/* Server and IP address ------------------------------------------------------*/
WiFiServer server(80); // Wifi server exemplar using port 80
IPAddress myIP;        // IP address in your local wifi net

/* The 'index' page flag ------------------------------------------------------*/
bool isIndexPage = true; // true : GET  request, client needs 'index' page;
// false: POST request, server sends empty page.
/* Server initialization -------------------------------------------------------*/
void Srvr__setup() {

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

#if WIFI_STA_MODE
    WiFi.mode(WIFI_STA);
    delay(200);
    WiFi.begin(ssid, password);

    // Waiting the connection to a router
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // Connection is complete
    Serial.println("");
    Serial.println("WiFi connected");

    // Show obtained IP address in local Wifi net
    Serial.println(myIP = WiFi.localIP());
#else
    WiFi.mode(WIFI_AP);
    delay(200);
    bool ok = WiFi.softAP(ssid, password);
    if ( !ok ) {
        while(true) {
            Serial.println("Can't begin SoftAP");
            delay(1000);
        }
    }
    // Show obtained IP address in local Wifi net
    Serial.println(myIP = WiFi.softAPIP() );
#endif

    // Start the server
    server.begin();
    Serial.println("Server started on :80");
}

bool sendIndexPage(WiFiClient client,IPAddress myIP) {
    Serial.print("> HomePage");
    #if SD_SUPPORT
        if ( SD.exists("/www/index.html") ) {
            // client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");

            File f = SD.open("/www/index.html");
            if ( !f ) { client.println("Oups failed to read index page"); return true; }
            char buffer[1024+1]; memset(buffer, 0x00, 1024+1);
            while( f.available() ) {
                int nb = f.read( buffer, 1024 );
                client.write( buffer, nb );
            }
            f.close();

            // this is done after
            // client.print("\r\n");
            // delay(1);
        } else
    #endif
    sendHtml(client, myIP);

    return true;
}

bool sendAppPage(WiFiClient client,IPAddress myIP) {
    Serial.print("> AppPage");
    #if SD_SUPPORT
        if ( SD.exists("/www/app.html") ) {
            File f = SD.open("/www/app.html");
            if ( !f ) { client.println("Oups failed to read app page"); return true; }
            char buffer[1024+1]; memset(buffer, 0x00, 1024+1);
            while( f.available() ) {
                int nb = f.read( buffer, 1024 );
                client.write( buffer, nb );
            }
            f.close();
        } else
    #endif
    sendHtml(client, myIP);

    return true;
}

/* Sending a script to the client's browser ------------------------------------*/
bool Srvr__file(WiFiClient client, int fileIndex, char *fileName)
{
    // Print log message: sending of script file
    Serial.print(fileName);

    // Sent to the 'client' the header describing the type of data.
    client.print(fileIndex == 0
                 ? "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n\r\n"
                 : "HTTP/1.1 200 OK\r\nContent-Type: text/javascript\r\n\r\n");

    // Choose the index of script
    // (ESP8266 can't to send all of code by one file
    // and needs split it on a few parts)
    switch (fileIndex) {
    case 0:
        sendCSS(client);
        break;
    case 1:
        sendJS_A(client);
        break;
    case 2:
        sendJS_B(client);
        break;
    case 3:
        sendJS_C(client);
        break;
    case 4:
        sendJS_D(client);
        break;
    }

    client.print("\r\n");
    delay(1);

    // Print log message: the end of request processing
    Serial.println(">>>");

    return true;
}

// AUTH request
const char* authReq = "POST /auth HTTP/1.1";
const int authReqLen = strlen( authReq );


void authenticate(WiFiClient client,IPAddress myIP) {
    Buff__bufInd = 0;
    while (client.available()) {
        // Read a character from 'client'
        int q = client.read();

        // Save it in the buffer and increment its index
        Buff__bufArr[Buff__bufInd++] = (byte)q;

        if (Buff__signature(Buff__bufInd - 4, "\r\n\r\n")) { break; }
    }

    int remaining = client.available();
    uint8_t authVars[ remaining + 1 ] ;
    authVars[remaining] = 0x00;
    client.read(authVars, remaining);

    const char* login="xtase";
    const char* pass ="esp32";
    char seq[64]; sprintf(seq, "login=%s&password=%s", login, pass);

    Serial.println("--------------");
    Serial.println( (char*)seq);
    Serial.println( (char*)authVars);
    Serial.println("--------------");


    bool authOK = strncmp( (char*)seq, (char*)authVars, strlen( seq ) ) == 0;

    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if ( authOK ) {
        client.println( "auth is OK" );

        // generate a token
        char token[16+1]; memset(token, 16+1, 0x00);
        ltoa(millis(),  token, 10 ); // 10 for Base10

        const char* resp = "<script>\n "\
        " sessionStorage.setItem('token', '%s'); \n" \
        " sessionStorage.setItem('host', '%s.%s.%s.%s'); \n" \
        " location.href='/app.html'; \n "\
        "</script>";

        client.printf( resp, token, String(myIP[0],DEC), String(myIP[1],DEC), String(myIP[2],DEC), String(myIP[3],DEC) );

    } else {
        // wrong auth goes to /index
        sendIndexPage(client, myIP);
    }

    client.print("\r\n");
    delay(1);
}



/* The server state observation loop -------------------------------------------*/
bool Srvr__loop()
{
    // Looking for a client trying to connect to the server
    WiFiClient client = server.available();

    // Exit if there is no any clients
    if (!client)
        return false;

    // Print log message: the start of request processing
    Serial.print("<<<");

    // Waiting the client is ready to send data
    while (!client.available())
        delay(1);

    // Set buffer's index to zero
    // It means the buffer is empty initially
    Buff__bufInd = 0;

    // While the stream of 'client' has some data do...
    while (client.available()) {
        // Read a character from 'client'
        int q = client.read();

        // Save it in the buffer and increment its index
        Buff__bufArr[Buff__bufInd++] = (byte)q;

        Serial.write( (char)q ); // FIXME : temp

        // If the carachter means the end of line, then...
        if ((q == 10) || (q == 13)) {
            // Clean the buffer
            Buff__bufInd = 0;
            continue;
        }

        // auth req ?
        if (Buff__bufInd == authReqLen) {
            if ( strncmp( authReq, Buff__bufArr, authReqLen ) == 0 ) {
                Serial.println("AUTH request !!!");
                authenticate(client, myIP);
                return true;
            }
        }

        // Requests of files
        if (Buff__bufInd >= 11) {
            if (Buff__signature(Buff__bufInd - 11, "/styles.css"))
                return Srvr__file(client, 0, "styles.css");

            if (Buff__signature(Buff__bufInd - 11, "/scriptA.js"))
                return Srvr__file(client, 1, "scriptA.js");

            if (Buff__signature(Buff__bufInd - 11, "/scriptB.js"))
                return Srvr__file(client, 2, "scriptB.js");

            if (Buff__signature(Buff__bufInd - 11, "/scriptC.js"))
                return Srvr__file(client, 3, "scriptC.js");

            if (Buff__signature(Buff__bufInd - 11, "/scriptD.js"))
                return Srvr__file(client, 4, "scriptD.js");

            if (Buff__signature(4, "/upload.html")) {
                client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
                // respond legacy page
                sendHtml(client, myIP);
                client.print("\r\n");
                delay(1);
                return true;
            }

            if (Buff__signature(4, "/app.html")) {
                client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
                sendAppPage(client, myIP);
                client.print("\r\n");
                delay(1);
                return true;
            }

            if (Buff__signature(4, "/favicon.ico")) {
                // respond 404
                client.print("HTTP/1.1 400 NOK\r\n");
                client.print("\r\n");
                delay(1);
                return true;
            }
        }

        // If the buffer's length is larger, than 4 (length of command's name), then...
        if (Buff__bufInd > 4) {
            // It is probably POST request, no need to send the 'index' page
            isIndexPage = false;

            if (Buff__signature(5, "/REPL:")) {
                Serial.print("\r\nREPLACE command\r\n");

                Buff__bufInd = 0;
                while (client.available()) {
                    int q = client.read();
                    Buff__bufArr[Buff__bufInd++] = (byte)q;
                    if (Buff__signature(Buff__bufInd - 2, "\r\n")) { break; }
                }

                if (Buff__signature(0, "file:")) {
                    Serial.println( "Sends the filename" );
                } else if (Buff__signature(0, "EOF")) {
                    Serial.println( "Sends the EOF" );
                } else {
                    for(int i=0; i < Buff__bufInd-2; i+=2) {
                        int ch = Buff__getByte(i);
                        if ( ch == -1 ) {
                            // can occurs @ end of string ...?
                            // Serial.println("Error decoding blob");
                            break;
                        }
                        Serial.write( (char)ch );
                    }
                    Serial.println( "=============" );
                }

                client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
                client.print("OK\n");
                client.print("\r\n");
                delay(1);
                return true;
            }


            // e-Paper driver initialization
            if (Buff__signature(Buff__bufInd - 4, "EPD")) {
                Serial.print("\r\nEPD\r\n");

//                // Getting of e-Paper's type
//                EPD_dispIndex = (int)Buff__bufArr[Buff__bufInd - 1] - (int)'a';

//                // Print log message: initialization of e-Paper (e-Paper's type)
//                Serial.printf("EPD %s", EPD_dispMass[EPD_dispIndex].title);

                // Initialization
//                EPD_dispInit();

                //client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
                break;
            }

            // Image loading
            if (Buff__signature(Buff__bufInd - 4, "LOAD")) {
                // Print log message: image loading
                Serial.print("LOAD");

                // Load data into the e-Paper
                // if there is loading function for current channel (black or red)
//                if (EPD_dispLoad != 0)
//                    EPD_dispLoad();


                //client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
                break;
            }

            // Select the next data channel
            if (Buff__signature(Buff__bufInd - 4, "NEXT")) {
                // Print log message: next data channel
                Serial.print("NEXT");

//                // Instruction code for for writting data into
//                // e-Paper's memory
//                int code = EPD_dispMass[EPD_dispIndex].next;

//                // If the instruction code isn't '-1', then...
//                if (code != -1) {
//                    // Print log message: instruction code
//                    Serial.printf(" %d", code);

//                    // Do the selection of the next data channel
//                    EPD_SendCommand(code);
//                    delay(2);
//                }

//                // Setup the function for loading choosen channel's data
//                EPD_dispLoad = EPD_dispMass[EPD_dispIndex].chRd;

                //client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
                break;
            }

            // If the loading is complete, then...
            if (Buff__signature(Buff__bufInd - 4, "SHOW")) {

//                // Show results and Sleep
//                EPD_dispMass[EPD_dispIndex].show();

                //Print log message: show
                Serial.print("\r\nSHOW");
                //client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
                break;
            }

            // If the routine reaches this code,
            // it means the there is no known commands,
            // the server has to send the 'index' page
            isIndexPage = true;
        }
    }

    // Clear data stream of the 'client'
    client.flush();

    // Sent to the 'client' the header describing the type of data.
    // In this case 'Content-Type' is 'text/html'
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");

    // Send the 'index' page if it's needed
    if (isIndexPage) {
        sendIndexPage(client, myIP);
    }
    else {
        client.print("Ok!");
    }

    client.print("\r\n");
    delay(1);

    // Print log message: the end of request processing
    Serial.println(">>>");
    return true;
}

