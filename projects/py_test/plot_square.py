# square_turtle.py
import turtle

def get_side():
    """
    循环提示用户输入正方形边长（像素）。
    接受整数或小数，要求 > 0。
    返回 float。
    """
    while True:
        s = input("Enter side length in pixels (positive number): ")
        try:
            val = float(s)
            if val <= 0:
                print("Please enter a positive number.")
                continue
            return val
        except ValueError:
            print("Invalid input — please enter a number (e.g. 150 or 120.5).")

def draw_square(side):
    """
    使用 turtle 画一个边长为 side 的正方形，颜色为 purple，
    并将正方形中心放在窗口中心。
    """
    screen = turtle.Screen()
    screen.title(f"Square — side {side}px")
    # 可选：调整窗口大小以便更好地显示较大的正方形
    # screen.setup(width=max(600, int(side+200)), height=max(600, int(side+200)))

    t = turtle.Turtle()
    t.speed(0)          # 最快绘制
    t.pensize(3)
    t.color("purple")   # 画笔颜色 purple
    t.hideturtle()

    # 将绘图起点移动到正方形左下角，使正方形居中显示
    half = side / 2.0
    t.penup()
    t.goto(-half, -half)
    t.pendown()
    t.showturtle()

    for _ in range(4):
        t.forward(side)
        t.left(90)

    t.hideturtle()
    # 等待用户关闭窗口（在大多数平台上 mainloop() 可用）
    screen.mainloop()

if __name__ == "__main__":
    side_length = get_side()
    draw_square(side_length)
