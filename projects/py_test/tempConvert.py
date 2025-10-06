# el.1TempConvert.py
TempStr = input("请输入带有符号的温度值: ")

if TempStr[-1] in ['F', 'f']:      # 如果最后一位是 F 或 f，说明输入的是华氏温度
    C = (eval(TempStr[0:-1]) - 32) / 1.8
    print("转换后的温度是{:.2f}C".format(C))

elif TempStr[-1] in ['C', 'c']:    # 如果最后一位是 C 或 c，说明输入的是摄氏温度
    F = 1.8 * eval(TempStr[0:-1]) + 32
    print("转换后的温度是{:.2f}F".format(F))

else:                              # 输入格式不正确
    print("输入格式错误")
