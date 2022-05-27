# asciify

- Converts `jpeg\png` images/videos to `ascii` and prints to terminal.
#### Build :
---
```shell
	$ sudo apt install ffmpeg
	$ git clone https://github.com/xenorivai/asciify.git
	$ make

```
- Usage : `./main [path-to-file] -[mode]{v,i}`
	- example :  
		`./main ./img/homer.png -i`

		`./main ./img/ball.mp4 -v`

##### Results :

<img src="./asciify/img/awesomeface.png" alt="awesomeface" style="width:200px;"/>

<img src="./results/awesomeface_ascii.jpg" alt="awesomeface" style="width:300px;height:300px"/>

<img src="./asciify/img/astro.jpg" alt="astro" style="width:300px;"/>

<img src="./results/astro_ascii.jpg" alt="astro_ascii" style="width:300px;"/>

![ball.mp4](/results/output.gif)

#### Dependencies:
- ffmpeg
- [stb_image](https://github.com/nothings/stb)