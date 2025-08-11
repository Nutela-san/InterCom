#include "InterCom.h"

SimpleCommand::SimpleCommand(){
    //nada
}

void SimpleCommand::begin(unsigned long baud_rate){
    Serial.begin(baud_rate);
    begin(&Serial);
}

void SimpleCommand::begin(Stream *port){
    port_comunication = port;
    port_comunication->println(F("-Enabled Terminal-"));
}

void SimpleCommand::addCommand(const char cmd_name[],float *dato2modify){
    uint8_t Command_lenght = sizeof((char*)cmd_name)/sizeof(cmd_name[0]);
    if(cmd_list_count <= max_cmd_available && Command_lenght<= max_cmd_lenght){
        strcpy(cmd_list[cmd_list_count].cmd_name,cmd_name);
        cmd_list[cmd_list_count].data2change = dato2modify;
    }
    cmd_list_count++;
}

void SimpleCommand::addCommand(const char cmd_name[],void (*subRutine)(void)){
    uint8_t Command_lenght = sizeof((char*)cmd_name)/sizeof(cmd_name[0]);
    if(cmd_list_count <= max_cmd_available && Command_lenght<= max_cmd_lenght){
        strcpy(cmd_list2[cmd_list_count].cmd_name,cmd_name);
        cmd_list2[cmd_list_count].subRutine = subRutine;
    }
    cmd_list_count++;
}

void SimpleCommand::list(void){
    char msj[50];
    sprintf(msj,"-- Lista de Commandos --");
    port_comunication->println(msj);
    for(uint8_t i=0;i<max_cmd_available;i++){
        if(strlen(cmd_list[i].cmd_name) != 0) {
            sprintf(msj,"%d.- '%s'",i+1, cmd_list[i].cmd_name);
            port_comunication->println(msj);
        }else if(strlen(cmd_list2[i].cmd_name) != 0){
            sprintf(msj,"%d.- '%s'",i+1, cmd_list2[i].cmd_name);
            port_comunication->println(msj);
        }
    }
}

void SimpleCommand::listen(void){
    while(port_comunication->available()){
        char ch = port_comunication->read();
        bool endline = check_endline(ch);
        bool backspace = check_backspace(ch);
        
        if(ch != '\r' && !endline && !backspace) cmd_readed[coun_char_read] = ch;
        if(en_echo && ch != '\r'){
            if(backspace){
                port_comunication->write(ch);
                port_comunication->write(' ');
            }
            port_comunication->write(ch);
        }
        if(endline){
            check_cmd();
            coun_char_read = 0;
            for(uint8_t i=0;i<max_cmd_lenght;i++){ //vaciando string
                cmd_readed[i]= 0;
            }
        }
        else if (coun_char_read>= max_cmd_lenght){
            coun_char_read = 0;
            for(uint8_t i=0;i<max_cmd_lenght;i++){ //vaciando string
                cmd_readed[i]= 0;
            }
        }
        else{
            if(backspace && coun_char_read>=0){
                cmd_readed[coun_char_read] = 0;
                coun_char_read--;
            }
            else{
                coun_char_read ++;
            }
        }
        
    }
}

void SimpleCommand::check_cmd(void){
    uint8_t exist = 0,i;
    uint8_t num_cmd = 0;
    for(i=0;i<max_cmd_available;i++){
        if(cmpCMDs(cmd_readed,cmd_list[i].cmd_name)){
            num_cmd = i;
            i = max_cmd_available;
            exist = 1;
        }
        else if (cmpCMDs(cmd_readed,cmd_list2[i].cmd_name)){
            num_cmd = i;
            i = max_cmd_available;
            exist = 2;
        }
        else{
            exist = 0;
        }
    }
        
    char mensj[50];
    switch(exist){
        case 1:{
            i=strlen(cmd_list[num_cmd].cmd_name);
            for(uint8_t j=i;j<max_cmd_lenght;j++){
                cmd_readed[j-i]= cmd_readed[j];
            }
            for(uint8_t j=max_cmd_lenght-i;j<max_cmd_lenght;j++){
                cmd_readed[j]= 0;
            }
            if(strlen(cmd_readed)!=0) *cmd_list[num_cmd].data2change = atof(cmd_readed);
            
            #ifdef ARDUINO_RASPBERRY_PI_PICO
            sprintf(mensj,"%s = %f\n",cmd_list[num_cmd].cmd_name,*cmd_list[num_cmd].data2change);
            #else
            char float_value[10];
            dtostrf(*cmd_list[num_cmd].data2change,1,3,float_value);
            sprintf(mensj,"%s = %s\n",cmd_list[num_cmd].cmd_name,float_value);
            #endif
            break;
        }
        case 2:{
            cmd_list2[num_cmd].subRutine();
            for(i=0;i<50;i++){
                mensj[i] = 0;
            }
            break;
        }
        case 0: {
            sprintf(mensj,"Error -%s-\n",cmd_readed);
            break;
        }
    }
    if(en_echo) port_comunication->print(mensj);
}

bool cmpCMDs(char cmd_in[], char cmd_list[]){
    uint8_t i;
    bool igual = false;
    for(i=0;i<max_cmd_lenght;i++){
        if(cmd_list[i]!= 0){
            if(cmd_in[i] == cmd_list[i]){
                igual = true;
            }
            else{
                igual = false;
                i = max_cmd_lenght;
            }
        }
    }
    return igual;
}

bool SimpleCommand::check_endline(char c){
    if(c == end_line) return true;
    return false;
}

bool SimpleCommand::check_backspace(char c){
    if(c == '\b' || c == 127) return true;
    return false;
}

void SimpleCommand::enable_echo(bool en){ 
    en_echo = en;
}