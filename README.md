# asciify
--
- Converts `jpeg\png` images to `ascii` and prints to terminal.
#### Build :
---
- 	Two options to build:
	- Cmake
	``` 
	mkdir -p build/
	cd build
	cmake -S .. -B .			
	```
	- Make
		- If you don't want to use cmake for some reason, this would put built files in the `/asciify` directory
	```
		make
	```
- Usage : `./main [path-to-image] `
	- example :  
		`./main ../img/awesomeface.png`

##### Results :

![awesomeface](/asciify/img/awesomeface.png)
![ascii_awesomeface](/results/awesomeface_ascii.jpg)

![astro](/asciify/img/astro.jpg) 
![astro_ascii](/results/astro_ascii.jpg)

![bike](/asciify/img/bike.jpg)
![bike_ascii](/results/bike_ascii.jpg)

![pikachu](/asciify/img/rsz_2surprised_pikachu.png)
![pikachu_ascii](/results/pikachu_ascii.jpg)

#### Dependencies:
- [stb_image](https://github.com/nothings/stb)

#### TODO
---
- asciify videos
