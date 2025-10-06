# 定义一个包含5个数字的列表
numbers = [10, 25, 7, 42, 18]

# 计算
length = len(numbers)
max_num = max(numbers)
min_num = min(numbers)
total = sum(numbers)
average = total / length if length > 0 else 0

# 输出结果
print(f"列表: {numbers}")
print(f"长度: {length}")
print(f"最大值: {max_num}")
print(f"最小值: {min_num}")
print(f"总和: {total}")
print(f"平均值: {average}")
