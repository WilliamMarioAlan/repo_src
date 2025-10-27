# e2.1DrawPython.py
import turtle
turtle.setup(650, 350, 200, 200)   # 设置画布大小和初始位置
turtle.penup()                     # 抬起画笔
turtle.fd(-250)                    # 向后移动
turtle.pendown()                   # 放下画笔
turtle.pensize(25)                 # 画笔粗细
turtle.pencolor("purple")          # 画笔颜色
turtle.seth(-40)                   # 设置起始角度
for i in range(4):                 # 循环 4 次
    turtle.circle(40, 80)          # 画圆弧（正方向）
    turtle.circle(-40, 80)         # 画圆弧（反方向）
turtle.circle(40, 80/2)            # 半个圆弧
turtle.fd(40)                      # 直线前进
turtle.circle(16, 180)             # 小半圆（蛇头）
turtle.fd(40 * 2/3)                # 向前一点（蛇舌部分）
turtle.done()

