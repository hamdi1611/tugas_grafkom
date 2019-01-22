#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <time.h>

#define FILE_NAME "text.txt"
#define MAX_SCREEN_WIDTH 1920
#define MAX_SCREEN_HEIGHT 1080
#define DELAY 200
#define DELTA_PIXELS 10

char *fbp = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
FILE *fp;
int file_height, file_width, spacing;
char display[MAX_SCREEN_HEIGHT][MAX_SCREEN_WIDTH];

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

int getRandomIn(int min, int max){
	return (rand() % (max - min + 1)) + min; 
}

void clearFBuffer(){
	memset(fbp, 0, (vinfo.bits_per_pixel / 8 * vinfo.yres * vinfo.xres));
}

void loadFile(const char *file_name){
	fp = fopen(file_name, "r");
    if(fp == NULL){
        perror("Error opening the file.\n");
        exit(EXIT_FAILURE);
    }
    fscanf(fp, "%d", &file_height);
    fscanf(fp, "%d", &file_width);
    fscanf(fp, "%d", &spacing);
    for(int i = 0; i < file_height && i < vinfo.yres; ++i){
        fscanf(fp, "%s", display[i]);
    }
    fclose(fp);
}

void draw(int offset_x, int offset_y){
	int r, g, b;
    for(int y = 0; y < vinfo.yres && y < file_height; ++y){
		if(y % spacing == 0){
			r = getRandomIn(0, 255);
			g = getRandomIn(0, 255);
			b = getRandomIn(0, 255);
		}
		if(y + offset_y < vinfo.yres && y + offset_y >= 0){
			for(int x = 0; x < vinfo.xres && x < file_width; ++x){
				if(display[y][x] == '1'){
					if(vinfo.bits_per_pixel == 16){
						put_pixel_RGB16(x + offset_x, y + offset_y, r, g, b);
					}
					else if(vinfo.bits_per_pixel == 24){
						put_pixel_RGB24(x + offset_x, y + offset_y, r, g, b);
					}
					else if(vinfo.bits_per_pixel == 32){
						put_pixel_RGB32(x + offset_x, y + offset_y, r, g, b);
					}
				}
			}
		}
    }
}

int main(int argc, char* argv[])
{
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
        // draw
		int offset_x = vinfo.xres / 3, offset_y = vinfo.yres;
        loadFile(FILE_NAME);
        clearFBuffer();
        for(int dy = 0; dy < vinfo.yres; dy += DELTA_PIXELS){
			draw(offset_x, offset_y - dy);
			usleep(DELAY * 1000);
			clearFBuffer();
		}
        clearFBuffer();
    }

    // cleanup
    munmap(fbp, screensize);
    if(ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo)){
        printf("Error re-setting variable information.\n");
    }
    close(fbfd);

    return 0;
  
}
