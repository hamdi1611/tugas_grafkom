#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <time.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0
#define RED 0
#define GREEN 255
#define BLUE 0
#define MAX_SCREEN_WIDTH 1920
#define MAX_SCREEN_HEIGHT 1080
#define DELAY 200
#define DELTA_PIXELS 10
#define SIZE 100
char *fbp = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
char display[MAX_SCREEN_HEIGHT][MAX_SCREEN_WIDTH];
enum state {RECEIVED, WAITING, STOP};
enum state isInput = WAITING;
char input;

void *read_arrow_key(){
	while(isInput != STOP){
		while(isInput == RECEIVED);
		if(input = getchar()){
			if(input == 27){
				getchar();
				input = getchar();
			}
			if(input == 'A'){
				printf("\n\r");
				isInput = STOP;
			}
			else{
				isInput = RECEIVED;
			}
		}
	}
	return 0;
}

void put_pixel_RGB24(int x, int y, int r, int g, int b)
{
    // x * 3 as every pixel is 3 consecutive bytes
    unsigned int pix_offset = x * 3 + y * finfo.line_length;
    // now this is about the same as 'fbp[pix_offset] = value'
    *((char*)(fbp + pix_offset)) = b;
    *((char*)(fbp + pix_offset + 1)) = g;
    *((char*)(fbp + pix_offset + 2)) = r;
}

void put_pixel_RGB16(int x, int y, int r, int g, int b)
{
    // x * 2 as every pixel is 2 consecutive bytes
    unsigned int pix_offset = x * 2 + y * finfo.line_length;
	//unsigned short c = ((r / 8) << 11) + ((g / 4) << 5) + (b / 8);
    unsigned short c = ((r / 8) * 2048) + ((g / 4) * 32) + (b / 8);
    // write 2 bytes at once
    *((unsigned short*)(fbp + pix_offset)) = c;
}

void put_pixel_RGB32(int x, int y, int r, int g, int b)
{
    // x * 2 as every pixel is 2 consecutive bytes
    unsigned int pix_offset = x * 4 + y * finfo.line_length;
	unsigned int c = (r << 16) + (g << 8) + b;
    // write 4 bytes at once
    *((unsigned int*)(fbp + pix_offset)) = c;
}

void draw_line(int x0, int y0, int x1, int y1){
	int x_start = x0, x_end = x1, y_start = y0, y_end = y1;
	char isHigh = TRUE;
	if(abs(y1 - y0) < abs(x1 - x0)){
		isHigh = FALSE;
	}
	if(x0 > x1 || y0 > y1){
		x_start = x1;
		y_start = y1;
		x_end = x0;
		y_end = y0;
	}
	int dx = x_end - x_start, dy = y_end - y_start;
	if(isHigh){
		int xi = 1;
		if(dx < 0){
			xi = -1;
			dx *= -1;
		}
		int D = 2 * dx - dy, x = x_start;
		for(int y = y_start; y <= y_end; ++y){
			if(vinfo.bits_per_pixel == 16){
				put_pixel_RGB16(x, y, RED, GREEN, BLUE);
			}
			else if(vinfo.bits_per_pixel == 24){
				put_pixel_RGB24(x, y, RED, GREEN, BLUE);
			}
			else if(vinfo.bits_per_pixel == 32){
				put_pixel_RGB32(x, y, RED, GREEN, BLUE);
			}
			if(D > 0){
				x = x + xi;
				D = D - 2 * dy;
			}
			D += 2 * dx;
		}
	}
	else{
		int yi = 1;
		if(dy < 0){
			yi = -1;
			dy *= -1;
		}
		int D = 2 * dy - dx, y = y_start;
		for(int x = x_start; x <= x_end; ++x){
			if(vinfo.bits_per_pixel == 16){
				put_pixel_RGB16(x, y, RED, GREEN, BLUE);
			}
			else if(vinfo.bits_per_pixel == 24){
				put_pixel_RGB24(x, y, RED, GREEN, BLUE);
			}
			else if(vinfo.bits_per_pixel == 32){
				put_pixel_RGB32(x, y, RED, GREEN, BLUE);
			}
			if(D > 0){
				y = y + yi;
				D = D - 2 * dx;
			}
			D += 2 * dy;
		}
	}
}

void draw_ship(int x, int y, int size){
	int x0_high = x - size / 2, x1_high = x + size / 2;
	int x0_low = x - size / 3, x1_low = x + size / 3;
	draw_line(x0_high, y, x1_high, y);
	draw_line(x0_low, y + size / 2, x1_low, y + size / 2);
	draw_line(x0_high, y, x0_low, y + size / 2);
	draw_line(x1_low, y + size / 2, x1_high, y);
	draw_line(x - size / 4, y, x - size / 4, y - size / 4);
	draw_line(x + size / 4, y, x + size / 4, y - size / 4);
	draw_line(x - size / 4, y - size / 4, x + size / 4, y - size / 4);
}

void draw_peluru(int x, int y, int size){
    int x0_high = x - size / 2, x1_high = x + size / 2;
	int x0_low = x - size / 3, x1_low = x + size / 3;

    draw_line(x - 5, y - size / 4 - 5, x + 5, y - size / 4 - 5);
    draw_line(x - 5, y - size / 4 + 5, x + 5, y - size / 4 + 5);
    draw_line(x - 5, y - size / 4 - 5, x - 5 , y - size / 4 + 5);
    draw_line(x + 5, y - size / 4 - 5, x + 5 , y - size / 4 + 5);
}

void move_peluru(int arr0[5][2], int arr1[5][2], int arr2[5][2], int arr3[5][2], int arr4[5][2], int size){
    for(int i = 0; i < 5; i++) {
		int x,y;
		int xmax = 30;
		int ymin = 30;
		int ymax = 1800;

		x = arr0[i][0];
		y = arr0[i][1];
        if ((x > xmax) && (y > ymin) && (y < ymax)) {
            draw_peluru(x, y, size);
            arr0[i][1] = y - 7;
        }

		x = arr1[i][0];
		y = arr1[i][1];
        if ((x > xmax) && (y > ymin) && (y < ymax)) {
            draw_peluru(x, y, size);
			arr1[i][0] = x + 1;
            arr1[i][1] = y - 6;
        }

		x = arr2[i][0];
		y = arr2[i][1];
        if ((x > xmax) && (y > ymin) && (y < ymax)) {
            draw_peluru(x, y, size);
			arr2[i][0] = x + 2;
            arr2[i][1] = y - 5;
        }

		x = arr3[i][0];
		y = arr3[i][1];
        if ((x > xmax) && (y > ymin) && (y < ymax)) {
            draw_peluru(x, y, size);
            arr3[i][0] = x - 1;
            arr3[i][1] = y - 6;
        }

		x = arr4[i][0];
		y = arr4[i][1];
        if ((x > xmax) && (y > ymin) && (y < ymax)) {
            draw_peluru(x, y, size);
            arr4[i][0] = x - 2;
            arr4[i][1] = y - 5;
        }
    }
}

void shoot(int x, int y, int arr[5][2]) {
    for(int i = 0; i < 5; i++)
    {
        if ((arr[i][0] <= 50) || (arr[i][1] <= 50)) {
            arr[i][0] = x;
            arr[i][1] = y;
            break;
        }
    }
}

void fly(int pesawat[7][3], int arr0[5][2], int arr1[5][2], int arr2[5][2], int arr3[5][2], int arr4[5][2]) {
	int panjang = 30;
	int lebar = 10;

	for(int i = 0; i < 7; i++) {
		if ((pesawat[i][1] > 20) && (pesawat[i][2] > 20)) {
			draw_line(pesawat[i][1] - panjang, pesawat[i][2], pesawat[i][1] + panjang, pesawat[i][2]);
		
			int x = pesawat[i][1];
			int y = pesawat[i][2];

			// Checking peluru
			for(int j = 0; j < 5; j++) {
				if ((arr0[j][1] <= y+lebar) && (arr0[j][1] >= y-lebar) && (arr0[j][0] <= x+panjang) && (arr0[j][0] >= x-panjang)) {
					pesawat[i][0] = 1;
					arr0[j][0] = 0;
					arr0[j][1] = 0;
				}
				if ((arr1[j][1] <= y+lebar) && (arr1[j][1] >= y-lebar) && (arr1[j][0] <= x+panjang) && (arr1[j][0] >= x-panjang)) {
					pesawat[i][0] = 1;
					arr1[j][0] = 0;
					arr1[j][1] = 0;
				}
				if ((arr2[j][1] <= y+lebar) && (arr2[j][1] >= y-lebar) && (arr2[j][0] <= x+panjang) && (arr2[j][0] >= x-panjang)) {
					pesawat[i][0] = 1;
					arr2[j][0] = 0;
					arr2[j][1] = 0;
				}
				if ((arr3[j][1] <= y+lebar) && (arr3[j][1] >= y-lebar) && (arr3[j][0] <= x+panjang) && (arr3[j][0] >= x-panjang)) {
					pesawat[i][0] = 1;
					arr3[j][0] = 0;
					arr3[j][1] = 0;
				}
				if ((arr4[j][1] <= y+lebar) && (arr4[j][1] >= y-lebar) && (arr4[j][0] <= x+panjang) && (arr4[j][0] >= x-panjang)) {
					pesawat[i][0] = 1;
					arr4[j][0] = 0;
					arr4[j][1] = 0;
				}
			}

			// aman
			if (pesawat[i][0] == 0) {
				if (pesawat[i][0] == 0) {
					pesawat[i][1] = pesawat[i][1] + 7;
					if (pesawat[i][1] > 1300) {
						pesawat[i][0] = 0;
						pesawat[i][1] = 0;
						pesawat[i][2] = 0;
					}
				}
			}
			// jatuh
			else {
				if (pesawat[i][2] > 300) {
					pesawat[i][0] = 0;
					pesawat[i][1] = 0;
					pesawat[i][2] = 0;
				}
				else {
					pesawat[i][2] = pesawat[i][2] + 5;
				}
			}
		}
	}
}

add_pesawat(int pesawat[7][3]) {
	for(int i = 0; i < 7; i++) {
		if ((pesawat[i][0] == 0) && (pesawat[i][1] == 0) && (pesawat[i][2] == 0)) {
			int temp = rand() % (20 + 1 - 5) + 5;
			pesawat[i][1] = temp*10;
			pesawat[i][2] = 50;
			break;
		}
	}
}

void clear_fbuffer(){
	memset(fbp, 0, (vinfo.bits_per_pixel / 8 * vinfo.yres * vinfo.xres));
}

void coloring(unsigned short c1, unsigned int c2) {
	for(int y = 590; y > 0; y--) {
		int color = 0;
		int isline = 0;
		for(int x = 0; x < vinfo.xres -1; x++) {
			if(vinfo.bits_per_pixel == 16){
				unsigned int pix_offset = x * 3 + y * finfo.line_length;
				unsigned int pix_offset2 = x * 3 + y * finfo.line_length + 3;
				
				if (color) {
					if (*((char*)(fbp + pix_offset + 2))) {
						color = 0;
					}
					else {
						put_pixel_RGB16(x, y, RED, GREEN, BLUE);
					}
				}
				else {
					if ((*((char*)(fbp + pix_offset + 2)))) {
						if ((*((char*)(fbp + pix_offset2 + 2)))) {
							// do nothing
						}
						else {
							color = 1;
						}
					}
				}
			}
			else if(vinfo.bits_per_pixel == 24){
				unsigned int pix_offset = x * 2 + y * finfo.line_length;
				unsigned int pix_offset2 = x * 2 + y * finfo.line_length + 2;
				
				if (color) {
					if (*((unsigned short*)(fbp + pix_offset))) {
						color = 0;
					}
					else {
						*((unsigned short*)(fbp + pix_offset)) = c1;
					}
				}
				else {
					if ((*((unsigned short*)(fbp + pix_offset)))) {
						if ((*((unsigned short*)(fbp + pix_offset2)))) {
							// do nothing
						}
						else {
							color = 1;
						}
					}
				}
			}
			else if(vinfo.bits_per_pixel == 32){
				unsigned int pix_offset = x * 4 + y * finfo.line_length;
				unsigned int pix_offset2 = x * 4 + y * finfo.line_length + 4;
				
				if (color) {
					if (*((unsigned int*)(fbp + pix_offset))) {
						color = 0;
					}
					else {
						*((unsigned int*)(fbp + pix_offset)) = c2;
					}
				}
				else {
					if ((*((unsigned int*)(fbp + pix_offset)))) {
						if ((*((unsigned int*)(fbp + pix_offset2)))) {
							isline = 1;
						}
						else {
							if (isline) {
								isline = 0;
							}
							else {
								color = 1;
							}
						}
					}
				}
			}
		}
	}
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	srand (time(NULL));
    int fbfd = 0;
    struct fb_var_screeninfo orig_vinfo;
    long int screensize = 0;

    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if(!fbfd){
      printf("Error: cannot open framebuffer device.\n");
      return(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get variable screen information
    if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)){
      printf("Error reading variable information.\n");
    }
    printf("Original %dx%d, %dbpp\n", vinfo.xres, vinfo.yres, 
       vinfo.bits_per_pixel );

    // Store for reset (copy vinfo to vinfo_orig)
    memcpy(&orig_vinfo, &vinfo, sizeof(struct fb_var_screeninfo));

    // Get fixed screen information
    if(ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)){
      printf("Error reading fixed information.\n");
    }

    // map fb to user mem 
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    fbp = (char*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

	unsigned short c1 = ((RED / 8) * 2048) + ((GREEN / 4) * 32) + (BLUE / 8);
	unsigned int   c2 = (RED << 16) + (GREEN << 8) + BLUE;

    if((int)fbp == -1){
        printf("Failed to mmap.\n");
    }
    else {
		system("setterm -cursor off && /bin/stty raw -echo && clear");
		int x = vinfo.xres / 2, y = vinfo.yres / 2;
		clear_fbuffer();
		draw_ship(vinfo.xres / 2, vinfo.yres / 2, SIZE);
		pthread_create(&tid, NULL, read_arrow_key, (void *)&tid);

		int peluru_kanan2[5][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
		int peluru_kanan1[5][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
		int peluru_tengah[5][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
		int peluru_kiri1[5][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
		int peluru_kiri2[5][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

		int counter = 0;
		int pesawat[7][3] = {{0, 100, 100}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

		for(;;){
			if(isInput == RECEIVED){
				//do something here while the other thread read input
				switch(input){
					case 'C' :
						x += DELTA_PIXELS;
						if(x + SIZE / 2 >= vinfo.xres){
							x = vinfo.xres - 1 - SIZE / 2;
						}
						break;
					case 'D' :
						x -= DELTA_PIXELS;
						if(x - SIZE / 2 < 0){
							x = SIZE / 2;
						}
						break;
                    case 'B' :
                        shoot(x, y - 30, peluru_tengah);
						shoot(x + 15, y - 25, peluru_kanan1);
						shoot(x + 10, y - 10, peluru_kanan2);
						shoot(x - 15, y - 25, peluru_kiri1);
						shoot(x - 10, y - 10, peluru_kiri2);
                        break;
				}
				isInput = WAITING;
			}
			else if(isInput == STOP){
				break;
			}
            draw_ship(x, y, SIZE);
			fly(pesawat, peluru_tengah, peluru_kanan1, peluru_kanan2, peluru_kiri1, peluru_kiri2);
			move_peluru(peluru_tengah, peluru_kanan1, peluru_kanan2, peluru_kiri1, peluru_kiri2, SIZE);
            coloring(c1, c2);

			if (counter == 50) {
				add_pesawat(pesawat);
				counter = 0;
			}
			else {
				counter = counter + 1;
			}

            //delay
            // for(int i = 0; i < 900000; i++){}
            
            clear_fbuffer();
		}
		clear_fbuffer();
		system("/bin/stty -raw echo && clear");
		pthread_exit(NULL);      
    }

    // cleanup
    munmap(fbp, screensize);
    if(ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo)){
        printf("Error re-setting variable information.\n");
    }
    close(fbfd);

    return 0;
  
}
