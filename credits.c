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

void make_huruf(int x, int y, char c, int size) {
	if (c == 'A') {
		const int n = 12;
		int arr[12][4] = {{0, 0, 3, 0}, {0, 0, 0, 5}, {3, 0, 3, 5}, {1, 1, 2, 1}, {1, 1, 1, 2}, {2, 1, 2, 2}, {1, 2, 2, 2}, {1, 3, 2, 3}, {1, 3, 1, 5}, {2, 3, 2, 5}, {0, 5, 1, 5}, {2, 5, 3, 5}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == 'D' || c == 'O' || c == '0') {
		const int n = 8;
		int arr[8][4] = {{0, 0, 3, 0}, {0, 5, 3, 5}, {0, 0, 0, 5}, {1, 1, 1, 4}, {2, 1, 2, 4}, {3, 0, 3, 5}, {1, 1, 2, 1}, {1, 4, 2, 4}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == 'E') {
		const int n = 16;
		int arr[16][4] = {{0, 0, 3, 0}, {0, 5, 3, 5}, {1, 1, 1.9, 2}, {2, 1, 3, 1}, {1, 2, 1.9, 2}, {2, 2, 3, 2}, {1, 3, 1.9, 3}, {2, 3, 3, 3}, {1, 4, 1.9, 4}, {2, 4, 3, 4}, {0, 0, 0, 5}, {1, 1, 1, 2}, {1, 3, 1, 4}, {3, 0, 3, 1}, {3, 2, 3, 3}, {3, 4, 3, 5}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == 'F') {
		const int n = 13;
		int arr[13][4] = {{0, 0, 3, 0}, {0, 5, 1, 5}, {1, 1, 1.9, 1}, {2, 1, 3, 1}, {1, 2, 1.9, 2}, {2, 2, 3, 2}, {1, 3, 1.9, 3}, {2, 3, 3, 3}, {0, 0, 0, 5}, {1, 1, 1, 2}, {1, 3, 1, 5}, {3, 0, 3, 1}, {3, 2, 3, 3}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == 'G' || c == '6') {
		const int n = 14;
		int arr[14][4] = {{0, 0, 0, 5}, {1, 1, 1, 2}, {1, 3, 1, 4}, {2, 3, 2, 4}, {3, 0, 3, 1}, {3, 2, 3, 5}, {0, 0, 3, 0}, {1, 1, 1.9, 1}, {2, 1, 3, 1}, {1, 2, 1.9, 2}, {2, 2, 3, 2}, {1, 3, 2, 3}, {1, 4, 2, 4}, {0, 5, 3, 5}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == 'H') {
		const int n = 12;
		int arr[12][4] = {{0, 0, 0, 5}, {1, 0, 1, 2}, {1, 3, 1, 5}, {2, 0, 2, 2}, {2, 3, 2, 5}, {3, 0, 3, 5}, {0, 0, 1, 0}, {2, 0, 3, 0}, {1, 2, 2, 2}, {1, 3, 2, 3}, {0, 5, 1, 5}, {2, 5, 3, 5}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == 'I') {
		const int n = 12;
		int arr[12][4] = {{0, 0, 0, 1}, {0, 4, 0, 5}, {1, 1, 1, 4}, {2, 1, 2, 4}, {3, 0, 3, 1}, {3, 4, 3, 5}, {0, 0, 3, 0}, {0, 1, 1, 1}, {2, 1, 3, 1}, {0, 4, 1, 4}, {2, 4, 3, 4}, {0, 5, 3, 5}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == 'J') {
		
	}
	if (c == 'L') {
		const int n = 7;
		int arr[7][4] = {{0, 0, 0, 5}, {1, 0, 1, 4}, {3, 4, 3, 5}, {0, 0, 1, 0}, {1, 4, 1.9, 4}, {2, 4, 3, 4}, {0, 5, 3, 5}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == 'N') {
		const int n = 8;
		int arr[8][4] = {{0, 0, 0, 5}, {1, 1, 1, 5}, {2, 1, 2, 5}, {3, 0, 3, 5}, {0, 0, 3, 0}, {1, 1, 2, 1}, {0, 5, 1, 5}, {2, 5, 3, 5}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == 'R') {
		const int n = 17;
		int arr[17][4] = {{0, 0, 0, 5}, {1, 1, 1, 2}, {1, 4, 1, 5}, {2, 1, 2, 2}, {2, 3, 2, 4}, {2, 4, 2, 5}, {3, 0, 3, 3}, {3, 4, 3, 5}, {0, 0, 3, 0}, {1, 1, 2, 1}, {1, 2, 2, 2}, {2, 3, 3, 3}, {0, 4, 1, 4}, {2, 4, 3, 4}, {0, 5, 1, 5}, {2, 5, 3, 5}, {1, 3, 1.9, 3}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == 'S' || c == '5') {
		const int n = 16;
		int arr[16][4] = {{0, 0, 0, 3}, {0, 4, 0, 5}, {1, 1, 1, 2}, {2, 3, 2, 4}, {3, 0, 3, 1}, {3, 2, 3, 5}, {0, 0, 3, 0}, {1, 1, 1.9, 1}, {2, 1, 3, 1}, {1, 2, 1.9, 2}, {2, 2, 3, 2}, {1, 3, 1.9, 3}, {2, 3, 3, 3}, {1, 4, 1.9, 4}, {2, 4, 3, 4}, {0, 5, 3, 5}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == 'T') {
		const int n = 8;
		int arr[8][4] = {{0, 0, 0, 1}, {3, 0, 3, 1}, {1, 1, 1, 5}, {2, 1, 2, 5}, {0, 0, 3, 0}, {0, 1, 1, 1}, {2, 1, 3, 1}, {1, 5, 2, 5}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == 'U') {
		const int n = 8;
		int arr[8][4] = {{0, 0, 0, 5}, {1, 0, 1, 4}, {2, 0, 2, 4}, {3, 0, 3, 5}, {0, 0, 1, 0}, {2, 0, 3, 0}, {1, 4, 2, 4}, {0, 5, 3, 5}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == 'Z' || c == '2') {
		const int n = 16;
		int arr[16][4] = {{0, 0, 0, 1}, {0, 2, 0, 5}, {1, 3, 1, 4}, {2, 1, 2, 2}, {3, 0, 3, 3}, {3, 4, 3, 5}, {0, 0, 3, 0}, {0, 1, 1, 1}, {2, 1, 3, 1}, {0, 2, 1, 2}, {2, 2, 3, 2}, {0, 3, 1, 3}, {2, 3, 3, 3}, {0, 4, 1, 4}, {2, 4, 3, 4}, {0, 5, 3, 5}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == '1') {
		const int n = 12;
		int arr[12][4] = {{0, 0, 0, 1}, {0, 4, 0, 5}, {1, 1, 1, 4}, {2, 1, 2, 4}, {2, 0, 2, 1}, {3, 4, 3, 5}, {0, 0, 2, 0}, {0, 4, 1, 4}, {2, 4, 3, 4}, {0, 5, 3, 5}, {0, 1, 1, 1}, {1.1, 1, 2, 1}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == '3') {
		const int n = 16;
		int arr[16][4] = {{0, 0, 0, 1}, {0, 2, 0, 3}, {0, 4, 0, 5}, {2, 1, 2, 2}, {2, 3, 2, 4}, {3, 0, 3, 5}, {0, 0, 3, 0}, {0, 1, 1, 1}, {2, 1, 3, 1}, {0, 2, 1, 2}, {2, 2, 3, 2}, {0, 3, 1, 3}, {2, 3, 3, 3}, {0, 4, 1, 4}, {2, 4, 3, 4}, {0, 5, 3, 5}};
		for (int i = 0; i < n; i++) {
			draw_line(x + size*arr[i][0], y + size*arr[i][1], x + size*arr[i][2], y + size*arr[i][3]);
		}
	}
	if (c == '4') {
		
	}
	if (c == '5') {
		
	}
	if (c == '7') {
		
	}
	//A, D, E, H, I, J, L, N, R, S, T, U, Z
    //1, 2, 3, 4, 5, 6, 7, 0
	
}

void make_baris(int x, int y, char baris[], int n, int size) {
	for(int i = 0; i < n; i++) {
		make_huruf(x+i*4*size, y, baris[i], size);
	}
}

void clear_fbuffer(){
	memset(fbp, 0, (vinfo.bits_per_pixel / 8 * vinfo.yres * vinfo.xres));
}

void coloring(unsigned short c1, unsigned int c2) {
	for(int y = 0; y < 800; y++) {
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
						if ((*((unsigned int*)(fbp + pix_offset2)))) {
							isline = 1;
						}
						else {
							if (isline) {
								isline = 0;
							}
							else {
								color = 0;
							}
						}
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
		pthread_create(&tid, NULL, read_arrow_key, (void *)&tid);

		for(;;){
			if(isInput == RECEIVED){
				//do something here while the other thread read input
				switch(input){
					case 'C' :
						break;
					case 'D' :
						break;
                    case 'B' :
                        
                        break;
				}
				isInput = WAITING;
			}
			else if(isInput == STOP){
				break;
			}
			
			// Menggambar ceria
			int size = 10;
			int x_awal = 100;
			int y_awal = 100;
			
            make_baris(x_awal, y_awal, "ANDRE HOSE", 10, size);
            make_baris(x_awal, y_awal+size*6, "SELDI", 5, size);
            make_baris(x_awal, y_awal+size*6*2, "GG", 3, size);
            make_baris(x_awal, y_awal+size*6*3, "TUGAS 3 IF3260", 14, size);
            
            //make_baris(make_baris(x_awal, y_awal, "TUGAS 3 IF3260", 14, size);
            //make_baris(x_awal, y_awal+size*6, "ANDRE 13516003", 14, size);
            //make_baris(x_awal, y_awal+size*6*2, "HOSE 13516027", 13, size);
            //make_baris(x_awal, y_awal+size*6*3, "SELDI 13516042", 14, size);
            //make_baris(x_awal, y_awal+size*6*4, "NUHA 13516120", 13, size);
            //make_baris(x_awal, y_awal+size*6*5, "ABI 13516141", 12, size);
            //make_baris(x_awal, y_awal+size*6*6, "HAZ 13516150", 12, size);
            //make_baris(x_awal, y_awal+size*6*7, "ADIT 13516153", 13, size);
            
            // mawarnai ceria
            coloring(c1, c2);

            //delay
            for(int i = 0; i < 9000000; i++){}
            
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
