#include <LEDMatrix.h>
#include <FastLED.h>

#include <LEDMatrixServer.h>

struct pad_t {
    CRGB color = CRGB(255, 255, 255);
    uint8_t x, y0, y1;
    void moveUp(){
      if (y0-1 >= 0){
        y0--;
        y1--;
      }
    }

    void moveDown(){
      if (y1+1 < MATRIX_HEIGHT){
        y0++;
        y1++;
      }
    }
} pad1, pad2;

struct ball_t {
    double x, y, dx, dy;
} ball;

class PongGame
{
public:
    PongGame(){};

    void begin(){
        ball.x = 8;
        ball.y = 8;

        double absoluteDx = random(1, 10)/10.0;
        ball.dx = (random(0, 2) * 2 - 1) * absoluteDx;
        ball.dy = (random(0, 2) * 2 - 1) * (1-absoluteDx);

        pad1.x = 0;
        pad1.y0 = 5;
        pad1.y1 = 9;

        pad2.x = MATRIX_WIDTH-1;
        pad2.y0 = 3;
        pad2.y1 = 7;
    }

    void predictSecondPad(){
      if (ball.dx > 0){
        if (ball.y < pad2.y0){
          pad2.moveUp();
        } else if (ball.y > pad2.y1){
          pad2.moveDown();
        }
      }
    }

    void render_pads(){
        for (int i=0; i<MATRIX_HEIGHT; i++){
          if ((i >= pad1.y0) && (i <= pad1.y1)){
            leds[XY(pad1.x, i)] = pad1.color;
          } else {
            leds[XY(pad1.x, i)] = CRGB(0, 0, 0);
          }
          
          if ((i >= pad2.y0) && (i <= pad2.y1)){
            leds[XY(pad2.x, i)] = pad2.color;
          } else {
            leds[XY(pad2.x, i)] = CRGB(0, 0, 0);
          }
        }
    }

    void render(){
        leds[XY((int)ball.x, (int)ball.y)] = CRGB(0, 0, 0);


        if (((ball.y + ball.dy) <= 0) || ((ball.y + ball.dy) >= MATRIX_HEIGHT)) {
            ball.dy = -ball.dy;
        }

        if ((ball.x + ball.dx) <= 1) {
            if (((ball.y + ball.dy) > pad1.y0) && ((ball.y + ball.dy) < pad1.y1)){
              ball.dx = -ball.dx;
            } else {
              begin();
            }
        }

        if ((ball.x + ball.dx) >= MATRIX_WIDTH-1){
            if (((ball.y + ball.dy) > pad2.y0) && ((ball.y + ball.dy) < pad2.y1)){
              ball.dx = -ball.dx;
            } else {
              begin();
            }
        }

        ball.y += ball.dy;
        ball.x += ball.dx;
        leds[XY((int)ball.x, (int)ball.y)] = mBallColor;

        predictSecondPad();
        render_pads();
        FastLED.show();
    }
private:
    CRGB mBallColor = CRGB(255, 0, 0);
};

PongGame game;

AsyncWebSocket gamesWS("/games/ws");


void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        if (strcmp((char*)data, "u") == 0) {
            pad1.moveUp();
        } else {
            pad1.moveDown();
        }
    }
}

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_CONNECT){
    //client connected
    os_printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->printf("Hello Client %u :)", client->id());
    client->ping();
  } else if(type == WS_EVT_DISCONNECT){
    //client disconnected
    os_printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
  } else if(type == WS_EVT_ERROR){
    //error was received from the other end
    os_printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if(type == WS_EVT_PONG){
    //pong message was received (in response to a ping request maybe)
    os_printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } else if(type == WS_EVT_DATA){
    //handleWebSocketMessage(arg, data, len);
    //data packet
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    if(info->final && info->index == 0 && info->len == len){
      //the whole message is in a single frame and we got all of it's data
      os_printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);
      if(info->opcode == WS_TEXT){
        data[len] = 0;
        os_printf("%s\n", (char*)data);
      } else {
        for(size_t i=0; i < info->len; i++){
          os_printf("%02x ", data[i]);
        }
        os_printf("\n");
      }
      if(info->opcode == WS_TEXT){
        client->text("I got your text message");
        handleWebSocketMessage(arg, data, len);
      }
      else
        client->binary("I got your binary message");
    } else {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if(info->index == 0){
        if(info->num == 0)
          os_printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
        os_printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      os_printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
      if(info->message_opcode == WS_TEXT){
        data[len] = 0;
        os_printf("%s\n", (char*)data);
      } else {
        for(size_t i=0; i < len; i++){
          os_printf("%02x ", data[i]);
        }
        os_printf("\n");
      }

      if((info->index + len) == info->len){
        os_printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if(info->final){
          os_printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          if(info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}

void setup(){
    Serial.begin(115200);
    randomSeed(analogRead(A0));
    setupFastLED();
    setupWiFi("OpenWRT", "22446688");
    
    gamesWS.onEvent(onEvent);
    server.addHandler(&gamesWS);
    server.begin();
    game.begin();    
    
}

void loop(){
  game.render();
  FastLED.delay(1000/30);
}
