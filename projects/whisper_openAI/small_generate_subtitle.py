import whisper
import os
import sys

def transcribe(video_path):
    # 🚨 检查文件是否存在
    if not os.path.exists(video_path):
        print("❌ 文件不存在:", video_path)
        return

    print(f"🚀 开始使用 small 模型识别：{video_path}")
    
    # ✅ 加载 Whisper 的 small 模型（第一次会自动下载）
    model = whisper.load_model("small")

    # ✅ 进行语音识别（不使用进度条）
    # fp16=False 表示使用 CPU 的浮点32精度（macOS CPU 不支持 FP16）
    result = model.transcribe(video_path, fp16=False)

    # ✅ 准备输出 SRT 字幕文件的路径（与输入文件同名，仅扩展名不同）
    srt_path = os.path.splitext(video_path)[0] + ".srt"

    # ✅ 写入 .srt 字幕文件（逐句处理识别段落）
    with open(srt_path, "w", encoding="utf-8") as f:
        for i, segment in enumerate(result["segments"]):
            start = segment["start"]
            end = segment["end"]
            text = segment["text"].strip()

            # 🎯 将秒数格式化为 SRT 所需的时间格式（hh:mm:ss,ms）
            def format_time(t):
                h = int(t // 3600)
                m = int((t % 3600) // 60)
                s = int(t % 60)
                ms = int((t - int(t)) * 1000)
                return f"{h:02}:{m:02}:{s:02},{ms:03}"

            # ✍️ 写入单条字幕段
            f.write(f"{i+1}\n")
            f.write(f"{format_time(start)} --> {format_time(end)}\n")
            f.write(f"{text}\n\n")

    print(f"✅ 字幕已生成：{srt_path}")

if __name__ == "__main__":
    # 📌 参数校验：检查是否提供了输入路径
    if len(sys.argv) < 2:
        print("📌 用法：python generate_subtitle.py <视频或音频文件路径>")
    else:
        transcribe(sys.argv[1])

