
#include "type.h"
#include "gui.h"
#include <string.h>

/**
 * @brief 获取gbk字符串的第index个字符的地址
 *
 * @param str
 * @param index
 * @return const char*
 */
const char *GBK_CharGet(const char *str, uint16_t index)
{
	uint16_t i = 0;
	uint16_t len = 0;
	if (!str)
	{
		return NULL;
	}
	while (str[i])
	{
		if (len == index)
		{
			return &str[i];
		}
		len++;
		if (str[i] & 0x80)
		{
			i += 2;
		}
		else
		{
			i++;
		}
	}
	return NULL;
}
/**
 * @brief 获取UNICODE的长度
 *
 * @param str
 * @return uint32_t
 */
uint32_t UNI_Strlen(uint16_t *str)
{
	uint16_t i = 0;
	if (str == NULL)
	{
		return 0;
	}
	while (str[i])
	{
		i++;
	}
	return i;
}
/**
 * @brief 获取GBK字符串长度
 *
 * @param str
 * @return uint32_t
 */
uint32_t GBK_Strlen(const char *str)
{
	uint16_t i = 0;
	uint16_t len = 0;
	if (!str)
	{
		return 0;
	}
	while (str[i])
	{
		if (str[i] & 0x80)
		{
			i += 2;
		}
		else
		{
			i++;
		}
		len++;
	}

	return len;
}

/**
 * @brief 判断两矩形是否相交
 *
 * @param x1
 * @param y1
 * @param w1
 * @param h1
 * @param x2
 * @param y2
 * @param w2
 * @param h2
 * @return int8_t
 */
int8_t isCollsionWithRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	// 矩形A位于矩形B的右侧
	if (x1 >= x2 && x1 >= x2 + w2)
	{
		return FALSE;
		// 矩形A位于矩形B的左侧
	}
	else if (x1 <= x2 && x1 + w1 <= x2)
	{
		return FALSE;
		// 矩形A位于矩形B的下侧
	}
	else if (y1 >= y2 && y1 >= y2 + h2)
	{
		return FALSE;
		// 矩形A位于矩形B的上侧
	}
	else if (y1 <= y2 && y1 + h1 <= y2)
	{
		return FALSE;
	}
	// 不相交都不满足，那就是相交了
	return TRUE;
}

/**
 * @brief 获得两个矩形的重叠部分
 *
 * @param r1
 * @param r2
 * @param res
 * @return BOOL
 */
BOOL GetOverLapRect(p_xrect_t r1, p_xrect_t r2, p_xrect_t res)
{
	//	assert(r1 != NULL && r2 != NULL && res!=NULL);

	res->x = MAX(r1->x, r2->x);
	res->y = MAX(r1->y, r2->y);
	res->w = MIN(r1->x + r1->w, r2->x + r2->w);
	res->h = MIN(r1->y + r1->h, r2->y + r2->h);
	if ((res->x > res->w) || (res->y > res->h))
	{
		res->x = r1->x;
		res->y = r1->y;
		res->w = 0;
		res->h = 0;
		return FALSE;
	}
	else
	{
		res->w -= res->x;
		res->h -= res->y;
	}
	return TRUE;
}
