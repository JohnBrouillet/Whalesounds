## WhaleSounds
WhaleSounds is a super user-friendly, free and open source application for marine sounds listening. 

It is primarily an educational software used during events.

Spectrogram is used as a simple example to teach people how acoustic researchers can study a sound. Besides, it is cool to watch.

## Usage
Currently, only .wav files are usable.

For conveniency when used on tablet, audio files are listed on a json file.
At this moment, this file must be named whale_data.json and put in a folder named data (whalesounds for Android). This folder must be where the executable is or at the root of Android memory.

Besides, json file has this architecture :
```json
{
	"Name of my button 1":
	{
		"group of related audio files 1":
		{
			"image": "path to a picture",
			"sons": ["path to audio file 1", "path to audio file 2",...],
			"credits": "credits",
			"description": "my description"
		},

		"group of related audio files 2":
		{
			"image": "path to a picture",
			"sons": ["path to audio file 1", "path to audio file 2",...],
			"credits": "credits",
			"description": "my description"
		},
	},

	"Name of my button 2":
	{
		"group of related audio files 1":
		{
			"image": "path to a picture",
			"sons": ["path to audio file 1", "path to audio file 2",...],
			"credits": "credits",
			"description": "my description"
		}
	}
}
```

Paths must be relative to data folder.
At runtime, json file is read to create buttons. For marine sounds listening, buttons refer to family of animals and a group of audio files represents a specie.

Buttons in the "configuration column" (appearing when "configuration" button is clicked) are a trick to zoom on the spectrogram and its color gradient when the software is used on tablet. On desktop, mouse wheel can be used for this matter. Zoom X or zoom Y must be checked to activate zooming and dragging mode.


## Building
QtCreator has been chosen for its conveniency to build Android applications in C++. 

## Libraries used
The software makes heavy use of Qt library for GUI and audio engine.
The class representing WAV file comes from "spectrum" example of Qt.

Widgets for plotting are made with [QCustomPlot](https://www.qcustomplot.com/) library based on Qt.
QRealFourier is distributed by [Christoph Stallman](https://github.com/visore/QRealFourier) and uses FFTReal by Laurent de Soras internally. Nevertheless, the code has been modified to support double precision.
