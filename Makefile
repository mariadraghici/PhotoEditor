build: image_editor

image_editor: image_editor.c
	gcc image_editor.c -lm -Wall -Wextra -std=c99 -o image_editor

clean:
	rm -f image_editor

