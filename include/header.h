
#define MCP23017_ADR 0x20
#define I2C_Freq 400000
#define I2C_SDA 14
#define I2C_SCL 32

/*
#define RD_1 1  //PA1
#define GN_1 2  //PA2
#define BE_1 3  //PA3
#define RD_2 4  //PA4
#define GN_2 5  //PA5
#define BE_2 6  //PA6
#define RD_3 7  //PA7
#define GN_3 8  //PB0
#define BE_3 9  //PB1
#define RD_4 10 //PB2
#define GN_4 11 //PB3
#define BE_4 12 //PB4
#define RD_5 13 //PB5
#define GN_5 14 //PB6
#define BE_5 15 //PB7
*/

//Structure
struct Quete {
	bool    rune[5];
	uint8_t sort[5][4];
};

Quete Quetes[5];

int RD[] = {1,4,7,10,13}; //PA1,PA4,PA7,PB2,PB5
int GN[] = {2,5,8,11,14}; //PA2,PA5,PB0,PB3,PB6
int BE[] = {3,6,9,12,15}; //PA3,PA6,PB1,PB4,PB7

uint8_t  DUMY[] =       {0x00,0x00,0x00,0x00};
uint8_t  FEU[] =        {0x74,0x16,0xC8,0x87}; //1
uint8_t  AIR[] =        {0x74,0x0B,0x8B,0x87}; //2
uint8_t  EAU[] =        {0x74,0x01,0xCA,0x87}; //3
uint8_t  TERRE[] =      {0x64,0xF5,0xF8,0x87}; //4
uint8_t  LUMIERE[] =    {0x64,0xEA,0x25,0x87}; //5
uint8_t  ILLUSION[] =   {0x64,0xDE,0x53,0x87}; //6
uint8_t  FIN[] =        {0x64,0xD2,0x89,0x87}; //7
uint8_t  CONTROLE[] =   {0x64,0xC6,0xDB,0x87}; //8
uint8_t  DEBUT[] =      {0x64,0xBD,0xBA,0x87}; //9
uint8_t  NEGATION[] =   {0x64,0xB2,0x5A,0x87}; //10
uint8_t  TEMPS[] =      {0x74,0x28,0x64,0x87}; //11
uint8_t  CREATURE[] =   {0x74,0x33,0xFE,0x87}; //12
uint8_t  PROTECTION[] = {0x74,0x3F,0x7F,0x87}; //13
uint8_t  OFFRANDE[] =   {0x74,0x4A,0xD2,0x87}; //14
uint8_t  ENERGIE[] =    {0x74,0x45,0x24,0x87}; //15
uint8_t  VIE[] =        {0x74,0x39,0xBA,0x87}; //16
uint8_t  CREATION[] =   {0x74,0x2E,0x2F,0x87}; //17
uint8_t  PERMANENCE[] = {0x74,0x22,0x85,0x87}; //18
uint8_t  ESPRIT[] =     {0x74,0x1C,0x7A,0x87}; //19
uint8_t  CHANGEMENT[] = {0x74,0x0B,0x80,0x87}; //20
