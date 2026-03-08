# コンパイラとオプション
CC = gcc
CFLAGS = -Wall -Wextra -g
# includeディレクトリを検索パスに追加
INCLUDES = -Iinclude

# 最終的な実行ファイル名
TARGET = emulator

# ソースとオブジェクトの指定
# ソースファイルを3つに増やす
SRCS = src/main.c src/cpu.c src/instruction.c
OBJS = src/main.o src/cpu.o src/instruction.o

# ヘッダーの検索パス
INCLUDES = -Iinclude

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
# デフォルトターゲット

# コンパイル処理 (src/*.c -> src/*.o)
src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 実行
run: all
	./$(TARGET)

# 掃除 (Windows/Linux両対応の消し方)
clean:
	-powershell -Command "Remove-Item -Path src/*.o, emulator.exe -ErrorAction SilentlyContinue"