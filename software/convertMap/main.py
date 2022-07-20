import sys, getopt

MapDict = {
#empty assume black
    "" : 0x0000
# Reserved
    ,"R": 0x0000 
#wall
    ,"X": 0x6b4d
#red
    ,"r": 0xF800
#red DOOR
    ,"RD": 0xF802
#green
    ,"g": 0x7E0
#Green Door
    ,"GD": 0x7E2
#black
    ,"bl": 0x0000 
#white
    ,"w": 0xFFFF
#white DOOR
    ,"WD": 0xFFFD
#gray
    ,"gr": 0x8410
#gray silver
    ,"gs": 0xc618
#blue
    ,"b": 0x1f
#blue door
    ,"BD": 0x1d
#purple
    ,"p": 0xa11e
#purple door
    ,"PD": 0xa11c
#black room
    ,"br": 0xce79
#black room door
    ,"BD": 0xce77
#yellow
    ,"Y": 0xff40
#gold door
    ,"GD": 0xd566
}

def generateMap(ifile, of, v):
    totalLines = 0
    totalUnCommentedLines = 0
    totalCells = 0
    totalErrorCells = 0
    minColPerRow = 999999 
    maxColPerRow = 0
    with open(ifile) as f:
        with open(of, "wb") as ofile:
            for index, line in enumerate(f):
                totalLines = totalLines +1
                if not line.startswith("#"):
                    totalUnCommentedLines = totalUnCommentedLines+1
                    columnOne = True
                    line = line.replace("\n","")
                    if v:
                        print(line)
                    ar = line.split(",")
                    if v:
                        print(ar)
                    data = []
                    color = None
                    ColPerRow = 0
                    for c in ar:
                        if columnOne:
                            columnOne = False
                        else:
                            totalCells=totalCells+1
                            ColPerRow=ColPerRow+1
                            if c is None:
                                c = "bl"

                            #ignore EOL
                            if c!="\n":
                                try:
                                    color = MapDict[c]
                                    data.append(color>>8)
                                    data.append(color&0xFF)
                                except:
                                    color = None
                                    totalErrorCells=totalErrorCells+1
                        
                                if color is None:
                                    print("missing entry in mapDict {} on line {}".format(c, index))

                    bytes = bytearray(data)
                    ofile.write(bytes)
                    if ColPerRow<minColPerRow:
                        minColPerRow = ColPerRow

                    if ColPerRow>maxColPerRow:
                        maxColPerRow = ColPerRow
    print("total rows processed ",totalLines)
    print("total uncommented lines processed ", totalUnCommentedLines)
    print("total cells processed ",totalCells)
    print("total errors in processing file ", totalErrorCells)
    print("Min Col Per Row ",minColPerRow)
    print("Max Col Per Row ",maxColPerRow)


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

    print ('Input file is ', inputfile)
    print ('Output file is ', outputfile)

    print ('Map Diction = ',MapDict)
    generateMap(inputfile, outputfile, Verbose)


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    main(sys.argv[1:])
