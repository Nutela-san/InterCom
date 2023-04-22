#ifndef _INTERCOM_H_
#define _INTERCOM_H_

#include <Arduino.h>
#include <string.h>
#include <stdlib.h>

#define max_cmd_available 20
#define max_cmd_lenght 20


bool cmpCMDs(char cmd_in[], char cmd_list[]);

struct Command
{
    char cmd_name[max_cmd_lenght]={0};
    float *data2change;
};

struct Command2funtion
{
    char cmd_name[max_cmd_lenght]={0};
    void (*subRutine)(void);
};

class SimpleCommand{

    private:
    Command cmd_list[max_cmd_available];
    Command2funtion cmd_list2[max_cmd_available];
    char cmd_readed[max_cmd_lenght]={0}, end_line = '\n';
    uint8_t coun_char_read=0, cmd_list_count = 0;
    Stream *port_comunication = nullptr; //!< Serial terminal variable if provided
    bool en_echo;
    
    void check_cmd(void);
    bool check_endline(char c);

    public:
    SimpleCommand(void);
    ~SimpleCommand(){}

    void begin(unsigned long baud_rate);
    void begin(Stream *port); // configurar la comunicacion para donde resivira los datos
    void addCommand(const char cmd_name[],float *dato2modify);
    void addCommand(const char cmd_name[],void (*subRutine)(void));
    void list(void);
    void listen(void);
    void enable_echo(bool en);
    
};

#endif