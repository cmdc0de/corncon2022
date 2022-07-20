
#include <cstdio>
#include <cstdlib>

int main(int arc, char *argv[]) {
   int r = atoi(argv[1])/8;
   int g = atoi(argv[2])/4;
   int b = atoi(argv[3])/8;
   int Bit16 = ((r&0b11111)<<11) | ((g&0b111111)<<5) | (b&0b11111);
   std::printf("%x\n",Bit16);
   return 0;
      
}
