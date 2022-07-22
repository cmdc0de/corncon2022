import sys, getopt

MapDict = {
#empty assume black
    "" : 0x0000
    ,"1" : 0x0000
    ,"2" : 0x0000
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
#gold 
    ,"GL": 0xd566
#white
   , "O": 0xFFFF
}

def generateMap(ifile, of, pathFile, v):
    totalLines = 0
    totalUnCommentedLines = 0
    totalCells = 0
    totalErrorCells = 0
    minColPerRow = 999999 
    maxColPerRow = 0
    pFile = None
    pathAll = []
    firstPathWrite = True
    if pathFile != None:
        pFile = open(pathFile, "w")

    with open(ifile) as f:
        with open(of, "wb") as ofile:
            path = []
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
                    pathBitCount = 0
                    color = None
                    ColPerRow = 0
                    path.append(0)
                    for c in ar:
                        if columnOne:
                            columnOne = False
                        else:
                            if c is None:
                                c = "bl"

                            #ignore EOL
                            if c!="\n":
                                try:
                                    if c == "-":
                                        color = MapDict["bl"]
                                    else:
                                        color = MapDict[c]
                                    data.append(color>>8)
                                    data.append(color&0xFF)
                                except:
                                    color = None
                                    totalErrorCells=totalErrorCells+1
                        
                                if color is None:
                                    print("missing entry in mapDict {} on line {}".format(c, index))

                            if pFile != None:
                                pindex = int((int(totalUnCommentedLines-1)*int(maxColPerRow))+int(ColPerRow))/int(8)
                                if v:
                                    print ("uncommented {} maxColPer row {} ColPerRow {} index: {} array Length: {}".format(int(totalUnCommentedLines), int(maxColPerRow), int (ColPerRow), int(pindex), len(path)))
                                if c == "o" or c == "O" or c == "-" or c == "GL":
                                    path[int(pindex)] |= 1<< int(pathBitCount)

                                if pathBitCount >= 7:
                                    pathBitCount = 0
                                    path.append(0)
                                else:
                                    pathBitCount = pathBitCount + 1

                            totalCells=totalCells+1
                            ColPerRow=ColPerRow+1

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
    if pFile != None:
        pFile.write("unsigned char PathData[] = \n")
        s = str(path)
        s = s.replace("[","{")
        s = s.replace("]","}")
        pFile.write(s)
        pFile.write(";")
        pFile.close()


def main(argv):
    Verbose = False
    inputfile = ''
    outputfile = ''
    pathFile = None
    try:
        opts, args = getopt.getopt(argv,"hvi:o:p:",["ifile=","ofile=","pfile=","verbose"])
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
        elif opt in ("-p", "--pfile"):
            pathFile = arg

    print ('Input file is ', inputfile)
    print ('Output file is ', outputfile)

    print ('Map Diction = ',MapDict)
    generateMap(inputfile, outputfile, pathFile, Verbose)


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    main(sys.argv[1:])
