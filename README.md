# PLT - Plotting Tool

PLT transforms a text file containing keywords, parameters, and data that describe a graph into output compatible with several soft and hard copy graphics devices. Graphics formats supported by PLT include PostScript, EPS, EMF, SVG, and CGM, in addition to onscreen displays.

## Building PLT

### MacOS
A Makefile is provided for MacOS. You can build and install PLT from the terminal using:
```bash
make -f makefile.mac clean
make -f makefile.mac build
sudo make -f makefile.mac install
```

### Linux
A Makefile is provided for Linux. You can build and install PLT from the terminal using:
```bash
make -f makefile.lnx clean
make -f makefile.lnx build
sudo make -f makefile.lnx install
```

### Windows
Visual Studio project files are available for building on Windows. 
1. Open the `VS18` directory.
2. Open the `plt.sln` solution file in Visual Studio.
3. Build the solution using the Visual Studio IDE (`Build` > `Build Solution`).
# plt
