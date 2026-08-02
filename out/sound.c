/* sound.c - play sound with specidified frequency and duration */

#ifdef WIN32

#include <conio.h>
#include <math.h>
#include <windows.h>
#include <mmsystem.h>

#define NBNK	    64
#define round(x)    ((int)((x)+0.5))

static int  head = 0;
static int  tail = 0;
static int  nbps = 2;	/* number of bytes per sample */
static int  ncda = 1;	/* number of channels of D/A */
static int  device_opened = 0;
static int  sound_enabled = 0;
static unsigned long rate = 44100;
static HWAVEOUT hWaveOut;
static MMRESULT Result;
static UINT cardNumber = 0;
static WAVEHDR whdro[NBNK];

static void
open_out()
{
    int i;
    WAVEFORMATEX  f;

    if (!device_opened) {
	memset(&f, 0, sizeof(f));
	f.cbSize = 0;
	f.nSamplesPerSec = rate;
	f.nChannels = ncda;
	f.wBitsPerSample = nbps * 8;
	f.nBlockAlign = ncda * nbps;
        f.nAvgBytesPerSec = (f.nBlockAlign * f.nSamplesPerSec);
        f.wFormatTag = WAVE_FORMAT_PCM;
	Result = waveOutOpen(&hWaveOut, cardNumber, (PWAVEFORMATEX)&f, 
	    0L, 0L, WAVE_ALLOWSYNC);
	/*
	 * MMSYSERR codes:
	 *  1 = unspecified error
	 *  2 = device ID out of range 
	 *  3 = driver enable failed 
	 *  4 = device already allocated 
	 *  6 = no device driver present
	 *  7 = memory allocation error
	 * 10 = invalied flag passed
	 * WAVERR codes:
	 * 32 = unsupported wave format 
	 */
	if (Result == 0) {
	    device_opened = 1;
	    for (i = 0; i < NBNK; i++) {
		whdro[i].lpData = NULL;
	    }
	    tail = head = 0;
        }
    }
}

static void
close_out()
{
    if (device_opened) {
	waveOutClose(hWaveOut);
	hWaveOut = 0;
	device_opened = 0;
    }
    tail = head = 0;
}

static int
busy_out(int b)
{
    if (whdro[b].lpData && (whdro[b].dwFlags & WHDR_DONE)) {
	free(whdro[b].lpData);
	waveOutUnprepareHeader(hWaveOut, &whdro[b], sizeof(WAVEHDR));
	whdro[b].lpData = NULL;
    }
    if (whdro[b].lpData == NULL) {
	tail = b;
	return (0);
    }
    return (1);
}

static void
wait_out(int b)
{
    while (busy_out(b)) {
        Sleep(10);
    }
}

static void
bank_out(short *s, int n, int b)
{
    whdro[b].lpData = (HPSTR) s;
    whdro[b].dwBufferLength = n * ncda * nbps;
    whdro[b].dwFlags = 0L;
    whdro[b].dwLoops = 0L;
    Result = waveOutPrepareHeader(hWaveOut, &whdro[b], sizeof(WAVEHDR));
    Result = waveOutWrite(hWaveOut, &whdro[b], sizeof(WAVEHDR));
    head = (b + 1) % NBNK;
}

static void
write_out(short *s, int n)
{
    if (device_opened) {
	wait_out(head);
	bank_out(s, n, head);
    }
}

void
sound_play(double frq, double dur)
{
    double w;
    int    i, m, n, d, e;
    short *s;
    static double amp = 32767 / 4;
    static double pi = 3.1415927;

    if (!sound_enabled)
        return;
    n = round(dur * rate);
    d = rate / 100;
    if (d > (n / 4)) {
	d = (n / 4);
    }
    m = n - d;
    e = m - d;
    s = (short *) calloc(n, sizeof(short));
    for (i = 0; i < n; i++) {
	if (i < d) {		    // ramp up
	    w = amp * (1 - cos((pi * i) / d)) / 2;
	} else if (i < e) {	    // full amplitude 
	    w = amp;
	} else if (i < m) {	    // ramp down
	    w = amp * (1 - cos((pi * (m - i)) / d)) / 2;
	} else {		    // zero amplitude
	    w = 0;
	} 
	s[i] = (i > m) ? 0 : round(w * sin(2 * pi * i * frq / rate));
    }
    open_out();
    write_out(s, n);
}

void
sound_wait()
{
    while (tail != head) {
	wait_out((tail + 1) % NBNK);
    }
    close_out();
}

void
sound_stop()
{
    waveOutReset(hWaveOut);
    sound_wait();
}

void
sound_enable(int e)
{
    if (device_opened) {
	sound_stop();
    }
    sound_enabled = e;
}

#else /* WIN32 */

void
sound_enable(int e)
{
}

void
sound_play(double frq, double dur)
{
}

void
sound_stop(void)
{
}

void
sound_wait(void)
{
}

#endif /* WIN32 */
