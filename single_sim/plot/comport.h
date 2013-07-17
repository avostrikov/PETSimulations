int  OpenAdrPort ();
int  WriteAdrPort(char* psOutput);
int  ReadAdrPort(char* psResponse, int iMax);
int  CloseAdrPort();

int  Doit(char* Command, int second);    
int  Do_online();    
int  Do_clear();    
