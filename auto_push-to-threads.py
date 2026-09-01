import time
from threads_py import ThreadsClient
import sys
import json
import os

def split_text(text: str, limit=490) -> list[str]:
    # Split the text by newlines and the custom delimiter
    ret = []
    for part in text.split('---SPLIT---'):
        lines = [line for line in part.split('\n')]
        _ret = []
        for line in lines:
            while len(line) > limit:
                _ret.append(line[:limit])
                line = line[limit:]
            if _ret and len(_ret[-1]) + len(line) + 1 <= limit:
                _ret[-1] += '\n' + line
            else:
                _ret.append(line)
        ret.extend(_ret)
    return ret

def load_secrets() -> dict[str, str | bool | dict | None]:
    """Load secrets from environment (preferred) or fallback to secret.json."""
    user_id= str(os.getenv("THREADS_USER_ID"))
    access_token = str(os.getenv("THREADS_ACCESS_TOKEN"))
    app_secret = str(os.getenv("THREADS_APP_SECRET"))
    if user_id and access_token and app_secret:
        return {"user_id": user_id, "access_token": access_token, "app_secret": app_secret, "persist": False}

    # fallback to file
    with open("secret.json", "r", encoding="utf-8") as file:
        ori : dict[str, str] = json.loads(file.read())
    return {"user_id": ori.get("user_id"), "access_token": ori.get("access_token"), "app_secret": ori.get("app_secret"), "persist": True, "ori": ori}


# 從 git hook 傳進來的參數
commit_message = sys.argv[1]
changed_files = sys.argv[2:]

# 取得 secrets（優先使用 env）
secrets = load_secrets()

# 初始化 ThreadsClient
threads = ThreadsClient(
    user_id=str(secrets["user_id"]),
    access_token=str(secrets["access_token"])
)

# 則刷新並保存
threads.refresh_access_token()

text = ""

# 準備發文內容
if commit_message[:7] != "-coding":
    text = commit_message[7:]
    text += "\n\n" + "今日的程式碼：\n"
    for i in changed_files:
        text += "https://github.com/Bryan0324/everyday_coding/blob/main/" + i + "\n"
    parts = split_text(text)
    
elif commit_message[:7] != "-not-cp":
    if commit_message[:6] != "-blank":
        text = "TOI初選錄取前每日一題競程" 
    else:
        commit_message = commit_message[6:]
        
    text += commit_message + "\n"

    text += "\n\n" + "今日的程式碼：\n"
    for i in changed_files:
        text += "https://github.com/Bryan0324/everyday_coding/blob/main/" + i + "\n"
    parts = split_text(text)
else :
    # feature 發文
    parts = split_text(commit_message[6:])


# Debug 模式，不發文
if commit_message[:6] == "-debug":
    print("Test mode, not posting to Threads.")
    sys.exit(0)

parent = threads.create_post(text=parts[0])
parent = parent.publish()
print("Published:", parent)
time.sleep(2)  # 等兩秒，避免發文太快被擋

for part in parts[1:]:
    parent = parent.reply(threads.create_post(
        text=part
    ))
    print("Published:", parent)
print("All done!")
