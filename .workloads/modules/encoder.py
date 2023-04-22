#dict_xbyte
xbyte = {
    # escape_charaters
    '\0':'0x00',
    '\v':'0x0a',
    '\f':'0x0b',
    '\b':'0x0c',
    '\t':'0x0d',
    '\r':'0x0e',
    '\n':'0x0f',
    '\\':'0x0g',
    '\"':'0x0h',
    '\'':'0x0i',

    # numbers
    '0':'0xa0',
	'1':'0xaa',
	'2':'0xab',
	'3':'0xac',
	'4':'0xad',
	'5':'0xae',
	'6':'0xaf',
	'7':'0xag',
	'8':'0xah',
	'9':'0xai',

    # low_letters
    'a':'0xb0',
	'b':'0xba',
	'c':'0xbb',
	'd':'0xbc',
	'e':'0xbd',
	'f':'0xbe',
	'g':'0xbf',
	'h':'0xbg',
	'i':'0xbh',
	'j':'0xbi',

    'k':'0xc0',
	'l':'0xca',
	'm':'0xcb',
	'n':'0xcc',
	'o':'0xcd',
	'p':'0xce',
	'q':'0xcf',
	'r':'0xcg',
	's':'0xch',
	't':'0xci',

    'u':'0xd0',
	'v':'0xda',
	'w':'0xdb',
	'x':'0xdc',
	'y':'0xdd',
	'z':'0xde',

    # high_letters
    'A':'0xdf',
	'B':'0xdg',
	'C':'0xdh',
	'D':'0xdi',

    'E':'0xe0',
	'F':'0xea',
	'G':'0xeb',
	'H':'0xec',
	'I':'0xed',
	'J':'0xee',
	'K':'0xef',
	'L':'0xeg',
	'M':'0xeh',
	'N':'0xei',

    'O':'0xf0',
	'P':'0xfa',
	'Q':'0xfb',
	'R':'0xfc',
	'S':'0xfd',
	'T':'0xfe',
	'U':'0xff',
	'V':'0xfg',
	'W':'0xfh',
	'X':'0xfi',

    'Y':'0xg0',
    'Z':'0xga',

    # special_charater
    '~':'0xgb',
	'`':'0xgc',
	'!':'0xgd',
	'@':'0xge',
	'#':'0xgf',
	'$':'0xgg',
	'%':'0xgh',
	'^':'0xgi',

    '&':'0xh0',
	'*':'0xha',
	'(':'0xhb',
	')':'0xhc',
	'-':'0xhd',
	'_':'0xhe',
	'+':'0xhf',
	'=':'0xhg',
	'{':'0xhh',
	'}':'0xhi',

    '[':'0xi0',
	']':'0xia',
	'|':'0xib',
	'/':'0xic',
	':':'0xid',
	';':'0xie',
	'<':'0xif',
	'>':'0xig',
	',':'0xih',
	'.':'0xii',

    # unknown_character
    '?':'0x??',

    # spacebar
    ' ':'0xsb',

    # xbyte_correction
    'bd!':'0xb!',
    'ok!':'0xo!'
}


# encode
def encode(data):
    encodexbyte = '0x'
    countdata = len(data)
    counter = 0
    while counter < countdata:
        procces = data[counter]
        strap = xbyte[procces][2:]

        encodexbyte = encodexbyte + strap
        counter += 1
    return encodexbyte


# decode
def decode(data):
    decodexbyte = ''
    numsdata = data[2:]
    countdata = len(numsdata) / 2
    counter1 = 0
    counter2 = 0
    counter3 = 2
    while counter1 < countdata:
        procces = '0x' + numsdata[counter2:counter3]
        strap = ''
        for k, v in xbyte.items():
            if procces == v:
                strap = k

        decodexbyte = decodexbyte + strap
        counter1 += 1
        counter2 += 2
        counter3 += 2
    return decodexbyte


# tester
#print(encode('Hello World'))
#print(decode('0xecbdcacacdsbfhcdcgcabc'))

# manual_tester
#print(encode('lolenseu'))