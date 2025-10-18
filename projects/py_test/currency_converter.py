def currency_converter():
    # 固定汇率
    exchange_rate = 6.5

    print("=== 货币兑换程序 ===")

    # 获取货币类型输入
    while True:
        currency_type = input("请输入要兑换的货币类型 (USD/CNY): ").upper()

        if currency_type in ['USD', 'CNY']:
            break
        else:
            print("输入错误！请输入 USD 或 CNY")

    # 获取金额输入
    while True:
        try:
            amount = float(input("请输入金额: "))
            if amount >= 0:
                break
            else:
                print("金额不能为负数，请重新输入！")
        except ValueError:
            print("输入无效！请输入数字！")

    # 进行货币兑换计算
    if currency_type == 'USD':
        # 美元转人民币
        result = amount * exchange_rate
        print(f"\n兑换结果:")
        print(f"{amount} 美元 = {result:.2f} 人民币")
        print(f"汇率: 1美元 = {exchange_rate}人民币")
    else:
        # 人民币转美元
        result = amount / exchange_rate
        print(f"\n兑换结果:")
        print(f"{amount} 人民币 = {result:.2f} 美元")
        print(f"汇率: 1美元 = {exch´ange_rate}人民币")


# 主程序
if __name__ == "__main__":
    currency_converter()

    # 可选：询问用户是否继续兑换
    while True:
        continue_choice = input("\n是否继续兑换？(y/n): ").lower()
        if continue_choice in ['y', 'yes', '是']:
            print("\n" + "=" * 30)
            currency_converter()
        elif continue_choice in ['n', 'no', '否']:
            print("感谢使用货币兑换程序！")
            break
        else:
            print("请输入 y 或 n")