<CsoundSynthesizer>

; Id: D03_FILT03.CSD mg (2006, rev.2009)
; author: marco gasperini (marcogsp at yahoo dot it)

; G.M. Koenig
; ESSAY (1957)

<CsOptions>
-W -f -oD03_FILT03.wav
</CsOptions>

<CsInstruments>

sr     = 192000
kr     = 192000
ksmps  = 1
nchnls = 1

;=============================================
; 243.3 FILTERINGS
;=============================================
	instr 1
if1	= p4
if2	= p5
ibw	= p5-p4
ifc	= if1+(ibw*.333)

ifile	= p6

a1	diskin2 ifile, 1

afilt	atonex a1, if1
afilt	tonex afilt*7, if2

	out afilt
	endin
;=============================================

</CsInstruments>
<CsScore>
t0	4572	; 76.2 cm/sec. tape speed (durations in cm)	

;			p4	p5	p6
;			if1	if2	ifile
i1	0	865.7	400	800	"D02_RM03.wav"

e

</CsScore>
</CsoundSynthesizer>