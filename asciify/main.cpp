#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
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
namespace fs = std::filesystem;

// 	\e[38;5;<ansi_color_code>m<your_text_here>\e[0m
string rgb_to_ansi(int r, int g, int b) {
	string s = "\e[1;48;5;232;38;5;";
	r = 5 * r / 255;
	g = 5 * g / 255;
	b = 5 * b / 255;
	int ansi = 16 + 36 * r + 6 * g + b;
	s += to_string(ansi);
	s += "m";

	return s;
}

string pixel_to_ascii(int r, int g, int b) {
	// string map = "  .:!+*e$@8.*es░▒▓█";
	string map = "   `^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
	string ret = "";
	int map_len = map.length();
	int index = int(map_len * (0.2126 * r + 0.7152 * g + 0.0722 * b) / 255); //brightness_to_ascii
	ret += map[index];
	ret = rgb_to_ansi(r, g, b) + ret + "\e[0m";
	return ret;
}


int main(int argc, char* argv[]) {
	if (argc < 3) {
		cerr << "usage : ./main [path-to-file] -mode{v,i}\n";
		return 1;
	}
//Parse args
	string mode = argv[2];
	// cerr << argv[0] << "\n" << argv[1] << "\n" << argv[2] << endl;

//Print terminal dimensions
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int tRow = w.ws_row;
	int tCol = w.ws_col;

//image
	if (mode[1] == 'i') {

	//Load image
		int width, height, bpp; //bpp - bytes per pixel
		uint8_t* data = stbi_load(argv[1], &width, &height, &bpp, 3);
		if (data == NULL) {
			cerr << "[STB_IMAGE] FAILED TO LOAD IMAGE\n";
			return 1;
		}
		cerr << "Image loaded!: " << argv[1] << " : " << width << " * " << height << endl;

	//Resize image to fit terminal
		float aspect_ratio = float(width) / float(height);
		int ow = tCol * 0.85; //use 85% of terminal width
		int oh = tRow;
		cerr << "Terminal: " << tCol << " * " << tRow << "  |  Aspect ratio: " << aspect_ratio << "  |  New image: " << ow << " * " << oh << endl;

		uint8_t* data_resize = new uint8_t[ow * oh * 3];
		stbir_resize_uint8(data, width, height, width * 3, data_resize, ow, oh, ow * 3, 3);
		stbi_write_png("out.png", ow, oh, 3, data_resize, ow * 3);

	//Pixel -> Char	
		string out = "";
		for (int j = 0; j < oh; j++) {
			out += "\t";
			for (int i = 0; i < ow; i++) {
				//sample RGB from resized image then pick from map
				double r = static_cast<double>(data_resize[(i + j * (ow)) * 3 + 0]);
				double g = static_cast<double>(data_resize[(i + j * (ow)) * 3 + 1]);
				double b = static_cast<double>(data_resize[(i + j * (ow)) * 3 + 2]);
				out += pixel_to_ascii(r, g, b);
			}
			out += "\n";
		}
		for (int k = 0; k < tCol; k++) {
			cout << '-';
		}

		cout << endl; //OUTPUT TO TERMINAL
		cerr << out;
		stbi_image_free(data);
	}
	
//video
	if(mode[1] == 'v'){
		const fs::path frames{"./img/frames"};

		if(!fs::exists(frames)){
			cout << "img/frames does not exist! .. Creating\n";
			system("mkdir -p ./img/frames/"); //if "img/frames" does not exist create it
		}

		string cmd = "ffmpeg -hide_banner -i ";
		cmd += argv[1];
		cmd += " -vf fps=fps=10/1 ./img/frames/%04d.bmp";

		system(cmd.c_str()); // 	ffmpeg -i <path-to-video-file> -vf fps=fps=10/1 ./img/frames/$img%04d.bmp

		int ow = tCol * 0.85, oh = tRow; //output image width and height
		uint8_t* data = NULL;
		uint8_t* data_resize = NULL;
		int width, height, bpp;
		string out = ""; //output ASCII string
		for (auto const& dir_entry : fs::directory_iterator { frames })
		{
			auto path = dir_entry.path().string(); // .img/frames/0xxx.bmp

			//Load image
			data = stbi_load(path.c_str(), &width, &height, &bpp, 3);
			if (data == NULL) {
				cerr << "[STB_IMAGE] FAILED TO LOAD IMAGE\n";
				return 1;
			}			
			data_resize = new uint8_t[ow * oh * 3];
			stbir_resize_uint8(data, width, height, width * 3, data_resize, ow, oh, ow * 3, 3);

			//Pixel -> Char	
			for (int j = 0; j < oh; j++) {
				out += "\t";
				for (int i = 0; i < ow; i++) {
					//sample RGB from resized image then pick from map
					double r = static_cast<double>(data_resize[(i + j * (ow)) * 3 + 0]);
					double g = static_cast<double>(data_resize[(i + j * (ow)) * 3 + 1]);
					double b = static_cast<double>(data_resize[(i + j * (ow)) * 3 + 2]);
					out += pixel_to_ascii(r, g, b);
				}
				out += "\n";
			}
			// cerr<<"\ec";
			cerr << out; //print to terminal
			out.clear();
			stbi_image_free(data);
			delete data_resize;
		}
		
		system("rm -r ./img/frames/*");
	}	
	return 0;
}
