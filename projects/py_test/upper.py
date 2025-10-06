# 从用户输入获取字符串
s = input("请输入一个字符串: ")

# 字符串长度
length = len(s)

# 首字母和最后一个字母（要考虑空字符串的情况）
first = s[0] if len(s) > 0 else ""
last = s[-1] if len(s) > 0 else ""

# 大写和小写
upper = s.upper()
lower = s.lower()

# 输出结果
print(f"字符串: {s}")
print(f"长度: {length}")
print(f"首字母: {first}")
print(f"末字母: {last}")
print(f"大写: {upper}")
print(f"小写: {lower}")
