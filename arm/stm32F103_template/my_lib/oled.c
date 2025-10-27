/**
  ******************************************************************************
  * @file    oled.c
  * @author  铁头山羊
  * @version V 1.0.0
  * @date    2022年9月4日
  * @brief   OLED显示器驱动程序
  ******************************************************************************
  */
	
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "oled.h"
#include "oled_default_font.h"
#include <math.h>

// 定义屏幕尺寸
#define OLED_SCREEN_COLS 128 
#define OLED_SCREEN_ROWS  64  
#define OLED_SCREEN_PAGES 8 

// SSD1306命令
#define SSD1306_CTRL_COMMAND           0x80  // Continuation bit=1, D/C=0; 1000 0000
#define SSD1306_CTRL_COMMAND_STREAM    0x00  // Continuation bit=0, D/C=0; 0000 0000
#define SSD1306_CTRL_DATA              0xc0  // Continuation bit=1, D/C=1; 1100 0000
#define SSD1306_CTRL_DATA_STREAM       0x40  // Continuation bit=0, D/C=1; 0100 0000

typedef struct
{
	int16_t X;
	int16_t Y;
	uint16_t Width;
	uint16_t Height;
}Rect; /* 矩形 */

static int OLED_SendCommand(OLED_TypeDef *OLED, const uint8_t Cmd, const uint8_t *Arg, uint16_t Size);
static int OLED_SendData(OLED_TypeDef *OLED, uint8_t *pData, uint16_t Size);
static void DrawCircleFrame(OLED_TypeDef *OLED, int16_t X, int16_t Y, uint16_t Radius);
static void FillCircle(OLED_TypeDef *OLED, int16_t X, int16_t Y, uint16_t Radius);
static void DrawRectFrame(OLED_TypeDef *OLED, int16_t X, int16_t Y, uint16_t Width, uint16_t Height);
static void FillRect(OLED_TypeDef *OLED, int16_t X, int16_t Y, uint16_t Width, uint16_t Height);
static int16_t unicode_2_glyph_idx(OLED_TypeDef *OLED, uint32_t Unicode);
static void DrawCharator(OLED_TypeDef *OLED, uint32_t Unicode);
static void BrushDot(OLED_TypeDef *OLED, int16_t x, int16_t y);
static void PenDot(OLED_TypeDef *OLED, int16_t x, int16_t y);
static void DrawBitmapEx(OLED_TypeDef *OLED,int16_t X, int16_t Y, uint16_t Width, uint16_t Height, const uint8_t *pBitmap);
static uint16_t GetGlyphWidth(OLED_TypeDef *OLED, uint32_t Unicode);

// 
// @简介：OLED初始化
//
// @参数：OLED - OLED显示器的句柄
//
// @返回值：0 - 成功，
//         -1 - 数据发送失败
//         -2 - 缓冲区分配失败
//
int OLED_Init(OLED_TypeDef *OLED, OLED_InitTypeDef *OLED_InitStruct)
{
	OLED->i2c_write_cb = OLED_InitStruct->i2c_write_cb;
	
	// 给缓冲区分配空间
	OLED->pBuffer = (uint8_t *)malloc(OLED_SCREEN_COLS * OLED_SCREEN_PAGES * sizeof(uint8_t) + 1);
	
	if(OLED->pBuffer == 0)
	{
		return -2;
	}
	
	OLED->pBuffer++;
	
	
	// 给缓冲区所有字节赋初值0
	memset(OLED->pBuffer, 0, OLED_SCREEN_COLS * OLED_SCREEN_PAGES * sizeof(uint8_t));
	
	uint8_t arg;
	
	if(OLED_SendCommand(OLED, 0xae, 0, 0)) return -1; /* display off*/
	arg = 0x80;
	if(OLED_SendCommand(OLED, 0xd5, &arg, 1)) return -1; /* clock divide ratio (0x00=1) and oscillator frequency (0x8) */
	arg = 0x3f;
	if(OLED_SendCommand(OLED, 0xa8, &arg, 1)) return -1; /* multiplex ratio */
	arg = 0x00;
	if(OLED_SendCommand(OLED, 0xd3, &arg, 1)) return -1; /* vertical shift */
	
	if(OLED_SendCommand(OLED, 0x40, 0, 0)) return -1; /* set display start line to 0 */
	arg = 0x14;
	if(OLED_SendCommand(OLED, 0x8d, &arg, 1)) return -1; /* [2] charge pump setting (p62): 0x014 enable, 0x010 disable, SSD1306 only, should be removed for SH1106 */
	arg = 0x00;
	if(OLED_SendCommand(OLED, 0x20, &arg, 1)) return -1; /* horizontal addressing mode */
	
	if(OLED_SendCommand(OLED, 0xa1, 0, 0)) return -1; /* segment remap a0/a1 */
	if(OLED_SendCommand(OLED, 0xc8, 0, 0)) return -1; /* c0: scan dir normal, c8: reverse */
	
	arg = 0x12;
	if(OLED_SendCommand(OLED, 0xda, &arg, 1)) return -1; /* com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5) */
	arg = 0xcf;
	if(OLED_SendCommand(OLED, 0x81, &arg, 1)) return -1; /* [2] set contrast control */
	arg = 0xf1;
	if(OLED_SendCommand(OLED, 0xd9, &arg, 1)) return -1; /* [2] pre-charge period 0x022/f1*/
	arg = 0x20;
	if(OLED_SendCommand(OLED, 0xdb, &arg, 1)) return -1; /* vcomh deselect level */
	
	// if vcomh is 0, then this will give the biggest range for contrast control issue #98
  // restored the old values for the noname constructor, because vcomh=0 will not work for all OLEDs, #116
	if(OLED_SendCommand(OLED, 0x2e, 0, 0)) return -1; /* Deactivate scroll */
	if(OLED_SendCommand(OLED, 0xa4, 0, 0)) return -1; /* output ram to display */
	if(OLED_SendCommand(OLED, 0xa6, 0, 0)) return -1; /* none inverted normal display mode */
	
	if(OLED_SendCommand(OLED, 0xaf, 0, 0)) return -1; /* display on*/
	
	OLED->PenWidth = 1; // 默认线宽1
	OLED->PenColor = PEN_COLOR_WHITE; // 默认白色画笔
	OLED->Brush = BRUSH_TRANSPARENT; // 默认白色画刷
	
	OLED->CursorX = 0;
	OLED->CursorY = 0; // 光标默认在屏幕的左上角
	
	OLED->RefreshProgress = 0;
	
	OLED->TextRegionX = 0; // 默认关闭文本框功能
	OLED->TextRegionY = 0;
	OLED->TextRegionWidth = 0;
	OLED->TextRegionHeight = 0;
	
	OLED->Font = &default_font; // 使用默认字体，8*8点阵字体
	
	return 0;
}

//
// @简介：设置剪切区域
// @参数：OLED - OLED显示器的句柄
// @参数：X - 剪切区域左上角的横坐标
// @参数：Y - 剪切区域左上角的纵坐标
// @参数：Wdith - 剪切区域宽度
// @参数：Height - 剪切区域高度
//
void OLED_StartClipRegion(OLED_TypeDef *OLED, int16_t X, int16_t Y, uint16_t Width, uint16_t Height)
{
	OLED->ClipRegionX = X;
	OLED->ClipRegionY = Y;
	OLED->ClipRegionWidth = Width;
	OLED->ClipRegionHeight = Height;
}

//
// @简介：关闭剪切区域
// @参数：OLED - OLED显示器的句柄
//
void OLED_StopClipRegion(OLED_TypeDef *OLED)
{
	OLED->ClipRegionX = 0;
	OLED->ClipRegionY = 0;
	OLED->ClipRegionWidth = 0;
	OLED->ClipRegionHeight = 0;
}

//
// @简介：获取屏幕的宽度
// @返回值：屏幕宽度，单位：像素
//
uint16_t OLED_GetScreenWidth(OLED_TypeDef *OLED)
{
	return OLED_SCREEN_COLS;
}

//
// @简介：获取屏幕高度
// @返回值：屏幕高度，单位：像素
//
uint16_t OLED_GetScreenHeight(OLED_TypeDef *OLED)
{
	return OLED_SCREEN_ROWS;
}

//
// @简介：清空缓冲区内容
// @参数：OLED - 显示器的句柄
//
void OLED_Clear(OLED_TypeDef *OLED)
{
	memset(OLED->pBuffer, 0, OLED_SCREEN_COLS * OLED_SCREEN_PAGES);
}

// 
// @简介：设置字体
// @参数：OLED - 显示器的句柄
// @参数：Font - 字体，如果要使用默认字体，可填写&default_font
//
void OLED_SetFont(OLED_TypeDef *OLED, const Font_TypeDef *Font)
{
	OLED->Font = Font;
}

//
// @简介：设置画笔的颜色和宽度
// @参数：OLED - 显示器的句柄
// @参数：Pen_Color - 画笔颜色
//             PEN_COLOR_TRANSPARENT - 透明
//             PEN_COLOR_WHITE - 白色
//             PEN_COLOR_BLACK - 黑色
// @参数：Width - 画笔宽度
// 
void OLED_SetPen(OLED_TypeDef *OLED, uint8_t Pen_Color, uint8_t Width)
{
	OLED->PenColor = Pen_Color;
	OLED->PenWidth = Width;
}

//
// @简介：设置画刷
// @参数：OLED - 显示器的句柄
// @参数：Brush_Color - 画刷颜色
//             BRUSH_TRANSPARENT - 透明
//             BRUSH_WHITE - 白色
//             BRUSH_BLACK - 黑色
// 
void OLED_SetBrush(OLED_TypeDef *OLED, uint8_t Brush_Color)
{
	OLED->Brush = Brush_Color;
}

//
// @简介：将光标设置到坐标点（X，Y）处
// @参数：OLED - 显示器的句柄
// @参数：X - 光标横坐标
// @参数：Y - 光标纵坐标
// 
void OLED_SetCursor(OLED_TypeDef *OLED, int16_t X, int16_t Y)
{
	OLED->CursorX = X;
	OLED->CursorY = Y;
}

//
// @简介：将光标的横坐标设置到X处
// @参数：OLED - 显示器的句柄
// @参数：X - 光标横坐标
// 
void OLED_SetCursorX(OLED_TypeDef *OLED, int16_t X)
{
	OLED->CursorX = X;
}

//
// @简介：将光标的纵坐标设置到Y处
// @参数：OLED - 显示器的句柄
// @参数：Y - 光标纵坐标
// 
void OLED_SetCursorY(OLED_TypeDef *OLED, int16_t Y)
{
	OLED->CursorY = Y;
}

//
// @简介：移动光标
// @参数：OLED - 显示器的句柄
// @参数：dX - 横向移动的距离
// @参数：dY - 纵向移动的距离
// 
void OLED_MoveCursor(OLED_TypeDef *OLED, int16_t dX, int16_t dY)
{
	OLED->CursorX += dX;
	OLED->CursorY += dY;
}

//
// @简介：横向移动光标
// @参数：OLED - 显示器的句柄
// @参数：dX - 横向移动的距离
// 
void OLED_MoveCursorX(OLED_TypeDef *OLED, int16_t dX)
{
	OLED->CursorX += dX;
}

//
// @简介：纵向移动光标
// @参数：OLED - 显示器的句柄
// @参数：dY - 纵向移动的距离
// 
void OLED_MoveCursorY(OLED_TypeDef *OLED, int16_t dY)
{
	OLED->CursorY += dY;
}

//
// 简介：获取光标的当前位置
// @参数：OLED - 显示器的句柄
// @参数：pXOut - 输出参数，用于接收光标的横坐标
// @参数：pYOut - 输出参数，用于接收光标的纵坐标
// 
void OLED_GetCursor(OLED_TypeDef *OLED, int16_t *pXOut, int16_t *pYOut)
{
	*pXOut = OLED->CursorX;
	*pYOut = OLED->CursorY;
}

//
// 简介：获取光标的横坐标
// @参数：OLED - 显示器的句柄
// @返回值：光标的横坐标值
// 
int16_t OLED_GetCursorX(OLED_TypeDef *OLED)
{
	return OLED->CursorX;
}

//
// 简介：获取光标的纵坐标
// @参数：OLED - 显示器的句柄
// @返回值：光标的纵坐标值
// 
int16_t OLED_GetCursorY(OLED_TypeDef *OLED)
{
	return OLED->CursorY;
}

#define min(x1,x2) x1>x2?x2:x1
#define max(x1,x2) x1>x2?x1:x2

static Rect GetOverlappedRect(Rect rect1, Rect rect2)
{
	Rect ret = {0,0,0,0};
	
	int16_t xl = max(rect1.X,rect2.X);
	int16_t xr = min(rect1.X + rect1.Width, rect2.X + rect2.Width);
	
	int16_t yt = max(rect1.Y,rect2.Y);
	int16_t yb = min(rect1.Y + rect1.Height, rect2.Y + rect2.Height);
	
	if(xl<xr&&yt<yb)
	{
		ret.X = xl;
		ret.Y = yt;
		ret.Width = xr - xl;
		ret.Height = yb - yt;
	}
	
	return ret;
}

static uint16_t GetGlyphWidth(OLED_TypeDef *OLED, uint32_t Unicode)
{
	if(OLED->Font == NULL) return 0; // 未设置字体
	
	int16_t idx = unicode_2_glyph_idx(OLED, Unicode);
	
	if(idx < 0) return 0; // 未找到对应的字形
	
	return OLED->Font->Glyphs[idx].Dwx0;
}

static void DrawCharator(OLED_TypeDef *OLED, uint32_t Unicode)
{
	if(OLED->Font == NULL) return; // 未设置字体
	
	int16_t idx = unicode_2_glyph_idx(OLED, Unicode);
	
	const Glyph_TypeDef *pGlyph = 0;
	
	if(idx >=0)
	{
		pGlyph = &OLED->Font->Glyphs[idx];
	}
	
	int16_t clipRegionXCpy, clipRegionYCpy, clipRegionWidthCpy, clipRegionHeightCpy;
	
	// 如果启用了文本框区域
	if(OLED->TextRegionWidth!=0 && OLED->TextRegionHeight!=0)
	{
		// 备份剪切区域
		clipRegionXCpy = OLED->ClipRegionX;
		clipRegionYCpy = OLED->ClipRegionY;
		clipRegionWidthCpy = OLED->ClipRegionWidth;
		clipRegionHeightCpy = OLED->ClipRegionHeight;
		
		// 将剪切区域设置到与文本框区域相交处
		
		if(OLED->ClipRegionWidth != 0 && OLED->ClipRegionHeight != 0)
		{
			Rect rect1 = {OLED->TextRegionX, OLED->TextRegionY, OLED->TextRegionWidth, OLED->TextRegionHeight};
			Rect rect2 = {OLED->ClipRegionX, OLED->ClipRegionY, OLED->ClipRegionWidth, OLED->ClipRegionHeight};
			Rect overlapped = GetOverlappedRect(rect1, rect2);
			
			OLED->ClipRegionX = overlapped.X;
			OLED->ClipRegionY = overlapped.Y;
			OLED->ClipRegionWidth = overlapped.Width;
			OLED->ClipRegionHeight = overlapped.Height;
		}
		else
		{
			OLED->ClipRegionX = OLED->TextRegionX;
			OLED->ClipRegionY = OLED->TextRegionY;
			OLED->ClipRegionWidth = OLED->TextRegionWidth;
			OLED->ClipRegionHeight = OLED->TextRegionHeight;
		}
		
		// 如果光标在文本框之外
		if(OLED->CursorX<OLED->TextRegionX || OLED->CursorX>=OLED->TextRegionX+OLED->TextRegionWidth)
		{
			OLED->CursorX = OLED->TextRegionX; // 让光标回到起始点
		}
		
		if(pGlyph != 0)
		{
			// 如果下一个字符的宽度超过了文本框
			if(OLED->CursorX+pGlyph->Dwx0 >= OLED->TextRegionX+OLED->TextRegionWidth)
			{
				OLED->CursorX = OLED->TextRegionX; // 让光标回到起始点
				OLED->CursorY = OLED->CursorY + OLED->Font->FBBy + OLED->Font->FBBYoff;
			}
		}
		
		// 如果为\r（回车）
		if(Unicode == '\r')
		{
			OLED->CursorX = OLED->TextRegionX;
		}
		else if(Unicode == '\n')
		{
			OLED->CursorY = OLED->CursorY + OLED->Font->FBBy + OLED->Font->FBBYoff;
		}
	}
	
	if(pGlyph != 0)
	{
		// 绘制背景
		FillRect(OLED, OLED->CursorX, OLED->CursorY - OLED->Font->FBBYoff - OLED->Font->FBBy, pGlyph->Dwx0, OLED->Font->FBBy);
		
		// 绘制字形
		DrawBitmapEx(OLED, OLED->CursorX + pGlyph->BBxoff0x, OLED->CursorY - pGlyph->BByoff0y - pGlyph->BBh, pGlyph->BBw, pGlyph->BBh, pGlyph->Bitmap);
	}
	
	// 如果启用了文本框区域
	if(OLED->TextRegionWidth!=0 && OLED->TextRegionHeight!=0)
	{
		// 恢复剪切区域
		OLED->ClipRegionX = clipRegionXCpy;
		OLED->ClipRegionY = clipRegionYCpy;
		OLED->ClipRegionWidth = clipRegionWidthCpy;
		OLED->ClipRegionHeight = clipRegionHeightCpy;
	}
	if(pGlyph != 0){
		OLED->CursorX += pGlyph->Dwx0;
	}
}

//
// 简介：从光标处开始绘制字符串
// @参数：OLED - 显示器的句柄
// @参数：Str - 要绘制的字符串
// 
void OLED_DrawString(OLED_TypeDef *OLED, const char *Str)
{
	// 注意这里使用的是UTF-8编码，因此先将其解析成Unicode
	uint16_t i;
	uint32_t unicode;
	uint8_t first, second, third, forth;
	
	i=0;
	for(;;)
	{
		first = Str[i++];
		if(first == '\0') break; 
		
		if ((first & (1 << 7)) == 0) // 1字节
		{
			unicode = first;
			DrawCharator(OLED, unicode);
		}
		else if ((first & (1 << 7 | 1 << 6 | 1 << 5)) == (1 << 7 | 1 << 6)) // 2字
		{
			first = first & 0x1f;
			
			second = Str[i++];
			if(second == '\0' || ((second & (1 << 7 | 1 << 6)) != 0x80)) break;
			second = second & 0x3f;
			
			unicode = ((uint32_t)first << 6) | second;
			DrawCharator(OLED, unicode);
		}
		else if ((first & (1 << 7 | 1 << 6 | 1 << 5 | 1 << 4)) == (1 << 7 | 1 << 6 | 1 << 5)) // 3字节
		{
			first = first & 0x0f;
			
			second = Str[i++];
			if(second == '\0' || ((second & (1 << 7 | 1 << 6)) != 0x80)) break;
			second = second & 0x3f;
			
			third = Str[i++];
			if(third == '\0' || ((third & (1 << 7 | 1 << 6)) != 0x80)) break;
			third = third & 0x3f;
			
			unicode = ((uint32_t)first << 12) | ((uint32_t)second << 6) | third;
			DrawCharator(OLED, unicode);
		}
		else if ((first & (1 << 7 | 1 << 6 | 1 << 5 | 1 << 4 | 1 << 3)) == (1 << 7 | 1 << 6 | 1 << 5 | 1 << 4)) // 4字节
		{
			first = first & 0x07;
			
			second = Str[i++];
			if(second == '\0' || ((second & (1 << 7 | 1 << 6)) != 0x80)) break;
			second = second & 0x3f;
			
			third = Str[i++];
			if(third == '\0' || ((third & (1 << 7 | 1 << 6)) != 0x80)) break;
			third = third & 0x3f;
			
			forth = Str[i++];
			if(forth == '\0' || ((forth & (1 << 7 | 1 << 6)) != 0x80)) break;
			forth = forth & 0x3f;
			
			unicode = ((uint32_t)first << 18) | ((uint32_t)second << 12) | ((uint32_t)second << 6) | forth;
			
			DrawCharator(OLED, unicode);
		}
	}
}


//
// @简介：设置文本框区域，同时将光标移动到文本框的第一个字符处
// @参数：OLED - 显示器的句柄
// @参数：X - 文本框左上角的横坐标
// @参数：Y - 文本框左上角的纵坐标
// @参数：Width - 文本框的宽度
// @参数：Height - 文本框的高度
//
void OLED_StartTextRegion(OLED_TypeDef *OLED, int16_t X, int16_t Y, uint16_t Width, uint16_t Height)
{
	OLED->TextRegionX = X;
	OLED->TextRegionY = Y;
	OLED->TextRegionWidth = Width;
	OLED->TextRegionHeight = Height;
	
	OLED->CursorX = X;
	OLED->CursorY = Y + OLED_GetFontHeight(OLED);
}

//
// @简介：取消文本框
// @参数：OLED - 显示器的句柄
//
void OLED_StopTextRegion(OLED_TypeDef *OLED)
{
	OLED->TextRegionX = 0;
	OLED->TextRegionY = 0;
	OLED->TextRegionWidth = 0;
	OLED->TextRegionHeight = 0;
}

//
// 简介：绘制格式化字符串（最大64字节）
// @参数：OLED - 显示器的句柄
// @参数：Format - 格式
// @参数：... - 可变参数
// 
void OLED_Printf(OLED_TypeDef *OLED, const char *Format, ...)
{
	char format_buffer[64];
	
	va_list argptr;
	__va_start(argptr, Format);
	vsprintf(format_buffer, Format, argptr);
	__va_end(argptr);
	OLED_DrawString(OLED, format_buffer);
}

//
// @简介：获取当前字体下字符串所占的宽度
// @参数：OLED - 显示器的句柄
// @参数：Str    - 字符串
// @返回值：宽度，单位：像素
// 
uint16_t OLED_GetStrWidth(OLED_TypeDef *OLED, const char *Str)
{
		// 注意这里使用的是UTF-8编码，因此先将其解析成Unicode
	uint16_t i;
	uint32_t unicode;
	uint8_t first, second, third, forth;
	uint16_t ret = 0;
	
	i=0;
	for(;;)
	{
		first = Str[i++];
		if(first == '\0') break; 
		
		if ((first & (1 << 7)) == 0) // 1字节
		{
			unicode = first;
			ret += GetGlyphWidth(OLED, unicode);
		}
		else if ((first & (1 << 7 | 1 << 6 | 1 << 5)) == (1 << 7 | 1 << 6)) // 2字
		{
			first = first & 0x1f;
			
			second = Str[i++];
			if(second == '\0' || ((second & (1 << 7 | 1 << 6)) != 0x80)) break;
			second = second & 0x3f;
			
			unicode = ((uint32_t)first << 6) | second;
			ret += GetGlyphWidth(OLED, unicode);
		}
		else if ((first & (1 << 7 | 1 << 6 | 1 << 5 | 1 << 4)) == (1 << 7 | 1 << 6 | 1 << 5)) // 3字节
		{
			first = first & 0x0f;
			
			second = Str[i++];
			if(second == '\0' || ((second & (1 << 7 | 1 << 6)) != 0x80)) break;
			second = second & 0x3f;
			
			third = Str[i++];
			if(third == '\0' || ((third & (1 << 7 | 1 << 6)) != 0x80)) break;
			third = third & 0x3f;
			
			unicode = ((uint32_t)first << 12) | ((uint32_t)second << 6) | third;
			ret += GetGlyphWidth(OLED, unicode);
		}
		else if ((first & (1 << 7 | 1 << 6 | 1 << 5 | 1 << 4 | 1 << 3)) == (1 << 7 | 1 << 6 | 1 << 5 | 1 << 4)) // 4字节
		{
			first = first & 0x07;
			
			second = Str[i++];
			if(second == '\0' || ((second & (1 << 7 | 1 << 6)) != 0x80)) break;
			second = second & 0x3f;
			
			third = Str[i++];
			if(third == '\0' || ((third & (1 << 7 | 1 << 6)) != 0x80)) break;
			third = third & 0x3f;
			
			forth = Str[i++];
			if(forth == '\0' || ((forth & (1 << 7 | 1 << 6)) != 0x80)) break;
			forth = forth & 0x3f;
			
			unicode = ((uint32_t)first << 18) | ((uint32_t)second << 12) | ((uint32_t)second << 6) | forth;
			
			ret += GetGlyphWidth(OLED, unicode);
		}
	}
	
	return ret;
}

// 
// @简介：获取当前字体的最大高度
// @参数：OLED - 显示器的句柄
// @返回值：字体最大高度
// 
uint16_t OLED_GetFontHeight(OLED_TypeDef *OLED)
{
	return OLED->Font->FontSize;
}

#define swap(x,y) do{(x) = (x) + (y); (y) = (x) - (y); (x) = (x) - (y); }while(0)

//
// @简介：画点
// @参数：OLED - OLED显示器的句柄	
// @参数：X - 横坐标
// @参数：Y - 纵坐标
//
void OLED_DrawDot(OLED_TypeDef *OLED)
{
	PenDot(OLED, OLED->CursorX, OLED->CursorY);
}	

//
// @简介：以当前光标位置为起点绘制直线
// @参数：OLED - OLED显示器的句柄	
// @参数：X - 终止点横坐标
// @参数：Y - 终止点纵坐标
// 
void OLED_DrawLine(OLED_TypeDef *OLED, int16_t X, int16_t Y)
{
	int16_t x, y;
	int16_t X0 = OLED->CursorX;
	int16_t Y0 = OLED->CursorY;
	int16_t X1 = X;
	int16_t Y1 = Y;
	
	if(OLED->PenColor == PEN_COLOR_TRANSPARENT) return; // 透明画笔不需要绘图
	
	if(X0 != X1)
	{
		if(X0 > X1)
		{
			swap(X0, X1);
			swap(Y0, Y1);
		}
		for(x=X0; x < X1; x++)
		{
			if(x < 0 || x >= OLED_SCREEN_COLS) continue;
			y = (int16_t)round(1.0 * (Y1 - Y0) * (x - X0) / (X1 - X0) + Y0);
			if(y < 0 || y >= OLED_SCREEN_ROWS) continue;
			
			PenDot(OLED, x, y);
		}
	}
	if(Y0 != Y1)
	{
		if(Y0 > Y1)
		{
			swap(X0, X1);
			swap(Y0, Y1);
		}
		for(y=Y0; y < Y1; y++)
		{
			if(y < 0 || y >= OLED_SCREEN_ROWS) continue;
			x = (int16_t)round(1.0 * (y - Y0) * (X1 - X0) / (Y1 - Y0) + X0);
			if(x < 0 || x >= OLED_SCREEN_COLS) continue;
			
			PenDot(OLED, x, y);
		}
	}
}

//
// @简介：以当前光标位置为起点绘制直线，绘制完成后光标移动到线段的终点
// @参数：OLED - OLED显示器的句柄	
// @参数：X - 终止点横坐标
// @参数：Y - 终止点纵坐标
// 
void OLED_LineTo(OLED_TypeDef *OLED, int16_t X, int16_t Y)
{
	OLED_DrawLine(OLED, X, Y);
	OLED->CursorX = X;
	OLED->CursorY = Y;
}

static void DrawCircleFrame(OLED_TypeDef *OLED, int16_t X, int16_t Y, uint16_t Radius)
{
	int16_t x, y, distance;
	
	if(OLED->PenColor == PEN_COLOR_TRANSPARENT) return; // 透明画笔不需要绘图

	if(X - Radius >= OLED_SCREEN_COLS || X + Radius < 0) return; // 绘图区域超出缓冲区范围
	if(Y - Radius >= OLED_SCREEN_ROWS || Y + Radius < 0) return; // 绘图区域超出缓冲区范围

	for(x=X-Radius; x<=X+Radius; x++)
	{
		if(x < 0 || x > OLED_SCREEN_COLS) continue; // x坐标超出缓冲区范围
		for(distance = 0; distance <= Radius; distance++)
		{
			if((x - X) * (x - X) + (distance + 1) * (distance + 1) > Radius * Radius) // x^2 + y ^ 2 > radius^2
			{
				if(Y+distance < OLED_SCREEN_ROWS && x >= 0 && x < OLED_SCREEN_COLS)
				{
					PenDot(OLED, x, Y+distance);
				}
				if(Y-distance > 0 && x >= 0 && x < OLED_SCREEN_COLS)
				{
					PenDot(OLED, x, Y-distance);
				}
				break;
			}
		}
	}

	for(y=Y-Radius; y<=Y+Radius; y++)
	{
		if(y < 0 || y > OLED_SCREEN_ROWS) continue;
		for(distance = 0; distance <= Radius; distance++)
		{
			if((y - Y) * (y - Y) + (distance + 1) * (distance + 1) > Radius * Radius)
			{
				if(X+distance < OLED_SCREEN_COLS && y >=0 && y < OLED_SCREEN_ROWS)
				{
					PenDot(OLED, X+distance, y);
				}
				if(X-distance > 0 && y >=0 && y < OLED_SCREEN_ROWS)
				{
					PenDot(OLED, X-distance, y);
				}
				break;
			}
		}
	}
}

static void FillCircle(OLED_TypeDef *OLED, int16_t X, int16_t Y, uint16_t Radius)
{
	int16_t x, distance;
	
	if(OLED->Brush == BRUSH_TRANSPARENT) return; // 透明画刷不需要绘图

	if(X - Radius >= OLED_SCREEN_COLS || X + Radius < 0) return;
	if(Y - Radius >= OLED_SCREEN_ROWS || Y + Radius < 0) return;

	for(x=X-Radius; x<=X+Radius; x++)
	{
		if(x < 0 || x > OLED_SCREEN_COLS) continue;
		for(distance = 0; distance <= Radius; distance++)
		{
			if((x - X) * (x - X) + distance*distance <= Radius * Radius)
			{
				if(Y+distance < OLED_SCREEN_ROWS)
				{
					BrushDot(OLED, x, Y+distance);
				}
				if(Y-distance > 0)
				{
					BrushDot(OLED, x, Y-distance);
				}
			}
			else
			{
				break;
			}
		}
	}
}

//
// @简介：以光标为圆心绘制圆形
// @参数：OLED - OLED显示器的句柄	
// @参数：X - 圆心横坐标
// @参数：Y - 圆心纵坐标
// @参数：Radius - 圆的半径
// 
void OLED_DrawCircle(OLED_TypeDef *OLED, uint16_t Radius)
{
	int16_t X = OLED->CursorX, Y = OLED->CursorY;
	
	if(OLED->PenColor != PEN_COLOR_TRANSPARENT)
	{
		DrawCircleFrame(OLED, X, Y, Radius);
	}
	
	if(OLED->Brush != BRUSH_TRANSPARENT)
	{
		FillCircle(OLED, X, Y, Radius);
	}
}

static void DrawRectFrame(OLED_TypeDef *OLED, int16_t X, int16_t Y, uint16_t Width, uint16_t Height)
{
	int16_t x,y;
	
	if(OLED->PenColor == PEN_COLOR_TRANSPARENT) return; // 透明画笔不需要绘图
	
	// 绘制左侧边
	x = X;
	if(x>=0 && x<OLED_SCREEN_COLS)
	{
		for(y=max(0,Y);y<Y+Height;y++)
		{
			PenDot(OLED, x, y);
		}
	}
	
	// 绘制右侧边
	x = X + Width -1;
	if(Width > 0 && x>=0 && x<OLED_SCREEN_COLS)
	{
		for(y=max(0,Y);y<Y+Height;y++)
		{
			PenDot(OLED, x, y);
		}
	}
	
	// 绘制上边
	y = Y;
	if(y>=0 && y<OLED_SCREEN_ROWS)
	{
		for(x=max(0,X);x<X+Width;x++)
		{
			PenDot(OLED, x, y);
		}
	}
	
	// 绘制下边
	y = Y + Height - 1;
	if(y>=0 && y<OLED_SCREEN_ROWS)
	{
		for(x=max(0,X);x<X+Width;x++)
		{
			PenDot(OLED, x, y);
		}
	}
}

static void FillRect(OLED_TypeDef *OLED, int16_t X, int16_t Y, uint16_t Width, uint16_t Height)
{
	if(OLED->Brush == BRUSH_TRANSPARENT) return; // 透明画刷不需要绘图
	
	int16_t x,y;
	
	for(x=X;x<X+Width;x++)
	{
		for(y=Y;y<Y+Height;y++)
		{
			BrushDot(OLED, x, y);
		}
	}
}

//
// @简介：以光标位置为左上角绘制矩形
// @参数：OLED - OLED显示器的句柄
// @参数：X - 矩形左上角的横坐标
// @参数：Y - 矩形左上角的纵坐标
// @参数：Width - 矩形宽度
// @参数：Height - 矩形高度
// 
void OLED_DrawRect(OLED_TypeDef *OLED, uint16_t Width, uint16_t Height)
{
	if(OLED->PenColor != PEN_COLOR_TRANSPARENT)
	{
		DrawRectFrame(OLED, OLED->CursorX, OLED->CursorY, Width, Height);
	}
	if(OLED->Brush != BRUSH_TRANSPARENT)
	{
		FillRect(OLED, OLED->CursorX, OLED->CursorY, Width, Height);
	}
}

//
// @简介：开始分段发送缓冲区内容到屏幕（每个8*8的区域为一个单元进行发送）
// @参数：OLED - OLED显示器的句柄
// @返回值：0 - 启动成功
//
int OLED_StartSendBuffer(OLED_TypeDef *OLED)
{
	uint8_t arg[2];
	
	OLED->RefreshProgress = 0;
	
	// 设置寻址模式为横向寻址模式
	arg[0] = 0x00;
	if(OLED_SendCommand(OLED, 0x20, arg, 1) != 0) return -1;

	// 设置列范围
	arg[0] = 0x00;
	arg[1] = 0x7f;
	if(OLED_SendCommand(OLED, 0x21, arg, 2) != 0) return -1;

	// 设置页范围
	arg[0] = 0x00;
	arg[1] = 0x07;
	if(OLED_SendCommand(OLED, 0x22, arg, 2) != 0) return -1;
	
	return 0;
}

//
// @简介：分段发送缓冲区内容到屏幕（每个8*8的区域为一个单元进行发送）
// @参数：OLED - OLED显示器的句柄
// @参数：pMoreOut - 输出参数，用于接收是否有后续数据需要发送
//                   0    - 当前分段为最后一个分段，至此所有分段均已发送结束
//                   非零 - 当前分段不是最后一个分段
// @返回值：0 - 成功
//
int OLED_EndSendBuffer(OLED_TypeDef *OLED, uint8_t *pMoreOut)
{
	if(OLED->RefreshProgress >= 127)
	{
		*pMoreOut = 0;
	}
	else
	{
		*pMoreOut = 1;
	}
	
	if(OLED->RefreshProgress >= 128)
	{
		return -1;
	}
	
	// 更新显示数据
	OLED_SendData(OLED, &OLED->pBuffer[OLED->RefreshProgress * 8], 8);
	
	OLED->RefreshProgress = (OLED->RefreshProgress + 1) % 128;
	
	return 0;
}

//
// @简介：将缓冲区数据一次性发送到屏幕
// @参数：OLED - OLED显示器的句柄
// @返回值：0 - 成功
//
int OLED_SendBuffer(OLED_TypeDef *OLED)
{
	uint8_t arg[2];

	// 设置寻址模式为横向寻址模式
	arg[0] = 0x00;
	if(OLED_SendCommand(OLED, 0x20, arg, 1) != 0) return -1;

	// 设置列范围
	arg[0] = 0x00;
	arg[1] = 0x7f;
	if(OLED_SendCommand(OLED, 0x21, arg, 2)) return -1;

	// 设置页范围
	arg[0] = 0x00;
	arg[1] = 0x07;
	if(OLED_SendCommand(OLED, 0x22, arg, 2)) return -1;
	
	// 更新显示数据
	if(OLED_SendData(OLED, OLED->pBuffer, OLED_SCREEN_COLS * OLED_SCREEN_PAGES) != 0)
	{
		return -1;
	}
	
	return 0;
}

static int OLED_SendCommand(OLED_TypeDef *OLED, uint8_t Cmd, const uint8_t *Arg, uint16_t Size)
{
	uint8_t buf[8];
	uint8_t i;
	
	buf[0] = SSD1306_CTRL_COMMAND_STREAM;
	buf[1] = Cmd;
	
	for(i=0;i<Size;i++)
	{
		buf[i + 2] =  Arg[i];
	}
	
	if(OLED->i2c_write_cb(OLED_SLAVE_ADDR, buf, i+2) != 0)
	{
		return -1; // 数据发送失败
	}
	
	return 0;
}

static int OLED_SendData(OLED_TypeDef *OLED, uint8_t *pData, uint16_t Size)
{
	int ret = 0;
	
	uint8_t tmp = *(pData - 1);
	*(pData - 1) = SSD1306_CTRL_DATA_STREAM;
	
	if(OLED->i2c_write_cb(OLED_SLAVE_ADDR, pData - 1, Size + 1) != 0)
	{
		ret = -1; // 数据发送失败
	}
	
	*(pData - 1) = tmp;
	
	return ret;
}

//
// @简介：绘制位图
// @参数：OLED - OLED显示器的句柄
// @参数：X - 位图左上角的横坐标
// @参数：Y - 位图左上角的纵坐标
// @参数：Width - 位图宽度
// @参数：Height - 位图高度
// @参数：pBitmap - 位图数据
//
static void DrawBitmapEx(OLED_TypeDef *OLED,int16_t X, int16_t Y, uint16_t Width, uint16_t Height, const uint8_t *pBitmap)
{
	int16_t x,y;
	
	if(OLED->Brush == BRUSH_TRANSPARENT && OLED->PenColor == PEN_COLOR_TRANSPARENT) return; // 透明画刷不做任何操作
	
	uint16_t penWidthCpy = OLED->PenWidth;
	
	OLED->PenWidth = 1;
	
	uint16_t nBytesPerRow = (uint16_t)ceil(Width / 8.0);
	
	for(x=0;x<Width;x++)
	{
		for(y=0;y<Height;y++)
		{
			// b0 b1 .. b7 b0 b1 
			if((pBitmap[x/8+y*nBytesPerRow] & (0x80>>(x%8))) != 0)
			{
				PenDot(OLED, X+x, Y+y);
			}
			else
			{
				BrushDot(OLED, X+x, Y+y);
			}
		}
	}
	
	OLED->PenWidth = penWidthCpy;
}

//
// @简介：以光标位置为左上角绘制位图
// @参数：X - 位图左上角横坐标
// @参数：Y - 位图左上角纵坐标
// @参数：Width - 位图宽度
// @参数：Height - 位图高度
// @参数：pBitmap - 位图数据，格式：每个字节表示横向的8个像素点，行末尾不足8个像素的用0补齐
//
void OLED_DrawBitmap(OLED_TypeDef *OLED, uint16_t Width, uint16_t Height, const uint8_t *pBitmap)
{
	DrawBitmapEx(OLED, OLED->CursorX, OLED->CursorY, Width, Height, pBitmap);
}

// 
// @简介：根据字符的unicode码获取字形序号
// @参数：OLED - OLED显示器句柄
// @参数：Unicode - 字符的Unicode编码
// @返回值：如果查找成功则返回字符序号，否则返回-1
//
static int16_t unicode_2_glyph_idx(OLED_TypeDef *OLED, uint32_t Unicode)
{
	int16_t ret = -1;
	
	if(OLED->Font == 0)
	{
		return -1;
	}
	
	uint32_t i;
	
	for(i = 0; i < OLED->Font->nChars; i++)
	{
		if(OLED->Font->Map[i] == Unicode)
		{
			ret = i;
			break;
		}
	}
	
	return ret;
}

static void PenDot(OLED_TypeDef *OLED, int16_t x, int16_t y)
{
	// 判断是否是透明画笔
	if(OLED->PenColor == PEN_COLOR_TRANSPARENT)
	{
		return; // 透明画笔不需要绘图
	}
	
	if(OLED->PenWidth == 0)
	{
		return;
	}
	
	uint16_t borderLeft, borderRight, borderTop, borderBottom;
	
	if(OLED->PenWidth % 2)
	{
		borderLeft = borderRight = borderTop = borderBottom = OLED->PenWidth / 2;
	}
	else
	{
		borderLeft = borderRight = borderTop = borderBottom = OLED->PenWidth / 2;
		borderLeft--;
		borderTop--;
	}
	
	// 绘图
	// 备份画刷
	uint8_t brushCpy = OLED->Brush;
	
	OLED->Brush = OLED->PenColor;
	
	int16_t i,j;
	
	for(i=x-borderLeft;i<=x+borderRight;i++)
	{
		for(j=y-borderTop;j<=y+borderBottom;j++)
		{
			BrushDot(OLED, i, j);
		}
	}
	
	// 还原画刷
	OLED->Brush = brushCpy;
}

static void BrushDot(OLED_TypeDef *OLED, int16_t x, int16_t y)
{
	// 判断是否是透明画笔
	if(OLED->Brush == BRUSH_TRANSPARENT)
	{
		return; // 透明画笔不需要绘图
	}
	
	// 判断绘图点是否在屏幕外部
	if(x < 0 || x >= OLED_SCREEN_COLS)
	{
		return;
	}
	
	if(y<0 || y >= OLED_SCREEN_ROWS)
	{
		return;
	}
	
	// 判断是否在绘图区域的外部
	if(OLED->ClipRegionWidth != 0 && OLED->ClipRegionHeight != 0)
	{
		if(x<OLED->ClipRegionX || x>=OLED->ClipRegionX + OLED->ClipRegionWidth)
		{
			return;
		}
		if(y<OLED->ClipRegionY || y>=OLED->ClipRegionY + OLED->ClipRegionHeight)
		{
			return;
		}
	}
	
	// 绘图
	if(OLED->Brush == BRUSH_WHITE) // 点亮
	{
		OLED->pBuffer[x + y / 8 * OLED_SCREEN_COLS] |= 1 << (y % 8);
	}
	else // 熄灭
	{
		OLED->pBuffer[x + y / 8 * OLED_SCREEN_COLS] &= ~(1 << (y % 8));
	}
}
