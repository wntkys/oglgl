# Проект настройки  
PROJECT_NAME = oglgl
BUILD_DIR = bin
OBJ_DIR = obj
SHADER_DIR = src/shaders

# Рекурсивно найти все файлы
SHADERS = $(shell find src -name "*.glsl")
SHADER_HEADERS = $(SHADERS:%.glsl=%.h)
SOURCES = $(shell find src -name "*.c") libs/gl3w/src/gl3w.c
OBJECTS = $(SOURCES:%.c=$(OBJ_DIR)/%.o)

# Остальные настройки
CC = gcc
CSTD = -std=c23
PKG_CONFIG_PACKAGES = sdl3 gl cglm

CFLAGS = $(CSTD) -Wall -Wextra $(shell pkg-config --cflags $(PKG_CONFIG_PACKAGES)) -Ilibs/gl3w/include -g -O0 -fno-omit-frame-pointer
LDFLAGS = $(shell pkg-config --libs $(PKG_CONFIG_PACKAGES)) -lm -ldl -g

.PHONY: all clean compile_commands.json shaders

all: shaders $(BUILD_DIR)/$(PROJECT_NAME)

# Генерация всех шейдерных заголовков
shaders: $(SHADER_HEADERS)

# Правило для конвертации .glsl в .h
%.h: %.glsl
	shader_minifier $< --format c-variables --preserve-externals -o $@

$(BUILD_DIR)/$(PROJECT_NAME): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Автоматически создавать поддиректории перед компиляцией
# Объектные файлы зависят от шейдерных заголовков
$(OBJ_DIR)/%.o: %.c $(SHADER_HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Создать основные директории
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)
	rm -f $(SHADER_HEADERS)

compile_commands.json:
	bear -- $(MAKE) clean all

run: $(BUILD_DIR)/$(PROJECT_NAME)
	./$<