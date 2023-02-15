#ifndef _INTERCOM_H
#define _INTERCOM_H

#include <Arduino.h>
#include <string.h>
#include <stdlib.h>

#define max_comand_available 20
#define max_comand_lenght 20


bool cmpCMDs(char cmd_in[], char cmd_list[]);

struct comand
{
    char comand_name[max_comand_lenght]={0};
    float *data2change;
};

struct comand2funtion
{
    char comand_name[max_comand_lenght]={0};
    void (*subRutine)(void);
};

class SimpleComand{

    private:
    comand cmd_list[max_comand_available];
    comand2funtion cmd_list2[max_comand_available];
    char cmd_readed[max_comand_lenght]={0}, end_line = '\n';
    uint8_t coun_char_read=0, cmd_list_count = 0;
    HardwareSerial *port_comunication = nullptr; //!< Serial terminal variable if provided
    bool en_echo;

    public:
    SimpleComand(void);
    ~SimpleComand(){}

    void begin(unsigned long baud_rate); // configurar la comunicacion para donde resivira los datos
    void addComand(char cmd_name[],float *dato2modify);
    void addComand(char cmd_name[],void (*subRutine)(void));
    void list(void);
    void lisent(void);
    void check_cmd(void);
    bool check_endline(char c);
    void enable_echo(bool en);
    
};

#endif