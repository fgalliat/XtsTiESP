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

#if SD_SUPPORT
// ** Warning !! **
// Due to a WiFi lib collision : some File's flags are redefined then,
// SdFat can no more CREATE a non-exisiting file !!!!
// redefining these symbols does the Job
// from /home/fgalliat/Arduino/libraries/SdFat-master/src/FatLib/FatApiConstants.h
#define O_RDONLY  0X00  ///< Open for reading only.
#define O_WRONLY  0X01  ///< Open for writing only.
#define O_RDWR    0X02  ///< Open for reading and writing.
#define O_AT_END  0X04  ///< Open at EOF.
#define O_APPEND  0X08  ///< Set append mode.
#define O_CREAT   0x10  ///< Create file if it does not exist.
#define O_TRUNC   0x20  ///< Truncate file to zero length.
#define O_EXCL    0x40  ///< Fail if the file exists.
#define O_SYNC    0x80  ///< Synchronized write I/O operations.
// #define FILE_WRITE (O_RDWR | O_CREAT | O_AT_END)
#endif

#include "my_wifi.h" // WiFi routines


#include "buff.h" // POST request data accumulator

#include "scripts.h" // JavaScript code
#include "css.h"     // Cascading Style Sheets
#include "html.h"    // HTML page of the tool

/* SSID and password of your WiFi net ----------------------------------------*/
const char *ssid     = "Xtase-ESP";
const char *password = "xtaseESP";   // in AP mode : password MUST be >= 8 bytes

#define WEB_LOGIN "xtase"
#define WEB_PASS  "esp32"

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


bool sendResource(WiFiClient client,IPAddress myIP, char* filename, char* mimeType) {
    Serial.print("> AnyResource");
    #if SD_SUPPORT
        if ( SD.exists( filename ) ) {
            File f = SD.open( filename );
            if ( !f ) { 
                client.println("Oups failed to read any page"); 

                client.print("HTTP/1.1 400 OK\r\nContent-Type: text/html\r\n\r\n");
                client.print("Oups : failed to read ");
                client.print(filename);
                client.println();
                client.print("\r\n");
                delay(1);

                return false; 
            }
            client.print("HTTP/1.1 200 OK\r\nContent-Type: ");
            client.print(mimeType);
            client.print("\r\n\r\n");

            char buffer[1024+1]; memset(buffer, 0x00, 1024+1);
            while( f.available() ) {
                int nb = f.read( buffer, 1024 );
                client.write( buffer, nb );
            }
            f.close();

            client.print("\r\n");
            delay(1);
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

#define TOKEN_MAX_LEN 16
char token[TOKEN_MAX_LEN+1];
int tokenLen = 0;


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

    const char* login= WEB_LOGIN;
    const char* pass = WEB_PASS;
    char seq[64]; sprintf(seq, "login=%s&password=%s", login, pass);

    Serial.println("--------------");
    // Serial.println( (char*)seq);
    Serial.println( (char*)authVars);
    Serial.println("--------------");


    bool authOK = strncmp( (char*)seq, (char*)authVars, strlen( seq ) ) == 0;

    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");

    if ( authOK ) {
        client.println( "auth is OK" );

        // generate a token
        memset(token, TOKEN_MAX_LEN+1, 0x00);
        ltoa(millis(),  token, 10 ); // 10 for Base10
        tokenLen = strlen(token);

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

char uploadFilename[64]; // beware filename can be up to 63 bytes long
int uploadFileLen = -1;
int uploadFileGot = -1;

/* The server state observation loop -------------------------------------------*/
bool Srvr__loop()
{

    if ( Serial.available() ) {
        int c = Serial.read();
        if ( c == 's' ) {
            scanNetworks();
        }
    }


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

            // if (Buff__signature(4, "/upload.html")) {
            //     client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
            //     // respond legacy page
            //     sendHtml(client, myIP);
            //     client.print("\r\n");
            //     delay(1);
            //     return true;
            // }

            if (Buff__signature(4, "/app.html")) {
                client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
                sendAppPage(client, myIP);
                client.print("\r\n");
                delay(1);
                return true;
            }

            if (Buff__signature(4, "/test.html")) {
                sendResource(client, myIP, "/www/test.html", "text/html");
                return true;
            }

            if (Buff__signature(4, "/upload.html")) {
                sendResource(client, myIP, "/www/upload.html", "text/html");
                return true;
            }

            if (Buff__signature(4, "/scannet")) {
                client.print("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
                scanNetworks(&client);
                client.print("\r\n");
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

                // read the token
                Buff__bufInd = 0;
                while (client.available()) {
                    int q = client.read();
                    Buff__bufArr[Buff__bufInd++] = (byte)q;
                    if (Buff__signature(Buff__bufInd - 1, ":")) { break; }
                }
                bool goodToken = Buff__signature(0, token); 
                goodToken &= Buff__signature(tokenLen, ":"); // endOfToken
                int headLen = Buff__bufInd;

                if ( goodToken ) {
                    // read the data
                    Buff__bufInd = 0;
                    while (client.available()) {
                        int q = client.read();
                        Buff__bufArr[Buff__bufInd++] = (byte)q;
                        if (Buff__signature(Buff__bufInd - 2, "\r\n")) { break; }
                    }

                    if (Buff__signature(0, "file:")) {
                        memset(uploadFilename, 0x00, 64);
                        sprintf(uploadFilename, "/www/");
                        int addr = 5; // "/www/".length()
                        int i = 5; // "file:".length();
                        for(; i < Buff__bufInd-2; i++) {
                            if ( Buff__bufArr[i] == ':' ) {
                                break;
                            }
                            // FIXME : ensure that name is not more than 63 bytes loong
                            uploadFilename[addr++] = Buff__bufArr[i];
                        }
                        uploadFilename[addr] = 0x00;
                        i++;
                        char fileLenStr[16]; 
                        addr = 0;
                        for(; i < Buff__bufInd-2; i++) {
                            fileLenStr[addr++] = Buff__bufArr[i];
                        }
                        fileLenStr[addr] = 0x00;
                        int fileLen = atoi( fileLenStr );

                        Serial.print( "Sends the filename (" );
                        Serial.print( uploadFilename );
                        Serial.print( ")-(" );
                        Serial.print( fileLen );
                        Serial.println( ")" );

                        uploadFileLen = fileLen;
                        uploadFileGot = 0;

                    } else if (Buff__signature(0, "EOF")) {
                        Serial.print( "Sends the EOF > " );
                        Serial.print( uploadFileGot );
                        Serial.print( " Vs " );
                        Serial.println( uploadFileLen );

                        if (uploadFileLen != uploadFileGot) {
                            Serial.println("MAY HAD a problem while uploading");
                        }

                    } else {

                        if ( uploadFileGot == 0 ) {
                            // Cf FILE_WRITE -> is append too
                            if ( SD.exists(uploadFilename) ) { SD.remove(uploadFilename); }
                        }

                        File uf = SD.open(uploadFilename, FILE_WRITE);

                        if ( !uf ) {
                            Serial.print("Error w/ file : "); Serial.println(uploadFilename);
                        } else {
                            for(int i=0; i < Buff__bufInd-2; i+=2) {
                                int ch = Buff__getByte(i);
                                if ( ch == -1 ) {
                                    // can occurs @ end of string ...?
                                    // Serial.println("Error decoding blob");
                                    break;
                                }
                                // Serial.write( (char)ch );
                                uf.write( (char)ch );
                                uploadFileGot++;
                            }
                            Serial.println( "=============" );

                            uf.flush();
                            uf.close();
                        }
                    }


                    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
                    client.print("OK\n");
                } else {
                    Serial.println("Wrong token !");

                    client.print("HTTP/1.1 400 ERROR\r\nContent-Type: text/html\r\n\r\n");
                    client.print("Wrong token\n");
                }
                
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

