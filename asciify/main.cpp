#include <iostream>
#include <fstream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "include/stb_image.h"
#include "include/stb_image_write.h"
#include "include/stb_image_resize.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Usage : ./main [path-to-image] [resize-factor]\n";
		return 1;
	}
	int width, height, bpp;
	uint8_t* data = stbi_load(argv[1], &width, &height, &bpp, 3);
	if (data == NULL) { cerr << "FAILED TO LOAD IMAGE\n";	return 1; }
	cout << "Image loaded!: " << argv[1] << " : " << width << " * " << height << endl;
	int rf = 1; //resize factor
	if (argv[2] == NULL) { cerr << "1:1 image NO resize";rf = 1; }
	else rf = *argv[2] - 48;

	int ow = width / rf;
	int oh = height / rf;
	cerr << "Resize factor: " << rf <<"\nNew image: "<< ow << " * " << oh << endl;

	uint8_t* data_resize = new uint8_t[ow * oh * 3]; // quarter res
	stbir_resize_uint8(data, width, height, width * 3, data_resize, ow, oh, ow * 3, 3);
	stbi_write_jpg("out.jpg", ow, oh, 3, data_resize, ow * 3);

	uint8_t* data_out = new uint8_t[ow * oh]; //black and white
	
	string map = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
	int map_len = map.length();
	string out = "";
	string buf = "";
	buf += char(13); //CR
	buf += char(10); //LF, buf is CRLF
	ofstream file;
	file.open("z.txt");

	for (int j = 0; j < oh; j++) {
		for (int i = 0; i < oh; i++) {
			//sample RGB from resized image, convert to grayscale, then pick from map
			double r = static_cast<double>(data_resize[(i + j * oh) * 3 + 0]);
			double g = static_cast<double>(data_resize[(i + j * oh) * 3 + 1]);
			double b = static_cast<double>(data_resize[(i + j * oh) * 3 + 2]);
			data_out[i + j * oh] = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);

			out += map[(data_out[i + j * oh] * map_len / 255)];
		}
		out += buf;
	}
	file << out;
	file.close();

	// stbi_write_jpg("out.jpg", oh, oh, 1, data_out, oh); //uncomment to get grayscale image
	stbi_image_free(data);
	return 0;
}
