/* emf.h - header for EMF device */

// Background Modes
#define OPAQUE		2
#define TRANSPARENT	1

// Colors
#define RGB(r,g,b)	(((r)&0xFF)|(((g)&0xFF)<<8)|(((b)&0xFF)<<16))

// Enhanced Metafile Records
#define EMR_ABORTPATH		68
#define EMR_BEGINPATH		59
#define EMR_CLOSEFIGURE		61
#define EMR_CREATEBRUSH		39
#define EMR_CREATEPEN		38 
#define EMR_DELETEOBJECT	40 
#define EMR_ENDPATH		60
#define EMR_EOF			14
#define EMR_EXTCREATEPEN	95	
#define EMR_EXTSELECTCLIPRGN	75	
#define EMR_FILLPATH		62
#define EMR_FILLRGN		71
#define EMR_HEADER		1 
#define EMR_LINETO		54 
#define EMR_MOVETOEX		27 
#define EMR_POLYBEZIER16	85 
#define EMR_POLYLINE		87 
#define EMR_POLYLINETO		89
#define EMR_RECTANGLE		43 
#define EMR_SELECTOBJECT	37 
#define EMR_SETBKCOLOR		25
#define EMR_SETBKMODE		18
#define EMR_SETPOLYFILLMODE	19
#define EMR_STROKEPATH		64

typedef struct _EnhancedMetaHeader {
    int32_t  RecordType;       /* Record type */
    int32_t  Size;             /* Size of the record in bytes */
    int32_t  BoundsLeft;       /* Left inclusive bounds */
    int32_t  BoundsTop;        /* Top inclusive bounds */
    int32_t  BoundsRight;      /* Right inclusive bounds */
    int32_t  BoundsBottom;     /* Bottom inclusive bounds */
    int32_t  FrameLeft;        /* Left side of inclusive picture frame */
    int32_t  FrameTop;         /* Top side of inclusive picture frame */
    int32_t  FrameRight;       /* Right side of inclusive picture frame */
    int32_t  FrameBottom;      /* Bottom side of inclusive picture frame */
    int32_t  Signature;        /* Signature ID (always 0x464D4520) */
    short Version[2];       /* Version of the metafile */
    int32_t  FileSize;         /* Size of the metafile in bytes */
    int32_t  NumOfRecords;     /* Number of records in the metafile */
    int32_t  NumOfHandles;     /* Number of handles in the handle table */
    int32_t  SizeOfDescrip;    /* Size of description string in shorts */
    int32_t  OffsOfDescrip;    /* Offset of description string in metafile */
    int32_t  NumPalEntries;    /* Number of color palette entries */
    int32_t  WidthDevPixels;   /* Width of reference device in pixels */
    int32_t  HeightDevPixels;  /* Height of reference device in pixels */
    int32_t  WidthDevMM;       /* Width of reference device in millimeters */
    int32_t  HeightDevMM;      /* Height of reference device in millimeters */
    int32_t  Padding[5];       /* ??? */
} EMFHEADER;

typedef struct _EnhancedMetaRecord {
    int32_t  Function;         /* Function number (defined in WINGDI.H) */
    int32_t  Size;             /* Total size of the record in bytes */
    int32_t  P[256];           /* Parameter values passed to GDI function */
} EMFRECORD;

typedef struct _EndOfRecord
{
    int32_t  Function;         /* End Of Record ID (14) */
    int32_t  Size;             /* Total size of the record in bytes */
    int32_t  NumPalEntries;    /* Number of color palette entries */
    int32_t  OffPalEntries;    /* Offset of color palette entries */
    int32_t  OffToEOF;         /* Offset to beginning of this record */
} EMFEOF;

typedef struct{
    int32_t left, top, right, bottom;
} RECT;

typedef struct{
    int32_t x, y;
} POINT;
