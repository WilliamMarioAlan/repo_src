import turtle

def draw_hexagon(side):
    """
    使用 turtle 绘制一个边长为 side 的正六边形。
    """
    t = turtle.Turtle()
    t.pensize(3)
    t.color("blue")  # 你可以改成其他颜色
    t.speed(3)

    for _ in range(6):
        t.forward(side)
        t.left(60)

    turtle.done()


if __name__ == "__main__":
    # 通过 input 输入边长
    side_length = input("请输入六边形的边长（像素）：")
    try:
        side_length = float(side_length)
        if side_length <= 0:
            print("请输入一个正数！")
        else:
            draw_hexagon(side_length)
    except ValueError:
        print("请输入数字！")
