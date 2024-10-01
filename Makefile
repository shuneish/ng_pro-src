# コンパイラとコンパイルオプションの定義
CC = gcc
CFLAGS = -Wall -Werror -g

# ターゲット実行ファイルの名前
TARGET = tc

# ソースファイルのリスト
SRCS = main.c scan.c id-list.c

# オブジェクトファイルのリスト（ソースファイルの拡張子を .o に変更）
OBJS = $(SRCS:.c=.o)

# デフォルトのターゲット（makeを実行したときにデフォルトで呼ばれる）
all: $(TARGET)

# 実行ファイルのビルドルール
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# 個々のソースファイルをオブジェクトファイルにコンパイルするルール
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# クリーンアップルール（中間ファイルを削除する）
clean:
	rm -f $(OBJS) $(TARGET)

# ファイルの依存関係（必要に応じて手動で追加）
main.o: scan.h
scan.o: scan.h
id-list.o: scan.h