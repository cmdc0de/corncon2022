import sys, getopt

CBLACK = 0x0000
CWHITE = 0xFFFF
CGRAY  = 0x0F0F
CPURPLE = 0xFF0F
CRED    = 0xFF00
CWALL   = 0xF0F0

def generateMap(ifile, of, v):
    with open(ifile) as f:
        with open(of, "wb") as ofile:
            for index, line in enumerate(f):
                if not line.startswith("#"):
                    columnOne = True
                    if v:
                        print(line)
                    ar = line.split(",")
                    if v:
                        print(ar)
                    data = []
                    for c in ar:
                        if v:
                            print(c)
                        if columnOne:
                            columnOne = False
                        elif c is None:
                            data.append(CBLACK>>8)
                            data.append(CBLACK&0xFF)
                        elif c == 'w':
                            data.append(CWHITE>>8)
                            data.append(CWHITE&0xFF)
                        elif c == 'X':
                            data.append(CWALL>>8)
                            data.append(CWALL&0xFF)
                        else:
                            data.append(CBLACK>>8)
                            data.append(CBLACK&0xFF)

                    bytes = bytearray(data)
                    ofile.write(bytes)


def main(argv):
    Verbose = False
    inputfile = ''
    outputfile = ''
    try:
        opts, args = getopt.getopt(argv,"hvi:o:",["ifile=","ofile=","verbose"])
    except getopt.GetoptError:
      print ('main.py -i <inputfile> -o <outputfile>')
      sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print ('test.py -i <inputfile> -o <outputfile>')
            sys.exit()
        elif opt in ("-i", "--ifile"):
            inputfile = arg
        elif opt in ("-o", "--ofile"):
            outputfile = arg
        elif opt in ("-v", "--verbose"):
            Verbose = True

    print ('Input file is "', inputfile)
    print ('Output file is "', outputfile)

    generateMap(inputfile, outputfile, Verbose)


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    main(sys.argv[1:])
