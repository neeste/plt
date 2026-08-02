# PLT User Guide

**Boys Town National Research Hospital**
*Stephen Neely & Edward Carney*

## Abstract

*(Note: This is an updated Markdown conversion of the PLT User's Guide, reflecting modernized installation instructions and incorporating details from the Windows Help file.)*

PLT transforms a text file containing keywords, parameters, and data that describe a graph into output compatible with several soft and hard copy graphics devices. Keywords and parameters are described in this document, and examples of input text files are provided along with their resulting graphs. Graphics formats supported by PLT include PostScript, EPS, EMF, SVG, and CGM, in addition to onscreen displays.

---

## Table of Contents
1. [Installation](#installation)
2. [Introduction](#introduction)
3. [Keywords](#keywords)
4. [Auxiliary Files](#auxiliary-files)
5. [Numeric Parameters](#numeric-parameters)
6. [String Parameters](#string-parameters)
7. [Messages](#messages)
8. [Macros](#macros)
9. [Functions](#functions)
10. [Internal Variables](#internal-variables)
11. [External Fonts](#external-fonts)
12. [Binary Data](#binary-data)
13. [Special Characters](#special-characters)
14. [Symbols, Line Types, Area Fill Patterns, and Colors](#symbols-line-types-area-fill-patterns-and-colors)
15. [Arithmetic Expressions](#arithmetic-expressions)

---

## Installation

### Windows
Visual Studio project files are available for building on Windows. 
1. Open the `VS18` directory.
2. Open the `plt.sln` solution file in Visual Studio.
3. Build the solution using the Visual Studio IDE (`Build` > `Build Solution`).
4. Run `Setup` if utilizing the packaged installer.

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

---

## Introduction

The PLT program generates plots that are defined by pltfiles. Pltfiles are ASCII files with a keyword, a transfer of control, a numeric parameter, a string parameter, a message, a macro, or data values on each line.

The maximum number of characters on any input line is 1024. The maximum number of data columns is 64.

### Keywords

Keywords have no arguments or values associated with them, but cause an action or change of mode. Only the first four characters are necessary to specify a keyword.

- **DATA**: Empties the data list.
- **DATLIST**: Prints contents of data buffer. An optional filename may be specified.
- **EXIT**: Immediately terminates the program.
- **FILTER**: Removes data values that are not within the range specified by XMIN, YMIN, XMAX, and YMAX.
- **FINISH**: Terminates and closes plot file.
- **KEEP**: Plots data and keeps data for re-plotting.
- **LOAD**: Append saved data to current data.
- **NEWFRAME**: Begin a new plot frame on the current page.
- **NEWPAGE**: Advances to a new page.
- **NEWRANGE**: Begin a new range of X and/or Y values in the current frame.
- **PALETTE**: Uses data to set RGB color palette.
- **PLAY**: Interprets data as musical notes.
- **PLOT**: Plots data and frame if necessary. Empties data list.
- **PRMLIST**: Prints out the current parameter values.
- **RESET**: Sets all parameters to their default values.
- **SAVE**: Save and clear data for later access by xsav, ysav, and zsav functions.
- **STAT**: Updates internal variables that describe statistics related to the data.
- **UWPH**: Unwrap phase by minimizing slope (assumes cycle=1).
- **VARLIST**: Prints out the current user variable values. An optional filename may be specified.

### Auxiliary Files

These commands have a file name as an argument. If "." does not appear in the filename then a default extension is added.

- **BDATA**: Includes specified file as a binary data file.
- **CALL**: Transfers control to another file.
- **INCLUDE**: Includes specified file as a ".plt" file.
- **STDFNT**: Load external font file as the standard font.
- **ALTFNT**: Load external font file as the alternate font.
- **SYMFNT**: Load external font file as the symbol font.

### Numeric Parameters

Numeric parameters are followed by "=" and either a number or a word. Only the first four characters are necessary to specify a parameter name. Default values are given below after each parameter name. The numbers in brackets refer to Notes at the end.

- **ANNCOL** (Default: -1): Annotation color. [6]
- **ANNLWT** (Default: 1): Annotation line weight. [1]
- **ANNSIZ** (Default: 1): Annotation size. [2]
- **ARONUM** (Default: 0): Arrowhead drawn at last position (1), first position (-1), or both (2). [9]
- **AROSIZ** (Default: 1): Arrowhead size. [2]
- **AROTIP** (Default: 0.1): Thickness of arrowhead tip (typically between 0 and 1). [9]
- **AROWID** (Default: 0.5): Width of arrowhead (typically between 0 and 1).
- **AXCOL** (Default: -1): Axes color. [6]
- **AXLWT** (Default: 1): Axes line weight. [1]
- **BGCOL** (Default: -1): Background color. [6]
- **CDEXT** (Default: 0): Extend cumulative distribution to XMIN and XMAX. [3]
- **CLIP** (Default: 0): Clip lines at frame boundary. [3]
- **CORNER** (Default: 1): Draw axis frame corners when XPERCENT or YPERCENT are less than 100. [3]
- **COUNT** (Default: 0): Use YDATA for cumulative distribution counts. [3]
- **EBNUM** (Default: 2): Number of error bars: 0 = none, 1 = upper bar only, 2 = both upper and lower, -1 = lower bar only.
- **EBWID** (Default: 1): Width of error bars scale factor. [2]
- **ECHO** (Default: 0): Echo each input line read. [3]
- **FGCOL** (Default: -1): Foreground color. [6]
- **FILCOL** (Default: -1): Fill color for symbols 10-19. [8]
- **FNCOL** (Default: -1): Fountain color. Specifies a different background color at the top of the page which gradually changes to BGCOL toward the bottom of the page. [6]
- **GAPSIZ** (Default: 1): Controls size of gap between line segments for plotting symbols. [2]
- **GRDCOL** (Default: -1): Grid color. [6]
- **GRDLWT** (Default: 1): Grid line weight. [1]
- **GRDSIZ** (Default: 1): Grid line pattern size. [7]
- **GRDTYPE** (Default: 1): Grid line type: 0 = solid lines, 1 = dotted lines, >1 = dashed lines.
- **GRID** (Default: 0): Plot grid lines at major intervals. [3]
- **HEADER** (Default: 1): Date, time, filename and page number printed at bottom of plot. [3]
- **LABCOL** (Default: -1): Label color. [6]
- **LABLWT** (Default: 1): Label line weight. [1]
- **LABSIZ** (Default: 1): Label size. [2]
- **LINSIZ** (Default: 1): Plot line pattern size. [7]
- **LINTYPE** (Default: 0): Plot line type: 0 = solid lines, 1 = dotted lines, >1 = dashed lines.
- **MAGNIFY** (Default: 1): Magnify entire plot by this scale factor.
- **MHALIGN** (Default: 0): Interpret MXLLC as a position for alternate message string horizontal alignment: 0=Normal, 1=Left, 2=Center, 3=Right.
- **MHKEY** (Default: 3): Controls horizontal size of shade boxes and lines in messages and axis labels. [2]
- **MHSP** (Default: 0): Controls horizontal spacing between characters in messages relative to their height.
- **MHXP** (Default: 1): Controls horizontal expansion of characters in messages.
- **MSGANGLE** (Default: 0): Message angle (degrees counterclockwise from x-axis).
- **MSGCOL** (Default: -1): Message color. [6]
- **MSGDAT** (Default: 0): Interpret MXLLC and MYLLC in coordinate system of the data instead of inches relative to XLLC and YLLC. [3]
- **MSGLWT** (Default: 1): Message line weight. [1]
- **MSGSIZ** (Default: 1): Message size. [2]
- **MSLANT** (Default: 0): Italic slant of characters in messages in degrees clockwise from upright.
- **MVALIGN** (Default: 0): Interpret MYLLC as a position for alternate message string vertical alignment: 0=Normal, 1=Top, 2=Cap, 3=Half, 4=Base, 5=Bottom.
- **MVKEY** (Default: 1): Controls vertical size of shade boxes and symbols in messages. [2]
- **MVSP** (Default: 1.64): Controls vertical spacing between lines in messages and axis labels.
- **MXDAT** (Default: 0): Interpret MXLLC in coordinate system of the data instead of inches relative to XLLC. [3]
- **MXLLC** (Default: 0): Message location in the x direction (inches).
- **MYDAT** (Default: 0): Interpret MYLLC in coordinate system of the data instead of inches relative to YLLC. [3]
- **MYLLC** (Default: 0): Message location in the y direction (inches).
- **NDATA** (Default: 1): Number of times that each data line is read.
- **NORM** (Default: 1): Normalize cumulative and frequency plots to 1. [3]
- **OPENAX** (Default: 0): Leave the top and right side of axes open. [3]
- **PLTCOL** (Default: -1): Plot color. [6]
- **PLTLWT** (Default: 1): Plot line weight. [1]
- **PLTYPE** (Default: 3): Plot type: 1 = Symbols only, 2 = Lines only, 3 = Both symbols and lines, 4 = Histogram, 5 = Rectangles, 6 = Polygon, 7 = Wave, 8 = contiGuous rectangles, 9 = Cumulative, and 10 = Frequency. (Single letter is sufficient.)
- **RANSKP** (Default: 0): Right annotation skip. [5]
- **RAXIS** (Default: 1): Right axis: 1=on, 0=off.
- **RBREAK** (Default: 0): Right-axis break location (percent).
- **RCYCLE** (Default: 0): Number of logarithmic cycles in the right axis.
- **RINT** (Default: 5): Right-axis intervals for tick marks. [4]
- **RMAX** (Default: 0): Right-axis maximum value.
- **RMIN** (Default: 0): Right-axis minimum value.
- **ROTATE** (Default: 0): Rotate plot: 0 = no rotation, 1 = 90 degrees, 2 = 180 degrees, and 3 = 270 degrees.
- **RPERCENT** (Default: 100): Percent of right axis used to position the limits.
- **RSHIFT** (Default: 0): Right-axis shift of tick marks and annotations (percent).
- **RTICK** (Default: 1): Controls length of the tick marks on the right axis. [2]
- **SHADE** (Default: 0): Shade in the histogram bars: 0=Hollow, 1=Solid, (2-39)=Hatch. SHADE value 40 fills the area with a gray value between black and white (see SHDGRAY). SHADE values 41-79 are the same as 1-39 except that the interior is filled with gray before the hatch pattern and outline are drawn.
- **SHDCOL** (Default: -1): Shade color. [6]
- **SHDGRAY** (Default: 1): Gray area fill for SHADE values 41-79. The gray values range from 0 (black) to 1 (white).
- **SHDLWT** (Default: 1): Shade hatch line weight. [1]
- **SIZFAC** (Default: 0): Size factor. Controls size of everything. For negative values PLT does not change the sizes from frame to frame. Positive values instruct PLT to compute sizes as if average axis length was equal to SIZFAC.
- **SMOOTH** (Default: 0): Smooth the data. [3]
- **SOLID** (Default: 0): Solid symbols are produced if possible. [3]
- **SORT** (Default: 0): Sort data in order of increasing x values. [3]
- **SPLINE** (Default: 0): Draw a B-spline instead of connecting points for PLTYPE=2 or 3.
- **STEM** (Default: 0): Draw a stem plot instead of connecting points for PLTYPE=2 or 3. [3]
- **SYMANG** (Default: 0): Rotation angle of symbol (degrees counter-clockwise) relative to normal orientation.
- **SYMBOL** (Default: 1): Symbol represents the number of symbol to plot (0-31).
- **SYMSIZ** (Default: 1): Symbol size. [2]
- **TANSKP** (Default: 0): Top annotation skip. [5]
- **TAXIS** (Default: 1): Top axis: 1=on, 0=off.
- **TBREAK** (Default: 0): Top-axis break location (percent).
- **TCYCLE** (Default: 0): Number of logarithmic cycles in the top axis.
- **TICDIR** (Default: 0): Tick direction: value of "inward” produces tick marks pointing inward. Otherwise, they are outward.
- **TICSIZ** (Default: 1): Controls length of the tick marks on all axes. [2]
- **TINT** (Default: 5): Top-axis intervals for tick marks. [4]
- **TMAX** (Default: 0): Top-axis maximum value.
- **TMIN** (Default: 0): Top-axis minimum value.
- **TPERCENT** (Default: 100): Percent of top axis used to position the limits.
- **TSHIFT** (Default: 0): Top-axis shift of tick marks and annotations (percent).
- **TTICK** (Default: 1): Controls length of the tick marks on the top axis. [2]
- **VXLLC** (Default: 0): X dimension virtual page offset (inches).
- **VYLLC** (Default: 0): Y dimension virtual page offset (inches).
- **WXLEN** (Default: 10.5): X dimension virtual page length (inches).
- **WXLLC** (Default: 10.5): X dimension virtual page offset from lower-left corner (inches).
- **WYLEN** (Default: 8): Y dimension virtual page length (inches).
- **WYLLC** (Default: 8): Y dimension virtual page offset from lower-left corner (inches).
- **XANSKP** (Default: 0): X-axis annotation skip. [5]
- **XAXIS** (Default: 1): X axis: 1=on, 0=off.
- **XBREAK** (Default: 0): X-axis break location (percent).
- **XCYCLE** (Default: 0): Number of logarithmic cycles in the x-axis.
- **XGAIN** (Default: 1): Scale factor applied to X data before plotting.
- **XINT** (Default: 5): X-axis intervals. [4]
- **XLEN** (Default: 6): X-axis length (inches).
- **XLLC** (Default: 2.2): Location of lower-left corner of axes in x direction (inches).
- **XMAX** (Default: 10): X-axis maximum value.
- **XMIN** (Default: 0): X-axis minimum value.
- **XOFST** (Default: 0): Offset applied to x data before plotting.
- **XPERCENT** (Default: 100): Percent of x axis used to position the limits.
- **XSHIFT** (Default: 0): X-axis shift of tick marks and annotations (percent).
- **XTICK** (Default: 1): Controls length of the tick marks on the bottom axis. [2]
- **YANSKP** (Default: 0): Y-axis annotation skip. [5]
- **YAXIS** (Default: 1): Y axis: 1=on, 0=off.
- **YBREAK** (Default: 0): Y-axis break location (percent).
- **YCYCLE** (Default: 0): Number of logarithmic cycles in the y axis.
- **YGAIN** (Default: 1): Scale factor applied to Y data before plotting.
- **YHOR** (Default: 0): Place the labels on the y axis horizontally. [3]
- **YINT** (Default: 5): Y-axis intervals. [4]
- **YLEN** (Default: 6): Y-axis length (inches).
- **YLLC** (Default: 1.5): Location of lower-left corner of axes in y direction (inches).
- **YMAX** (Default: 10): Y axis maximum value.
- **YMIN** (Default: 0): Y axis minimum value.
- **YOFST** (Default: 0): Offset applied to y data before plotting.
- **YPERCENT** (Default: 100): Percent of y axis used to position the limits.
- **YSHIFT** (Default: 0): Y-axis shift of tick marks and annotations (percent).
- **YTICK** (Default: 1): Controls length of the tick marks on the left axis. [2]
- **ZGAIN** (Default: 1): Scale factor applied to z data before plotting.
- **ZOFST** (Default: 0): Offset applied to z data before plotting.

______________________________

Note 1: Negative values select absolute line weights, positive values multiply the default line weight, and zero eliminates the lines.

Note 2: Positive values are used as multipliers of default size. Negative values are used as absolute size in inches.

Note 3: YES = Y = 1 and NO = N = 0.

Note 4: Intervals for tic marks are of the form MM.N where MM and N are the number of major and minor intervals respectively. Zero value indicates axis line with no tick marks.

Note 5: If set to zero no annotations are to be skipped. Positive values indicate the number of annotations to be skipped. The first digit after the decimal point indicates an offset to the first annotation not skipped. Negative values produce no annotations.

Note 6: The interpretation of color is device dependent. A small positive value selects a pen number (1-9) on pen plotter devices (HPGL and Tektronix) and selects a predefined color index (0-15) on other devices that support color output (PC, PostScript, and CGM). A negative value for FGCOL, BGCOL, or FNCOL indicates that device default colors should be used. A negative value for all other color parameters indicates that the color should be set to the same value as FGCOL.

Note 7: The line pattern size controls the length of the dash plus space for line types greater than (or equal to) 1. Negative values select absolute pattern size in inches, positive values multiply the default pattern size.

Note 8: A negative value for this parameter indicates the fill color should be the same value as PLTCOL. A positive value is an index into the PLT color palette.

Note 9: Arrowheads are only drawn for PLTYPE=2 (lines). If AROTIP is greater than 0, then arrowhead will be filled with color specified by SHDCOL.

### String Parameters

String parameters are followed by an equal sign and a string. Only the first four characters are necessary to specify a parameter name. The default value of all string parameters is a NULL string.

- **RANNOT**: Right-axis annotation.
- **RFMT**: Right-axis annotation format.
- **RLABEL**: Right-axis label.
- **SELECT**: Logical expression specifying selection of data rows.
- **TANNOT**: Top-axis annotation.
- **TFMT**: Top-axis annotation format.
- **TLABEL**: Top-axis label.
- **XANNOT**: X-axis annotation.
- **XDATA**: X data definition.
- **XFMT**: X-axis annotation format.
- **XLABEL**: X-axis label.
- **YANNOT**: Y-axis annotation.
- **YDATA**: Y data definition.
- **YFMT**: Y-axis annotation format.
- **YLABEL**: Y-axis label.
- **ZDATA**: Z data definition.

The parameters XDATA, YDATA, and ZDATA are interpreted as arithmetic expressions that may include column numbers, which are specified as $1, $2, …, $64.

### Messages

Messages are quoted strings, which may be several lines long. The position of the message may be specified immediately preceding the open quote on the first line of the message or by setting the parameters MXLLC and MYLLC.

### Macros

Macros are invoked by a line that begins with "%macro_name" followed by up to 99 arguments (delimited by spaces). There are five internal macros, %define, %undefine, %repeat, %for, and %msg.

**%define <macro_name>**

New macros may be defined using the %define macro. The first argument of %define is the new macro name (without the % character) and the second argument may contain the body of the new macro. If the %define macro has no second argument, the body of the new macro will be obtained from the succeeding lines of the pltfile up to a line that begins with “%%". Occurrences of $1, $2 ... $99 within the body of the macro are replaced by arguments when the new macro is invoked.

**%undefine <macro_name>**

The %undefine macro removes the macro definition.

**%msg <x> <y> <format> …**

Formatted messages are produced by the %msg macro. The first two arguments are the x and y locations used to located the message on the plot. The third argument is a format string, which may contain C-style, floating-point format specifications embedded in text. For each floating-point specification in the format string, there should be an additional argument with an arithmetic expression for the value of the corresponding floating-point number.

**%repeat <count>**

The %repeat macro is similar to the %define macro except that the first argument is a repeat count instead of a new macro name. Any occurrence of $0 in the macro body will be replaced by an index of the current iteration.

**%for <item> …**

The %for macro is similar to the %repeat macro except that the number of repetitions is determined by the number of arguments on the first line. Any occurrence of $0 in the macro body of %for will be replaced by an index of the current iteration. Any occurrence of $1 in the body of %for will be replaced by the corresponding argument on the first line.

In addition to these internal macros, PLT has one pre-installed macro. A pre-stalled macro differs from an internal macro in that its definition can be replaced by a new definition or removed by %undefine.

**%straline <slope> <y_intercept> <lintype> <x1> <x2>**

draws a line between x1 and x2 with the specified slope, y_intercept, and lintype, also assigns to the variables a & b the y-values at x1 & x2.

### Functions

Any macro that is defined by a single arithmetic statement may also be used as a function within another arithmetic statement. Functions are invoked by the function name followed by parentheses. Any function arguments are delimited by commas within the parentheses. In addition to user-defined functions, there are several internally defined functions:

- **abs(x)**: absolute value of x
- **atan(x)**: arctangent of x
- **atanh(x)**: inverse hyperbolic tangent of x
- **ceil(x)**: least integer greater than x
- **cos(x)**: cosine of x
- **cms(x,y)**: complex magnitude squared
- **cmr(x,y,u,v)**: complex multiply real part
- **cmi(x,y,u,v)**: complex multiply imaginary part
- **cdr(x,y,u,v)**: complex divide real part
- **cdi(x,y,u,v)**: complex divide imaginary part
- **erf(x)**: error function of x
- **exp(x)**: natural number e to the power of x
- **floor(x)**: greatest integer less than x
- **ifelse(c,x,y)**: returns x if c is non-zero, otherwise returns y
- **limit(a,x,b)**: returns a if x is less than a, returns b if x is greater than b, otherwise returns x
- **ln(x)**: natural logarithm of x
- **log(x)**: base 10 logarithm of x
- **max(x,y,...)**: maximum value of x, y, ...
- **min(x,y,...)**: minimum value of x, y, ...
- **round(n)**: nearest integer
- **select(n,...)**: return the nth argument after the first argument
- **sin(x)**: sine of x
- **sqrt(x)**: square-root of x
- **tan(x)**: tangent of x
- **tanh(x)**: hyperbolic tangent of x
- **xdat(i)**: returns the i-th value from the x-data list
- **xsav(i)**: returns the i-th value from the x-save list
- **ydat(i)**: returns the i-th value from the y-data list
- **ysav(i)**: returns the i-th value from the y-save list
- **zdat(i)**: returns the i-th value from the z-data list
- **zsav(i)**: returns the i-th value from the z-save list
- **zdev(p)**: converts percentile score to z-score
- **ztop(z)**: converts z-score to percentile score

### Internal Variables

Internal variables values can be used in arithmetic expressions. These variables are identified by names that begin with ``$''. Most of the internal variables represent statistics that are computed from the data when the keyword STAT is used:

- **$corr_xy**: correlation between x-data and y-data values
- **$lr_a**: linear-regression coefficient a in y=a+bx
- **$lr_b**: linear-regression coefficient b in y=a+bx
- **$lr_c**: linear-regression coefficient c in x=c+dy
- **$lr_d**: linear-regression coefficient d in x=c+dy
- **$n**: number of data values
- **$sum_x**: sum of x-data values
- **$sum_xx**: sum of x-data values squared
- **$sum_xy**: sum of x-data values times y-data values
- **$sum_y**: sum of y-data values
- **$sum_yy**: sum of y-data values squared
- **$x_max**: maximum value of x-data
- **$x_mean**: mean value of x-data
- **$x_min**: minimum value of x-data
- **$x_sd**: standard deviation of x-data values
- **$y_max**: maximum value of y-data
- **$y_mean**: mean value of y-data
- **$y_min**: minimum value of y-data
- **$y_sd**: standard deviation of y-data values

There are three additional internal variables:

- **$device**: number of the current output device
- **$random**: random number, uniformly distributed between zero and one.
- **$x**: current x-data value, useful for defining y=f($x)
- **$y**: current y-data value
- **$z**: current z-data value

### External Fonts

The internal definitions that plt program normally uses for symbols and text characters can be replaced by loading external definitions from plt font files. Use SYMFNT to redefine symbols, STDFNT to redefine the standard character set and ALTFNT to replace the alternate character set (usually Greek letters). The plt font files are created and modified by a separate program called PLTFNT.

### Binary Data

Plt can read binary data files using the BDATA command. The binary data file must have a short header followed by integer data values. All integer values are assumed to stored in the file with the least-significant byte first. (This is the standard integer storage on the PC.) The header contains four two-byte integers: (1) a magic number for identification = 12345 decimal, (2) the number of bytes per integer value in the data (must be 2), (3) the number of coordinate values (x, y, z) to be specified for each data point (must be 1, 2, or 3), and (4) the number of data points in the file.

### Special Characters

Some characters have special meaning in messages or labels.

- **[str]**: Subscript str.
- **{str}**: Superscript str.
- **^str^**: Shift to alternate font for str.
- **|str|**: If str is null or "*", then the current symbol is inserted. If str is a number (0-31), then the symbol with the corresponding number is inserted. If the number is preceded by an '_' then a line with corresponding line type is inserted. If the number is preceded by a '#' then a box with corresponding shade type is inserted. Otherwise, str and the surrounding "|" are taken literally.
- **$**: Replace "$" in string with space.
- **;**: Begin comment.
- **!**: Begin comment and write comment to error output.
- **\c**: Insert character c without special meaning indicated above, where c is [ ] { } ^ $ | ; ! " or \.
- **\-**: Hyphen (short dash).
- **\**: Half space (backslash – space).
- **\h**: Half back space.
- **\b**: Full back space.
- **\.**: Set “dot” position.
- **\r**: Return to “dot” position.

### Symbols

- ***Symbol*** (Default: *Shape*): *Symbol*
- **0** (Default: Open square): 10
- **1** (Default: Open circle): 11
- **2** (Default: Open triangle): 12
- **3** (Default: Open Inverted triangle): 13
- **4** (Default: Open diamond): 14
- **5** (Default: Hourglass): 15
- **6** (Default: 8-pt Asterisk): 16
- **7** (Default: Plus sign): 17
- **8** (Default: 'Picnic table'): 18
- **9** (Default: Up-pointing arrow): 19

### Line Types

- ***Lintype***: *Description*
- **0**: Solid line
- **1**: Dotted line
- **2 to 9**: Dashed lines (dash:space ratio is appx. Lintype minus 1)

### Area Fill Patterns

- ***Shade***: *Description*
- **0**: Open (unfilled) with outline
- **1**: Solid (filled) with no outline (see Shdcol)
- **2 to 4**: Narrow cross-hatch with no outline
- **5 to 10**: Narrow single-hatch with no outline
- **11 to 13**: Wide cross-hatch with no outline
- **14 to 19**: Wide single-hatch with no outline
- **20**: Background-filled with outline (see Bgcol)
- **21 to 39**: Same as 1 to 19 except with outline
- **40**: Gray-filled with no outline (see Shdgray)
- **41 to 79**: Same as 1 to 39 except with gray background (see Shdgray)
- **80**: Background-filled with no outline (see Bgcol)
- **81 to 119**: Same as 1 to 39 except with filled background (see Bgcol)

### Standard Colors

- ***Index*** (Default: *Description*): *Index*
- **0** (Default: black): 8
- **1** (Default: blue): 9
- **2** (Default: green): 10
- **3** (Default: cyan): 11
- **4** (Default: red): 12
- **5** (Default: magenta): 13
- **6** (Default: brown): 14
- **7** (Default: lt. gray): 15

### Arithmetic Expressions

The following tables lists the operators supported by PLT in the order of precedence (from highest to lowest).

- **operator**: operation
- **~**: logical negate
- **^**: exponentiation
- *** /**: multiplication, division
- **+ -**: addition, subtraction
- **< <= > >=**: less than, greater than
- **== ~=**: equals, not equals
- **&**: logical and
- **|**: logical or

Arithmetic expressions may also include the name of any numeric parameter, internal variable, internal function, or single-line, user-defined macro. The value of any numeric parameter may be specified as an arithmetic expression. When specifying a numeric parameter, if the first character is “n” or “y”, the value is taken to be 0 or 1, respectively. Certain numeric parameters (mhalign, mvalign, pltype, and ticdir) give additional special meaning to the first character.

Several string parameters (xdata, ydata, zdata, and select) may also be specified by arithmetic expressions. The evaluation of these expressions is deferred until data is read. Data columns are referenced within these expressions as $1, $2, …, $64. Data rows may be conditionally included by a specifying a logical expression for the select parameter.

### Command-Line Options

The command-line options are:

- **-e**: Causes the input file to be echoed on the error output.
- **-cm**: Use centimeters instead of inches as the basic unit.
- **-mm**: Use millimeters instead of inches as the basic unit.
- **-m0**: Output to stdout in Tektronix 4663 plotter language.
- **-m1**: Output to stdout for Tektronix 4010 terminal.
- **-m2**: Output to stdout in Impress language.
- **-m3**: Output to stdout in HPGL graphics language.
- **-m4**: Output directly to a window (default).
- **-m5**: Output encapsulated PostScript to stdout.
- **-m6**: Output CGM (computer graphics metafile) to stdout.
- **-v**: Print PLT program version.

### What’s New?

Version 5.16 12-Apr-20

Cleaned up for MacOS Exclude Nan data

Version 4.84 7-May-07

Added keywords save and load Added functions round, xsav, ysav, and zsav

Version 4.82 14-Mar-07

EPS export is rotated STEM parameter selects stem plots
