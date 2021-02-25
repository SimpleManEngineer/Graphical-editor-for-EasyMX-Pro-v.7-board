#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "inc/hw_memmap.h" //Memory mapping of the used device (maps all the pins ports etc. to specific numbers)
#include "driverlib/fpu.h" //Routines for manipulating the floating-point unit in the Cortex-M processor.
#include "driverlib/sysctl.h" //Driver for the system controller
#include "driverlib/rom.h"		//Macros to facilitate calling functions in the ROM
#include "driverlib/pin_map.h" //Mapping of peripherals to pins for all parts
#include "driverlib/uart.h" 	// Driver for the UART
#include "grlib/grlib.h"	//Prototypes for the low level primitives provided by the graphics library
#include "drivers/ili9341_240x320x262K.h" //Display driver for the MULTI-INNO TECHNOLOGY
										// MI0283QT-9 TFT display with an ILI9341 controller.
#include "utils/uartstdio.h"		//Prototypes for the UART console functions.
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/systick.h"
#include "drivers/touch.h"

#define GPIO_PINS_ALL GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7

volatile uint32_t g_ui32SysTickCount;

void SysTickIntHandler(void)
{
    g_ui32SysTickCount++;
}

inline bool IsInCircle(int32_t X,int32_t Y,int32_t cX,int32_t cY,int32_t radius){
	return ( (X-cX)*(X-cX) + (Y-cY)*(Y-cY) < (radius*radius) );
}

int_fast32_t i32CenterX, i32CenterY;
//int i32CenterX_temp = 100;
//int i32CenterY_temp = 100;
tContext sContext;
tRectangle sRect;

unsigned long color[15]={ClrRed,ClrBlack,ClrGreen,ClrBlue,ClrPurple,ClrYellow,ClrOrange,ClrBrown,ClrPink,ClrWhite,ClrGray,ClrViolet,ClrGold,ClrCyan,ClrSilver};

int colorPos[15][2];
int fontPos[5][2];
int shapePos[4][2];
int curr_color,curr_font,curr_shape;

void drawRect(int x,int y,unsigned long color,int fontWidth){
    sRect.i16XMin = x;
    sRect.i16YMin = y;
    sRect.i16XMax = x+fontWidth;
    sRect.i16YMax = y+fontWidth;
    GrContextForegroundSet(&sContext, color);
    GrRectFill(&sContext, &sRect);
}

void drawTriangle(int x, int min_y,int max_y, unsigned long color,int fontWidth){
    float xOffset=0.5;
    int y=min_y;
    while(y<=max_y){
        sRect.i16XMin = x-xOffset;
        sRect.i16YMin = y;
        sRect.i16XMax = x+xOffset;
        sRect.i16YMax = ++y;
        GrContextForegroundSet(&sContext, color);
        GrRectFill(&sContext, &sRect);
        xOffset+=0.5;
    }
}

void drawCircle(int x, int y, unsigned long color, int fontWidth){
    GrContextForegroundSet(&sContext, color);
    GrCircleFill(&sContext,x,y,fontWidth);
}

void RectInRect(int x,int y,int n){
    int counter=0,x_temp=x,y_temp=y;

    drawRect(x,y,ClrBrown,31);

    for(int i=0;i<n;i++){ 
        if(i==4) break;
        drawRect(x_temp+3,y_temp+5,ClrGold,6);
        x_temp+=15;
        counter++;

        if(counter==2){ x_temp=x; y_temp+=15; counter=0;}
    }

    if(n==5) drawRect(x+11,y+12,ClrGold,6);
}

void drawLineIcon(int x,int y_min,int y_max){
    for(int i=y_min;i<=y_max;i++) drawRect(x,i,ClrBlack,4);
}

void initScreen(){
    int x_mid=160;
    for(int i=0;i<240;i+=5) drawRect(x_mid,i,ClrBlack,5);
    for(int i=x_mid;i<320;i+=5) drawRect(i,0,ClrBlack,5);

    //colors
    int x=x_mid+5,y=5;
    for(int i=0;i<15;i++){
        drawRect(x,y,color[i],31);
        colorPos[i][0]=x;
        colorPos[i][1]=y;
        if(x+40>320){ x=x_mid+5; y+=31; }
        else x+=31;
    }

    curr_color=0;

    for(int i=x_mid;i<320;i+=5) drawRect(i,y,ClrBlack,5);

    //fonts
    y+=5;
    x=x_mid+5;
    for(int i=0;i<5;i++){
        RectInRect(x,y,i+1);
        fontPos[i][0]=x;
        fontPos[i][1]=y;
        x+=31;
    }

    curr_font=3;

    for(int i=x_mid;i<320;i+=5) drawRect(i,y+31,ClrBlack,5);

    //shapes
    y+=31;
    x=x_mid+5;
    drawRect(x,y+5,ClrOrange,31);
    drawCircle(x+15,y+20,ClrBlack,10);
    shapePos[0][0]=x;
    shapePos[0][1]=y+5;

    drawRect(x+=31,y+5,ClrOrange,31);
    drawLineIcon(x+15,y+8,y+28);
    shapePos[1][0]=x;
    shapePos[1][1]=y+5;

    drawRect(x+=31,y+5,ClrOrange,31);
    drawRect(x+5,y+13,ClrBlack,15);
    shapePos[2][0]=x;
    shapePos[2][1]=y+5;

    drawRect(x+=31,y+5,ClrOrange,31);
    drawTriangle(x+13,y+12,y+27,ClrBlack,6);
    shapePos[3][0]=x;
    shapePos[3][1]=y+5;

    curr_shape=1;

    for(int i=x_mid;i<320;i+=5) drawRect(i,y+35,ClrBlack,5);
    y+=23;
}

int ifColor(){
    int i=0;
    for(i=0;i<15;i++){
        if(
            (i32CenterX>=colorPos[i][0] && i32CenterX<=colorPos[i][0]+31) &&
            (i32CenterY>=colorPos[i][1] && i32CenterY<=colorPos[i][1]+31)
        ) return i;
    }
    return curr_color;
}

int ifFont(){
    int i=0;
    for(i=0;i<5;i++){
        if(
            (i32CenterX>=fontPos[i][0] && i32CenterX<=fontPos[i][0]+31) &&
            (i32CenterY>=fontPos[i][1] && i32CenterY<=fontPos[i][1]+31)
        ) return i+1;
    }
    return curr_font;
}

int ifShape(){
    int i=0;
    for(i=0;i<4;i++){
        if(
            (i32CenterX>=shapePos[i][0] && i32CenterX<=shapePos[i][0]+31) &&
            (i32CenterY>=shapePos[i][1] && i32CenterY<=shapePos[i][1]+31)
        ) return i;
    }
    return curr_shape;
}
//drawRect(i32CenterX,i32CenterY,color[curr_color],curr_font);

void drawShape(int x,int y){
    switch(curr_shape){
        case 0:
            drawCircle(i32CenterX,i32CenterY,color[curr_color],30);
        break;

        case 1:
            drawRect(i32CenterX,i32CenterY,color[curr_color],curr_font);
        break;

        case 2:
            drawRect(i32CenterX,i32CenterY,color[curr_color],30);
        break;
        
        case 3:
            drawTriangle(i32CenterX,i32CenterY,i32CenterY+20,color[curr_color],30);
        break;
    }
}

int32_t TouchCallback(uint32_t ulMessage, int32_t lX, int32_t lY)
{
    //ulMessage=2->pierwsze dotkniecie,3->przetrzymanie,4->puszczenie

    i32CenterX = lX;
	i32CenterY = lY;

    if(i32CenterX>160){ curr_color=ifColor(); curr_font=ifFont(); curr_shape=ifShape(); }
    else drawShape(i32CenterX,i32CenterY);

	return 0;
}

int main(void)
{
    ROM_FPULazyStackingEnable();

    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

    ILI9341_240x320x262K_Init();

    GrContextInit(&sContext, &g_sILI9341_240x320x262K);

    TouchScreenInit();
    TouchScreenCalibrate(&sContext);
    TouchScreenCallbackSet(TouchCallback);

    initScreen();

    while(1)
    {

    }
}
