#include <graphics.h>

void draw(char c, int x, int y, int size){
	switch(c){
		case 'A' :
			line(x + size / 2, y, x + size / 4, y + size);
			line(x + size / 2, y, x + size * 3 / 4, y + size);
			line(x + size * 2 / 5 , y + size / 2, x + size * 2 / 3, y + size / 2);
			break;

		case 'B' :
			line(x + size / 4, y, x + size / 4, y + size);
			ellipse(x + size / 4, y + size / 4, 270, 90, size / 2, size / 4); 
			ellipse(x + size / 4, y + size * 3 / 4, 270, 90, size / 2, size / 4); 
			break;

		case 'C' :
			arc(x + size * 3 / 4, y + size / 2, 90, 270, size / 2);
			break;

		case 'D' :
			line(x + size / 4, y, x + size / 4 , y + size);
			arc(x + size / 4, y + size / 2, 270, 90, size / 2); 
			break;

		case 'E' :
			line(x + size / 4, y, x + size / 4, y + size); 
			line(x + size / 4, y, x + size * 3 / 4, y);
			line(x + size / 4, y + size / 2, x + size * 3 / 4, y + size / 2);
			line(x + size / 4, y + size, x + size * 3 / 4, y + size);
			break;

		case 'F' :
			line(x + size / 4, y, x + size / 4, y + size); 
			line(x + size / 4, y, x + size * 3 / 4, y);
			line(x + size / 4, y + size / 2, x + size * 3 / 4, y + size / 2);
			break;

		case 'G' :
			arc(x + size * 3 / 4, y + size / 2, 90, 270, size / 2);
			line(x + size * 3 / 4, y + size, x + size * 3 / 4, y + size / 2);
			line(x + size / 2, y + size / 2, x + size * 3 / 4, y + size / 2);
			break;

		case 'H' :
			line(x + size / 4, y, x + size / 4, y + size);
			line(x + size / 4, y + size / 2, x + size * 3 / 4, y + size / 2);
			line(x + size * 3 / 4, y, x + size * 3 / 4, y + size); 
			break;

		case 'I' :
			line(x + size / 4, y, x + size * 3 / 4, y);
			line(x + size / 2, y, x + size / 2, y + size);
			line(x + size / 4, y + size, x + size * 3 / 4, y + size);
			break;

		case 'J' :
			line(x + size / 4, y, x + size * 3 / 4, y);
			line(x + size / 2, y, x + size / 2, y + size);
			arc(x + size / 2, y + size * 5 / 8, 90, 45, size * 3 / 8);
			break;

		case 'K' :
			line(x + size / 4, y, x + size / 4, y + size);
			line(x + size / 4, y + size / 2, x + size * 3 / 4, y);
			line(x + size / 4, y + size / 2, x + size * 3 / 4, y + size);
			break;

		case 'L' :
			line(x + size / 4, y, x + size / 4, y + size);
			line(x + size / 4, y + size, x + size * 3 / 4, y + size);
			break;

		case 'M' :
			line(x + size / 4, y, x + size / 4, y + size);
			line(x + size / 4, y, x + size / 2, y + size / 2);
			line(x + size * 3 / 4, y, x + size / 2, y + size / 2);
			line(x + size * 3 / 4, y, x + size * 3 / 4, y + size); 
			break;

		case 'N' :
			line(x + size / 4, y, x + size / 4, y + size);
			line(x + size / 4, y, x + size * 3 / 4, y + size);
			line(x + size * 3 / 4, y, x + size * 3 / 4, y + size); 
			break;

		case 'O' :
			ellipse(x + size / 2, y + size / 2, 0, 360, size / 4, size / 2);
			break;

		case 'P' :
			line(x + size / 4, y, x + size / 4, y + size);
			ellipse(x + size / 4, y + size / 4, 270, 90, size / 2, size / 4); 
			break;

		case 'Q' :
			ellipse(x + size / 2, y + size / 2, 0, 360, size / 4, size / 2);
			line(x + size * 2 / 3, y + size * 7 / 8, x + size * 3 / 4, y + size);
			break;

		case 'R' :
			line(x + size / 4, y, x + size / 4, y + size);
			ellipse(x + size / 4, y + size / 4, 270, 90, size / 2, size / 4); 
			line(x + size / 2, y + size / 2, x + size * 3 / 4, y + size);
			break;

		case 'S' :
			arc(x + size / 2, y + size / 4, 90, 315, size / 4);
			circle(x + size / 2, y + size / 2, 1);
			arc(x + size / 2, y + size * 3 / 4, 270, 45, size / 4);
			break;

		case 'T' :
			line(x + size / 4, y, x + size * 3 / 4, y);
			line(x + size / 2, y, x + size / 2, y + size);
			break;

		case 'U' :
			line(x + size / 4, y, x + size / 4, y + size * 3 / 4);
			arc(x + size / 2, y + size * 3 / 4, 0, 180, size / 4);
			line(x + size * 3 / 4, y, x + size * 3 / 4, y + size * 3 / 4);
			break;

		case 'V' :
			line(x + size / 4, y, x + size / 2, y + size);
			line(x + size * 3 / 4, y, x + size / 2, y + size);
			break;

		case 'W' :
			line(x + size / 4, y, x + size * 3 / 8, y + size);
			line(x + size * 3 / 8, y + size, x + size / 2, y + size / 2);
			line(x + size / 2, y + size / 2, x + size * 5 / 8, y + size);
			line(x + size * 3 / 4, y, x + size * 5 / 8, y + size);
			break;

		case 'X' :
			line(x + size / 4, y, x + size * 3 / 4, y + size);
			line(x + size * 3 / 4, y, x + size / 4, y + size);
			break;

		case 'Y' :
			line(x + size / 4, y, x + size / 2, y + size / 2);
			line(x + size * 3 / 4, y, x + size / 2, y + size / 2);
			line(x + size / 2, y + size / 2, x + size / 2, y + size);
			break;

		case 'Z' :
			line(x + size / 4, y, x + size * 3 / 4, y);
			line(x + size * 3 / 4, y, x + size / 4, y + size);
			line(x + size / 4, y + size, x + size * 3 / 4, y + size);
			break;

		case '0' :
			ellipse(x + size / 2, y + size / 2, 0, 360, size / 4, size / 2);
			circle(x + size / 2, y + size / 2, 1);
			break;

		case '1' :
			line(x + size / 4, y, x + size / 2, y);
			line(x + size / 2, y, x + size / 2, y + size);
			line(x + size / 4, y + size, x + size * 3 / 4, y + size);
			break;

		case '2' :
			arc(x + size / 2, y + size / 4, 180, 360, size / 4);
			line(x + size * 3 / 4, y + size / 4, x + size / 4, y + size);
			line(x + size / 4, y + size, x + size * 3 / 4, y + size);
			break;

		case '3' :
			arc(x + size / 2, y + size / 4, 225, 450, size / 4);
			arc(x + size / 2, y + size * 3 / 4, 270, 45, size / 4);
			break;

		case '4' :
			line(x + size * 5 / 8, y, x + size / 4, y + size / 2);
			line(x + size / 4, y + size / 2, x + size * 3 / 4, y + size / 2);
			line(x + size * 5 / 8, y, x + size * 5 / 8, y + size);
			break;

		case '5' :
			line(x + size / 4, y, x + size * 3 / 4, y);
			line(x + size / 4, y, x + size / 4, y + size / 3);
			ellipse(x + size / 4, y + size * 2 / 3, 270, 90, size / 2, size / 3); 
			break;

		case '6' :
			ellipse(x + size / 2, y + size / 2, 150, 315, size / 4, size / 2);
			ellipse(x + size / 2, y + size * 2 / 3, 0, 360, size / 4, size / 3);
			break;

		case '7' :
			line(x + size / 4, y, x + size * 3 / 4, y);
			line(x + size * 3 / 4, y, x + size / 4, y + size);
			break;

		case '8' :
			circle(x + size / 2, y + size / 4, size / 4);
			circle(x + size / 2, y + size * 3 / 4, size / 4);
			break;

		case '9' :
			ellipse(x + size / 2, y + size / 3, 0, 360, size / 4, size / 3);
			ellipse(x + size / 2, y + size / 2, -30, 135, size / 4, size / 2);
			break;
		case '<' :
			line(x + size * 3 / 4, y, x + size / 4, y + size / 2);
			line(x + size / 4, y + size / 2, x + size * 3 / 4, y + size);
			break;
	}
}

int offset(int *x, int size, int min, int max){
	int temp = *x;
	if(*x + size < max){
		*x += size;
	}
	else{
		*x = min;
	}
	return temp;
}

int main()
{
	int gd = DETECT, gm;
	initgraph(&gd,&gm,NULL);
	int size = 20, min_x = 100, max_x = getmaxx(), min_y = 0, max_y = getmaxy();
	int x = min_x, y, dy = 1, c = 0;
	for(y = 0; y < max_y && kbhit() == 0; y += dy, ++c)
	{
		cleardevice();
		draw('T', offset(&x, size, min_x, max_x), y, size);
		draw('U', offset(&x, size, min_x, max_x), y, size);
		draw('G', offset(&x, size, min_x, max_x), y, size);
		draw('A', offset(&x, size, min_x, max_x), y, size);
		draw('S', offset(&x, size, min_x, max_x), y, size);
		draw(' ', offset(&x, size, min_x, max_x), y, size);
		draw('1', offset(&x, size, min_x, max_x), y, size);
		draw(' ', offset(&x, size, min_x, max_x), y, size);
		draw('G', offset(&x, size, min_x, max_x), y, size);
		draw('R', offset(&x, size, min_x, max_x), y, size);
		draw('A', offset(&x, size, min_x, max_x), y, size);
		draw('F', offset(&x, size, min_x, max_x), y, size);
		draw('I', offset(&x, size, min_x, max_x), y, size);
		draw('K', offset(&x, size, min_x, max_x), y, size);
		draw('A', offset(&x, size, min_x, max_x), y, size);
		draw(' ', offset(&x, size, min_x, max_x), y, size);
		draw('K', offset(&x, size, min_x, max_x), y, size);
		draw('O', offset(&x, size, min_x, max_x), y, size);
		draw('M', offset(&x, size, min_x, max_x), y, size);
		draw('P', offset(&x, size, min_x, max_x), y, size);
		draw('U', offset(&x, size, min_x, max_x), y, size);
		draw('T', offset(&x, size, min_x, max_x), y, size);
		draw('E', offset(&x, size, min_x, max_x), y, size);
		draw('R', offset(&x, size, min_x, max_x), y, size);
		x = min_x;
		offset(&y, 2 * size, min_y, max_y);
		draw('A', offset(&x, size, min_x, max_x), y, size);
		draw('N', offset(&x, size, min_x, max_x), y, size);
		draw('D', offset(&x, size, min_x, max_x), y, size);
		draw('R', offset(&x, size, min_x, max_x), y, size);
		draw('E', offset(&x, size, min_x, max_x), y, size);
		draw('A', offset(&x, size, min_x, max_x), y, size);
		draw('S', offset(&x, size, min_x, max_x), y, size);
		draw(' ', offset(&x, size, min_x, max_x), y, size);
		draw('H', offset(&x, size, min_x, max_x), y, size);
		draw('A', offset(&x, size, min_x, max_x), y, size);
		draw('L', offset(&x, size, min_x, max_x), y, size);
		draw('I', offset(&x, size, min_x, max_x), y, size);
		draw('M', offset(&x, size, min_x, max_x), y, size);
		draw(' ', offset(&x, size, min_x, max_x), y, size);
		draw('1', offset(&x, size, min_x, max_x), y, size);
		draw('3', offset(&x, size, min_x, max_x), y, size);
		draw('5', offset(&x, size, min_x, max_x), y, size);
		draw('1', offset(&x, size, min_x, max_x), y, size);
		draw('6', offset(&x, size, min_x, max_x), y, size);
		draw('0', offset(&x, size, min_x, max_x), y, size);
		draw('0', offset(&x, size, min_x, max_x), y, size);
		draw('3', offset(&x, size, min_x, max_x), y, size);
		x = min_x;
		offset(&y, 2 * size, min_y, max_y);
		draw('J', offset(&x, size, min_x, max_x), y, size);
		draw('O', offset(&x, size, min_x, max_x), y, size);
		draw('S', offset(&x, size, min_x, max_x), y, size);
		draw('E', offset(&x, size, min_x, max_x), y, size);
		draw(' ', offset(&x, size, min_x, max_x), y, size);
		draw('H', offset(&x, size, min_x, max_x), y, size);
		draw('O', offset(&x, size, min_x, max_x), y, size);
		draw('S', offset(&x, size, min_x, max_x), y, size);
		draw('E', offset(&x, size, min_x, max_x), y, size);
		draw('A', offset(&x, size, min_x, max_x), y, size);
		draw(' ', offset(&x, size, min_x, max_x), y, size);
		draw('1', offset(&x, size, min_x, max_x), y, size);
		draw('3', offset(&x, size, min_x, max_x), y, size);
		draw('5', offset(&x, size, min_x, max_x), y, size);
		draw('1', offset(&x, size, min_x, max_x), y, size);
		draw('6', offset(&x, size, min_x, max_x), y, size);
		draw('0', offset(&x, size, min_x, max_x), y, size);
		draw('2', offset(&x, size, min_x, max_x), y, size);
		draw('7', offset(&x, size, min_x, max_x), y, size);
		x = min_x;
		setcolor(c);
		delay(10);
		if(c == 15){
			c = 2;
		}
	}
	closegraph();
	return 0;
}
