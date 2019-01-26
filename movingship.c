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
#define MAX_SCREEN_WIDTH 1920
#define MAX_SCREEN_HEIGHT 1080
#define DELAY 200
#define DELTA_PIXELS 10
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
	int r = 255, g = 0, b = 0;
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
				put_pixel_RGB16(x, y, r, g, b);
			}
			else if(vinfo.bits_per_pixel == 24){
				put_pixel_RGB24(x, y, r, g, b);
			}
			else if(vinfo.bits_per_pixel == 32){
				put_pixel_RGB32(x, y, r, g, b);
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
				put_pixel_RGB16(x, y, r, g, b);
			}
			else if(vinfo.bits_per_pixel == 24){
				put_pixel_RGB24(x, y, r, g, b);
			}
			else if(vinfo.bits_per_pixel == 32){
				put_pixel_RGB32(x, y, r, g, b);
			}
			if(D > 0){
				y = y + yi;
				D = D - 2 * dx;
			}
			D += 2 * dy;
		}
	}
}

void clear_fbuffer(){
	memset(fbp, 0, (vinfo.bits_per_pixel / 8 * vinfo.yres * vinfo.xres));
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

    if((int)fbp == -1){
        printf("Failed to mmap.\n");
    }
    else {
		system("setterm -cursor off && /bin/stty raw -echo && clear");
		int x = vinfo.xres / 2, y = vinfo.yres / 2;
		clear_fbuffer();
		draw_line(vinfo.xres / 2, vinfo.yres / 2, 0, 0);
		pthread_create(&tid, NULL, read_arrow_key, (void *)&tid);
		for(;;){
			if(isInput == RECEIVED){
				//do something here while the other thread read input
				switch(input){
					case 'C' :
						x += DELTA_PIXELS;
						if(x >= vinfo.xres){
							x = vinfo.xres - 1;
						}
						clear_fbuffer();
						draw_line(x, y, 0, 0);
						break;
					case 'D' :
						x -= DELTA_PIXELS;
						if(x < 0){
							x = 0;
						}
						clear_fbuffer();
						draw_line(x, y, 0, 0);
						break;
				}
				isInput = WAITING;
			}
			else if(isInput == STOP){
				break;
			}
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
