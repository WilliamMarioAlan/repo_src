/**
  ******************************************************************************
  * @file    oled.h
  * @author  铁头山羊
  * @version V 1.0.0
  * @date    2022年9月4日
  * @brief   OLED显示器驱动程序
  ******************************************************************************
  */
#ifndef _OLED_H_
#define _OLED_H_

#include "stm32f10x.h"
#include "oled_font.h"

#define OLED_SLAVE_ADDR 0x78

extern const Font_TypeDef default_font;

//
// @颜色
//
#define OLED_COLOR_TRANSPARENT 0x00 // 透明
#define OLED_COLOR_WHITE       0x01 // 白色
#define OLED_COLOR_BLACK       0x02 // 黑色

//
// @画笔
//
#define PEN_COLOR_TRANSPARENT OLED_COLOR_TRANSPARENT // 透明画笔
#define PEN_COLOR_WHITE       OLED_COLOR_WHITE // 白色画笔
#define PEN_COLOR_BLACK       OLED_COLOR_BLACK // 黑色画笔

//
// @画刷
//
#define BRUSH_TRANSPARENT OLED_COLOR_TRANSPARENT // 透明画刷
#define BRUSH_WHITE       OLED_COLOR_WHITE       // 白色画刷
#define BRUSH_BLACK       OLED_COLOR_BLACK       // 黑色画刷

typedef struct
{
	int (*i2c_write_cb)(uint8_t addr, const uint8_t *pdata, uint16_t size); // i2c写数据回调函数
}OLED_InitTypeDef;

typedef struct 
{
	int (*i2c_write_cb)(uint8_t addr, const uint8_t *pdata, uint16_t size); // i2c写数据回调函数
	
	uint8_t *pBuffer; // 作画区
	const Font_TypeDef *Font;
	uint8_t PenColor;
	uint8_t PenWidth;
	uint8_t Brush;
	int16_t CursorX;
	int16_t CursorY;
	uint16_t RefreshProgress;
	
	int16_t ClipRegionX;
	int16_t ClipRegionY;
	uint16_t ClipRegionWidth;
	uint16_t ClipRegionHeight;
	
	int16_t TextRegionX;
	int16_t TextRegionY;
	uint16_t TextRegionWidth;
	uint16_t TextRegionHeight;
	
}OLED_TypeDef;

//
// @基本操作
//

// @屏幕初始化
int OLED_Init(OLED_TypeDef *OLED, OLED_InitTypeDef *OLED_InitStruct);
// @清空屏幕
void OLED_Clear(OLED_TypeDef *OLED);
// @获取屏幕宽度
uint16_t OLED_GetScreenWidth(OLED_TypeDef *OLED);
// @获取屏幕高度
uint16_t OLED_GetScreenHeight(OLED_TypeDef *OLED);
// @发送缓冲区的内容到屏幕
int OLED_SendBuffer(OLED_TypeDef *OLED);
// @开始发送缓冲区的内容到屏幕（分段发送）
int OLED_StartSendBuffer(OLED_TypeDef *OLED);
// @继续发送缓冲区的内容到屏幕（分段发送）
int OLED_EndSendBuffer(OLED_TypeDef *OLED, uint8_t *pMoreOut);


//
// @光标
//

// @设置光标位置
void OLED_SetCursor(OLED_TypeDef *OLED, int16_t X, int16_t Y);
// @设置光标的X坐标
void OLED_SetCursorX(OLED_TypeDef *OLED, int16_t X);
// @设置光标的Y坐标
void OLED_SetCursorY(OLED_TypeDef *OLED, int16_t Y);
// @移动光标
void OLED_MoveCursor(OLED_TypeDef *OLED, int16_t dX, int16_t dY);
// @沿X轴方向移动光标
void OLED_MoveCursorX(OLED_TypeDef *OLED, int16_t dX);
// @沿Y轴方向移动光标
void OLED_MoveCursorY(OLED_TypeDef *OLED, int16_t dY);
// @获取光标当前位置
void OLED_GetCursor(OLED_TypeDef *OLED, int16_t *pXOut, int16_t *pYOut);
// @获取光标X坐标
int16_t OLED_GetCursorX(OLED_TypeDef *OLED);
// @获取光标Y坐标
int16_t OLED_GetCursorY(OLED_TypeDef *OLED);


//
// @画笔和画刷
//

// @设置画笔
void OLED_SetPen(OLED_TypeDef *OLED, uint8_t Pen_Color, uint8_t Width);
// @设置画刷
void OLED_SetBrush(OLED_TypeDef *OLED, uint8_t Brush_Color);


//
// @绘图
//

// @画点
void OLED_DrawDot(OLED_TypeDef *OLED);
// @画线
void OLED_DrawLine(OLED_TypeDef *OLED, int16_t X, int16_t Y);
// @画线
void OLED_LineTo(OLED_TypeDef *OLED, int16_t X, int16_t Y);
// @画圆
void OLED_DrawCircle(OLED_TypeDef *OLED, uint16_t Radius);
// @画矩形
void OLED_DrawRect(OLED_TypeDef *OLED, uint16_t Width, uint16_t Height);
// @画位图
void OLED_DrawBitmap(OLED_TypeDef *OLED, uint16_t Width, uint16_t Height, const uint8_t *pBitmap);


//
// @字符串和字体
//

// @显示字符串
void OLED_DrawString(OLED_TypeDef *OLED, const char *Str);
// @格式化打印字符串
void OLED_Printf(OLED_TypeDef *OLED, const char *Format, ...);
// @开启文本区域
void OLED_StartTextRegion(OLED_TypeDef *OLED, int16_t X, int16_t Y, uint16_t Width, uint16_t Height);
// @停止文本区域
void OLED_StopTextRegion(OLED_TypeDef *OLED);
// @设置字体
void OLED_SetFont(OLED_TypeDef *OLED, const Font_TypeDef *Font);
// @获取当前字体下的字符串宽度
uint16_t OLED_GetStrWidth(OLED_TypeDef *OLED, const char *Str);
// @获取字体高度
uint16_t OLED_GetFontHeight(OLED_TypeDef *OLED);

//
// @剪切区域
//

// @设置剪切区域
void OLED_StartClipRegion(OLED_TypeDef *OLED, int16_t X, int16_t Y, uint16_t Width, uint16_t Height);
// @停止剪切区域
void OLED_StopClipRegion(OLED_TypeDef *OLED);

#endif
