/* version.h */

#define REV	"Version: 6.00, Date: 2-Aug-2026"
#define COPY	"Copyright 1988-2026"
#define BTNRH	"Boys Town National Research Hospital"
#define RIGHTS	"Non-profit redistribution permitted."

/******************************************************************
        Changes to PLT since version 4.0
*******************************************************************
 Revision 6.00 1-Aug-2026
 * Future proof code with Gemini
 Revision 5.16 17-Apr-20
 * Fixed memory allocation bug in ldfont function
 * Fixed CGM color bug in d6.c
 * Replaced long with int32_t in d7.c and emf.h
 Revision 5.15 23-Mar-20
 * Cleaned up for Xcode
 * Renamed move & draw functions in md.c to avoid global conflict
 * Renamed variable inch & function get_str to avoid global conflict
 Revision 5.14 26-Feb-20
 * Added detection and exclusion of NaN data
 * Cleaned up for VS18
 Revision 5.13 9-Dec-14
 * Fixed bug (removing leading spaces) in nxtlin
 Revision 5.12 9-Nov-14
 * Fixed bug in PostScript Bounding Box
 Revision 5.11 16-Jun-14
 * Fixed bug in parsing macro args
 Revision 5.10 5-Jun-14
 * Increase # args of macros and functions (MAXNARGS) from 8 to 64
 Revision 5.09 3-Dec-13
 * Eliminate tick marks and annotations beyond frame
 Revision 5.08 3-Aug-13
 * Fixed another lindot bug
 Revision 5.07 25-Jun-13
 * Fixed lindot bug in text
 Revision 5.06 25-Jun-13
 * Added parameter lindot
 Revision 5.05 18-Dec-12
 * Fixed memory access bug in mac_install
 Revision 5.04 5-Oct-12
 * Cleaned up (out & tplt) for Xcode
 Revision 5.03 29-Sep-12
 * Added "merge include" option -i
 Revision 5.02 18-Aug-12
 * Match arrow color to line color
 Revision 5.01 27-Oct-11
 * Increased ctext_.mesg buffer size to avoid crash
 * Corrected $$$d syntax in %for macro
 * Added sequence syntax to %for macro
 Revision 5.00 25-Jun-11
 * Converted Help to HTML
 Revision 4.99 8-Mar-10
 * Suppressed compiler error about read in opnout.c
 Revision 4.98 12-Sep-09
 * Cleaned up for Mac
 Revision 4.97 2-Aug-09
 * Adjusted EMF frame to equal Bounds with 1-mm margin
 * Fixed display for iPhoneOS 3.0
 Revision 4.96 3-Mar-09
 * Reset axes info only when page is new (bug fix)
 * Add EPS rotate to command-line options
 * Add usage to command-line options
 Revision 4.95 19-Nov-08
 * Save axes info only when frame is new
 * Save/Restore graphics state on Mac fill
 * Build plt-mac for OSX 10.4
 Revision 4.94 30-Oct-08
 * Fixed black background in EMF file
 * Fixed bug in log-file name
 Revision 4.93 13-Oct-08
 * Implemented "play" for Windows version
 Revision 4.92 10-Oct-08
 * Zoom, reset, and pan with mouse
 * Added page,frame,data modes to info_line
 Revision 4.91 26-Sep-08
 * Prepend ellipsis to shortened filename in header
 * Modify get_str for Mac
 Revision 4.90 16-Sep-08
 * Adjusted Windows zoom
 * Added Mac features
 Revision 4.89 7-Sep-08
 * Modified for Mac
 Revision 4.88 14-Mar-08
 * Exit after fatal error (in Windows)
 Revision 4.87 24-Feb-08
 * Fixed bug reading data with letters appended to numbers
 Revision 4.86 27-Jul-07
 * Put limit on x,y range in PostScript export
 * Fix histogram for ymin>0
 Revision 4.85 17-Jul-07
 * Fixed update bug after print
 Revision 4.84 7-May-07
 * Added keywords save and load
 * Added functions round, xsav, ysav, & zsav
 Revision 4.83 22-Apr-07
 * Fixed log stem plot
 Revision 4.82 22-Mar-07
 * EPS output is rotated
 * Added STEM parameter
 Revision 4.81 11-Sep-06
 * Tweaked right-click zoom feature
 Revision 4.80 3-Jun-06
 * Fixed SVG header for Firefox
 Revision 4.79 11-Oct-05
 * minor change to EPS BoundingBox
 Revision 4.78 18-Aug-05
 * limit density of line plots
 * use circles for dotted lines
 * shift-right-click to zoom out
 Revision 4.77 10-Apr-05
 * Allow clear background
 Revision 4.76 28-Mar-05
 * Fixed X window version
 Revision 4.75 7-Feb-05
 * Reverted d4win32.c to version 4.66 to fix Win98
 * Fixed spelling of ARONUM parameter
 * Pre-installed straline macro
 * Fixed shortcut list in wplt menus
 * Added Windows display zoom function
 * Fixed bounding-box bug in EMF
 * Improved "frequency" plot type
 * Added likelihood parameter
 Revision 4.74 31-Dec-04
 * Used Bezier curves to outline symbols
 * Enhanced CGM dots
 * Improved win32 clipping and curves
 * Improved EMF fills and curves
 * Added SVG export
 Revision 4.73 16-Dec-04
 * Use Bezier curves to represent text arcs
 * Add d_curv function to PostScript device
 * Strip path from Windows title
 * Improve tpcl
 Revision 4.72 9-Dec-04
 * Improved positioning of labels with subscripts
 Revision 4.71 6-Dec-04
 * Fixed paging bug by clearing putback buffer
 * Removed unused flags
 * Fixed replot on second file open
 Revision 4.70 18-Nov-04
 * Fixed functions cmr, cmi, cdr, cdi
 Revision 4.69 13-Sep-04
 * Added modulo operator "%"
 Revision 4.68 12-Jul-04
 * Added "select" function
 Revision 4.67 9-Jun-04
 * Fixed CGM export bug
 * Changed EMF export to "fillwhite"
 Revision 4.66 5-Apr-04
 * Fixed user variable parsing bug
 Revision 4.65 17-Mar-04
 * Added support for EMF file output
 Revision 4.64 13-Dec-03
 * Added support for DJGPP C compiler
 Revision 4.63 13-Aug-03
 * Fixed auto-update feature
 Revision 4.62 28-Jul-03
 * Adjusted position of filename in plot "header".
 * Use header parameter values (2-63) to suppress various header elements
 * Reduce page flipping when editing mulit-page plots
 * Allow page navigation using Home, End, PgUp, & PgDn
 * Automatic update when pltfile is modified
 * Eliminate page increment on empty pages
 Revision 4.61 22-May-03
 * Added functions tanh & atanh.
 Revision 4.60 06-Apr-03
 * Changed "aroend" to "aronum"
 * Windows: prompt when messages are written to log file.
 * Windows: display cursor coordinates when left button pressed.
 Revision 4.59 15-Mar-03
 * Fixed bug in repeated CGM export
 * Added parameters for arrows: AROEND,AROSIZ,AROTIP,AROWID
 Revision 4.58 20-Feb-03
 * Allow user variable names to be 4 characters long
 * Open include files relative to current PLT directory
 Revision 4.57 17-Feb-03
 * Corrected aspect ratio for Windows full-screen mode
 * Allow spaces in included filenames
 Revision 4.56 6-Feb-03
 * Corrected openax to be overrriden when top or right are set
 * Redefined parameter NORM to normalize to other than one
 * Extended endpoints on cumulative distribution
 Revision 4.55 18-Sep-02
 * Added "Batch Print" function to wplt
 Revision 4.54 22-Feb-02
 * Added "for" macro
 Revision 4.53 15-Jan-02
 * Fixed bug that incorrectly filled open symbols
 * Look at environment variable PLT_EDIT for editor
 Revision 4.52 5-Nov-01
 * Added special characters \b, \h, \r, & \.
 Revision 4.51 19-Jun-01
 * Add date to error log
 Revision 4.50 24-May-01
 * Change outfile to return FALSE if file fails to open
 Revision 4.49 10-Mar-01
 * Add "select" parameter to specify a subset data rows
 * Add "copy" and "error log" functions to "view" menu
 * Remove plt.log when opening file
 Revision 4.48 6-Mar-01
 * Fix bug affecting EPS output under Windows
 Revision 4.47 27-Feb-01
 * Fix bug affecting CGM output under Windows
 Revision 4.46 12-Feb-01
 * Allow parameter changes within symbol keys
 Revision 4.45 9-Feb-01
 * Add parameter FILCOL to specify fill color of symbols 10-19
 Revision 4.44 1-Feb-01
 * Fix EPS BoundingBox in WPLT
 Revision 4.43 5-Jan-01
 * Fixed cumplt for npts=1
 Revision 4.42 19-Oct-00
 * Added keyword "uwph" to unwrap phase
 * Changed "Save As" to "Export" in WPLT
 Revision 4.41 2-Aug-00
 * Fix "Save As" dialog for WPLT
 Revision 4.40 3-Jul-00
 * suppress Print dialog when no PLT file in WPLT
 Revision 4.39 22-Jun-00
 * implement Save As function in WPLT
 Revision 4.38 19-Jun-00
 * fix WPLT printing under both Win98 and Win2000
 Revision 4.37 17-Mar-00
 * fix font finding bug by not prepending path of executable in getfn()
 Revision 4.36 1-Nov-99
 * added upper-case phi to alternate font
 Revision 4.35 11-Feb-99
 * added complex math functions: cms, cmr, cmi, cdr, cdi
 Revision 4.34 28-Jan-99
 * removed forced "open" dialog when no filename
 * (source code lost for previous a "4.34" version dated 12-Jan-99)
 Revision 4.33 26-Aug-98
 * fixed function name parsing in lookup_function()
 Revision 4.32 17-Aug-98
 * fixed bug in win32 selection of pen for polyline
 * doubled dev_adupi for win32 to improve arc smoothness.
 Revision 4.31 6-Aug-98
 * fixed bug in updating color of lines
 Revision 4.30 18-Jun-98
 * Improved Print command in windows version.
 * Added "Reset Aspect" command
 * Added default path to "include" file names
 Revision 4.29 5-Jun-98
 * Added Print command to windows version.
 Revision 4.27 12-May-98
 * WIN32 improvements: window resize and notepad edit
 Revision 4.26 11-May-98
 * WIN32 improvements: fixed memory leak, added File/Open
 * remove ANSI_C define from output.h and makefile.*
 Revision 4.25 13-Mar-98
 * outline SHADE=60
 Revision 4.24 7-Feb-98
 * improvements to win32 support
 Revision 4.23 3-Nov-97
 * increased max number of macro args from 9 to 64
 Revision 4.22 29-Sep-97
 * free data memory before program exits
 Revision 4.21 14-Aug-97
 * Added WIN32 support
 Revision 4.20 26-Jun-97
 * Added pltype=10 for frequency distributions
 Revision 4.19 19-May-97
 * partial WIN32 support
 Revision 4.18 8-May-97
 * removed TSX code
 * cleaned up for lint
 * added support for Linux
 Revision 4.17 9-Mar-97
 * increased MAXLINE to 1024
 Revision 4.16 18-Oct-96
 * Added "\-" as special character hyphen.
 Revision 4.15 13-Oct-96
 * Added internal functions sqrt, floor, ceil, erf, and exp.
 Revision 4.14 3-Oct-96
 * Functionally same as 4.13c.
 * Linux support added to Unix version.
 * Arguments added to "signal" functions in opnout() (to avaoid warnings).
 Revision 4.13c 96/01/25
 * fix disappearing dots in CGM files
 Revision 4.13b 95/08/21
 * increased string length to avoid bug in datl & varl
 Revision 4.13a 95/05/02
 * added "EPSF-2.0" to first line of PostScript output
 Revision 4.13 95/04/03
 * fixed positioning of labels re annotations
 Revision 4.12 94/06/29
 * same as version 4.11m
 Revision 4.11m 94/06/28
 * modified cstat() to avoid FP emulator bug
 Revision 4.11l 94/05/21
 * added keyword "filter" to filter data
 Revision 4.11k 94/04/12
 * added automatic byte-swapping for external fonts
 * changed logic for break from dirlst search in opnfnt()
 Revision 4.11j 94/04/9
 * replaced "ifdef NEXT" with byte-order test
 * removed include macro %include
 Revision 4.11i 94/03/9
 * fixed PostScript output to limit stroke size to 1024
 * added deferred include macro %include
 * added internal variable $n = number of data points for statistics
 Revision 4.11h 94/02/28
 * added file name to datlist
 Revision 4.11g 94/01/24
 * undid changes to clip limits
 Revision 4.11f 94/01/21
 * changed clip limits to window size
 * added ch.sp. and ch.exp.fac. to labels (same as messages)
 Revision 4.11d 93/12/02
 * added include levels for push-back buffer
 Revision 4.11c 93/12/01
 * added clip=2 for clipping at xmin, xmax, ymin, ymax
 * improved cum. dist. plots
 Revision 4.11b 93/11/30
 * added functions ifelse() and ztop()
 * added relational operators < > <= >= ==
 Revision 4.11a 93/11/29
 * added pltype=9 for cumulative distributions
 Revision 4.11  93/10/26
 * Fixed PostScript wrap-around bug.
 * Added null def. for symbol 31.
 * Clip solid rectangles.
 Revision 4.10 93-07-23
* Same as 4.9i (copied from celnext)
 Revision 4.9i 93-07-05
* Bypassed symbol-in-text for characters used as symbols
 Revision 4.9h 93-06-24
* Allow standard and alternate characters to be used as symbols
 Revision 4.9g 93-06-04
* Allow expressions in definition of message keys
 Revision 4.9f 93-06-03
* Added internal functions: xdat(), ydat(), zdat()
 Revision 4.9e 93-06-02
* Fixed bug in internal variables $sum_xx, $sumyy, $sum_xy
 Revision 4.9d 93-
* Made end-of-page prompt sound dependent on PLT_BELL
 Revision 4.9a 92-12-22
* Fixed bug in CGM filled symbols
 Revision 4.9  92-12-21
* Same as 4.8f.  Zip files put in ftp archive
 Revision 4.8f 92-11-30
* Added internal functions min, max, and abs
 Revision 4.8e 92-10-02
* Fixed bug in evaluating $random
 Revision 4.8d
* Fixed bug in setting color of error bars
 Revision 4.8c
* Fixed bug in parsing of arithmetic expressions
 Revision 4.8b
* Fixed bug in setting color of symbols in messages
* Fixed bug in setting gray level of shaded areas
* Added solid color backgrounds to CGM output
 Revision 4.8a
* Added parameters xaxis,yaxis,taxis,raxis to turn off axes
 Revision 4.8  92/08/28
 * Added keyword "stat" to compute statisics
 * Added internal varibles for "stat" ($sum_x, $sum_y, ...).
 * Added built-in macro "MSG" for messages with formated data values.
 Revision 4.7  92/4/28
 * Fixed bug in clipping and filling symbols
 * Added user defined functions
 * Added syntax checking for arithmetic expressions
 * Processed all source code with "indent"
 Revision 4.6  91/11/15
 * Fixed bug in unary minus with exponent.
 * Added functions: sin,cos,tan,ln,log.
 * Added $x substitution in expressions.
 * Added keywords EXIT and PLAY.
 Revision 4.5  91/11/06
 * Increased MAXLINE to 512 and NCOLS to 64.
 Revision 4.4  91/06/21
 * Checks environment for PLT_FONTS.
 * Parses floating-point numbers without atof().
 * Avoids floating-point error when XMIN=XMAX.
 Revision 4.3  91/06/20
 * Allow parameter setting on command line.
 Revision 4.2  91/06/20
 * Fixed minor bug in log annotation.
 * Changed default label size to equal annotation size.
 * Wide strokes are default on PC.
 Revision 4.1  91/05/12
 * Fixed PostScript newpage bug.
 * Fixed PC floating-point bug.
 Revision 4.0  91/03/08
 * Distribution version.
******************************************************************/
