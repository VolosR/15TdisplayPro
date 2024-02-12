#include <TFT_eSPI.h>
#include <SPI.h>
#include <SD.h>
#include <ESP32Time.h>
#include <TouchDrvCSTXXX.hpp>
#include "utilities.h"
#include "JpgFunction.h"
#include <XPowersLib.h>
#include "bigFont.h"
#include "tinyFont.h"
#include "midleFont.h"
#include "smallFont.h"
#include "NotoSansBold15.h"

PowersSY6970 PMU;
ESP32Time rtc(0); 

//CONSTRUCTORS::::::::::::::::::::::::::::::::
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
TFT_eSprite spr = TFT_eSprite(&tft);
TFT_eSprite header = TFT_eSprite(&tft);
TFT_eSprite footer = TFT_eSprite(&tft);

TouchDrvCSTXXX touch;
int16_t xx[5], yy[5];
int x,y;

//COLORS::::::::::::::::::::::::::::::::::::::

#define sprback 0x3CFB
#define back 0x1D3F
unsigned short grays[13];

//VARIABLES:::::::::::::::::::::::::::::::::::
#define N 4
String gameTime="";

int box=55;
int posX[4];
int posY[4];
bool help=1;
bool selectFase=0;
int brightness=4;
int voltage=0;
int  volbar=0;

int taken[4][4]={{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};

int numbers[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,0,14,15}};
int numbersFinish[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,0}};
int numbersTaken[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int numbersTakenS[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int random1=0;
int random2=0;
int temp=0;

bool finish=0;
bool search=0;
int n=0;

int pos[2] = {0, 0};
int left=0;
int top=170;
bool finded=0;
int moves=0;
int sec=0;
int no=0;

const int width = 212;
const int height = 212;
const int splitSize = 53;
unsigned short splitIMG[16][splitSize*splitSize];
unsigned short iii[44944];
String chosen_image="/image1.jpg";
unsigned int pp[16]={0,53,106,159,11236,11289,11342,11395,22472,22525,22578,22631,33708,33761,33814,33867};

void setBrightness(uint8_t value)
{
    static uint8_t level = 0;
    static uint8_t steps = 16;
    if (value == 0) {
        digitalWrite(BOARD_TFT_BL, 0);
        delay(3);
        level = 0;
        return;
    }
    if (level == 0) {
        digitalWrite(BOARD_TFT_BL, 1);
        level = steps;
        delayMicroseconds(30);
    }
    int from = steps - level;
    int to = steps - value;
    int num = (steps + to - from) % steps;
    for (int i = 0; i < num; i++) {
        digitalWrite(BOARD_TFT_BL, 0);
        digitalWrite(BOARD_TFT_BL, 1);
    }
    level = value;
}
static int jpegDrawCallback(JPEGDRAW *pDraw)
{   
    if(selectFase)
    {sprite.pushImage(pDraw->x, pDraw->y,pDraw->iWidth,pDraw->iHeight,pDraw->pPixels);}
    else{
    spr.pushImage(pDraw->x, pDraw->y,pDraw->iWidth,pDraw->iHeight,pDraw->pPixels);
    String bb=String(no)+"/"+String(pDraw->iHeight)+" "+String(pDraw->iWidth);
    Serial.println(bb);
    no++;
    
    
    if(no==27){
    memcpy(iii, (uint16_t*)spr.getPointer(), 212 * 212 * sizeof(uint16_t));


      int index=0;
                
                for(int p=0;p<16;p++)
                {
                index=0;
                for(int i=0;i<53;i++){
                for(int j=0;j<53;j++){
                if(p>0)  
                splitIMG[p][index] = iii[pp[p-1]+j+(i*212)];
                else
                splitIMG[p][index]=TFT_BLACK;
                index++;}
                }
                }}
    }
    return 1;
}


void setup()
{

    Serial.begin(115200);
    Serial.println("begining");

    rtc.setTime(30, 24, 0, 0, 0, 2023,0); 
    tft.init();
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(1);

    // Initialize capacitive touch
    touch.setPins(BOARD_TOUCH_RST, BOARD_TOUCH_IRQ);
    touch.begin(Wire, CST226SE_SLAVE_ADDRESS, BOARD_I2C_SDA, BOARD_I2C_SCL);

    sprite.createSprite(222,222);
    spr.createSprite(212,212);
    header.createSprite(222,150);
    footer.createSprite(222,70);
    sprite.setTextDatum(4);
    sprite.setSwapBytes(true);
    spr.setSwapBytes(true);
    
    setBrightness(brightness);

    PMU.init(Wire, BOARD_I2C_SDA, BOARD_I2C_SCL, SY6970_SLAVE_ADDRESS);
    PMU.enableADCMeasure();

    for(int i=0;i<4;i++)
    {
      posX[i]=i*box;
      posY[i]=posX[i];
    }

     int co=220;
     for(int i=0;i<13;i++)
     {grays[i]=tft.color565(co, co, co);
     co=co-20;}

  
   initSD(); 
   delay(200);
   gameStart();
    gameStart();
   
  
  
}

void loop()
{

 uint8_t touched = touch.getPoint(xx, yy, touch.getSupportTouchPoint());
 if (touched)
    {
        if(selectFase)
        { 
           if(xx[0]>0 && xx[0]<112 && yy[0]>top && yy[0]<top+106)
           {chosen_image="/image1.jpg"; gameStart();}

           if(xx[0]>112 && xx[0]<220 && yy[0]>top && yy[0]<top+106)
           {chosen_image="/image2.jpg"; gameStart();}

            if(xx[0]>0 && xx[0]<112 && yy[0]>top+106 && yy[0]<top+220)
           {chosen_image="/image3.jpg"; gameStart();}

             if(xx[0]>112 && xx[0]<220 && yy[0]>top+106 && yy[0]<top+220)
           {chosen_image="/image4.jpg"; gameStart();}
        }
        else{
        x=xx[0]-left;
        y=yy[0]-top;
        pos[0]=xx[0];
        pos[1]=yy[0];

         if(xx[0]>180 && xx[0]<222 && yy[0]>405 && yy[0]<480)
        {help=!help; drawFooter(); drawSprite(); delay(40);}

        if(xx[0]>10 && xx[0]<180 && yy[0]>405 && yy[0]<480)
        {brightness++; if(brightness>8) brightness=1;  setBrightness(brightness); drawFooter(); delay(40);}

        if(xx[0]>0 && xx[0]<100 && yy[0]>100 && yy[0]<164)
        { gameStart(); }

        if(xx[0]>104 && xx[0]<212 && yy[0]>100 && yy[0]<164)
        { selectFase=1; drawSprite(); delay(200); }
        
         if(finish==0)
         for(int i=0;i<4;i++)
         for(int j=0;j<4;j++)
          { 
            
            if(x>posX[j] && x<posX[j]+64 && y>posX[i] && y<posX[i]+64 )
            {
              
              if(j>0)
              if(finded==0)
              if(taken[i][j-1]==0){
              finded=1;   
              taken[i][j]=0;
              taken[i][j-1]=1;
              numbers[i][j-1]=numbers[i][j];
              numbers[i][j]=0;}

              if(j<3)
              if(finded==0)
              if(taken[i][j+1]==0){
                finded=1;  
              taken[i][j]=0;
              taken[i][j+1]=1;
              numbers[i][j+1]=numbers[i][j];
              numbers[i][j]=0;
             }

             if(i<3)
              if(finded==0)
              if(taken[i+1][j]==0){
                finded=1;  
              taken[i][j]=0;
              taken[i+1][j]=1;
              numbers[i+1][j]=numbers[i][j];
              numbers[i][j]=0;
             }

             if(i>0)
              if(finded==0)
              if(taken[i-1][j]==0){
                finded=1;  
              taken[i][j]=0;
              taken[i-1][j]=1;
              numbers[i-1][j]=numbers[i][j];
              numbers[i][j]=0;

              
             }

              if(finded==1){
                 moves++;
              }
             
             }
            }
            drawSprite(); 
            delay(180); 
          }}

    
    finded=0;
    checkFinish();  
    gameTime=String(rtc.getTime()); 
    drawHeader(); 
}

void drawSprite()
{

  sprite.setTextColor(TFT_WHITE,TFT_BLACK);
  sprite.fillSprite(sprback);
  sprite.setTextDatum(4);

  if(selectFase)
  {
  jpegDraw("/image1.jpg", jpegDrawCallback, false, 2, 2, 106, 106);
  jpegDraw("/image2.jpg", jpegDrawCallback, false, 112, 2, 106, 106);
  jpegDraw("/image3.jpg", jpegDrawCallback, false, 2, 112, 106, 106);
  jpegDraw("/image4.jpg", jpegDrawCallback, false, 112, 112, 106, 106);
  }else{
  if(finish==1)
  {
    sprite.fillRoundRect(0,0,280,280,8,0xE6AE);
    sprite.setTextColor(TFT_PURPLE,0xE6AE);
    
    sprite.drawString("Well Done!",140,20);
    sprite.drawString("Moves: "+String(moves),140,160);
    sprite.drawString("time: "+String(sec),140,190);
  }
  
  int cc=0;

  if(finish==0)
  for(int j=0;j<4;j++)
  for(int i=0;i<4;i++)
if(taken[j][i]==1){

sprite.pushImage(posX[i]+2,posY[j]+2,53,53,splitIMG[numbers[j][i]]);
if(help){
sprite.fillCircle(posX[i]+10,posY[j]+10,7,TFT_BLACK);
sprite.drawString(String(numbers[j][i]),posX[i]+10,posY[j]+10);
}
if(numbers[j][i]==numbersFinish[j][i])
sprite.fillCircle(posX[i]+4,posY[j]+4,1,TFT_RED);
cc++;
}
else{
sprite.fillRoundRect(posX[i]+2,posY[j]+2,53,53,1,TFT_BLACK);
cc++;
}}

sprite.pushSprite(left,top);
  }

 void checkFinish()
 {
  finish=1;
  for(int i=0;i<4;i++)
  for(int j=0;j<4;j++)
  if(finish==1)
  if(numbers[i][j]!=numbersFinish[i][j])
  finish=0;
  }

  void shufle()
  {
     for(int i=0;i<16;i++)
     numbersTaken[i]=numbersTakenS[i];

     for(int i=0;i<4;i++)
     for(int j=0;j<4;j++)
     taken[i][j]=1;
        
     for(int s=0;s<200;s++)
    {
     random1=random(0,16); 
     random2=random(0,16); 
     temp=numbersTaken[random1];
     numbersTaken[random1]=numbersTaken[random2];
     numbersTaken[random2]=temp;
     
     }
         
       int r=0;
         for(int i=0;i<4;i++)
         for(int j=0;j<4;j++) 
         {
          numbers[i][j]=numbersTaken[r];
         
          if(numbersTaken[r]==0)
          taken[i][j]=0;
           r++;
      }   
    }
   
    void drawHeader()
    {
    header.fillSprite(grays[10]);
    header.setTextDatum(0);

    header.setTextColor(grays[0],grays[10]);
    header.loadFont(midleFont);
    header.drawString("15 puzzle GAME",10,10);
    header.unloadFont();

    header.fillSmoothRoundRect(164,10,50,24,4,0xA000,grays[10]);
    header.fillSmoothRoundRect(190,13,20,18,2,grays[1],TFT_RED);

    header.fillSmoothRoundRect(10, 96, 96, 44, 4, grays[6]);
    header.fillSmoothRoundRect(116, 96, 96, 44, 4, grays[6]);
    header.fillSmoothRoundRect(164, 45, 50, 40, 4, TFT_ORANGE);


    header.loadFont(NotoSansBold15);
    header.setTextColor(grays[0],0xA000);
    header.drawString("SD",168,15);
    header.setTextColor(grays[0],grays[6]);
    
    header.unloadFont();

    

    header.loadFont(bigFont);
    header.setTextColor(grays[2],grays[10]);
    header.drawString(gameTime,10,45);
    header.unloadFont();

  

  
    header.setTextDatum(4);

    header.loadFont(smallFont);
    header.setTextColor(grays[1],grays[6]);
     header.drawString("NEW GAME",58,120);
     header.drawString("IMAGE",164,120);
     header.unloadFont();

      header.loadFont(tinyFont);
    header.setTextColor(TFT_BLACK,TFT_ORANGE);
    header.drawString("MOVES",188,58);
    header.drawString(String(moves),188,76);
    header.unloadFont();

    header.pushSprite(0,10);

    }
    void drawFooter()
    {
     footer.fillSprite(grays[10]);
     footer.fillSmoothRoundRect(160,10,50,50,3,grays[8],grays[10]);
     footer.fillSmoothRoundRect(10,10,50,50,3,grays[4],grays[10]);
     footer.fillSmoothRoundRect(12,12,46,46,3,grays[10],grays[4]);
     footer.drawRect(74,10,74,24,grays[4]);
     footer.drawRect(16,36,34,18,grays[4]);

     footer.fillSmoothRoundRect(165,36,40,18,3,0xA000,grays[8]);
     footer.fillSmoothRoundRect(169+(help*22),40,10,10,3,grays[1],0xA000);

    voltage=PMU.getBattVoltage();
    volbar=map(voltage,2800,4200,0,6);
     for(int i=0;i<volbar;i++)
     footer.fillRect(18+(i*5),38,3,14,TFT_DARKGREEN);

     for(int i=0;i<brightness;i++)
     footer.fillRect(78+(i*8),14,5,16,sprback);

    footer.loadFont(tinyFont);
    footer.setTextColor(grays[3],grays[10]);
    footer.drawString("BAT:",16,16);

    footer.setTextColor(grays[3],grays[8]);
    footer.drawString("HELP",170,16);

    footer.setTextColor(TFT_ORANGE,grays[10]);
    footer.drawString("BRIGHTNESS",74,44);
    //footer.drawString(String(moves),188,76);
    footer.unloadFont();

     footer.pushSprite(0,400);
    }
  
    void gameStart()
    {
     
      no=0;
      sec=0;
      moves=0;
      finish=0;
      selectFase=0;
 
    rtc.setTime(0, 0, 0, 1, 1, 2023,0);
    jpegDraw(chosen_image.c_str(), jpegDrawCallback, true, 0, 0, 212, 212);

    shufle();
    while(isSolvable(numbers)==0)
    {shufle();}
    
    drawSprite();
    drawHeader();
    drawFooter();
    
  }

   int getInvCount(int arr[])
{
    int inv_count = 0;
    for (int i = 0; i < N * N - 1; i++)
    {
        for (int j = i + 1; j < N * N; j++)
        {
            // count pairs(arr[i], arr[j]) such that
              // i < j but arr[i] > arr[j]
            if (arr[j] && arr[i] && arr[i] > arr[j])
                inv_count++;
        }
    }
    return inv_count;
}
 
// find Position of blank from bottom
int findXPosition(int puzzle[N][N])
{
    // start from bottom-right corner of matrix
    for (int i = N - 1; i >= 0; i--)
        for (int j = N - 1; j >= 0; j--)
            if (puzzle[i][j] == 0)
                return N - i;
}
 
// This function returns true if given
// instance of N*N - 1 puzzle is solvable
bool isSolvable(int puzzle[N][N])
{
    // Count inversions in given puzzle
    int invCount = getInvCount((int*)puzzle);
 
    // If grid is odd, return true if inversion
    // count is even.
    if (N & 1)
        return !(invCount & 1);
 
    else     // grid is even
    {
        int pos = findXPosition(puzzle);
        if (pos & 1)
            return !(invCount & 1);
        else
            return invCount & 1;
    }
}   

bool initSD()
{
  
    SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
    if (!SD.begin(BOARD_SD_CS)) {
        Serial.println("Card Mount Failed");
  
        return false;
    }
  
    return true;
}




