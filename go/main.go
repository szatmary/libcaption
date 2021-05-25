package main

func main() {

}

const (
	Rows = 15
	Cols = 32
)

type frameBufferChar struct {
	underline bool
	style     int
	char      string // TODO make this a rune
}

type Frame struct {
	status    byte
	timestamp float64
	// State
	underline bool
	style     int
	rollup    int
	row, col  int
	ccData    uint16

	// TODO add CC1-4 buffers
	front  [Rows][Cols]frameBufferChar
	back   [Rows][Cols]frameBufferChar
	active *[Rows][Cols]frameBufferChar
}

var parityTable = func() [128]byte {
	var table [128]byte
	bx := func(b, x int) byte { return byte(b << x & 0x80) }
	for i := 0; i < len(table); i++ {
		table[i] = byte((i & 0x7F)) | (0x80 ^ bx(i, 1) ^ bx(i, 2) ^ bx(i, 3) ^ bx(i, 4) ^ bx(i, 5) ^ bx(i, 6) ^ bx(i, 7))
	}
	return table
}()

func ParityByte(ccData byte) byte {
	return parityTable[0x7F&ccData]
}

func ParityWord(ccData uint16) uint16 {
	a, b := parityTable[0x7F&byte(ccData>>8)], parityTable[0x7F&byte(ccData>>0)]
	return uint16(a)<<8 | uint16(b)
}

func isPadding(ccData uint16) bool { return 0x8080 == ccData }

const (
	eia608_control_resume_caption_loading     = 0x1420
	eia608_control_backspace                  = 0x1421
	eia608_control_alarm_off                  = 0x1422
	eia608_control_alarm_on                   = 0x1423
	eia608_control_delete_to_end_of_row       = 0x1424
	eia608_control_roll_up_2                  = 0x1425
	eia608_control_roll_up_3                  = 0x1426
	eia608_control_roll_up_4                  = 0x1427
	eia608_control_resume_direct_captioning   = 0x1429
	eia608_control_text_restart               = 0x142A
	eia608_control_text_resume_text_display   = 0x142B
	eia608_control_erase_display_memory       = 0x142C
	eia608_control_carriage_return            = 0x142D
	eia608_control_erase_non_displayed_memory = 0x142E
	eia608_control_end_of_caption             = 0x142F

	eia608_tab_offset_0 = 0x1720 // not rally a thing
	eia608_tab_offset_1 = 0x1721
	eia608_tab_offset_2 = 0x1722
	eia608_tab_offset_3 = 0x1723
)

func isControl(ccData uint16) bool { return 0x1420 == (0x7670&ccData) || 0x1720 == (0x7770&ccData) }
func (f *Frame) backspace() {
	if f.col > 0 {
		f.col--
	}
	f.active[f.row][f.col] = frameBufferChar{}
}

func (f *Frame) parseControl(ccData uint16) error {
	var cmd, cc uint16
	if 0 != 0x0200&ccData {
		// not tabs
		cc = (ccData&0x0800)>>10 | (ccData&0x0100)>>8
		cmd = 0x167F & ccData
	} else {
		// tabs
		cc = (ccData & 0x0800) >> 11
		cmd = 0x177F & ccData
	}
	cc = cc // TODO!

	switch cmd {
	// Switch to paint on
	case eia608_control_resume_direct_captioning:
		f.rollup = 0
		f.active = &f.front
		return nil //LIBCAPTION_OK;

	case eia608_control_erase_display_memory:
		f.front = [Rows][Cols]frameBufferChar{} // set defaults
		return nil                              //LIBCAPTION_READY;

		// ROLL-UP
	case eia608_control_roll_up_2:
		f.rollup = 1
		f.active = &f.front
		return nil //LIBCAPTION_OK

	case eia608_control_roll_up_3:
		f.rollup = 2
		f.active = &f.front
		return nil //LIBCAPTION_OK

	case eia608_control_roll_up_4:
		f.rollup = 3
		f.active = &f.front
		return nil //LIBCAPTION_OK

	case eia608_control_carriage_return:
		// TODO!
		return nil //LIBCAPTION_OK
	case eia608_control_backspace:
		f.backspace()
		return nil //LIBCAPTION_OK
	case eia608_control_delete_to_end_of_row:
		for i := f.col; i < Cols; i++ {
			f.active[f.row][i] = frameBufferChar{}
		}
		return nil //LIBCAPTION_OK

	// POP ON
	case eia608_control_resume_caption_loading:
		f.rollup = 0
		f.active = &f.back
		return nil //LIBCAPTION_OK;

	case eia608_control_erase_non_displayed_memory:
		f.back = [Rows][Cols]frameBufferChar{}
		return nil //LIBCAPTION_OK;

	case eia608_control_end_of_caption:
		f.front, f.back = f.back, [Rows][Cols]frameBufferChar{}
		f.active = &f.back
		return nil //LIBCAPTION_READY

	// cursor positioning
	case eia608_tab_offset_1:
		f.col = 1
		return nil //LIBCAPTION_OK;
	case eia608_tab_offset_2:
		f.col = 2
		return nil //LIBCAPTION_OK;
	case eia608_tab_offset_3:
		f.col = 3
		return nil //LIBCAPTION_OK;

	// Unhandled
	default:
		// case eia608_control_alarm_off:
		// case eia608_control_alarm_on:
		// case eia608_control_text_restart:
		// case eia608_control_text_resume_text_display:
		return nil //LIBCAPTION_OK

	}
}

const (
	eia608_style_white   = 0
	eia608_style_green   = 1
	eia608_style_blue    = 2
	eia608_style_cyan    = 3
	eia608_style_red     = 4
	eia608_style_yellow  = 5
	eia608_style_magenta = 6
	eia608_style_italics = 7
)

func isPreamble(ccData uint16) bool { return 0x1040 == (0x7040 & ccData) }

func (f *Frame) parsePreamble(ccData uint16) error {
	rowMap := []int{10, -1, 0, 1, 2, 3, 11, 12, 13, 14, 4, 5, 6, 7, 8, 9}
	f.row = rowMap[((0x0700&ccData)>>7)|((0x0020&ccData)>>5)]
	// cc := !!(0x0800 & ccData) // TODO handle channels!
	f.underline = 0x0001&ccData == 1

	if 0 != 0x0010&ccData {
		f.style = eia608_style_white
		f.col = int(4 * ((0x000E & ccData) >> 1))
	} else {
		f.style = int((0x000E & ccData) >> 1)
		f.col = 0
	}
	return nil
}

func isMidRowChange(ccData uint16) bool { return 0x1120 == (0x7770 & ccData) }
func (f *Frame) parseMidRowChange(ccData uint16) error {
	// cc := !!(0x0800 & ccData); TODO!
	if 0x1120 == (0x7770 & ccData) {
		f.style = int((0x000E & ccData) >> 1)
		f.underline = 0x0001&ccData == 1
	}
	return nil
}

func (f *Frame) writeChar(i uint16) error {
	// TODO check charMap range!
	char := charMap[i]

	// TODO check cow/col range!
	f.active[f.row][f.col] = frameBufferChar{
		char:      string(char),
		underline: f.underline,
		style:     f.style,
	}
	f.col++
	return nil
}

func isBasicNA(ccData uint16) bool   { return 0x0000 != (0x6000 & ccData) }
func isSpecialNA(ccData uint16) bool { return 0x1130 == (0x7770 & ccData) }
func isWesternEu(ccData uint16) bool { return 0x1220 == (0x7660 & ccData) }
func (f *Frame) parseText(ccData uint16) error {
	ccData &= 0x7F7F // strip off parity bits

	// Handle Basic NA BEFORE we strip the channel bit
	if isBasicNA(ccData) {
		if err := f.writeChar((ccData >> 8) - 0x20); err != nil {
			return err
		}
		ccData &= 0x00FF
		if 0x0020 <= ccData && 0x0080 > ccData {
			// we got first char, yes. But what about second char?
			return f.writeChar(ccData - 0x20)
		}
		return nil
	}

	// Check then strip second channel toggle
	// ccToggle := ccData & 0x0800 // TODO CC1-4
	ccData = ccData & 0xF7FF
	if isSpecialNA(ccData) {
		// Special North American character
		return f.writeChar(ccData - 0x1130 + 0x60)
	}

	if 0x1220 <= ccData && 0x1240 > ccData {
		// Extended Western European character set, Spanish/Miscellaneous/French
		f.backspace()
		return f.writeChar(ccData - 0x1220 + 0x70)
	}

	if 0x1320 <= ccData && 0x1340 > ccData {
		// Extended Western European character set, Portuguese/German/Danish
		f.backspace()
		return f.writeChar(ccData - 0x1320 + 0x90)
	}

	return nil //
}

func NewFrame() *Frame {
	return &Frame{col: Cols - 1}
}

func (f *Frame) Decode(ccData uint16, timestamp float64) error {
	// parity error, just skip it
	if ParityWord(ccData) != ccData {
		return nil
	}

	if isPadding(ccData) {
		return nil
	}

	// TODO
	// if (0 > frame->timestamp || frame->timestamp == timestamp || LIBCAPTION_READY == frame->status) {
	//     frame->timestamp = timestamp;
	//     frame->status = LIBCAPTION_OK;
	// }

	// TODO
	// // skip duplicate controll commands. We also skip duplicate specialna to match the behaviour of iOS/vlc
	// if ((eia608_is_specialna(ccData) || eia608_is_control(ccData)) && ccData == frame->state.ccData) {
	//     frame->status = LIBCAPTION_OK;
	//     return frame->status;
	// }

	f.ccData = ccData
	if isControl(ccData) {
		return f.parseControl(ccData)
	}
	if isPreamble(ccData) {
		return f.parsePreamble(ccData)
	}
	if isMidRowChange(ccData) {
		return f.parseMidRowChange(ccData)
	}
	if f.active == nil {
		// We joind an in progrees stream, We must wait for a controll charcter to tell us what mode we are in
		return nil
	}
	if isBasicNA(ccData) || isSpecialNA(ccData) || isWesternEu(ccData) {
		return f.parseText(ccData)
	}
	return nil
}
