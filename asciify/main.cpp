#include <iostream>
#include <fstream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "include/stb_image.h"
#include "include/stb_image_write.h"
#include "include/stb_image_resize.h"

using namespace std;

// 	\e[38;5;<ansi_color_code>m<your_text_here>\e[0m
string rgb_to_ansi(int r, int g, int b){
	string s = "\e[1;48;5;232;38;5;";
	r = 5 * r/255;
	g = 5 * g/255;
	b = 5 * b /255;
	int ansi = 16+36*r+6*g+b;
	s += to_string(ansi);
	s += "m";
	
	return s;
}

string pixel_to_ascii(int r, int g, int b){
	string map = "  .:!+*e$@8.*es░▒▓█";
	string ret = "";
	int map_len = map.length();
	int index = int(map_len * (0.2126*r + 0.7152*g + 0.0722*b) / 255); //brightness_to_ascii
	ret += map[index];
	ret = rgb_to_ansi(r,g,b) + ret  + "\e[0m";
	return ret;
}


int main(int argc, char* argv[]) {
	if (argc < 1) {
		cerr << "usage : ./main [path-to-image]\n";
		return 1;
	}

//Print terminal dimensions
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int tRow = w.ws_row;
	int tCol = w.ws_col;
	cerr << "Terminal: "<< tRow << " * " << tCol << endl;

//Load image
	int width, height, bpp;
	uint8_t* data = stbi_load(argv[1], &width, &height, &bpp, 3);
	if (data == NULL) { 
		cerr << "[STB_IMAGE] FAILED TO LOAD IMAGE\n";
		return 1; 
	}
	cerr << "Image loaded!: " << argv[1] << " : " << width << " * " << height << endl;

//Resize image to fit terminal
	float aspect_ratio = float(width) / float(height);
	float rf = float(tRow) / float(height);
	int oh = int(float(height) * rf);
	int ow = aspect_ratio * oh;

	cerr << "\tAspect ratio: " << aspect_ratio << endl;
	cerr << "\tResize factor: " << rf <<"\n\tNew image: "<< ow << " * " << oh << endl;

	uint8_t* data_resize = new uint8_t[ow * oh * 3];
	stbir_resize_uint8(data, width, height, width * 3, data_resize, ow, oh, ow * 3, 3);
	stbi_write_jpg("out.jpg", ow, oh, 3, data_resize, ow * 3);

//Pixel -> Char	
	string out = "";
	string buf = "";
	buf += char(13); //CR
	buf += char(10); //LF, buf is CRLF
	ofstream file;
	file.open("out.txt");

	for (int j = 0; j < oh; j++) {
		for (int i = 0; i < oh; i++) {
			//sample RGB from resized image, convert to grayscale, then pick from map
			double r = static_cast<double>(data_resize[(i + j * oh) * 3 + 0]);
			double g = static_cast<double>(data_resize[(i + j * oh) * 3 + 1]);
			double b = static_cast<double>(data_resize[(i + j * oh) * 3 + 2]);

			out += pixel_to_ascii(r,g,b);
		}
		out += buf;
	}
	file << out;
	file.close();

	for(int k = 0 ; k < tCol ; k++){
		cerr << '*';
	}
	cerr <<'\n'<<out;

	// stbi_write_jpg("out.jpg", oh, oh, 1, data_out, oh); //uncomment to get grayscale image
	stbi_image_free(data);
	return 0;
}
