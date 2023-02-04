cpued:main.c cpu.c run cpued_gui
	gcc main.c -o ../cpued -lm
run:run.c
	gcc run.c -o ../run
cpued_gui:main_GUI.c
	gcc `pkg-config --cflags gtk+-3.0` main_GUI.c -o ../cpued_gui `pkg-config --libs gtk+-3.0` -lX11 -lm