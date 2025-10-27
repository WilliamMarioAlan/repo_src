# calc_two_numbers.py

# 输入两个整数
a = int(input("请输入第一个数: "))
b = int(input("请输入第二个数: "))

# 运算
sum_val = a + b        # 和
diff_val = a - b       # 差
prod_val = a * b       # 积
if b != 0:
    div_val = a / b    # 商（小数）
    mod_val = a % b    # 余数
else:
    div_val = "除数不能为0"
    mod_val = "除数不能为0"

# 输出结果
print("两个数的和:", sum_val)
print("两个数的差:", diff_val)
print("两个数的积:", prod_val)
print("两个数的商:", div_val)
print("两个数的余数:", mod_val)
